#include "SymbolTable.hpp"

const std::unordered_map<std::string, enum TokenType> SymbolTable::ms_symbols = {
    // Arithmetic operators
    {"+", TT_ADD},
    {"-", TT_SUB},
    {"*", TT_MUL},
    {"/", TT_DIV},
    {"mod", TT_MOD},
    {"div", TT_DIVINT},

	// Logical, relational operators and assignments
    {"or", TT_OR},
    {"and", TT_AND},
    {"not", TT_NOT},
    {"==", TT_EQUAL},
    {"<>", TT_DIFFERENCE},
    {">", TT_GREATER},
    {">=", TT_GREATER_EQUAL},
    {"<", TT_LOWER},
    {"<=", TT_LOWER_EQUAL},
    {":=", TT_ASSIGN},

    // Symbols
    {";", TT_SEMICOLON},
    {",", TT_COMMA},
    {".", TT_PERIOD},
    {":", TT_COLON},
    {"(", TT_OPEN_PARENTHESES},
    {")", TT_CLOSE_PARENTHESES},
    {"\"", TT_QUOTES},

    // Keywords
    {"program", TT_PROGRAM},
    {"var", TT_VAR},
    {"integer", TT_TYPE_INTEGER},
    {"real", TT_TYPE_REAL},
    {"string", TT_TYPE_STRING},
    {"begin", TT_BEGIN},
    {"end", TT_END},
    {"for", TT_FOR},
    {"to", TT_TO},
    {"while", TT_WHILE},
    {"do", TT_DO},
    {"break", TT_BREAK},
    {"continue", TT_CONTINUE},
    {"if", TT_IF},
    {"else", TT_ELSE},
    {"then", TT_THEN},
    {"write", TT_WRITE},
    {"writeln", TT_WRITELN},
    {"read", TT_READ},
    {"readln", TT_READLN}
};

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::contains(const std::string& token) const {
	return ms_symbols.find(token) != ms_symbols.end();
}

enum TokenType SymbolTable::find(const std::string& token) const {
    auto it = ms_symbols.find(token);
    return (it != ms_symbols.end()) ? it->second : TT_VAR_NAME;
}
