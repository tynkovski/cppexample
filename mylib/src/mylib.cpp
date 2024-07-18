#include "mylib.h"

std::string getBranch(std::string branch) {
    cpr::Response response = cpr::Get(cpr::Url{BASE_URL + branch});
    if (response.status_code != 200) {
        throw std::invalid_argument("received invalid argument: " + branch);
    }
    return response.text;
}

boost::property_tree::ptree readJson(std::string str) {
    std::stringstream encoded(str);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(encoded, root);
    return root;
}
