#include <iostream>
#include <string>
#include <cctype>
#include "Lexer.hpp"
#include "TokenType.hpp"

enum State {
    STATE_INITIAL,
    STATE_SLASH,
    STATE_COMMENT_LINE,
    STATE_COMMENT_BLOCK,
    STATE_LESS_THAN,
    STATE_GREATER_THAN,
    STATE_ALNUM,
    STATE_DIGIT,
    STATE_REAL,
    STATE_STRING,
    STATE_ZERO,
    STATE_OCTAL,
    STATE_HEX,
    STATE_ASSIGN,
    STATE_EQUAL,
    STATE_FINAL
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
    State state = STATE_INITIAL;
    Lexeme lexeme;

    lexeme.setPosition(m_line, m_column);

    while (state != STATE_FINAL) {
        int c = fgetc(m_input);

        // std::cout << ((char)c != '\n' ? (char)c : '|') << ' ' << std::to_string(m_line) << ' ' << std::to_string(m_column) << std::endl; // rascunho

        switch (state) {
            case STATE_INITIAL:
                if (c == ' ' || c == '\t' || c == '\r') {
                    lexeme.column++;
                    state = STATE_INITIAL;

                } else if (c == '\n') {
                    newLine(lexeme);
                    state = STATE_INITIAL;

                } else if (isalpha(c)) {
                    lexeme.token += (char)c;
                    state = STATE_ALNUM;

                } else if (isdigit(c)) {
                    lexeme.token += (char)c;
                    state = STATE_DIGIT;

                } else if (c == '+' || c == '-' || c == '*' || c == ';' || c == ','
                || c == '.' || c == ':' || c == '(' || c == ')') {
                    lexeme.token += (char)c;
                    lexeme.type = m_symbolTable.find(lexeme.token);
                    state = STATE_FINAL;

                } else if (c == '/') {
                    lexeme.token += (char)c;
                    state = STATE_SLASH;

                } else if (c == '{') {
                    state = STATE_COMMENT_BLOCK;

                } else if (c == ':') {
                    state = STATE_ASSIGN;

                } else if (c == '<') {
                    lexeme.token += (char)c;
                    state = STATE_LESS_THAN;

                } else if (c == '>') {
                    lexeme.token += (char)c;
                    state = STATE_GREATER_THAN;

                } else if (c == '=') {
                    lexeme.token += (char)c;
                    state = STATE_EQUAL;

                } else if (c == '\"') {
                    state = STATE_STRING;

                } else if (c == '0') {
                    state = STATE_ZERO;

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

            case STATE_SLASH:
                if (c == '/') {
                    lexeme.token.pop_back();
                    state = STATE_COMMENT_LINE;
                } else {
                    rollbackFile(c);
                    lexeme.type = TT_DIV;
                    state = STATE_FINAL;
                }
                break;

            case STATE_COMMENT_LINE:
                if (c == '\n') {
                    newLine(lexeme);
                    state = STATE_INITIAL;
                } else if (c == EOF) {
                    lexeme.type = TT_END_OF_FILE;
                    state = STATE_FINAL;
                } else {
                    state = STATE_COMMENT_LINE;
                }
                break;

            case STATE_LESS_THAN:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_LOWER_EQUAL;
                    state = STATE_FINAL;
                } else if (c == '>') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_DIFFERENCE;
                    state = STATE_FINAL;
                } else {
                    rollbackFile(c);
                    lexeme.type = TT_LOWER;
                    state = STATE_FINAL;
                }
                break;

            case STATE_GREATER_THAN:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_GREATER_EQUAL;
                    state = STATE_FINAL;
                } else {
                    rollbackFile(c);
                    lexeme.type = TT_GREATER;
                    state = STATE_FINAL;
                }
                break;

            case STATE_ALNUM:
                if (isalnum(c)) {
                    lexeme.token += (char)c;
                    state = STATE_ALNUM;
                } else {
                    rollbackFile(c);
                    lexeme.type = m_symbolTable.find(lexeme.token);
                    state = STATE_FINAL;
                }
                break;

            case STATE_ZERO:
                if (isdigit(c)) {
                    state = STATE_OCTAL;
                    lexeme.token += (char)c;
                } else if (c == 'x') {
                    state = STATE_HEX;
                }
                break;

            case STATE_OCTAL:
                if (isdigit(c)) {
                    lexeme.token += (char)c;
                } else {
                    rollbackFile(c);
                    lexeme.type = TT_LITERAL_OCTAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_HEX:
                if (isdigit(c)) {
                    lexeme.token += (char)c;
                } else {
                    rollbackFile(c);
                    lexeme.type = TT_LITERAL_HEX;
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
                    rollbackFile(c);
                    lexeme.type = TT_LITERAL_DECIMAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_REAL:
                if (isdigit(c)) {
                    lexeme.token += (char)c;
                    state = STATE_REAL;
                } else {
                    rollbackFile(c);
                    lexeme.type = TT_LITERAL_REAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_STRING:
                if (c == '\n') {
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                } else if (c != '\"') {
                    lexeme.token += (char)c;
                } else {
                    lexeme.type = TT_LITERAL_STRING;
                    state = STATE_FINAL;
                }
                break;

            case STATE_COMMENT_BLOCK:
                if (c == '}') {
                    state = STATE_INITIAL;
                } else if (c == EOF) {
                    lexeme.type = TT_UNEXPECTED_EOF;
                    state = STATE_FINAL;
                } else {
                    state = STATE_COMMENT_BLOCK;
                }
                break;

            case STATE_ASSIGN:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_ASSIGN;
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                }
                break;

            case STATE_EQUAL:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_EQUAL;
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                }
                break;

            default:
                throw std::string("invalid state: " + std::to_string(state));
        }

        m_column++;
    }

    return lexeme;
}

// Updates line and column of Lexer and Lexeme when there's a new line '\n'
void Lexer::newLine(Lexeme& lexeme) {
    m_line += 1;
    m_column = 0;

    lexeme.setPosition(m_line, m_column + 1);
}

void Lexer::rollbackFile(int c) {
    if (c != EOF) {
        ungetc(c, m_input);
    }
    m_column--;
}
