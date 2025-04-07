#include <iostream>
#include <string>
#include <cctype>
#include "Lexer.hpp"
#include "TokenType.hpp"

enum State {
	// Intermediate states
	STATE_INITIAL,
	STATE_LINE_COMMENT1,
	STATE_LINE_COMMENT2,
	STATE_BLOCK_COMMENT,
	STATE_LOWER,
	STATE_GREATER_EQUAL,
	STATE_ALNUM,
	STATE_DIGIT,
	STATE_REAL,
	STATE_STRING,

	// All states after this are final
	STATE_FINAL,
	STATE_FINAL_POSSIBLE_KEYWORD,

	// Final states
	// STATE_WORD_FINAL,
	// STATE_LINE_COMMENT_FINAL,
	// STATE_BLOCK_COMMENT_FINAL,
	// STATE_INTEGER_FINAL,
	// STATE_REAL_FINAL,
	// STATE_STRING_FINAL,
};

Lexer::Lexer(const char* filename) : m_line(1), m_column(1) {
	m_input = fopen(filename, "r");
	if (!m_input) {
		throw std::string("Unable to open file");
	}
}

Lexer::~Lexer() {
	if (m_input != nullptr) {
		fclose(m_input);
	}
}

Lexeme Lexer::nextToken() {
	State state;
	Lexeme lexeme;

	state = STATE_INITIAL;

	while (state < STATE_FINAL) {
		int c = fgetc(m_input);

		std::cout << "[" << state << ", " << c
		          << " ('" << (char) c << "')]" << std::endl;

		switch (state) {
			case STATE_INITIAL:
				if (c == ' ' || c == '\t' || c == '\r') {
					state = STATE_INITIAL;
				} else if (c == '\n') {
					m_line++;
					m_column = 1;
					state = STATE_INITIAL;
				} else if (c == '/') {
					state = STATE_LINE_COMMENT1;
				} else if (c == '{') {
					state = STATE_BLOCK_COMMENT;
				} else if (c == '<') {
					lexeme.token += (char)c;
					state = STATE_LOWER;
				} else if (c == '>' || c == '=') {
					lexeme.token += (char)c;
					state = STATE_GREATER_EQUAL;
				} else if (c == '_' || isalpha(c)) {
					lexeme.token += (char)c;
					state = STATE_ALNUM;
				} else if (isdigit(c)) {
					lexeme.token += (char)c;
					state = STATE_DIGIT;
				} else if (c == '\"') {
					state = STATE_STRING;
				} else {
					if (c == EOF) {
						lexeme.type = TT_END_OF_FILE;
						state = STATE_FINAL;
					} else {
						lexeme.token += (char)c;
						lexeme.type = TT_INVALID_TOKEN;
						state = STATE_FINAL;
					}
				}
				break;

			case STATE_LINE_COMMENT1:
				if (c == '/') {
					state = STATE_LINE_COMMENT2;
				} else {
					lexeme.token += (char)c;
					lexeme.type = TT_INVALID_TOKEN;
					state = STATE_FINAL;
				}
				break;

			case STATE_LINE_COMMENT2:
				if (c == '\n' || c == EOF) {
					lexeme.token = TT_TYPE_STRING;
					state = STATE_FINAL;
				} else {
					lexeme.token += (char)c;
					state = STATE_LINE_COMMENT2;
				}
				break;

			case STATE_LOWER:
				if (c == '=' || c == '>') {
					lexeme.token += (char)c;
					state = STATE_FINAL_POSSIBLE_KEYWORD;
				} else {
					if (c != EOF) {
						ungetc(c, m_input);
					}
					state = STATE_FINAL;
				}
				break;

			case STATE_GREATER_EQUAL:
				if (c == '=') {
					lexeme.token += (char)c;
					state = STATE_FINAL_POSSIBLE_KEYWORD;
				} else {
					if (c != EOF) {
						ungetc(c, m_input);
					}
					state = STATE_FINAL;
				}
				break;

			case STATE_ALNUM:
				if (c == '_' || isalnum(c)) {
					lexeme.token += (char)c;
					state = STATE_ALNUM;
				} else {
					if (c != EOF) {
						ungetc(c, m_input);
					}
					state = STATE_FINAL;
				}
				break;

			case STATE_DIGIT:
				if (isdigit(c)) {
					lexeme.token += (char)c;
					state = STATE_DIGIT;
				} else if (c == '.') {
					lexeme.token += (char)c;
					state = STATE_REAL;
				} else {
					if (c != EOF) {
						ungetc(c, m_input);
					}
					lexeme.type = TT_TYPE_INTEGER;
					state = STATE_FINAL;
				}
				break;

			case STATE_REAL:
				if (isdigit(c)) {
					lexeme.token += (char)c;
					state = STATE_REAL;
				} else {
					if (c != EOF) {
						ungetc(c, m_input);
					}
					lexeme.type = TT_TYPE_REAL;
					state = STATE_FINAL;
				}
				break;

			case STATE_STRING:
				if (c != '\"') {
					lexeme.token += (char)c;
				} else {
					lexeme.type = TT_LITERAL_STRING;
					state = STATE_FINAL;
				}
				break;

			case STATE_BLOCK_COMMENT:
				if (c == '}') {
					lexeme.token = TT_TYPE_STRING;
					state = STATE_FINAL;
				} else if (c == EOF) {
					lexeme.type = TT_UNEXPECTED_EOF;
					state = STATE_FINAL;
				}
				break;

			default:
				throw std::string("invalid state: " + std::to_string(state));
		}

		m_column++;
	}

	if (state == STATE_FINAL_POSSIBLE_KEYWORD) {
		lexeme.type = m_st.find(lexeme.token);
	}

	return lexeme;
}
