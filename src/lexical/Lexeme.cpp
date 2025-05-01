#include "Pascal--/lexical/Lexeme.hpp"

Lexeme::Lexeme() : token(""), type(TT_EOF), line(0), column(0) {
}

Lexeme::Lexeme(
    const std::string& token, enum TokenType type, int line, int column
) : token(token), type(type), line(line), column(column) {
}

Lexeme::~Lexeme() {
}

std::string Lexeme::str() const {
    std::string line_ = std::to_string(line);
    std::string column_ = std::to_string(column);

    return "(\"" + token + "\", " + tt2str(type) + ", " +
           line_ + ", " + column_ + ")";
}
