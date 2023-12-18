#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

struct location_of_number_in_input {
    int row {0};
    int column {0};

    // Cheeky little hash function courtesy of chat jippity.
    struct hash_function {
        size_t operator()(const location_of_number_in_input& obj) const {
            return (std::hash<int>()(obj.row) ^ (std::hash<int>()(obj.column) << 1));
        }
    };

    bool operator==(const location_of_number_in_input& other) const {
        return (row == other.row) && (column == other.column);
    }
};

struct directions {
    int vertical_offset {};
    int horizontal_offset {};
};

struct location_value_combo {
    location_of_number_in_input location {};
    int value {};
};

typedef std::unordered_map<location_of_number_in_input, int, location_of_number_in_input::hash_function> location_to_value_map;

void get_input_lines(std::string& filename, std::vector<std::vector<char>>& the_matrix) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        exit(1);
    }
    std::string line {};
    unsigned int row {0};
    while(std::getline(file, line)) {
        std::vector<char> line_as_vector {};
        the_matrix.push_back(line_as_vector);
        for (char& c: line) {
            the_matrix[row].push_back(c);
        }
        row++;
    }
}


bool is_symbol(char c) {
    return !std::isalnum(static_cast<unsigned char>(c)) && !std::isspace(static_cast<unsigned char>(c)) && c != '.';
}


void find_number_and_append_to_vector(int row, int column, const std::vector<std::vector<char>>& the_matrix, std::vector<location_value_combo>& location_value_pairs) {
    bool entry_is_number = std::isdigit(the_matrix.at(row).at(column));
    if (!entry_is_number) {
        return;
    }
    location_value_combo lvc {};
    lvc.location.row = row;
    int origin = column;
    while (column > -1 && column < the_matrix.at(row).size() && std::isdigit(the_matrix.at(row).at(column))) {
        column--;
    }
    lvc.location.column = column + 1;
    column = origin;
    while (column > -1 && column < the_matrix.at(row).size() && std::isdigit(the_matrix.at(row).at(column))) {
        column++;
    }
    std::string number_as_string {};
    for (int i {lvc.location.column}; i < column; i++) {
        number_as_string.push_back(the_matrix.at(row).at(i));
    }
    lvc.value = std::stoi(number_as_string);
    location_value_pairs.push_back(lvc);
}


void find_adjacent_numbers_and_add_to_map(const std::vector<std::vector<char>>& the_matrix, int row, int column, location_to_value_map& location_to_values) {
    const static std::vector<directions> possible_directions { {-1, -1}, {-1, 0}, {-1, 1},
                                                               {0, -1}, {0, 1},
                                                               {1, -1}, {1, 0}, {1, 1} };
    std::vector<location_value_combo> location_value_pairs {};
    for (auto& direction: possible_directions) {
        int row_to_check {row + direction.vertical_offset};
        int column_to_check {column + direction.horizontal_offset};
        bool row_is_valid = row_to_check > -1 && row_to_check < the_matrix.size();
        bool column_is_valid = column_to_check > -1 && column_to_check < the_matrix.at(0).size(); // Ew
        if (row_is_valid && column_is_valid) {
            find_number_and_append_to_vector(row_to_check, column_to_check, the_matrix, location_value_pairs);
        }
    }
    for (auto& pair: location_value_pairs) {
        location_to_values[pair.location] = pair.value;
    }
}


void populate_location_to_value_map(const std::vector<std::vector<char>>& the_matrix, location_to_value_map& location_to_value) {
    for (int row {0}; row < the_matrix.size(); row++) {
        for (int column {0}; column < the_matrix.at(row).size(); column++) {
            bool character_is_symbol = is_symbol(the_matrix.at(row).at(column));
            if (character_is_symbol) {
                find_adjacent_numbers_and_add_to_map(the_matrix, row, column, location_to_value);
            }
        }
    }
}


int compute_sum_of_part_numbers(location_to_value_map& location_to_value) {
    int sum_of_part_numbers {0};
    for (const auto& pair: location_to_value) {
        sum_of_part_numbers += pair.second;
    }
    return sum_of_part_numbers;
}


int main() {
    std::vector<std::vector<char>> the_matrix{};
    std::string input_filename {"input.txt"};
    location_to_value_map location_to_value{};
    get_input_lines(input_filename, the_matrix);
    populate_location_to_value_map(the_matrix, location_to_value);
    int sum_of_part_numbers {compute_sum_of_part_numbers(location_to_value)};
    std::cout << "The sum of the part numbers in the schematic given by " << input_filename << " is " << sum_of_part_numbers << std::endl;

    return 0;
}