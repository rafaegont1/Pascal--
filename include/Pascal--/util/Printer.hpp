#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include "Pascal--/interpreter/Command.hpp"
#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/lexical/Lexeme.hpp"

class Printer {
public:
    static void printLexemes(const std::vector<Lexeme>& lexemes);
    static void printCommands(const std::vector<Command>& commands);
    static void printVariableTypes(const std::unordered_map<std::string, VariableInfo>& variableTypes);
    static void printTypeErrors(const std::vector<std::string>& typeErrors);
    static std::string varTypeToString(VarType type);
};

#endif // PRINTER_HPP 