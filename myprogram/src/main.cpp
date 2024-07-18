#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include "mylib.h"

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::future<std::string> fut1 = std::async(&getBranch, "p10");
    std::future<std::string> fut2 = std::async(&getBranch, "sisyphus");

    std::string str1 = fut1.get();
    std::string str2 = fut2.get();

    ArchPackages first = parsePackages(str1);
    ArchPackages second = parsePackages(str2);

    boost::property_tree::ptree comparisonResult = comparePackages(first, second);

    std::ostringstream result;

    boost::property_tree::write_json(result, comparisonResult);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout<<result.str() << std::endl;

    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;

    return 0;
}
