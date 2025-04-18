#include "pascal--/lexical/Lexeme.hpp"

Lexeme::Lexeme() : token(""), line(0), column(0), type(TT_END_OF_FILE) {
}

Lexeme::Lexeme(
    const std::string& token, int line, int column, enum TokenType type
) : token(token), line(line), column(column), type(type) {
}

Lexeme::~Lexeme() {
}

std::string Lexeme::str() const {
    std::string line_ = std::to_string(line);
    std::string column_ = std::to_string(column);

    return "(" + line_ + ", " + column_
        + ", \"" + token + "\", " + tt2str(type) + ")";
}
