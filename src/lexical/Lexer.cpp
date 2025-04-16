#include "pascal--/lexical/Lexer.hpp"

#include <iostream>
#include <string>
#include "pascal--/lexical/TokenType.hpp"

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
    STATE_COLON,
    STATE_EQUAL,
    STATE_FINAL
};

Lexer::Lexer(const char* filename) : m_line{1}, m_column{1} {
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

    lexeme.line = m_line;
    lexeme.column = m_column;

    while (state != STATE_FINAL) {
        int c = fgetc(m_input);

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

                } else if (c == '0') {
                    state = STATE_ZERO;

                } else if ('1' <= c && c <= '9') {
                    lexeme.token += (char)c;
                    state = STATE_DIGIT;

                } else if (
                    c == '+' || c == '-' || c == '*' || c == ';' || c == ',' ||
                    c == '.' || c == '(' || c == ')'
                ) {
                    lexeme.token += (char)c;
                    lexeme.type = SymbolTable::find(lexeme.token);
                    state = STATE_FINAL;

                } else if (c == '/') {
                    lexeme.token += (char)c;
                    state = STATE_SLASH;

                } else if (c == '{') {
                    state = STATE_COMMENT_BLOCK;

                } else if (c == ':') {
                    state = STATE_COLON;

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
                    ungetChar(c);
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
                    ungetChar(c);
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
                    ungetChar(c);
                    lexeme.type = TT_GREATER;
                    state = STATE_FINAL;
                }
                break;

            case STATE_ALNUM:
                if (isalnum(c)) {
                    lexeme.token += (char)c;
                    state = STATE_ALNUM;
                } else {
                    ungetChar(c);
                    lexeme.type = SymbolTable::find(lexeme.token);
                    state = STATE_FINAL;
                }
                break;

            case STATE_ZERO:
                if (isdigit(c)) {
                    lexeme.token += (char)c;
                    state = STATE_OCTAL;
                } else if (c == 'x') {
                    state = STATE_HEX;
                }
                break;

            case STATE_OCTAL:
                if ('0' <= c && c <= '7') {
                    lexeme.token += (char)c;
                } else if (isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                } else {
                    ungetChar(c);
                    lexeme.type = TT_LITERAL_OCTAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_HEX:
                if (isdigit(c) || ('A' <= c && c <= 'F')) {
                    lexeme.token += (char)c;
                } else if (isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                } else {
                    ungetChar(c);
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
                } else if (isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                } else {
                    ungetChar(c);
                    lexeme.type = TT_LITERAL_DECIMAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_REAL:
                if (isdigit(c)) {
                    lexeme.token += (char)c;
                    state = STATE_REAL;
                } else if (isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                } else {
                    ungetChar(c);
                    lexeme.type = TT_LITERAL_REAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_STRING:
                if (c == '\n') {
                    // lexeme.token += (char)c; // TODO: será que eu coloco o '\n' aqui? ele vai dar quebra de linha no print
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                } else if (lexeme.token.back() == '\\') {
                    switch (c) {
                        case 'n':  lexeme.token.back() = '\n'; break;
                        case 't':  lexeme.token.back() = '\t'; break;
                        case 'r':  lexeme.token.back() = '\r'; break;
                        case '\\': lexeme.token.back() = '\\'; break;
                        case '\"': lexeme.token.back() = '\"'; break;
                        default:   lexeme.type = TT_INVALID_TOKEN; break;
                    }
                    state = STATE_STRING;
                } else if (c != '\"') {
                    lexeme.token += (char)c;
                    state = STATE_STRING;
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

            case STATE_COLON:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_ASSIGN;
                    state = STATE_FINAL;
                } else {
                    ungetChar(c);
                    lexeme.type = TT_COLON;
                    state = STATE_FINAL;
                }
                break;

            case STATE_EQUAL:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_EQUAL;
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_EQUAL;
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

void Lexer::newLine(Lexeme& lexeme) {
    m_line += 1;
    m_column = 0;

    lexeme.line = m_line;
    lexeme.column = m_column + 1;
}

void Lexer::ungetChar(int c) {
    if (c != EOF) {
        ungetc(c, m_input);
    }
    m_column--;
}
