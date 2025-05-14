#include "Pascal--/lexical/SymbolTable.hpp"

#include <unordered_map>

namespace SymbolTable {

static const std::unordered_map<std::string, enum TokenType> s_symbols = {
    // Arithmetic operators
    {"+", TT_ADD},
    {"-", TT_SUB},
    {"*", TT_MUL},
    {"/", TT_DIV},
    {"mod", TT_MOD},
    {"div", TT_FLOORDIV},

    // Logical, relational operators and assignments
    {"or", TT_OR},
    {"and", TT_AND},
    {"not", TT_NOT},
    {"=", TT_EQL},
    {"==", TT_EQL},
    {"<>", TT_NEQ},
    {">", TT_GTR},
    {">=", TT_GEQ},
    {"<", TT_LSS},
    {"<=", TT_LEQ},
    {":=", TT_ASSIGN},

    // Symbols
    {";", TT_SEMICOLON},
    {",", TT_COMMA},
    {".", TT_PERIOD},
    {":", TT_COLON},
    {"(", TT_LPAREN},
    {")", TT_RPAREN},
    {"\"", TT_QUOTES},

    // Keywords
    {"program", TT_PROGRAM},
    {"var", TT_VAR},
    {"integer", TT_TYPE_INT},
    {"real", TT_TYPE_REAL},
    {"string", TT_TYPE_STR},
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

bool contains(const std::string& token) {
    return s_symbols.find(token) != s_symbols.end();
}

enum TokenType find(const std::string& token) {
    auto it = s_symbols.find(token);
    return (it != s_symbols.end()) ? it->second : TT_INVALID;
}

} // namespace SymbolTable
