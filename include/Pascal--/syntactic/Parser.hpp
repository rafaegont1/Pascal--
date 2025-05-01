#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"

class Parser {
public:
    Parser(const std::vector<Lexeme>& lexemes);
    virtual ~Parser();

    void start();

private:
    std::vector<Lexeme>::const_iterator m_lexeme;

    void advance();
    void eat(enum TokenType type);

    // ------------------------------------
    //  main function
    // ------------------------------------
    void proc_function();
    // ------------------------------------
    //  variable declarations
    // ------------------------------------
    void proc_declarations();
    void proc_declaration();
    void proc_listIdent();
    void proc_restIdentList();
    void proc_restDeclaration();
    void proc_type();
};

#endif // PARSER_HPP
