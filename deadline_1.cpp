#include <iostream>
#include <cstring>
#include <fstream>

struct Arguments {
    int number_of_lines = -1;
    const char* kFilename;
    bool is_tail = false;
    char delimiter = '\n';
};

Arguments ParseArguments (int argc, char* argv[]) {
    Arguments arguments;

    for (int i = 1; i < argc; ++i) {
        if (strncmp(argv[i], "-t", 2) == 0 || strncmp(argv[i], "--tail", 6) == 0) {
            arguments.is_tail = true;
        } else if (strncmp(argv[i], "-l", 2) == 0) {
            ++i;
            arguments.number_of_lines = atoi(argv[i]);
            if (arguments.number_of_lines == 0) {
                std::cerr << "ERROR: invalid number of lines" << std::endl;
            }
        } else if (strncmp(argv[i], "--lines=", 8) == 0) {
            arguments.number_of_lines = atoi(argv[i] + strlen("--lines="));
            if (arguments.number_of_lines == 0) {
                std::cerr << "ERROR: invalid number of lines" << std::endl;
            }
        } else if (strncmp(argv[i], "-d", 2) == 0) {
            if (strlen(argv[++i]) == 1) {
                arguments.delimiter = argv[i][0];
            } else {
                std::cerr << "ERROR: invalid delimiter" << std::endl;
            }
        } else if (strncmp(argv[i], "--delimiter=", 12) == 0) {
            if (strlen(argv[i]) == 13) {
                arguments.delimiter = argv[i][12];
            } else {
                std::cerr << "ERROR: invalid delimiter" << std::endl;
            }
        } else {
            arguments.kFilename = argv[i];
        }
    }

    return arguments;
}

void PrintFile(Arguments& arguments) {
    std::ifstream file(arguments.kFilename);
    if (!file.is_open()) {
        std::cerr << "ERROR: file opening error" << std::endl;

        return;
    }
    char reading_char;
    int counter = 0;

    if (arguments.number_of_lines == -1) {
            while (file.get(reading_char)) {
                std::cout << reading_char;
            }
    } else if (arguments.is_tail == false) {
        while (file.get(reading_char)) {
            if (reading_char == arguments.delimiter) {
                ++counter;
            }
            if (counter >= arguments.number_of_lines) {
                break;
            }
            std::cout << reading_char;
        }
        std::cout << arguments.delimiter;
    } else if (arguments.is_tail == true) {
        int counter_of_lines = 1;
        while (file.get(reading_char)) {
            if (reading_char == arguments.delimiter) {
                ++counter_of_lines;
            }
        }
        file.clear();
        file.seekg(0);
        while (counter < (counter_of_lines - arguments.number_of_lines)) {
            file.get(reading_char);
            if (reading_char == arguments.delimiter) {
                ++counter;
            }
        }
        while (file.get(reading_char)) {
            std::cout << reading_char;
        }
    }

    file.close();
}

int main(int argc, char* argv[]) {
    Arguments arguments = ParseArguments (argc, argv);
    PrintFile (arguments);

    return EXIT_SUCCESS;
}