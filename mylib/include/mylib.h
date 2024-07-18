#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cpr/cpr.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct Package {
    std::string name;
    int epoch;
    std::string version;
    std::string release;
    std::string arch;
    std::string disttag;
    int buildtime;
    std::string source;

    Package(
        std::string name,
        int epoch,
        std::string version,
        std::string release,
        std::string arch,
        std::string disttag,
        int buildtime,
        std::string source
    );

    bool operator==(const Package& other) const;
    bool operator<(const Package& other) const;
    bool operator>(const Package& other) const;
};

template<>
struct std::hash<Package> {
    size_t operator()(const Package& pkg) const {
        return std::hash<std::string>()(pkg.name) ^ std::hash<int>()(pkg.epoch) ^
               std::hash<std::string>()(pkg.version) ^ std::hash<std::string>()(pkg.release) ^
               std::hash<std::string>()(pkg.arch) ^ std::hash<std::string>()(pkg.disttag) ^
               std::hash<int>()(pkg.buildtime) ^ std::hash<std::string>()(pkg.source);
    }
};

using ArchPackages = std::unordered_map<std::string, std::unordered_set<Package>>;

const std::string BASE_URL = "https://rdb.altlinux.org/api/export/branch_binary_packages/";

std::string getBranch(std::string branch);

ArchPackages parsePackages(std::string jsonStr);

boost::property_tree::ptree packageToPTree(const Package& pkg);

boost::property_tree::ptree comparePackages(const ArchPackages& first, const ArchPackages& second);
