#ifndef PARSE_TREE_NODE_HPP
#define PARSE_TREE_NODE_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"

class ParseTreeNode {
public:
    enum SymbolType {ST_TERMINAL, ST_NON_TERMINAL};

    enum NonTerminalType {
        // ------------------------------------
        //  main function
        // ------------------------------------
        NTT_FUNCTION,
        // ------------------------------------
        //  variable declarations
        // ------------------------------------
        NTT_DECLARATIONS,
        NTT_DECLARATION,
        NTT_LISTAIDENT,
        NTT_RESTOIDENTLIST,
        NTT_RESTODECLARATION,
        NTT_TYPE,
        // ------------------------------------
        //  program statements
        // ------------------------------------
        NTT_BLOCO,
        NTT_STMTLIST,
        NTT_STMT,
        // ---------------------------
        //  statement descriptions
        // ---------------------------
        // for command
        NTT_FORSTMT,
        NTT_ENDFOR,
        // IO commands
        NTT_IOSTMT,

        NTT_OUTLIST,
        NTT_RESTOOUTLIST,
        NTT_OUT,
        // while command
        NTT_WHILESTMT,
        // if command
        NTT_IFSTMT,
        NTT_ELSEPART,
        // ------------------------------
        //  expressions
        // ------------------------------
        NTT_ATRIB,
        NTT_EXPR,
        NTT_OR,
        NTT_RESTOOR,
        NTT_AND,
        NTT_RESTOAND,
        NTT_NOT,
        NTT_REL,
        NTT_RESTOREL,
        NTT_ADD,
        NTT_RESTOADD,
        NTT_MULT,
        NTT_RESTOMULT,
        NTT_UNO,
        NTT_FATOR,
    };

    ParseTreeNode(enum NonTerminalType type);
    ParseTreeNode(const Lexeme* terminal);
    virtual ~ParseTreeNode();

    enum SymbolType type() const { return m_type; }
    enum NonTerminalType symbol_non_terminal();
    const Lexeme* symbol_terminal();

    std::vector<ParseTreeNode> children;

private:
    enum SymbolType m_type;
    union {
        enum NonTerminalType non_terminal;
        const Lexeme* terminal;
    } m_symbol;
};

#endif // PARSE_TREE_NODE_HPP
