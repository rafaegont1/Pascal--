#include "Lexeme.hpp"

Lexeme::Lexeme() : token{""}, type{TT_END_OF_FILE} {
}

Lexeme::~Lexeme() {
}

void Lexeme::setPosition(int line, int column) {
    this->line = line;
    this->column = column;
}

std::string Lexeme::str() const {
    std::string line_ = std::to_string(line);
    std::string column_ = std::to_string(column);

    return "(" + line_ + ", " + column_
        + ", \"" + token + "\", " + tt2str(type) + ")";
}
