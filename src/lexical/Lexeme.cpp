#include "Pascal--/lexical/Lexeme.hpp"
#include "Pascal--/lexical/TokenType.hpp"

#include <format>

Lexeme::Lexeme() : token(""), type(TT_EOF), line(0), column(0) {
}

Lexeme::Lexeme(
    const std::string& token,
    enum TokenType type,
    uint16_t line, uint16_t column
) : token(token), type(type), line(line), column(column) {
}

Lexeme::~Lexeme() {
}

std::string Lexeme::str() const {
    return std::format(
        "(\"{}\", {}, {}, {})",
        token, tt2str(type), line, column
    );
}
