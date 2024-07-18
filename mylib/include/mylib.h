#include <stdexcept>
#include <string>
#include <cpr/cpr.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

const std::string BASE_URL = "https://rdb.altlinux.org/api/export/branch_binary_packages/";

std::string getBranch(std::string branch);

boost::property_tree::ptree readJson(std::string str);


