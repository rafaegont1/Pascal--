#include <iostream>

#include "pascal--/lexical/Lexer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <program>" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create a vector of lexemes and print them
    try {
        Lexer lexer;
        auto& lexemes = lexer.scan_file(argv[1]);

        for (const auto& lexeme : lexemes) {
            std::cout << lexeme.str() << std::endl;
        }

    } catch (const std::string& error) {
        std::cerr << "Error: " << error << std::endl;
    }

    return 0;
}
