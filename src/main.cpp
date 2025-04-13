#include <iostream>

#include "pascal--/lexical/Lexer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [program]" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        Lexer lexer(argv[1]);
        Lexeme lexeme;

        do {
            lexeme = lexer.nextToken();
            std::cout << "Token: " << lexeme.str() << std::endl;
        } while (lexeme.type != TT_END_OF_FILE);

    } catch (const std::string& error) {
        std::cerr << "error: " << error << std::endl;
    }

    return 0;
}
