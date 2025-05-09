#include "Pascal--/syntactic/Parser.hpp"

#include <iostream> // rascunho

Parser::Parser(const std::vector<Lexeme>& lexemes) : m_lexemes(lexemes) {
}

Parser::~Parser() {
}

void Parser::start() {
    proc_function();
}

inline const Lexeme& Parser::current_lexeme() {
    return m_lexemes[m_pos];
}

void Parser::consume(enum TokenType expected) {
    if (expected == current_lexeme().type) {
        m_pos++;
    } else {
        throw std::string(
            "syntactic error -> expected " + tt2str(expected) +
            ", found " + tt2str(current_lexeme().type) +
            "\nline: " + std::to_string(current_lexeme().line) +
            "\ncolumn: " + std::to_string(current_lexeme().column)
        );
    }
}

// ------------------------------------
//  main function
// ------------------------------------

// <function*> -> 'program' 'IDENT' ';' <declarations> 'begin' <stmtList> 'end' '.' ;
void Parser::proc_function() {
    consume(TT_PROGRAMSYM);
    consume(TT_IDENT);
    consume(TT_SEMICOLON);
    proc_declarations();
    consume(TT_BEGINSYM);
    proc_stmtList();
    consume(TT_ENDSYM);
    consume(TT_PERIOD);
}

// ------------------------------------
//  variable declarations
// ------------------------------------

// <declarations> -> var <declaration> <restDeclaration> ;
void Parser::proc_declarations() {
    consume(TT_VARSYM);
    proc_declaration();
    proc_restDeclaration();
}

// <declaration> -> <listIdent> ':' <type> ';' ;
void Parser::proc_declaration() {
    proc_listIdent();
    consume(TT_COLON);
    proc_type();
    consume(TT_SEMICOLON);
}

// <listIdent> -> 'IDENT' <restIdentList> ;
void Parser::proc_listIdent() {
    consume(TT_IDENT);
    proc_restIdentList();
}

// <restIdentList> -> ',' 'IDENT' <restIdentList> | & ;
void Parser::proc_restIdentList() {
    if (current_lexeme().type == TT_COMMA) {
        consume(TT_COMMA);
        consume(TT_IDENT);
        proc_restIdentList();
    }
}

// <restDeclaration> -> <declaration><restDeclaration> | & ;
void Parser::proc_restDeclaration() {
    if (current_lexeme().type == TT_IDENT) {
        proc_declaration();
        proc_restDeclaration();
    }
}

// <type> -> 'integer' | 'real' | 'string' ;
void Parser::proc_type() {
    switch (current_lexeme().type) {
        case TT_TYPE_INT:
            consume(TT_TYPE_INT);
            break;

        case TT_TYPE_REAL:
            consume(TT_TYPE_REAL);
            break;

        case TT_TYPE_STR:
            consume(TT_TYPE_STR);
            break;

        default:
            // TODO: jogar um erro namoral aqui
            throw std::string("DEU PAU 1: " + current_lexeme().str());
            break;
    }
}

// ------------------------------------
//  program statements
// ------------------------------------

// <block> -> 'begin' <stmtList> 'end' ';' ;
void Parser::proc_block() {
    consume(TT_BEGINSYM);
    proc_stmtList();
    consume(TT_ENDSYM);
    consume(TT_SEMICOLON);
}

// <stmtList> -> <stmt> <stmtList> | & ;
void Parser::proc_stmtList() {
    switch (current_lexeme().type) {
        case TT_FORSYM:
        case TT_READSYM:
        case TT_WRITESYM:
        case TT_READLNSYM:
        case TT_WRITELNSYM:
        case TT_WHILESYM:
        case TT_IDENT:
        case TT_IFSYM:
        case TT_BEGINSYM:
        case TT_BREAKSYM:
        case TT_CONTINUESYM:
        case TT_SEMICOLON:
            proc_stmt();
            proc_stmtList();
            break;

        default:
            break;
    }
}

// <stmt> -> <forStmt>
//    | <ioStmt>
//    | <whileStmt>
//    | <atrib> ';'
//    | <ifStmt>
//    | <block>
//    | 'break'';'
//    | 'continue'';'
//    | ';' ;
void Parser::proc_stmt() {
    switch (current_lexeme().type) {
        case TT_FORSYM:
            proc_forStmt();
            break;

        case TT_READSYM:
        case TT_WRITESYM:
        case TT_READLNSYM:
        case TT_WRITELNSYM:
            proc_ioStmt();
            break;

        case TT_WHILESYM:
            proc_whileStmt();
            break;

        case TT_IDENT:
            proc_atrib();
            consume(TT_SEMICOLON);
            break;

        case TT_IFSYM:
            proc_ifStmt();
            break;

        case TT_BEGINSYM:
            proc_block();
            break;

        case TT_BREAKSYM:
            consume(TT_BREAKSYM);
            consume(TT_SEMICOLON);
            break;

        case TT_CONTINUESYM:
            consume(TT_CONTINUESYM);
            consume(TT_SEMICOLON);
            break;

        case TT_SEMICOLON:
            consume(TT_SEMICOLON);
            break;

        default:
            // TODO: jogar um erro namoral aqui
            throw std::string("DEU PAU 3: " + current_lexeme().str());
    }
}

// ---------------------------
//  statement descriptions
// ---------------------------

// for command

// <forStmt> -> 'for' <atrib> 'to' <endFor> 'do' <stmt> ;
void Parser::proc_forStmt() {
    consume(TT_FORSYM);
    proc_atrib();
    consume(TT_TOSYM);
    proc_endFor();
    consume(TT_DOSYM);
    proc_stmt();
}

// <endFor> -> 'IDENT' | 'NUMint' ;
void Parser::proc_endFor() {
    switch (current_lexeme().type) {
        case TT_IDENT:
            consume(TT_IDENT);
            break;

        case TT_LITERAL_OCT:
            consume(TT_LITERAL_OCT);
            break;

        case TT_LITERAL_DEC:
            consume(TT_LITERAL_DEC);
            break;

        case TT_LITERAL_HEX:
            consume(TT_LITERAL_HEX);
            break;

        default:
            // TODO: jogar um erro namoral aqui
            throw std::string("DEU PAU 4: " + current_lexeme().str());
    }
}

// IO commands
// <ioStmt> -> 'read' '(' 'IDENT' ')' ';'
//           | 'write' '(' <outList> ')' ';' ;
//           | 'readln' '(' 'IDENT' ')' ';'
//           | 'writeln' '(' <outList> ')' ';' ;
void Parser::proc_ioStmt() {
    switch (current_lexeme().type) {
        case TT_READSYM:
            consume(TT_READSYM);
            consume(TT_LPAREN);
            consume(TT_IDENT);
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            break;

        case TT_WRITESYM:
            consume(TT_WRITESYM);
            consume(TT_LPAREN);
            proc_outList();
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            break;

        case TT_READLNSYM:
            consume(TT_READLNSYM);
            consume(TT_LPAREN);
            consume(TT_IDENT);
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            break;

        case TT_WRITELNSYM:
            consume(TT_WRITELNSYM);
            consume(TT_LPAREN);
            proc_outList();
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);

        default:
            break;
    }
}

// <outList> -> <out><restoOutList>;
void Parser::proc_outList() {
    proc_out();
    proc_restoOutList();
}

// <restoOutList> -> ',' <outList> | &;
void Parser::proc_restoOutList() {
    if (current_lexeme().type == TT_COMMA) {
        consume(TT_COMMA);

        switch (current_lexeme().type) {
            case TT_LITERAL_STR:
            case TT_IDENT:
            case TT_LITERAL_OCT:
            case TT_LITERAL_DEC:
            case TT_LITERAL_HEX:
            case TT_LITERAL_REAL:
                proc_outList();
            break;

            default:
                break;
        }
    }
}

// <out> -> 'STR' | 'IDENT' | 'NUMint' | 'NUMfloat' ;
void Parser::proc_out() {
    switch (current_lexeme().type) {
        case TT_LITERAL_STR:
            consume(TT_LITERAL_STR);
            break;

        case TT_IDENT:
            consume(TT_IDENT);
            break;

        case TT_LITERAL_OCT:
            consume(TT_LITERAL_OCT);
            break;

        case TT_LITERAL_DEC:
            consume(TT_LITERAL_DEC);
            break;

        case TT_LITERAL_HEX:
            consume(TT_LITERAL_HEX);
            break;

        case TT_LITERAL_REAL:
            consume(TT_LITERAL_REAL);
            break;

        default:
            break;
    }
}

// while command

// <whileStmt> -> 'while' <expr> 'do' <stmt> ;
void Parser::proc_whileStmt() {
    consume(TT_WHILESYM);
    proc_expr();
    consume(TT_DOSYM);
    proc_stmt();
}

// if command

// <ifStmt> -> 'if' <expr> 'then' <stmt> <elsePart> ;
void Parser::proc_ifStmt() {
    consume(TT_IFSYM);
    proc_expr();
    consume(TT_THENSYM);
    proc_stmt();
    proc_elsePart();
}

// <elsePart> -> 'else' <stmt> | & ;
void Parser::proc_elsePart() {
    if (current_lexeme().type == TT_ELSESYM) {
        consume(TT_ELSESYM);

        switch (current_lexeme().type) {
            case TT_FORSYM:
            case TT_WHILESYM:
            case TT_IDENT:
            case TT_IFSYM:
            case TT_BEGINSYM:
            case TT_BREAKSYM:
            case TT_CONTINUESYM:
            case TT_SEMICOLON:
                proc_stmt();
                break;

            default:
                // TODO: jogar um erro namoral aqui
                throw std::string("DEU PAU 5: " + current_lexeme().str());
        }
    }
}

// ------------------------------
//  expressions
// ------------------------------

// <atrib> -> 'IDENT' ':=' <expr> ;
void Parser::proc_atrib() {
    consume(TT_IDENT);
    consume(TT_ASSIGN);
    proc_expr();
}

// <expr> -> <or> ;
void Parser::proc_expr() {
    proc_or();
}

// <or> -> <and> <restoOr> ;
void Parser::proc_or() {
    proc_and();
    proc_restoOr();
}

// <restoOr> -> 'or' <and> <restoOr> | & ;
void Parser::proc_restoOr() {
    if (current_lexeme().type == TT_OR) {
        consume(TT_OR);
        proc_and();
        proc_restoOr();
    }
}

// <and> -> <not> <restoAnd> ;
void Parser::proc_and() {
    proc_not();
    proc_restoAnd();
}

// <restoAnd> -> 'and' <not> <restoAnd> | & ;
void Parser::proc_restoAnd() {
    if (current_lexeme().type == TT_AND) {
        consume(TT_AND);
        proc_and();
        proc_restoAnd();
    }
}

// <not> -> 'not' <not> | <rel> ;
void Parser::proc_not() {
    if (current_lexeme().type == TT_NOT) {
        consume(TT_NOT);
        proc_not();
    } else {
        proc_rel();
    }
}

// <rel> -> <add> <restoRel> ;
void Parser::proc_rel() {
    proc_add();
    proc_restoRel();
}

// <restoRel> -> '==' <add> | '<>' <add>
//             | '<' <add> | '<=' <add>
//             | '>' <add> | '>=' <add> | & ;
void Parser::proc_restoRel() {
    switch (current_lexeme().type) {
        case TT_EQL:
            consume(TT_EQL);
            proc_add();
            break;

        case TT_NEQ:
            consume(TT_NEQ);
            proc_add();
            break;

        case TT_LSS:
            consume(TT_LSS);
            proc_add();
            break;

        case TT_LEQ:
            consume(TT_LEQ);
            proc_add();
            break;

        case TT_GTR:
            consume(TT_GTR);
            proc_add();
            break;

        case TT_GEQ:
            consume(TT_GEQ);
            proc_add();
            break;

        default:
            break;
    }
}

// <add> -> <mult> <restoAdd> ;
void Parser::proc_add() {
    proc_mult();
    proc_restoAdd();
}

// <restoAdd> -> '+' <mult> <restoAdd>
//             | '-' <mult> <restoAdd> | & ;
void Parser::proc_restoAdd() {
    switch (current_lexeme().type) {
        case TT_ADD:
            consume(TT_ADD);
            proc_mult();
            proc_restoAdd();
            break;

        case TT_SUB:
            consume(TT_SUB);
            proc_mult();
            proc_restoAdd();
            break;

        default:
            break;
    }
}

// <mult> -> <uno> <restoMult> ;
void Parser::proc_mult() {
    proc_uno();
    proc_restoMult();
}

// <restoMult> -> '*' <uno> <restoMult>
//             |  '/' <uno> <restoMult>
//             |  'mod' <uno> <restoMult> | & ;
//             |  'div' <uno> <restoMult> | & ;
void Parser::proc_restoMult() {
    switch (current_lexeme().type) {
        case TT_MUL:
            consume(TT_MUL);
            proc_uno();
            proc_restoMult();
            break;

        case TT_DIV:
            consume(TT_DIV);
            proc_uno();
            proc_restoMult();
            break;

        case TT_MOD:
            consume(TT_MOD);
            proc_uno();
            proc_restoMult();
            break;

        case TT_FLOORDIV:
            consume(TT_FLOORDIV);
            proc_uno();
            proc_restoMult();
            break;

        default:
            break;
    }
}

// <uno> -> '+' <uno> | '-' <uno> | <fator> ;
void Parser::proc_uno() {
    switch (current_lexeme().type) {
        case TT_ADD:
            consume(TT_ADD);
            proc_uno();
            break;

        case TT_SUB:
            consume(TT_SUB);
            proc_uno();
            break;

        default:
            proc_fator();
            break;
    }
}

// <fator> -> 'NUMint' | 'NUMfloat'
//          | 'IDENT'  | '(' <expr> ')' | 'STR' ;
void Parser::proc_fator() {
    switch (current_lexeme().type) {
        case TT_LITERAL_OCT:
            consume(TT_LITERAL_OCT);
            break;

        case TT_LITERAL_DEC:
            consume(TT_LITERAL_DEC);
            break;

        case TT_LITERAL_HEX:
            consume(TT_LITERAL_HEX);
            break;

        case TT_IDENT:
            consume(TT_IDENT);
            break;

        case TT_LPAREN:
            consume(TT_LPAREN);
            proc_expr();
            consume(TT_RPAREN);
            break;

        case TT_LITERAL_STR:
            consume(TT_LITERAL_STR);
            break;

        default:
            break;
    }
}
