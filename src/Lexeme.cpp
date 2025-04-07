#include "Lexeme.hpp"

Lexeme::Lexeme() : token{""}, type{TT_END_OF_FILE} {
}

Lexeme::~Lexeme() {
}

std::string Lexeme::str() const {
	return "(\"" + token + "\", " + tt2str(type) + ")";
}
