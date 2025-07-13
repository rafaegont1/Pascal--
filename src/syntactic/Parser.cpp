#include "Pascal--/syntactic/Parser.hpp"
#include "Pascal--/interpreter/Command.hpp"
#include "Pascal--/util/exception.hpp"
#include "Pascal--/util/Printer.hpp"

#include <vector>
#include <iostream>
#include <cctype>
#include <stdexcept>

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
        throw CompilerError(
            "expected '" + tt2str(expected) + "', found '" + m_lexeme->token + "'",
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
    std::vector<std::string> varNames;
    proc_listIdent(varNames);
    consume(TT_COLON);
    VarType type = proc_type();
    consume(TT_SEMICOLON);

    // Store type information for all declared variables
    for (const auto& varName : varNames) {
        m_variableTypes[varName] = VariableInfo(type, varName);
        switch (type) {
            case VarType::INTEGER:
                addCommand(Command::Mnemonic::ASSIGN, varName, "0");
                break;
            case VarType::REAL:
                addCommand(Command::Mnemonic::ASSIGN, varName, "0.0");
                break;
            case VarType::STRING:
                addCommand(Command::Mnemonic::ASSIGN, varName, "");
                break;
        }
    }
}

// <listIdent> -> 'IDENT' <restIdentList> ;
void Parser::proc_listIdent(std::vector<std::string>& varNames) {
    std::string varName = m_lexeme->token;
    consume(TT_IDENT);
    varNames.push_back(varName);
    proc_restIdentList(varNames);
}

// <restIdentList> -> ',' 'IDENT' <restIdentList> | & ;
void Parser::proc_restIdentList(std::vector<std::string>& varNames) {
    if (m_lexeme->type == TT_COMMA) {
        consume(TT_COMMA);
        std::string varName = m_lexeme->token;
        consume(TT_IDENT);
        varNames.push_back(varName);
        proc_restIdentList(varNames);
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
VarType Parser::proc_type() {
    switch (m_lexeme->type) {
        case TT_TYPE_INT:
            consume(TT_TYPE_INT);
            return VarType::INTEGER;

        case TT_TYPE_REAL:
            consume(TT_TYPE_REAL);
            return VarType::REAL;

        case TT_TYPE_STR:
            consume(TT_TYPE_STR);
            return VarType::STRING;

        default:
            throw CompilerError(
                "Expected integer, real or string, found",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// ------------------------------------
//  program statements
// ------------------------------------

// <block> -> 'begin' <stmtList> 'end' ';' ;
void Parser::proc_block(
    const std::string& startLabel, const std::string& endLabel,
    const std::string& loopVar, const std::string& incTemp
) {
    consume(TT_BEGIN);
    proc_stmtList(startLabel, endLabel, loopVar, incTemp);
    consume(TT_END);
    consume(TT_SEMICOLON);
}

// <stmtList> -> <stmt> <stmtList> | & ;
void Parser::proc_stmtList(
    const std::string& startLabel, const std::string& endLabel,
    const std::string& loopVar, const std::string& incTemp
) {
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
            proc_stmt(startLabel, endLabel, loopVar, incTemp);
            proc_stmtList(startLabel, endLabel, loopVar, incTemp);
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
void Parser::proc_stmt(
    const std::string& startLabel, const std::string& endLabel,
    const std::string& loopVar, const std::string& incTemp
) {
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
            proc_block(startLabel, endLabel, loopVar, incTemp);
            break;

        case TT_BREAK:
            consume(TT_BREAK);
            consume(TT_SEMICOLON);
            if (endLabel.empty()) {
                throw CompilerError("break keyword outside loop",
                    m_lexeme->line, m_lexeme->column);
            }
            addCommand(Command::Mnemonic::JMP, endLabel);
            // if (m_loopStartLabels.empty()) {
            //     throw CompilerError("break keyword outside loop",
            //         m_lexeme->line, m_lexeme->column);
            // }
            // addCommand(Command::Mnemonic::JMP, m_loopEndLabels.top());
            break;

        case TT_CONTINUE:
            consume(TT_CONTINUE);
            consume(TT_SEMICOLON);
            if (startLabel.empty()) {
                throw CompilerError("continue keyword outside loop",
                    m_lexeme->line, m_lexeme->column);
            }
            if (!loopVar.empty()) {
                incrementFor(loopVar, incTemp);
            }
            addCommand(Command::Mnemonic::JMP, startLabel);
            // if (m_loopStartLabels.empty()) {
            //     throw CompilerError("continue keyword outside loop",
            //         m_lexeme->line, m_lexeme->column);
            // }
            // addCommand(Command::Mnemonic::JMP, m_loopStartLabels.top());
            break;

        case TT_SEMICOLON:
            consume(TT_SEMICOLON);
            break;

        default:
            throw CompilerError(
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
    std::string bodyLabel = generateLabel();
    std::string endLabel = generateEndLabel();

    consume(TT_FOR);

    // Capturar o nome da variável do loop
    std::string loopVar = m_lexeme->token;
    proc_atrib();

    addCommand(Command::Mnemonic::LABEL, startLabel);

    consume(TT_TO);

    std::string endValue = m_lexeme->token;
    proc_endFor();

    std::string temp = generateTemp();
    if (endValue.find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos) {
        addCommand(Command::Mnemonic::LEQ, "TEMP:" + temp, loopVar, endValue);
    } else {
        addCommand(Command::Mnemonic::LEQ, "TEMP:" + temp, loopVar, endValue);
    }
    std::string condition = "TEMP:" + temp;

    addCommand(Command::Mnemonic::IF, condition, bodyLabel, endLabel);
    addCommand(Command::Mnemonic::LABEL, bodyLabel);

    std::string incTemp = generateTemp();

    consume(TT_DO);
    proc_stmt(startLabel, endLabel, loopVar, incTemp);

    incrementFor(loopVar, incTemp);

    addCommand(Command::Mnemonic::JMP, startLabel);
    addCommand(Command::Mnemonic::LABEL, endLabel);
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
            throw CompilerError(
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
            
            // Verificação semântica: variável deve estar declarada
            validateVariableExists(readVar);
            
            // Determinar o tipo correto baseado na declaração da variável
            Command::ReadType readType = getReadTypeForVariable(readVar);
            addCommand(Command::Mnemonic::CALL, Command::CallType::READ, readVar, readType);
            break;
        }
        case TT_WRITE: {
            consume(TT_WRITE);
            consume(TT_LPAREN);
            proc_outList(WriteNewLine::NO);
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
            
            // Verificação semântica: variável deve estar declarada
            validateVariableExists(readlnVar);
            
            // Determinar o tipo correto baseado na declaração da variável
            Command::ReadType readType = getReadTypeForVariable(readlnVar);
            addCommand(Command::Mnemonic::CALL, Command::CallType::READLN, readlnVar, readType);
            break;
        }
        case TT_WRITELN: {
            consume(TT_WRITELN);
            consume(TT_LPAREN);
            proc_outList(WriteNewLine::YES);
            consume(TT_RPAREN);
            consume(TT_SEMICOLON);
            break;
        }
        default:
            throw CompilerError(
                "Poorly formated Read or Write stament",
                m_lexeme->line, m_lexeme->column
            );
    }
}

// <outList> -> <out><restoOutList>;
void Parser::proc_outList(WriteNewLine new_line) {
    proc_out(new_line);
    proc_restoOutList(new_line);
}

// <restoOutList> -> ',' <outList> | &;
void Parser::proc_restoOutList(WriteNewLine new_line) {
    if (m_lexeme->type == TT_COMMA) {
        consume(TT_COMMA);

        switch (m_lexeme->type) {
            case TT_LITERAL_STR:
            case TT_IDENT:
            case TT_LITERAL_OCT:
            case TT_LITERAL_DEC:
            case TT_LITERAL_HEX:
            case TT_LITERAL_REAL:
                proc_out(new_line);
                proc_restoOutList(new_line);
                break;

            default:
                break;
        }
    }
}

// <out> -> 'STR' | 'IDENT' | 'NUMint' | 'NUMfloat' ;
void Parser::proc_out(WriteNewLine new_line) {
    Command::CallType callType;

    if (new_line == WriteNewLine::YES) {
        // Check if this is the last argument by looking ahead
        auto lookahead = m_lexeme + 1;
        // If there's a comma after this token, it's not the last
        callType = (lookahead->type == TT_COMMA) ? Command::CallType::WRITE : Command::CallType::WRITELN;
    } else {
        callType = Command::CallType::WRITE;
    }

    switch (m_lexeme->type) {
        case TT_LITERAL_STR: {
            std::string value = m_lexeme->token;
            consume(TT_LITERAL_STR);
            addCommand(Command::Mnemonic::CALL, callType, value, Command::WriteType::STRING);
            break;
        }
        case TT_IDENT: {
            std::string value = m_lexeme->token;
            consume(TT_IDENT);
            addCommand(Command::Mnemonic::CALL, callType, value, Command::WriteType::VARIABLE);
            break;
        }
        case TT_LITERAL_OCT:
        case TT_LITERAL_DEC:
        case TT_LITERAL_HEX:
        case TT_LITERAL_REAL: {
            std::string value = m_lexeme->token;
            consume(m_lexeme->type);
            addCommand(Command::Mnemonic::CALL, callType, value, Command::WriteType::STRING);
            break;
        }
        default:
            throw CompilerError(
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

    addCommand(Command::Mnemonic::LABEL, startLabel);

    proc_expr();
    std::string condition = popExpression();

    // IF cond, bodyLabel, endLabel
    addCommand(Command::Mnemonic::IF, condition, bodyLabel, endLabel);
    addCommand(Command::Mnemonic::LABEL, bodyLabel);

    consume(TT_DO);
    proc_stmt(startLabel, endLabel);

    addCommand(Command::Mnemonic::JMP, startLabel);
    addCommand(Command::Mnemonic::LABEL, endLabel);
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
    addCommand(Command::Mnemonic::IF, condition, thenLabel, elseLabel);

    // THEN block
    addCommand(Command::Mnemonic::LABEL, thenLabel);
    consume(TT_THEN);
    proc_stmt();
    addCommand(Command::Mnemonic::JMP, finalEndLabel);

    // ELSE/ELSE IF block
    addCommand(Command::Mnemonic::LABEL, elseLabel);
    proc_elsePart(finalEndLabel);

    // END label
    if (endLabel.empty()) {
        addCommand(Command::Mnemonic::LABEL, finalEndLabel);
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
                    break;
                default:
                    throw CompilerError(
                        "Invalid else statement",
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
    std::string value = popExpression();

    // Validate variable exists and assignment types
    validateVariableExists(varName);
    validateAssignment(varName, value);

    addCommand(Command::Mnemonic::ASSIGN, varName, value);
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
        std::string right = popExpression();
        std::string left = popExpression();
        std::string temp = generateTemp();
        addCommand(Command::Mnemonic::OR, "TEMP:" + temp, left, right);
        pushExpression("TEMP:" + temp);
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
        std::string right = popExpression();
        std::string left = popExpression();
        std::string temp = generateTemp();
        addCommand(Command::Mnemonic::AND, "TEMP:" + temp, left, right);
        pushExpression("TEMP:" + temp);
    }
}

// <not> -> 'not' <not> | <rel> ;
void Parser::proc_not() {
    if (m_lexeme->type == TT_NOT) {
        consume(TT_NOT);
        proc_not();
        std::string operand = popExpression();
        std::string temp = generateTemp();
        addCommand(Command::Mnemonic::NOT, "TEMP:" + temp, operand);
        pushExpression("TEMP:" + temp);
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
            
            // Verificação semântica: tipos devem ser compatíveis para comparação
            validateComparison(left, right, "=");
            
            std::string temp = generateTemp();
            addCommand(Command::Mnemonic::EQL, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_NEQ: {
            consume(TT_NEQ);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            
            // Verificação semântica: tipos devem ser compatíveis para comparação
            validateComparison(left, right, "<>");
            
            std::string temp = generateTemp();
            addCommand(Command::Mnemonic::NEQ, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_LSS: {
            consume(TT_LSS);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            
            // Verificação semântica: tipos devem ser compatíveis para comparação
            validateComparison(left, right, "<");
            
            std::string temp = generateTemp();
            addCommand(Command::Mnemonic::LSS, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_LEQ: {
            consume(TT_LEQ);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            
            // Verificação semântica: tipos devem ser compatíveis para comparação
            validateComparison(left, right, "<=");
            
            std::string temp = generateTemp();
            addCommand(Command::Mnemonic::LEQ, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_GTR: {
            consume(TT_GTR);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            
            // Verificação semântica: tipos devem ser compatíveis para comparação
            validateComparison(left, right, ">");
            
            std::string temp = generateTemp();
            addCommand(Command::Mnemonic::GTR, "TEMP:" + temp, left, right);
            pushExpression("TEMP:" + temp);
            break;
        }
        case TT_GEQ: {
            consume(TT_GEQ);
            proc_add();
            std::string right = popExpression();
            std::string left = popExpression();
            
            // Verificação semântica: tipos devem ser compatíveis para comparação
            validateComparison(left, right, ">=");
            
            std::string temp = generateTemp();
            addCommand(Command::Mnemonic::GEQ, "TEMP:" + temp, left, right);
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
            addCommand(Command::Mnemonic::ADD, "TEMP:" + temp, left, right);
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
            addCommand(Command::Mnemonic::SUB, "TEMP:" + temp, left, right);
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
            addCommand(Command::Mnemonic::MUL, "TEMP:" + temp, left, right);
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
            addCommand(Command::Mnemonic::DIV, "TEMP:" + temp, left, right);
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
            addCommand(Command::Mnemonic::MOD, "TEMP:" + temp, left, right);
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
            addCommand(Command::Mnemonic::IDIV, "TEMP:" + temp, left, right);
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
            pushExpression(value);
            consume(m_lexeme->type);
            break;
        }
        case TT_IDENT: {
            std::string value = m_lexeme->token;
            pushExpression(value);
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
            pushExpression(value);
            consume(TT_LITERAL_STR);
            break;
        }
        default:
            throw CompilerError(
                "Invalid fator",
                m_lexeme->line, m_lexeme->column
            );
    }

}

// Métodos auxiliares para geração de código intermediário
std::string Parser::popExpression() {
    if (m_expressionStack.empty()) {
        throw std::runtime_error("tried to access empty expression stack");
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

// Simplified code generation methods
void Parser::addCommand(
    Command::Mnemonic mnemonic, const std::string& dst,
    const std::string& src1, const std::string& src2
) {
    Command cmd;
    cmd.mnemonic = mnemonic;
    cmd.dst = std::string(dst);
    cmd.src1 = std::string(src1);
    cmd.src2 = std::string(src2);
    m_commands.push_back(cmd);
}

void Parser::addCommand(
    Command::Mnemonic mnemonic, Command::CallType callType,
    const std::string& src1, Command::ReadType readType
) {
    Command cmd;
    cmd.mnemonic = mnemonic;
    cmd.dst = callType;
    cmd.src1 = std::string(src1);
    cmd.src2 = readType;
    m_commands.push_back(cmd);
}

void Parser::addCommand(
    Command::Mnemonic mnemonic, Command::CallType callType,
    const std::string& src1, Command::WriteType writeType
) {
    Command cmd;
    cmd.mnemonic = mnemonic;
    cmd.dst = callType;
    cmd.src1 = std::string(src1);
    cmd.src2 = writeType;
    m_commands.push_back(cmd);
}

// Simple utility methods for code generation
bool Parser::isIntegerLiteral(const std::string& str) {
    if (str.empty()) return false;

    // Handle hex, octal, decimal
    if (str.length() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        return str.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
    }
    if (str.length() > 1 && str[0] == '0') {
        return str.find_first_not_of("01234567") == std::string::npos;
    }

    return str.find_first_not_of("0123456789") == std::string::npos;
}

bool Parser::isRealLiteral(const std::string& str) {
    if (str.empty()) return false;

    bool hasDot = false;
    bool hasDigit = false;

    for (char c : str) {
        if (std::isdigit(c)) {
            hasDigit = true;
        } else if (c == '.' && !hasDot) {
            hasDot = true;
        } else {
            return false;
        }
    }

    return hasDigit && hasDot;
}

bool Parser::isStringLiteral(const std::string& str) {
    return str.length() >= 2 && str[0] == '"' && str[str.length() - 1] == '"';
}

// Type checking methods
void Parser::addTypeError(const std::string& error) {
    m_typeErrors.push_back(error);
}

void Parser::validateAssignment(const std::string& varName, const std::string& value) {
    auto it = m_variableTypes.find(varName);
    if (it != m_variableTypes.end()) {
        VarType expectedType = it->second.type;
        VarType actualType = getValueType(value);

        // Skip type checking for temporary variables (results of expressions)
        if (value.substr(0, 5) == "TEMP:") {
            return; // Temporary variables will be resolved at runtime
        }

        if (!isTypeCompatible(expectedType, actualType)) {
            addTypeError(
                "Type error: Cannot assign "
                + Printer::varTypeToString(actualType) + " to variable '"
                + varName + "' of type" + Printer::varTypeToString(expectedType)
            );
        }
    } else {
        // Variable not declared - could add warning here
        addTypeError("Warning: Variable '" + varName + "' not declared");
    }
}

bool Parser::isTypeCompatible(VarType expectedType, VarType actualType) {
    // Direct type matches
    if (expectedType == actualType) {
        return true;
    }

    // Type promotions
    if ((expectedType == VarType::REAL || expectedType == VarType::INTEGER)
        || (actualType == VarType::REAL || actualType == VarType::INTEGER)) {
        return true; // Integer can be promoted to real and vice versa
    }

    return false;
}

VarType Parser::getValueType(const std::string& value) {
    if (isIntegerLiteral(value)) {
        return VarType::INTEGER;
    } else if (isRealLiteral(value)) {
        return VarType::REAL;
    } else if (isStringLiteral(value)) {
        return VarType::STRING;
    } else {
        // Assume it's a variable - check its declared type
        auto it = m_variableTypes.find(value);
        if (it != m_variableTypes.end()) {
            return it->second.type;
        }
        // If it's not a literal and not a declared variable, assume it's a string
        return VarType::STRING;
    }
}

void Parser::incrementFor(
    const std::string& loopVar, const std::string& incTemp
) {
    addCommand(Command::Mnemonic::ADD, "TEMP:" + incTemp, loopVar, "1");
    addCommand(Command::Mnemonic::ASSIGN, loopVar, "TEMP:" + incTemp);
}

// Semantic analysis methods
void Parser::validateComparison(const std::string& left, const std::string& right, const std::string& operator_) {
    VarType leftType = getValueType(left);
    VarType rightType = getValueType(right);
    
    if (!areTypesComparable(leftType, rightType)) {
        addTypeError(
            "Semantic error: Cannot compare " + Printer::varTypeToString(leftType) + 
            " with " + Printer::varTypeToString(rightType) + " using operator '" + operator_ + "'"
        );
    }
}

void Parser::validateVariableExists(const std::string& varName) {
    auto it = m_variableTypes.find(varName);
    if (it == m_variableTypes.end()) {
        addTypeError("Semantic error: Variable '" + varName + "' is not declared");
    }
}

Command::ReadType Parser::getReadTypeForVariable(const std::string& varName) {
    auto it = m_variableTypes.find(varName);
    if (it != m_variableTypes.end()) {
        switch (it->second.type) {
            case VarType::INTEGER:
                return Command::ReadType::INTEGER;
            case VarType::REAL:
                return Command::ReadType::REAL;
            case VarType::STRING:
                return Command::ReadType::STRING;
        }
    }
    // Fallback para string se variável não encontrada
    return Command::ReadType::STRING;
}

bool Parser::areTypesComparable(VarType leftType, VarType rightType) {
    // Tipos iguais são sempre comparáveis
    if (leftType == rightType) {
        return true;
    }
    
    // Integer e Real são comparáveis entre si
    if ((leftType == VarType::INTEGER || leftType == VarType::REAL) &&
        (rightType == VarType::INTEGER || rightType == VarType::REAL)) {
        return true;
    }
    
    // String só é comparável com String
    if (leftType == VarType::STRING || rightType == VarType::STRING) {
        return leftType == VarType::STRING && rightType == VarType::STRING;
    }
    
    return false;
}
