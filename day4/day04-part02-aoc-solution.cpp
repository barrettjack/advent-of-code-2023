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
            score_for_card += 1;
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


void process_scratchcards(std::ifstream& file) {
    std::string line;
    std::vector<std::pair<std::string, int>> cards_and_multiplicities {};
    while (std::getline(file, line)) {
        std::size_t colon_position {line.find(':')};
        line.erase(0, colon_position + 2);
        auto card_contents_and_multiplicity {std::make_pair(line, 1)};
        cards_and_multiplicities.push_back(card_contents_and_multiplicity);
    }
    for (int i {0}; i < cards_and_multiplicities.size(); i++) {
        std::set<int> winning_numbers {};
        int score {0};
        std::istringstream card_contents_as_stream(cards_and_multiplicities.at(i).first);
        insert_winning_numbers(winning_numbers, card_contents_as_stream);
        compute_score_for_card(score, card_contents_as_stream, winning_numbers);
        if (score > 0) {
            for (int j {i + 1}; j < i + score + 1; j++) {
                try {
                    cards_and_multiplicities.at(j).second += cards_and_multiplicities.at(i).second;
                } catch (std::out_of_range& e) {
                    std::cerr << e.what() << '\n';
                }
            }
        }
    }
    int card_count {0};
    for (auto& pair: cards_and_multiplicities) {
        card_count += pair.second;
    }
    std::cout << "The final card count is: " << card_count << '\n';
}


int main() {
    std::ifstream file("input.txt");
    std::string line {};
    if (file) {
        process_scratchcards(file);
    }
    return 0;
}