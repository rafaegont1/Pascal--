#include <print>

#include "Pascal--/lexical/Lexer.hpp"
#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/util/exception.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::println("Usage: {} <program>", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];

    try {
        Lexer lexer;
        const std::vector<Lexeme>& lexemes = lexer.scan_file(filename);
        Parser parser(lexemes);

        // print lexemes found within lexer
        std::println("LEXEME LIST");
        std::println("-----------");
        for (const auto& lexeme : lexemes) {
            std::println("{}", lexeme.str());
        }
        std::println("");

        parser.start();
        std::println("None errors were found!");

    } catch (const LexicalError& e) {
        std::println("Lexical Error at {}:{}: {}", e.line(), e.column(), e.what());
        exit(EXIT_FAILURE);

    } catch (const SyntaxError& e) {
        std::println("Syntax Error at {}:{}: {}", e.line(), e.column(), e.what());
        exit(EXIT_FAILURE);

    } catch (const std::exception& e) {
        std::println(stderr, "Error: {}", e.what());
        exit(EXIT_FAILURE);
    }

    return 0;
}
