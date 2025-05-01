#include "Pascal--/syntactic/ParseTreeNode.hpp"

ParseTreeNode::ParseTreeNode(enum NonTerminalType type)
: m_type(ParseTreeNode::ST_NON_TERMINAL) {
    m_symbol.non_terminal = type;
}

ParseTreeNode::ParseTreeNode(const Lexeme* terminal)
: m_type(ParseTreeNode::ST_TERMINAL) {
    m_symbol.terminal = terminal;
}

ParseTreeNode::~ParseTreeNode() {
}

// static std::string st2str(enum ParseTreeNode::NonTerminalType type) {
//     switch (type) {
//         // ------------------------------------
//         //  main function
//         // ------------------------------------
//         case ParseTreeNode::NTT_FUNCTION: return "function";
//         // ------------------------------------
//         //  variable declarations
//         // ------------------------------------
//         case ParseTreeNode::NTT_DECLARATIONS: return "declarations";
//         case ParseTreeNode::NTT_DECLARATION: return "declaration";
//         case ParseTreeNode::NTT_LISTAIDENT: return "listaident";
//         case ParseTreeNode::NTT_RESTOIDENTLIST: return "restoidentlist";
//         case ParseTreeNode::NTT_RESTODECLARATION: return "restodeclaration";
//         case ParseTreeNode::NTT_TYPE: return "type";
//         // ------------------------------------
//         //  program statements
//         // ------------------------------------
//         case ParseTreeNode::NTT_BLOCO: return "bloco";
//         case ParseTreeNode::NTT_STMTLIST: return "stmtlist";
//         case ParseTreeNode::NTT_STMT: return "stmt";
//         // ---------------------------
//         //  statement descriptions
//         // ---------------------------
//         // for command
//         case ParseTreeNode::NTT_FORSTMT: return "forstmt";
//         case ParseTreeNode::NTT_ENDFOR: return "endfor";
//         // IO commands
//         case ParseTreeNode::NTT_IOSTMT: return "iostmt";

//         case ParseTreeNode::NTT_OUTLIST: return "outlist";
//         case ParseTreeNode::NTT_RESTOOUTLIST: return "restooutlist";
//         case ParseTreeNode::NTT_OUT: return "out";
//         // while command
//         case ParseTreeNode::NTT_WHILESTMT: return "whilestmt";
//         // if command
//         case ParseTreeNode::NTT_IFSTMT: return "ifstmt";
//         case ParseTreeNode::NTT_ELSEPART: return "elsepart";
//         // ------------------------------
//         //  expressions
//         // ------------------------------
//         case ParseTreeNode::NTT_ATRIB: return "atrib";
//         case ParseTreeNode::NTT_EXPR: return "expr";
//         case ParseTreeNode::NTT_OR: return "or";
//         case ParseTreeNode::NTT_RESTOOR: return "restoor";
//         case ParseTreeNode::NTT_AND: return "and";
//         case ParseTreeNode::NTT_RESTOAND: return "restoand";
//         case ParseTreeNode::NTT_NOT: return "not";
//         case ParseTreeNode::NTT_REL: return "rel";
//         case ParseTreeNode::NTT_RESTOREL: return "restorel";
//         case ParseTreeNode::NTT_ADD: return "add";
//         case ParseTreeNode::NTT_RESTOADD: return "restoadd";
//         case ParseTreeNode::NTT_MULT: return "mult";
//         case ParseTreeNode::NTT_RESTOMULT: return "restomult";
//         case ParseTreeNode::NTT_UNO: return "uno";
//         case ParseTreeNode::NTT_FATOR: return "fator";
//     }
// }
