#include "Pascal--/lexical/Lexer.hpp"

#include <cctype>
#include "Pascal--/lexical/TokenType.hpp"
#include "Pascal--/lexical/SymbolTable.hpp"
#include "Pascal--/util/File.hpp"
#include "Pascal--/util/exception.hpp"

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

const std::vector<Lexeme>& Lexer::scan_file(const char* filename) {
    Lexeme lexeme;
    m_file.open(filename);

    while (!m_file.is_at_EOF()) {
        lexeme = make_lexeme();
        m_lexemes.push_back(std::move(lexeme));
    }

    m_file.close();

    return m_lexemes;
}

static inline bool is_octal(char c) {
    return ('0' <= c && c <= '7');
}

static inline bool is_hex(char c) {
    return (std::isdigit(c) || ('A' <= c && c <= 'F'));
}

Lexeme Lexer::make_lexeme() {
    Lexeme lexeme;
    char c = m_file.peek();
    enum State state = STATE_INITIAL;

    while (state != STATE_FINAL) {
        switch (state) {
            case STATE_INITIAL:
                // whitespaces are skipped
                if (std::isspace(c)) {
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

                // check if 'c' is a punctuation
                } else if (std::ispunct(c)) {
                    lexeme.token += (char)c;
                    lexeme.line = m_file.line();
                    lexeme.column = m_file.column();
                    lexeme.type = SymbolTable::find(lexeme.token);
                    c = m_file.advance();
                    if (lexeme.type == TT_INVALID) {
                        throw CompilerError(
                            "invalid token",
                            lexeme.line, lexeme.column
                        );
                    }
                    state = STATE_FINAL;

                // end of file or invalid character
                } else {
                    if (c == '\0') {
                        lexeme.line = m_file.line();
                        lexeme.column = m_file.column();
                        lexeme.type = TT_EOF;
                        state = STATE_FINAL;
                    } else {
                        throw CompilerError(
                            "invalid token",
                            lexeme.line, lexeme.column
                        );
                    }
                }
                break;

            case STATE_ALNUM:
                if (std::isalnum(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_ALNUM;
                } else {
                    // check if the token is in SymbolTable. if it doesn't, it's
                    // a variable name
                    enum TokenType symbol = SymbolTable::find(lexeme.token);
                    lexeme.type = (symbol != TT_INVALID) ? symbol : TT_IDENT;
                    state = STATE_FINAL;
                }
                break;

            case STATE_ZERO:
                if (is_octal(c)) {
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
                } else if ('8' <= c && c <= '9') {
                    throw CompilerError(
                        "8 and 9 aren't octal digits",
                        lexeme.line, lexeme.column
                    );
                } else if (std::isalpha(c)) {
                    throw CompilerError(
                        "unexpected alphabetical character",
                        lexeme.line, lexeme.column
                    );
                } else {
                    lexeme.type = TT_LITERAL_DEC;
                    state = STATE_FINAL;
                }
                break;

            case STATE_OCTAL:
                if (is_octal(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_OCTAL;
                } else if (std::isalpha(c)) {
                    throw CompilerError(
                        "unexpected alphabetical character",
                        lexeme.line, lexeme.column
                    );
                } else if ('8' <= c && c <= '9') {
                    throw CompilerError(
                        "8 and 9 aren't octal digits",
                        lexeme.line, lexeme.column
                    );
                } else {
                    lexeme.type = TT_LITERAL_OCT;
                    state = STATE_FINAL;
                }
                break;

            case STATE_HEX:
                if (is_hex(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_HEX;
                } else if ('a' <= c && c <= 'f') {
                    throw CompilerError(
                        "hexadecimals must use upper case letters",
                        lexeme.line, lexeme.column
                    );
                } else if (std::isalpha(c)) {
                    throw CompilerError(
                        "unexpected alphabetical character",
                        lexeme.line, lexeme.column
                    );
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
                    throw CompilerError(
                        "unexpected alphabetical character",
                        lexeme.line, lexeme.column
                    );
                } else {
                    lexeme.type = TT_LITERAL_DEC;
                    state = STATE_FINAL;
                }
                break;

            case STATE_REAL:
                if (std::isdigit(c)) {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_REAL;
                } else if (std::isalpha(c)) {
                    throw CompilerError(
                        "unexpected alphabetical character",
                        lexeme.line, lexeme.column
                    );
                } else if (c == '.') {
                    throw CompilerError(
                        "real number with more than one period",
                        lexeme.line, lexeme.column
                    );
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
                    lexeme.type = TT_EOF;
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
                    throw CompilerError(
                        "end of file before closing multi-line comment",
                        lexeme.line, lexeme.column
                    );
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
                    lexeme.type = TT_LEQ;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else if (c == '>') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_NEQ;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_LSS;
                    state = STATE_FINAL;
                }
                break;

            case STATE_GREATER_THAN:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_GEQ;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_GTR;
                    state = STATE_FINAL;
                }
                break;

            case STATE_EQUAL:
                if (c == '=') {
                    lexeme.token += (char)c;
                    lexeme.type = TT_EQL;
                    c = m_file.advance();
                    state = STATE_FINAL;
                } else {
                    lexeme.type = TT_EQL;
                    state = STATE_FINAL;
                }
                break;

            case STATE_STRING:
                if (c == '\n') {
                    throw CompilerError(
                        "new line while trying to tokenize string literal",
                        lexeme.line, lexeme.column
                    );
                } else if (!lexeme.token.empty() && lexeme.token.back() == '\\') {
                    switch (c) {
                        case 'n':  lexeme.token.back() = '\n'; break;
                        case 't':  lexeme.token.back() = '\t'; break;
                        case 'r':  lexeme.token.back() = '\r'; break;
                        case '\\': lexeme.token.back() = '\\'; break;
                        case '\"': lexeme.token.back() = '\"'; break;
                        default:   throw CompilerError(
                            "not defined escape code",
                            lexeme.line, lexeme.column
                        );
                    }
                    c = m_file.advance();
                    state = STATE_STRING;
                } else if (c != '\"') {
                    lexeme.token += (char)c;
                    c = m_file.advance();
                    state = STATE_STRING;
                } else {
                    lexeme.type = TT_LITERAL_STR;
                    c = m_file.advance();
                    state = STATE_FINAL;
                }
                break;

            default:
                throw CompilerError(
                    "invalid '" + std::to_string(state) + "' state number in lexer", 
                    lexeme.line, lexeme.column
                );
        }
    }

    return lexeme;
}
