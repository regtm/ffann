#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "utils.hpp"

using namespace std;

// data file is expected as lines of id, class, x, y
std::vector<struct data_point> parse_data_file(std::string file_descriptor, string delimiter){
    std::ifstream data_file(file_descriptor);

    std::vector<struct data_point> set;

    for(std::string line; std::getline(data_file, line); ){
        //std::cout << line << std::endl;
        std::vector<std::string> tokens = split_string(line, delimiter);
        //std::cout << tokens[0] << tokens[1] << tokens[2] << tokens[3] << std::endl;
        struct data_point parsed_line = {std::stoi(tokens[0]),
                                         std::stoi(tokens[1]),
                                         std::vector<double> {std::stod(tokens[2]), std::stod(tokens[3])}
                                        };
        set.push_back(parsed_line);

    }

    return set;
}

vector<string> split_string(string str, string delim){
    size_t pos = 0, prev = 0;
    vector<string> tokens;

    while(pos < str.length()){
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();    // string::npos is -1 which is returned by find if the pattern is not found
        string token = str.substr(prev, pos - prev);    // pos - prev is the length of the token
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }

    return tokens;
}