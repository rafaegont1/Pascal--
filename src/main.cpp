#include <iostream>

#include "Pascal--/lexical/Lexer.hpp"
#include "Pascal--/syntactic/Parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <program>" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        Lexer lexer;
        const std::vector<Lexeme>& lexemes = lexer.scan_file(argv[1]);
        Parser parser(lexemes);

        // print lexemes found within lexer
        std::cout << "LEXEME LIST\n"
                  << "-----------\n";
        for (const auto& lexeme : lexemes) {
            std::cout << lexeme.str() << '\n';
        }
        std::cout << std::endl;

        parser.start();
        std::cout
            << "As análises léxica e sintática não encontraram nenhum erro!"
            << std::endl;

    } catch (const std::string& error) {
        std::cerr << "Error: " << error << std::endl;
    }

    return 0;
}
