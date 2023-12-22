#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <set>

void compute_score_for_card(int& score_for_card, std::istringstream& line_as_stream, std::set<int>& winning_numbers) {
    std::string my_number_as_string;
    while (line_as_stream >> my_number_as_string) {
        int my_number {std::stoi(my_number_as_string)};
        if (winning_numbers.contains(my_number)) {
            if (score_for_card == 0) {
                score_for_card = 1;
            } else if (score_for_card > 0) {
                score_for_card *= 2;
            }
        }
    }
}


void insert_winning_numbers(std::set<int>& winning_numbers, std::istringstream& line_as_stream) {
    std::string winning_number;
    line_as_stream >> winning_number;
    while (winning_number != "|") {
        winning_numbers.insert(std::stoi(winning_number));
        line_as_stream >> winning_number;
    }
}


void get_card_number(int& card_number, std::istringstream& line) {
    std::string ignored_portion;
    line >> ignored_portion;
    std::string portion_containing_card_number;
    line >> portion_containing_card_number;
    portion_containing_card_number.pop_back();
    card_number = std::stoi(portion_containing_card_number);
}


void process_scratchcards(std::ifstream& file) {
    std::string line;
    int sum_of_scratchcard_points {0};
    while (std::getline(file, line)) {
        int card_number {0};
        int score_for_card {0};
        std::set<int> winning_numbers;
        std::istringstream line_as_stream{line};
        get_card_number(card_number, line_as_stream);
        insert_winning_numbers(winning_numbers, line_as_stream);
        compute_score_for_card(score_for_card, line_as_stream, winning_numbers);
        sum_of_scratchcard_points += score_for_card;
    }
    std::cout << "The sum of scratchcard points is: " << sum_of_scratchcard_points << '\n';
}


int main() {
    std::ifstream file("input-small.txt");
    std::string line {};
    if (file) {
        process_scratchcards(file);
    }
    return 0;
}