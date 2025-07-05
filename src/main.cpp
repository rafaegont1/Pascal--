#include <iostream>

#include "Pascal--/lexical/Lexer.hpp"
#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/util/exception.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <program>" << std::endl;
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];

    try {
        Lexer lexer;
        const std::vector<Lexeme>& lexemes = lexer.scan_file(filename);
        Parser parser(lexemes);

        // print lexemes found within lexer
        std::cout << "LEXEME LIST" << std::endl;
        std::cout << "-----------" << std::endl;
        for (const auto& lexeme : lexemes) {
            std::cout << lexeme.str() << std::endl;
        }
        std::cout << "" << std::endl;

        parser.start();
        std::cout << "None errors were found!" << std::endl;

        // Print intermediate code
        std::cout << "" << std::endl;
        std::cout << "INTERMEDIATE CODE" << std::endl;
        std::cout << "=================" << std::endl;
        parser.printIntermediateCode();

    } catch (const LexicalError& e) {
        std::cout << "Lexical Error at " << e.line() << ":" << e.column()
            << ": " << e.what() << std::endl;
        exit(EXIT_FAILURE);

    } catch (const SyntaxError& e) {
        std::cout << "Syntax Error at " << e.line() << ":" << e.column()
            << ": " << e.what() << std::endl;
        exit(EXIT_FAILURE);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
