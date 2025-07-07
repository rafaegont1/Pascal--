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

    // Step 1: Lexical Analysis
    Lexer lexer;
    try {
        lexer.scan_file(filename);
    } catch (const CompilerError& e) {
        std::cerr << "Lexical Error at " << e.line() << ":" << e.column()
            << ": " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    const std::vector<Lexeme>& lexemes = lexer.lexemes();

#ifdef VERBOSE
    // Print lexemes found within lexer
    Printer::printLexemes(lexemes);
#endif // VERBOSE

    // Step 2: Parsing and Code Generation
    Parser parser(lexemes);
    try {
        parser.start();
    } catch (const CompilerError& e) {
        std::cerr << "Syntax Error at " << e.line() << ":" << e.column()
            << ": " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef VERBOSE
    if (parser.hasTypeErrors()) {
        std::cerr << "Type errors found during parsing, but continuing to show generated commands..." << std::endl;
    } else {
        std::cout << "No errors were found!" << std::endl;
    }

    // Print variable types
    Printer::printVariableTypes(parser.getVariableTypes());

    // Print generated commands
    Printer::printCommands(parser.getCommands());
#endif // VERBOSE

    // Show type errors if any occurred
    if (parser.hasTypeErrors()) {
        Printer::printTypeErrors(parser.getTypeErrors());
        std::cerr << "Execution skipped due to type errors." << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef VERBOSE
    // Step 3: Execution using the simplified interpreter
    std::cout << "=========" << std::endl;
    std::cout << "EXECUTION" << std::endl;
    std::cout << "=========" << std::endl;
#endif // VERBOSE

    Interpreter interpreter;
    try {
        interpreter.execute(parser.getCommands(), parser.getVariableTypes());
    } catch (const std::runtime_error& e) {
        std::cerr << "Interpreter error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef VERBOSE
    // Print final state
    interpreter.printState();
#endif // VERBOSE

    return 0;
}
