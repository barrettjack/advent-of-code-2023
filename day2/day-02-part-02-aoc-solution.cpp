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


void update_minimums(int& min_red_cubes, int& min_green_cubes, int& min_blue_cubes, CubeCount& cubes_in_hand) {
    if (cubes_in_hand.red > min_red_cubes) {
        min_red_cubes = cubes_in_hand.red;
    }
    if (cubes_in_hand.green > min_green_cubes) {
        min_green_cubes = cubes_in_hand.green;
    }
    if (cubes_in_hand.blue > min_blue_cubes) {
        min_blue_cubes = cubes_in_hand.blue;
    }
}


int compute_sum_of_game_powers(std::vector<std::string>& games) {
    int sum_of_powers {0};
    for (auto& game: games) {
        size_t colon_position {game.find(':')};
        int game_id {0};
        if (colon_position != std::string::npos) {
            std::string game_id_string {game.substr(FIRST_CHAR_IN_GAME_ID, colon_position - FIRST_CHAR_IN_GAME_ID)};
            game_id = std::stoi(game_id_string);
            game.erase(0, colon_position + 2);
        }
        std::vector<std::string> vector_of_hands {game_as_vector(game)};
        int min_red_cubes {0};
        int min_green_cubes {0};
        int min_blue_cubes {0};
        for (auto& hand: vector_of_hands) {
            std::vector<std::string> draws_from_bag {process_hand(hand)};
            CubeCount cubes_in_hand {0, 0, 0};
            process_hand_draws(draws_from_bag, cubes_in_hand);
            update_minimums(min_red_cubes, min_green_cubes, min_blue_cubes, cubes_in_hand);
        }
        sum_of_powers += min_red_cubes * min_green_cubes * min_blue_cubes;
    }
    return sum_of_powers;
}


int main() {
    std::vector<std::string> games {get_games("day02-input.txt")};
    int sum_of_game_powers {compute_sum_of_game_powers(games)};
    std::cout << "Sum of IDs of possible games: " << sum_of_game_powers << std::endl;
    return 0;
}
