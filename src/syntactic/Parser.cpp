#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/lexical/TokenType.hpp"

Parser::Parser(const std::vector<Lexeme>& lexemes) : m_lexeme(lexemes.begin()) {
}

Parser::~Parser() {
}

void Parser::start() {
}

void Parser::advance() {
    m_lexeme++;
}

void Parser::eat(enum TokenType type) {
    if (type == m_lexeme->type) {
        advance();
    } else {
        // TODO: jogar um erro aqui
    }
}

// ------------------------------------
//  main function
// ------------------------------------

// <function*> -> 'program' 'IDENT' ';' <declarations> 'begin' <stmtList> 'end' '.' ;
void Parser::proc_function() {
    eat(TT_PROGRAMSYM);
    eat(TT_IDENT);
    eat(TT_SEMICOLON);

    eat(TT_BEGINSYM);

    eat(TT_ENDSYM);
    eat(TT_PERIOD);
}

// ------------------------------------
//  variable declarations
// ------------------------------------
// <declarations> -> var <declaration> <restDeclaration> ;
void Parser::proc_declarations() {
    eat(TT_VARSYM);
    proc_declaration();
}

// <declaration> -> <listIdent> ':' <type> ';' ;
void Parser::proc_declaration() {
}

// <listIdent> -> 'IDENT' <restIdentList> ;
void Parser::proc_listIdent() {
}

// <restIdentList> -> ',' 'IDENT' <restIdentList> | & ;
void Parser::proc_restIdentList() {
}

// <restDeclaration> -> <declaration><restDeclaration> | & ;
void Parser::proc_restDeclaration() {
}

// <type> -> 'integer' | 'real' | 'string' ;
void Parser::proc_type() {
}

// ------------------------------------
//  program statements
// ------------------------------------
// <bloco> -> 'begin' <stmtList> 'end' ';' ;
// <stmtList> -> <stmt> <stmtList> | & ;
// <stmt> -> <forStmt>
//    | <ioStmt>
//    | <whileStmt>
//    | <atrib> ';'
//    | <ifStmt>
//    | <bloco>
//    | 'break'';'
//    | 'continue'';'
//    | ';' ;

// ---------------------------
//  statement descriptions
// ---------------------------

// for command
// <forStmt> -> 'for' <atrib> 'to' <endFor> 'do' <stmt> ;
// <endFor> -> 'IDENT' | 'NUMint' ;

// IO commands
// <ioStmt> -> 'read' '(' 'IDENT' ')' ';'
//           | 'write' '(' <outList> ')' ';' ;
//           | 'readln' '(' 'IDENT' ')' ';'
//           | 'writeln' '(' <outList> ')' ';' ;

// <outList> -> <out><restoOutList>;
// <restoOutList> -> ',' <outList> | &;
// <out> -> 'STR' | 'IDENT' | 'NUMint' | 'NUMfloat' ;

// while command
// <whileStmt> -> 'while' <expr> 'do' <stmt> ;

// if command
// <ifStmt> -> 'if' <expr> 'then' <stmt> <elsePart> ;
// <elsePart> -> 'else' <stmt> | & ;

// ------------------------------
//  expressions
// ------------------------------

// <atrib> -> 'IDENT' ':=' <expr> ;

// <expr> -> <or> ;
// <or> -> <and> <restoOr> ;
// <restoOr> -> 'or' <and> <restoOr> | & ;
// <and> -> <not> <restoAnd> ;
// <restoAnd> -> 'and' <not> <restoAnd> | & ;
// <not> -> 'not' <not> | <rel> ;
// <rel> -> <add> <restoRel> ;
// <restoRel> -> '==' <add> | '<>' <add>
//             | '<' <add> | '<=' <add>
//             | '>' <add> | '>=' <add> | & ;
// <add> -> <mult> <restoAdd> ;
// <restoAdd> -> '+' <mult> <restoAdd>
//             | '-' <mult> <restoAdd> | & ;
// <mult> -> <uno> <restoMult> ;
// <restoMult> -> '*' <uno> <restoMult>
//             |  '/' <uno> <restoMult>
//             |  'mod' <uno> <restoMult> | & ;
//             |  'div' <uno> <restoMult> | & ;
// <uno> -> '+' <uno> | '-' <uno> | <fator> ;
// <fator> -> 'NUMint' | 'NUMfloat'
//          | 'IDENT'  | '(' <expr> ')' | 'STR' ;
