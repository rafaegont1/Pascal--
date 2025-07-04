#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"
#include "Pascal--/lexical/TokenType.hpp"
#include "Pascal--/syntactic/IntermediateCode.hpp"
#include <vector>
#include <string>

class Parser {
public:
    Parser(const std::vector<Lexeme>& lexemes);
    virtual ~Parser();

    void start();
    // Métodos para obter e imprimir o código intermediário
    const IntermediateCode& getIntermediateCode() const { return m_intermediateCode; }
    void printIntermediateCode() const { m_intermediateCode.printCode(); }

private:
    std::vector<Lexeme>::const_iterator m_lexeme;
    IntermediateCode m_intermediateCode;
    std::vector<std::string> m_expressionStack;
    int m_tempCounter = 0;
    int m_labelCounter = 0;

    // Métodos auxiliares para geração de código intermediário
    std::string popExpression();
    void pushExpression(const std::string& expr);
    std::string generateTemp();
    std::string generateLabel();
    std::string generateEndLabel();

    void consume(enum TokenType expected);

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
    void proc_outList(const std::string& writeType = "WRITE");
    void proc_restoOutList();
    void proc_out(const std::string& writeType = "WRITE");
    void proc_whileStmt();
    void proc_ifStmt(const std::string& endLabel = "");
    void proc_elsePart(const std::string& endLabel = "");
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
