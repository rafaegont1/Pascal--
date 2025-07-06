#include <iostream>

#include "Pascal--/lexical/Lexer.hpp"
#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/interpreter/Interpreter.hpp"
#include "Pascal--/util/exception.hpp"
#include "Pascal--/util/Printer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <program>" << std::endl;
        exit(EXIT_FAILURE);
    }

    const char* filename = argv[1];

    try {
        // Step 1: Lexical Analysis
        Lexer lexer;
        const std::vector<Lexeme>& lexemes = lexer.scan_file(filename);

        // Print lexemes found within lexer
        Printer::printLexemes(lexemes);

        // Step 2: Parsing and Code Generation
        Parser parser(lexemes);
        parser.start();

        if (parser.hasTypeErrors()) {
            std::cout << "Type errors found during parsing, but continuing to show generated commands..." << std::endl;
        } else {
            std::cout << "No errors were found!" << std::endl;
        }

        // Print variable types
        Printer::printVariableTypes(parser.getVariableTypes());

        // Print generated commands
        Printer::printCommands(parser.getCommands());

        // Show type errors if any occurred
        if (parser.hasTypeErrors()) {
            Printer::printTypeErrors(parser.getTypeErrors());
            std::cout << "Execution skipped due to type errors." << std::endl;
        } else {
            // Step 3: Execution using the simplified interpreter
            std::cout << "" << std::endl;
            std::cout << "EXECUTION" << std::endl;
            std::cout << "=========" << std::endl;

            Interpreter interpreter;
            interpreter.execute(parser.getCommands(), parser.getVariableTypes());

            // Print final state
            interpreter.printState();
        }

    } catch (const LexicalError& e) {
        std::cout << "Lexical Error at " << e.line() << ":" << e.column() << ": " << e.what() << std::endl;
        exit(EXIT_FAILURE);

    } catch (const SyntaxError& e) {
        std::cout << "Syntax Error at " << e.line() << ":" << e.column() << ": " << e.what() << std::endl;
        exit(EXIT_FAILURE);

    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
