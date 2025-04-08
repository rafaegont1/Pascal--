#include <iostream>
#include <cstdlib>
#include <vector>

#include "Lexer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [Pascal-- program]" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        Lexeme lexeme;
        Lexer lexer(argv[1]);

        do {
            lexeme = lexer.nextToken();
            std::cout << "Token: " << lexeme.str()
                << " | line: " << std::to_string(lexeme.line)
                << " | column: " << std::to_string(lexeme.column) << std::endl;
        } while (lexeme.type != TT_END_OF_FILE);

    } catch (const std::string& error) {
        std::cerr << "error: " << error << std::endl;
    }

    return 0;
}
