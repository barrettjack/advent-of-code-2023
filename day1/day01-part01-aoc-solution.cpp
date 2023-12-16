
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <cctype>

int main() {
    std::ifstream file("day01-part01-aoc-input.txt");
    std::string line;
    std::string number_to_add_as_string;
    std::deque<char> numbers_on_line;
    int running_sum {0};

    if (file.is_open()) {
        while (getline(file, line)) {
            for (char c: line) {
                if (std::isdigit(c)) {
                    numbers_on_line.push_back(c);
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
