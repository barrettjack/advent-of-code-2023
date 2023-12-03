
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <cctype>
#include <unordered_map>

const std::unordered_map<std::string, char> string_to_char = {
        {"one", '1'},
        {"two", '2'},
        {"three", '3'},
        {"four", '4'},
        {"five", '5'},
        {"six", '6'},
        {"seven", '7'},
        {"eight", '8'},
        {"nine", '9'}
};

int main() {
    std::ifstream file("day01-part01-aoc-input.txt");
    std::string line;
    std::string number_to_add_as_string;
    std::deque<char> numbers_on_line;
    int running_sum {0};

    if (file.is_open()) {
        while (getline(file, line)) {
            for (int i {0}; i < line.length(); i++){
                if (std::isdigit(line.at(i))) {
                    numbers_on_line.push_back(line.at(i));
                } else {
                    for (int j {static_cast<int>(line.length())}; j > i; j--) {
                        std::string substring {line.substr(i, j - i)};
                        if (string_to_char.count(substring) > 0) {
                            numbers_on_line.push_back(string_to_char.at(substring));
                            break;
                        }
                    }
                }
            }
            if (numbers_on_line.size() == 0)
                continue;

            number_to_add_as_string.append(1, numbers_on_line.front());
            number_to_add_as_string.append(1, numbers_on_line.back());
            running_sum += std::stoi(number_to_add_as_string);
            number_to_add_as_string.clear();
            numbers_on_line.clear();
        }
    }
    file.close();
    std::cout << "The sum of all the calibration values is: " << running_sum << std::endl;
    return 0;
}
