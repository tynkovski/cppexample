#include <stdio.h>
#include <iostream>
#include <thread>
#include <future>
#include <fstream>
#include <string>
#include <chrono>
#include <stdexcept>
#include "mylib.h"

int main(int argc, char ** argv) {
    std::string b1;
    std::string b2;
    std::string outfile;
    int flag = 0;
    while(1) {
        std::cout <<"> ";
        std::string line;
        std::getline (std::cin, line);
        if (line == "help") {
            std::cout << "enter q to quit, \nb1 to set first branch, \nb2 to set second branch and o to set outfile. enter compare to run comparing two brances.\n";
            if (b1 != "") {
                std::cout << "b1 = " << b1 << "\n";
            }
            if (b2 != "") {
                std::cout << "b2 = " << b2<< "\n";
            }
            if (outfile != "") {
                std::cout << "o = " << outfile << "\n";
            }

        } else if (line == "b1") {
            std::cout << "set name of first branch: ";
            std::cin >> b1;
            flag = flag | 0b001;
        } else if (line == "b2") {
            std::cout << "set name of second branch: ";
            std::cin >> b2;
            flag = flag | 0b010;
        } else if (line == "o") {
            std::cout << "set outfile: ";
            std::cin >> outfile;
            flag = flag | 0b100;
        }  else if (line == "compare") {
            if (flag == 0b111) {
                try {
                    auto start = std::chrono::high_resolution_clock::now();

                    std::future<std::string> fut1 = std::async(&getBranch, b1);
                    std::future<std::string> fut2 = std::async(&getBranch, b2);

                    std::string str1 = fut1.get();
                    std::string str2 = fut2.get();

                    ArchPackages first = parsePackages(str1);
                    ArchPackages second = parsePackages(str2);

                    boost::property_tree::ptree comparisonResult = comparePackages(first, second);

                    std::ofstream out("out.txt");
                    std::ostringstream result;

                    boost::property_tree::write_json(result, comparisonResult);

                    auto stop = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
                    out << result.str() << std::endl;
                    out.close();
                    std::cout << "time taken by program: " << duration.count() << " seconds" << std::endl;
                    std::cout << "outfile at: " << outfile << std::endl;
                    flag = 0;
                } catch(const std::invalid_argument &e) {
                    std::cerr << e.what() << '\n';
                } catch (...) {
                    std::cerr << "some error occured\n";
                }
            } else {
                if ((flag & 0b100) == 0) {
                    std::cout << "you have to set outfile!\n";
                }
                if ((flag & 0b010) == 0) {
                    std::cout << "you have to set second branch!\n";
                }
                if ((flag & 0b001) == 0) {
                    std::cout << "you have to set first branch!\n";
                }
            }
        } else if (line == "q") break;
    }

    return 0;
}

