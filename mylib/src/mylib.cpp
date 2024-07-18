#include "../include/mylib.h"

Package::Package(
    std::string name,
    int epoch,
    std::string version,
    std::string release,
    std::string arch,
    std::string disttag,
    int buildtime,
    std::string source
) {
    this->name = name;
    this->epoch = epoch;
    this->version = version;
    this->release = release;
    this->arch = arch;
    this->disttag = disttag;
    this->buildtime = buildtime;
    this->source = source;
}

bool Package::operator==(const Package& other) const {
    return name == other.name && epoch == other.epoch && version == other.version &&
           release == other.release && arch == other.arch && disttag == other.disttag &&
           buildtime == other.buildtime && source == other.source;
}

bool Package::operator<(const Package& other) const {
    if (version < other.version) return true;
    if (version > other.version) return false;
    return release < other.release;
}

bool Package::operator>(const Package& other) const {
    if (version > other.version) return true;
    if (version < other.version) return false;
    return release > other.release;
}

std::string getBranch(std::string branch) {
    cpr::Response response = cpr::Get(cpr::Url{BASE_URL + branch});
    if (response.status_code != 200) {
        throw std::invalid_argument("received invalid argument: " + branch);
    }
    return response.text;
}

boost::property_tree::ptree readJson(std::string jsonStr) {
    std::stringstream encoded(jsonStr);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(encoded, root);
    return root;
}

ArchPackages parsePackages(std::string jsonStr) {
    ArchPackages packages;
    try {
        boost::property_tree::ptree root = readJson(jsonStr);
        for (const auto& item : root.get_child("packages")) {
            Package pkg = Package(
                item.second.get<std::string>("name"),
                item.second.get<int>("epoch"),
                item.second.get<std::string>("version"),
                item.second.get<std::string>("release"),
                item.second.get<std::string>("arch"),
                item.second.get<std::string>("disttag"),
                item.second.get<int>("buildtime"),
                item.second.get<std::string>("source")
            );
            packages[pkg.arch].insert(pkg);
        }
    } catch (boost::property_tree::json_parser_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << '\n';
    } catch (boost::property_tree::ptree_bad_path& e) {
        std::cerr << "Invalid JSON structure: " << e.what() << '\n';
    }

    return packages;
}

boost::property_tree::ptree packageToPTree(const Package& pkg) {
    boost::property_tree::ptree pt;
    pt.put("name", pkg.name);
    pt.put("epoch", pkg.epoch);
    pt.put("version", pkg.version);
    pt.put("release", pkg.release);
    pt.put("arch", pkg.arch);
    pt.put("disttag", pkg.disttag);
    pt.put("buildtime", pkg.buildtime);
    pt.put("source", pkg.source);
    return pt;
}

boost::property_tree::ptree comparePackages(const ArchPackages& first, const ArchPackages& second) {
    boost::property_tree::ptree result;

    for (const auto& [arch, packages1] : first) {
        boost::property_tree::ptree onlyInFirst, onlyInSecond, greaterInFirst;
        auto packages2 = second.find(arch);
        if (packages2 != second.end()) {
            for (const auto& pkg : packages1) {
                if (packages2->second.find(pkg) == packages2->second.end()) {
                    onlyInFirst.push_back(std::make_pair("", packageToPTree(pkg)));
                }
            }

            for (const auto& pkg : packages2->second) {
                if (packages1.find(pkg) == packages1.end()) {
                    onlyInSecond.push_back(std::make_pair("", packageToPTree(pkg)));
                }
            }

            for (const auto& pkg1 : packages1) {
                for (const auto& pkg2 : packages2->second) {
                    if (pkg1.name == pkg2.name && pkg1 > pkg2) {
                        greaterInFirst.push_back(std::make_pair("", packageToPTree(pkg1)));
                        break;
                    }
                }
            }

        } else {
            for (const auto& pkg : packages1) {
                onlyInFirst.push_back(std::make_pair("", packageToPTree(pkg)));
            }
        }
        result.add_child(arch + ".only_in_first", onlyInFirst);
        result.add_child(arch + ".only_in_second", onlyInSecond);
        result.add_child(arch + ".greater_in_first", greaterInFirst);
    }

     for (const auto& [arch, packages2] : second) {
        if (first.find(arch) == first.end()) {
            boost::property_tree::ptree onlyInSecond;
            for (const auto& pkg : packages2) {
                onlyInSecond.push_back(std::make_pair("", packageToPTree(pkg)));
            }
            result.add_child(arch + ".only_in_second", onlyInSecond);
        }
    }

    return result;
}
