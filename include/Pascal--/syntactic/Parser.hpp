#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"
#include "Pascal--/lexical/TokenType.hpp"

class Parser {
public:
    Parser(const std::vector<Lexeme>& lexemes);
    virtual ~Parser();

    void start();

private:
    const std::vector<Lexeme>& m_lexemes;
    int m_pos = 0;

    inline enum TokenType current_token_type();
    void consume(enum TokenType type);

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
    // ------------------------------------
    //  program statements
    // ------------------------------------
    void proc_block();
    void proc_stmtList();
    void proc_stmt();
    // ---------------------------
    //  statement descriptions
    // ---------------------------
    // for command
    void proc_forStmt();
    void proc_endFor();
    // IO commands
    void proc_ioStmt();
    void proc_outList();
    void proc_restoOutList();
    void proc_out();
    void proc_whileStmt();
    void proc_ifStmt();
    void proc_elsePart();
    // ------------------------------
    //  expressions
    // ------------------------------
    void proc_atrib();
    void proc_expr();
    void proc_or();
    void proc_restoOr();
    void proc_and();
    void proc_restoAnd();
    void proc_not();
    void proc_rel();
    void proc_restoRel();
    void proc_add();
    void proc_restoAdd();
    void proc_mult();
    void proc_restoMult();
    void proc_uno();
    void proc_fator();
};

#endif // PARSER_HPP
