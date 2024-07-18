#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <vector>
#include "mylib.h"

template <typename T>
std::vector<T> as_vector(
    boost::property_tree::ptree const& pt,
    boost::property_tree::ptree::key_type const& key
) {
    std::vector<T> r;
    for (auto& item : pt.get_child(key)) {
        r.push_back(item.second.get_value<T>());
    }
    return r;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string a = getBranch("paaaa");
    std::cout << a << std::endl;

    return 0;
    std::future<std::string> fut1 = std::async(&getBranch, "p1099999");
    std::future<std::string> fut2 = std::async(&getBranch, "sisyphus");

    std::string str1 = fut1.get();
    std::string str2 = fut2.get();

    std::future<boost::property_tree::ptree> fut3 = std::async(&readJson, str1);
    std::future<boost::property_tree::ptree> fut4 = std::async(&readJson, str2);

    boost::property_tree::ptree root1 = fut3.get();
    boost::property_tree::ptree root2 = fut4.get();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;
    std::cout << root1.get<int>("length") << ", " << root2.get<int>("length") << std::endl;

    return 0;
}
