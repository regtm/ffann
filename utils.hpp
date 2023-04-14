#pragma once
#include <string>
#include <vector>

using namespace std;

struct data_point{
    int id;
    int classification;
    std::vector<double> input;
};

std::vector<data_point> parse_data_file(std::string file_descriptor, string delimiter = " ");

vector<string> split_string(string str, string delim);