#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/util/exception.hpp"

#include <vector>

Parser::Parser(const std::vector<Lexeme>& lexemes) : m_lexeme(lexemes.begin()) {
}

Parser::~Parser() {
}

void Parser::start() {
    proc_function();
    consume(TT_EOF);
}

void Parser::consume(enum TokenType expected) {
    if (expected == m_lexeme->type) {
        m_lexeme++;
    } else {
        throw SyntaxError(
            "expected " + tt2str(expected) + ", found" + m_lexeme->token,
            m_lexeme->line, m_lexeme->column
        );
    }
}

// ------------------------------------
//  main function
// ------------------------------------

// <function*> -> 'program' 'IDENT' ';' <declarations> 'begin' <stmtList> 'end' '.' ;
void Parser::proc_function() {
    consume(TT_PROGRAM);
    consume(TT_IDENT);
    consume(TT_SEMICOLON);
    proc_declarations();
    consume(TT_BEGIN);
    proc_stmtList();
    consume(TT_END);
    consume(TT_PERIOD);
}

// ------------------------------------
//  variable declarations
// ------------------------------------

// <declarations> -> var <declaration> <restDeclaration> ;
void Parser::proc_declarations() {
    consume(TT_VAR);
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
    if (m_lexeme->type == TT_COMMA) {
        consume(TT_COMMA);
        consume(TT_IDENT);
        proc_restIdentList();
    }
}

// <restDeclaration> -> <declaration><restDeclaration> | & ;
void Parser::proc_restDeclaration() {
    if (m_lexeme->type == TT_IDENT) {
        proc_declaration();
        proc_restDeclaration();
    }
}

// <type> -> 'integer' | 'real' | 'string' ;
void Parser::proc_type() {
    switch (m_lexeme->type) {
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
            throw SyntaxError(
                "Expected integer, real or string, found",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// ------------------------------------
//  program statements
// ------------------------------------

// <block> -> 'begin' <stmtList> 'end' ';' ;
void Parser::proc_block() {
    consume(TT_BEGIN);
    proc_stmtList();
    consume(TT_END);
    consume(TT_SEMICOLON);
}

// <stmtList> -> <stmt> <stmtList> | & ;
void Parser::proc_stmtList() {
    switch (m_lexeme->type) {
        case TT_FOR:
        case TT_READ:
        case TT_WRITE:
        case TT_READLN:
        case TT_WRITELN:
        case TT_WHILE:
        case TT_IDENT:
        case TT_IF:
        case TT_BEGIN:
        case TT_BREAK:
        case TT_CONTINUE:
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
    switch (m_lexeme->type) {
        case TT_FOR:
            proc_forStmt();
            break;

        case TT_READ:
        case TT_WRITE:
        case TT_READLN:
        case TT_WRITELN:
            proc_ioStmt();
            break;

        case TT_WHILE:
            proc_whileStmt();
            break;

        case TT_IDENT:
            proc_atrib();
            consume(TT_SEMICOLON);
            break;

        case TT_IF:
            proc_ifStmt("");
            break;

        case TT_BEGIN:
            proc_block();
            break;

        case TT_BREAK:
            consume(TT_BREAK);
            consume(TT_SEMICOLON);
            break;

        case TT_CONTINUE:
            consume(TT_CONTINUE);
            consume(TT_SEMICOLON);
            break;

        case TT_SEMICOLON:
            consume(TT_SEMICOLON);
            break;

        default:
            throw SyntaxError(
                "Undefined statement , expected flow control, ; ,break, continue or attribuition",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// ---------------------------
//  statement descriptions
// ---------------------------

// for command

// <forStmt> -> 'for' <atrib> 'to' <endFor> 'do' <stmt> ;
void Parser::proc_forStmt() {
    std::string startLabel = generateLabel();
    std::string endLabel = generateEndLabel();
    std::string bodyLabel = generateLabel();

    consume(TT_FOR);

    // Capturar o nome da variável do loop
    std::string loopVar = m_lexeme->token;
    proc_atrib();

    m_intermediateCode.addInstruction(OpCode::LABEL, startLabel);

    consume(TT_TO);

    std::string endValue = m_lexeme->token;
    proc_endFor();

    std::string temp = generateTemp();
    if (endValue.find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos) {
        m_intermediateCode.addInstruction(OpCode::LEQ, "TEMP:" + temp, "VAR:" + loopVar, "LIT:" + endValue);
    } else {
        m_intermediateCode.addInstruction(OpCode::LEQ, "TEMP:" + temp, "VAR:" + loopVar, "VAR:" + endValue);
    }
    std::string condition = "TEMP:" + temp;

    m_intermediateCode.addInstruction(OpCode::IF, condition, bodyLabel, endLabel);

    m_intermediateCode.addInstruction(OpCode::LABEL, bodyLabel);
    consume(TT_DO);
    proc_stmt();

    std::string incTemp = generateTemp();
    m_intermediateCode.addInstruction(OpCode::ADD, "TEMP:" + incTemp, "VAR:" + loopVar, "LIT:1");
    m_intermediateCode.addInstruction(OpCode::MOV, "VAR:" + loopVar, "TEMP:" + incTemp);

    m_intermediateCode.addInstruction(OpCode::JUMP, startLabel);
    m_intermediateCode.addInstruction(OpCode::LABEL, endLabel);
}

// <endFor> -> 'IDENT' | 'NUMint' ;
void Parser::proc_endFor() {
    switch (m_lexeme->type) {
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
            throw SyntaxError(
                "Expected variable or literal value, found",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// IO commands
// <ioStmt> -> 'read' '(' 'IDENT' ')' ';'
//           | 'write' '(' <outList> ')' ';' ;
//           | 'readln' '(' 'IDENT' ')' ';'
//           | 'writeln' '(' <outList> ')' ';' ;
void Parser::proc_ioStmt() {
    switch (m_lexeme->type) {
        case TT_READ: {
            consume(TT_READ);
            consume(TT_LPAREN);
            std::string readVar = m_lexeme->token;
            consume(TT_IDENT);
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            m_intermediateCode.addInstruction(OpCode::CALL, "READ", "VAR:" + readVar);
            break;
        }
        case TT_WRITE: {
            consume(TT_WRITE);
            consume(TT_LPAREN);
            proc_outList("WRITE");
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            break;
        }
        case TT_READLN: {
            consume(TT_READLN);
            consume(TT_LPAREN);
            std::string readlnVar = m_lexeme->token;
            consume(TT_IDENT);
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            m_intermediateCode.addInstruction(OpCode::CALL, "READLN", "VAR:" + readlnVar);
            break;
        }
        case TT_WRITELN: {
            consume(TT_WRITELN);
            consume(TT_LPAREN);
            proc_outList("WRITELN");
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            break;
        }
        default:
            throw SyntaxError(
                "Poorly formated Read or Write stament",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// <outList> -> <out><restoOutList>;
void Parser::proc_outList(const std::string& writeType) {
    proc_out(writeType);
    proc_restoOutList();
}

// <restoOutList> -> ',' <outList> | &;
void Parser::proc_restoOutList() {
    if (m_lexeme->type == TT_COMMA) {
        consume(TT_COMMA);

        switch (m_lexeme->type) {
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
void Parser::proc_out(const std::string& writeType) {
    switch (m_lexeme->type) {
        case TT_LITERAL_STR: {
            std::string value = m_lexeme->token;
            consume(TT_LITERAL_STR);
            m_intermediateCode.addInstruction(OpCode::CALL, writeType, "LIT:" + value);
            break;
        }
        case TT_IDENT: {
            std::string value = m_lexeme->token;
            consume(TT_IDENT);
            m_intermediateCode.addInstruction(OpCode::CALL, writeType, "VAR:" + value);
            break;
        }
        case TT_LITERAL_OCT:
        case TT_LITERAL_DEC:
        case TT_LITERAL_HEX:
        case TT_LITERAL_REAL: {
            std::string value = m_lexeme->token;
            consume(m_lexeme->type);
            m_intermediateCode.addInstruction(OpCode::CALL, writeType, "LIT:" + value);
            break;
        }
        default:
            throw SyntaxError(
                "Invalid output information, expected string, variable or number, found",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// while command

// <whileStmt> -> 'while' <expr> 'do' <stmt> ;
void Parser::proc_whileStmt() {
    std::string startLabel = generateLabel();
    std::string endLabel = generateEndLabel();
    std::string bodyLabel = generateLabel();

    consume(TT_WHILE);

    m_intermediateCode.addInstruction(OpCode::LABEL, startLabel);

    proc_expr();
    std::string condition = popExpression();

    // IF cond, bodyLabel, endLabel
    m_intermediateCode.addInstruction(OpCode::IF, condition, bodyLabel, endLabel);

    m_intermediateCode.addInstruction(OpCode::LABEL, bodyLabel);
    consume(TT_DO);
    proc_stmt();

    m_intermediateCode.addInstruction(OpCode::JUMP, startLabel);
    m_intermediateCode.addInstruction(OpCode::LABEL, endLabel);
}

// if command

// <ifStmt> -> 'if' <expr> 'then' <stmt> <elsePart> ;
void Parser::proc_ifStmt(const std::string& endLabel) {
    consume(TT_IF);
    proc_expr();

    std::string thenLabel = generateLabel();
    std::string elseLabel = generateLabel();
    std::string finalEndLabel = endLabel.empty() ? generateEndLabel() : endLabel;

    std::string condition = popExpression();
    // IF cond, thenLabel, elseLabel
    m_intermediateCode.addInstruction(OpCode::IF, condition, thenLabel, elseLabel);

    // THEN block
    m_intermediateCode.addInstruction(OpCode::LABEL, thenLabel);
    consume(TT_THEN);
    proc_stmt();
    m_intermediateCode.addInstruction(OpCode::JUMP, finalEndLabel);

    // ELSE/ELSE IF block
    m_intermediateCode.addInstruction(OpCode::LABEL, elseLabel);
    proc_elsePart(finalEndLabel);

    // END label
    if (endLabel.empty()) {
        m_intermediateCode.addInstruction(OpCode::LABEL, finalEndLabel);
    }
}

// <elsePart> -> 'else' <stmt> | & ;
void Parser::proc_elsePart(const std::string& endLabel) {
    if (m_lexeme->type == TT_ELSE) {
        consume(TT_ELSE);
        if (m_lexeme->type == TT_IF) {
            proc_ifStmt(endLabel); // encadeia else if
        } else {
            switch (m_lexeme->type) {
                case TT_FOR:
                case TT_WHILE:
                case TT_IDENT:
                case TT_BEGIN:
                case TT_BREAK:
                case TT_CONTINUE:
                case TT_SEMICOLON:
                    proc_stmt();
                    break;
                default:
                    throw SyntaxError(
                        "Invalid else statment",
                        m_lexeme->line, m_lexeme->column
                    );
            }
        }
    }
}

// ------------------------------
//  expressions
// ------------------------------

// <atrib> -> 'IDENT' ':=' <expr> ;
void Parser::proc_atrib() {
    std::string varName = m_lexeme->token;
    consume(TT_IDENT);
    consume(TT_ASSIGN);
    proc_expr();
    // Gera código para atribuição
    std::string value = popExpression();
    m_intermediateCode.addInstruction(OpCode::MOV, "VAR:" + varName, value);
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
    if (m_lexeme->type == TT_OR) {
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
    if (m_lexeme->type == TT_AND) {
        consume(TT_AND);
        proc_and();
        proc_restoAnd();
    }
}

// <not> -> 'not' <not> | <rel> ;
void Parser::proc_not() {
    if (m_lexeme->type == TT_NOT) {
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
    switch (m_lexeme->type) {
        case TT_EQL: {
            consume(TT_EQL);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::EQL, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_NEQ: {
            consume(TT_NEQ);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::NEQ, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_LSS: {
            consume(TT_LSS);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::LSS, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_LEQ: {
            consume(TT_LEQ);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::LEQ, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_GTR: {
            consume(TT_GTR);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::GRT, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_GEQ: {
            consume(TT_GEQ);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::GEQ, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
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
    switch (m_lexeme->type) {
        case TT_ADD: {
            consume(TT_ADD);
            proc_mult();
            proc_restoAdd();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::ADD, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_SUB: {
            consume(TT_SUB);
            proc_mult();
            proc_restoAdd();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::SUB, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
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
    switch (m_lexeme->type) {
        case TT_MUL: {
            consume(TT_MUL);
            proc_uno();
            proc_restoMult();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::MUL, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_DIV: {
            consume(TT_DIV);
            proc_uno();
            proc_restoMult();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::DIV, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_MOD: {
            consume(TT_MOD);
            proc_uno();
            proc_restoMult();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::MOD, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_FLOORDIV: {
            consume(TT_FLOORDIV);
            proc_uno();
            proc_restoMult();
            std::string right = popExpression();
            std::string left = popExpression();
            std::string temp = generateTemp();
            m_intermediateCode.addInstruction(OpCode::DIV, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        default:
            break;
    }
}

// <uno> -> '+' <uno> | '-' <uno> | <fator> ;
void Parser::proc_uno() {
    switch (m_lexeme->type) {
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
    switch (m_lexeme->type) {
        case TT_LITERAL_OCT:
        case TT_LITERAL_DEC:
        case TT_LITERAL_HEX:
        case TT_LITERAL_REAL: {
            std::string value = m_lexeme->token;
            pushExpression("LIT:" + value);
            consume(m_lexeme->type);
            break;
        }
        case TT_IDENT: {
            std::string value = m_lexeme->token;
            pushExpression("VAR:" + value);
            consume(TT_IDENT);
            break;
        }
        case TT_LPAREN:
            consume(TT_LPAREN);
            proc_expr();
            consume(TT_RPAREN);
            break;
        case TT_LITERAL_STR: {
            std::string value = m_lexeme->token;
            pushExpression("LIT:" + value);
            consume(TT_LITERAL_STR);
            break;
        }
        default:
            throw SyntaxError(
                "Invalid fator",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// Métodos auxiliares para geração de código intermediário
std::string Parser::popExpression() {
    if (m_expressionStack.empty()) {
        throw std::string("Erro: tentativa de acessar pilha de expressões vazia");
    }
    std::string result = m_expressionStack.back();
    m_expressionStack.pop_back();
    return result;
}

void Parser::pushExpression(const std::string& expr) {
    m_expressionStack.push_back(expr);
}

std::string Parser::generateTemp() {
    return "Temp" + std::to_string(++m_tempCounter);
}

std::string Parser::generateLabel() {
    return "L_" + std::to_string(++m_labelCounter);
}

std::string Parser::generateEndLabel() {
    return "endLabel" + std::to_string(++m_labelCounter);
}
