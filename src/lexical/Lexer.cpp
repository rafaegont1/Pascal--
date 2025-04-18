#include "pascal--/lexical/Lexer.hpp"

#include <iostream>
#include <string>
#include "pascal--/lexical/TokenType.hpp"
#include "pascal--/util/File.hpp"

enum State {
    STATE_INITIAL,
    STATE_ALNUM,
    STATE_ZERO,
    STATE_OCTAL,
    STATE_HEX,
    STATE_DIGIT,
    STATE_REAL,
    STATE_SLASH,
    STATE_COMMENT_SINGLE_LINE,
    STATE_COMMENT_MULTI_LINE,
    STATE_COLON,
    STATE_LESS_THAN,
    STATE_GREATER_THAN,
    STATE_EQUAL,
    STATE_STRING,
    STATE_FINAL
};

Lexer::Lexer() {
}

Lexer::~Lexer() {
}

bool static inline isOctal(char c) {
    return ('0' <= c && c <= '7');
}

bool static inline isHex(char c) {
    return (std::isdigit(c) || ('A' <= c && c <= 'F'));
}

const std::vector<Lexeme>& Lexer::scanFile(const char* filename) {
    m_file.open(filename);

    while (!m_file.isAtEOF()) {
        Lexeme lexeme = makeLexeme();
        if (lexeme.type != TT_END_OF_FILE) {
            m_lexemes.push_back(std::move(lexeme));
        }
    }

    m_file.close();

    return m_lexemes;
}

Lexeme Lexer::makeLexeme() {
    Lexeme lexeme;
    char c = m_file.peek();
    State state = STATE_INITIAL;

    while (state != STATE_FINAL) {
        switch (state) {
            case STATE_INITIAL:
                // whitespaces are skipped
                if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                    c = m_file.advance();
                    state = STATE_INITIAL;

                // this can be either a variable name or a keyword
                } else if (std::isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_ALNUM;

                // this can be a octal, hexadecimal or a real number
                } else if (c == '0') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_ZERO;

                // this can be either a decimal or real number
                } else if ('1' <= c && c <= '9') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_DIGIT;

                // check if 'c' is one of those symbols
                } else if (
                    c == '+' || c == '-' || c == '*' || c == ';' || c == ',' ||
                    c == '.' || c == '(' || c == ')'
                ) {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    lexeme.type = SymbolTable::find(lexeme.token);
                    c = m_file.advance();
                    state = STATE_FINAL;

                // this can be either a division or a single-line comment
                } else if (c == '/') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_SLASH;

                // beginning of a multi-line comment
                } else if (c == '{') {
                    c = m_file.advance();
                    state = STATE_COMMENT_MULTI_LINE;

                // this can be either a colon or a assign (:=)
                } else if (c == ':') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_COLON;

                // can be less than (<), less or equal (<=) or difference (<>)
                } else if (c == '<') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_LESS_THAN;

                // this can be either a greater than (>) or greater or equal (>=)
                } else if (c == '>') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_GREATER_THAN;

                // this is a equal to relational operator
                // (both '=' and '==' are 'equal to')
                } else if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_EQUAL;

                // this is the start of a literal string. this token finishes
                // when another " is found. there must be a closing " before a
                // new line
                } else if (c == '\"') {
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    c = m_file.advance();
                    state = STATE_STRING;

                // end of file or invalid character
                } else {
                    if (c == '\0') {
                        lexeme.line = m_file.line();
                        lexeme.column = m_file.column();
                        lexeme.type = TT_END_OF_FILE;
                        state = STATE_FINAL;
                    } else {
                        lexeme.token += (char)c;
                        lexeme.line = m_file.line();
                        lexeme.column = m_file.column();
                        lexeme.type = TT_INVALID_TOKEN;
                        state = STATE_FINAL;
                    }
                }
                break;

            case STATE_ALNUM:
                if (std::isalnum(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_ALNUM;
                } else {
                    lexeme.type = SymbolTable::find(lexeme.token);
                    state = STATE_FINAL;
                }
                break;

            case STATE_ZERO:
                if (isOctal(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_OCTAL;
                } else if (c == 'x') {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_HEX;
                } else if (c == '.') {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_REAL;
                } else if (std::isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                    // THROW ERROR HERE
                } else {
                    lexeme.type = TT_LITERAL_DECIMAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_OCTAL:
                if (isOctal(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                } else if (std::isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                    // THROW ERROR HERE
                } else {
                    lexeme.type = TT_LITERAL_OCTAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_HEX:
                if (isHex(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                } else if (std::isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                    // THROW ERROR HERE
                } else {
                    lexeme.type = TT_LITERAL_HEX;
                    state = STATE_FINAL;
                }
                break;

            case STATE_DIGIT:
                if (std::isdigit(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_DIGIT;
                } else if (c == '.') {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_REAL;
                } else if (std::isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                    // THROW ERROR HERE
                } else {
                    lexeme.type = TT_LITERAL_DECIMAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_REAL:
                if (std::isdigit(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_REAL;
                } else if (std::isalpha(c)) {
                    lexeme.token += (char)c;
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                    // THROW ERROR HERE
                } else {
                    lexeme.token += '0';
                    lexeme.type = TT_LITERAL_REAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_SLASH:
                if (c == '/') {
                    lexeme.token.clear();
                    c = m_file.advance();
                    state = STATE_COMMENT_SINGLE_LINE;
                } else {
                    lexeme.type = TT_DIV;
                    state = STATE_FINAL;
                }
                break;

            case STATE_COMMENT_SINGLE_LINE:
                if (c == '\n') {
                    state = STATE_INITIAL;
                } else if (c == '\0') {
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    lexeme.type = TT_END_OF_FILE;
                    state = STATE_FINAL;
                } else {
                    c = m_file.advance();
                    state = STATE_COMMENT_SINGLE_LINE;
                }
                break;

            case STATE_COMMENT_MULTI_LINE:
                if (c == '}') {
                    c = m_file.advance();
                    state = STATE_INITIAL;
                } else if (c == '\0') {
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    lexeme.type = TT_UNEXPECTED_EOF;
                    state = STATE_FINAL;
                } else {
                    c = m_file.advance();
                    state = STATE_COMMENT_MULTI_LINE;
                }
                break;

            case STATE_COLON:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_ASSIGN;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_COLON;
                    state = STATE_FINAL;
                }
                break;

            case STATE_LESS_THAN:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_LOWER_EQUAL;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else if (c == '>') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_DIFFERENCE;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_LOWER;
                    state = STATE_FINAL;
                }
                break;

            case STATE_GREATER_THAN:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_GREATER_EQUAL;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_GREATER;
                    state = STATE_FINAL;
                }
                break;

            case STATE_EQUAL:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_EQUAL;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_EQUAL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_STRING:
                if (c == '\n') {
                    // lexeme.token += (char)c; // TODO: será que eu coloco o '\n' aqui? ele vai dar quebra de linha no print
                    lexeme.type = TT_INVALID_TOKEN;
                    state = STATE_FINAL;
                    // THROW ERROR HERE
                } else if (lexeme.token.back() == '\\') {
                    switch (c) {
                        case 'n':  lexeme.token.back() = '\n'; break;
                        case 't':  lexeme.token.back() = '\t'; break;
                        case 'r':  lexeme.token.back() = '\r'; break;
                        case '\\': lexeme.token.back() = '\\'; break;
                        case '\"': lexeme.token.back() = '\"'; break;
                        default:   lexeme.type = TT_INVALID_TOKEN; break;
                    }
                    c = m_file.advance();
                    state = STATE_STRING;
                } else if (c != '\"') {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_STRING;
                } else {
                    lexeme.type = TT_LITERAL_STRING;
                    c = m_file.advance();
                    state = STATE_FINAL;
                }
                break;

            default:
                throw std::string(
                    "lexical error -> invalid state number " +
                    std::to_string(state)
                );
        }
    }

    return lexeme;
}

std::string Lexer::lexicalError(
    const std::string& message, const std::string& token
) {
    return std::string("lexical error -> " + message +
        "\n\ttoken: " + token +
        "\n\tline: " + std::to_string(m_file.line()) +
        "\n\tcolumn: " + std::to_string(m_file.column())
    );
}
