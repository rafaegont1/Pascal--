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
    {"program", TT_PROGRAMSYM},
    {"var", TT_VARSYM},
    {"integer", TT_TYPE_INTEGER},
    {"real", TT_TYPE_REAL},
    {"string", TT_TYPE_STRING},
    {"begin", TT_BEGINSYM},
    {"end", TT_ENDSYM},
    {"for", TT_FORSYM},
    {"to", TT_TOSYM},
    {"while", TT_WHILESYM},
    {"do", TT_DOSYM},
    {"break", TT_BREAKSYM},
    {"continue", TT_CONTINUESYM},
    {"if", TT_IFSYM},
    {"else", TT_ELSESYM},
    {"then", TT_THENSYM},
    {"write", TT_WRITESYM},
    {"writeln", TT_WRITELNSYM},
    {"read", TT_READSYM},
    {"readln", TT_READLNSYM}
};

bool contains(const std::string& token) {
    return s_symbols.find(token) != s_symbols.end();
}

enum TokenType find(const std::string& token) {
    auto it = s_symbols.find(token);
    return (it != s_symbols.end()) ? it->second : TT_INVALID;
}

} // namespace SymbolTable
