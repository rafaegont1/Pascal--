#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"
#include "Pascal--/util/File.hpp"

class Lexer {
public:
    Lexer();
    virtual ~Lexer();

    const std::vector<Lexeme>& scan_file(const char* filename);
    const std::vector<Lexeme>& lexemes() { return m_lexemes; }

private:
    Lexeme make_lexeme();

    std::vector<Lexeme> m_lexemes;
    File m_file;
};

#endif // LEXER_HPP
