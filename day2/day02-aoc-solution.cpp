#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

const int FIRST_CHAR_IN_GAME_ID = 5;

struct CubeCount {
    int red = 0;
    int green = 0;
    int blue = 0;
};

std::vector<std::string> get_games(std::string filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        exit(1);
    }

    std::vector<std::string> games;
    std::string line;

    while (std::getline(file, line)) {
        games.push_back(line);
    }

    file.close();

    return games;
}


// LOL look at these dudes.
std::vector<std::string> game_as_vector(std::string& game) {
    std::vector<std::string> substrings;
    size_t start_position {0};
    size_t found_position {game.find("; ")};
    while (found_position != std::string::npos) {
        std::string substring = game.substr(start_position, found_position - start_position);
        substrings.push_back(substring);
        start_position = found_position + 2;
        found_position = game.find("; ", start_position);
    }
    substrings.push_back(game.substr(start_position));
    return substrings;
}
std::vector<std::string> process_hand(std::string& hand) {
    std::vector<std::string> substrings;
    size_t start_position {0};
    size_t comma_position {hand.find(", ")};
    while (comma_position != std::string::npos) {
        std::string substring {hand.substr(start_position, comma_position - start_position)};
        substrings.push_back(substring);
        start_position = comma_position + 2;
        comma_position = hand.find(", ", start_position);
    }
    substrings.push_back(hand.substr(start_position));
    return substrings;
}


int& determine_colour(std::string& colour, CubeCount& cubes_in_hand) {
    std::unordered_map<std::string, int*> field_map = {
        {"red", &cubes_in_hand.red},
        {"green", &cubes_in_hand.green},
        {"blue", &cubes_in_hand.blue}
    };
    return *(field_map.at(colour));
}


void process_hand_draws(std::vector<std::string>& draws_from_bag, CubeCount& cubes_in_hand) {
    for (auto& part_of_draw: draws_from_bag) {
        std::istringstream part_of_draw_as_stream(part_of_draw);
        std::string count, colour;
        part_of_draw_as_stream >> count;
        part_of_draw_as_stream >> colour;
        determine_colour(colour, cubes_in_hand) += std::stoi(count);
    }
}


bool hand_is_possible(CubeCount& cubes_in_hand, CubeCount& cubes_in_bag) {
    return !(cubes_in_hand.red > cubes_in_bag.red || cubes_in_hand.green > cubes_in_bag.green || cubes_in_hand.blue > cubes_in_bag.blue);
}


int compute_sum_of_possible_game_ids(std::vector<std::string>& games, CubeCount cubes_in_bag) {
    int id_count {0};
    for (auto& game: games) {
        size_t colon_position {game.find(':')};
        int game_id {0};
        if (colon_position != std::string::npos) {
            std::string game_id_string {game.substr(FIRST_CHAR_IN_GAME_ID, colon_position - FIRST_CHAR_IN_GAME_ID)};
            game_id = std::stoi(game_id_string);
            game.erase(0, colon_position + 2);
        }
        bool early_escape_flag = false;
        std::vector<std::string> vector_of_hands {game_as_vector(game)};
        for (auto& hand: vector_of_hands) {
            std::vector<std::string> draws_from_bag {process_hand(hand)};
            CubeCount cubes_in_hand {0, 0, 0};
            process_hand_draws(draws_from_bag, cubes_in_hand);
            if (!hand_is_possible(cubes_in_hand, cubes_in_bag)) {
                early_escape_flag = true;
                break;
            }
        }
        if (early_escape_flag) {
            continue;
        }

        id_count += game_id;
    }
    return id_count;
}


int main() {
    std::vector<std::string> games {get_games("day02-input.txt")};
    CubeCount cubes_in_bag {12, 13, 14};
    int sum_of_possible_game_ids {compute_sum_of_possible_game_ids(games, cubes_in_bag)};
    std::cout << "Sum of IDs of possible games: " << sum_of_possible_game_ids << std::endl;
    return 0;
}
