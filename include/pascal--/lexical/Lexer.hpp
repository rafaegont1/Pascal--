#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include "pascal--/lexical/Lexeme.hpp"
#include "pascal--/lexical/SymbolTable.hpp"
#include "pascal--/util/File.hpp"

class Lexer {
public:
    Lexer();
    virtual ~Lexer();

    const std::vector<Lexeme>& scanFile(const char* filename);

    const std::vector<Lexeme>& lexemes() { return m_lexemes; }

private:
    Lexeme makeLexeme();
    std::string lexicalError(
        const std::string& message, const std::string& token
    );

    File m_file;
    std::vector<Lexeme> m_lexemes;
};

#endif // LEXER_HPP
