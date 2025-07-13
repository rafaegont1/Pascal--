#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"
#include "Pascal--/lexical/TokenType.hpp"
#include "Pascal--/interpreter/Command.hpp"
#include <vector>
#include <string>
#include <unordered_map>

// Type system
enum class VarType : uint8_t { INTEGER, REAL, STRING };
enum class WriteNewLine : uint8_t { YES, NO };

struct VariableInfo {
    VarType type;
    std::string name;

    VariableInfo() : type(VarType::INTEGER), name("") {}
    VariableInfo(VarType t, const std::string& n) : type(t), name(n) {}
};

class Parser {
public:
    Parser(const std::vector<Lexeme>& lexemes);
    virtual ~Parser();

    void start();
    // Public interface
    const std::vector<Command>& getCommands() const { return m_commands; }
    const std::unordered_map<std::string, VariableInfo>& getVariableTypes() const { return m_variableTypes; }

private:
    std::vector<Lexeme>::const_iterator m_lexeme;
    std::vector<Command> m_commands;
    std::vector<std::string> m_expressionStack;
    std::unordered_map<std::string, VariableInfo> m_variableTypes;
    int m_tempCounter = 0;
    int m_labelCounter = 0;

    // Métodos auxiliares para geração de código intermediário
    std::string popExpression();
    void pushExpression(const std::string& expr);
    std::string generateTemp();
    std::string generateLabel();
    std::string generateEndLabel();

    // Simplified code generation methods
    void addCommand(Command::Mnemonic mnemonic, const std::string& dst = "",
                   const std::string& src1 = "", const std::string& src2 = "");
    void addCommand(Command::Mnemonic mnemonic, Command::CallType callType,
                   const std::string& src1, Command::ReadType readType);
    void addCommand(Command::Mnemonic mnemonic, Command::CallType callType,
                   const std::string& src1, Command::WriteType writeType);

    // Simple utility methods
    bool isIntegerLiteral(const std::string& str);
    bool isRealLiteral(const std::string& str);
    bool isStringLiteral(const std::string& str);

    // Type checking methods
    void validateExpr(const std::string& lhs, const std::string& rhs);
    void validateAssignment(const std::string& varName, const std::string& value);
    bool isTypeCompatible(VarType type1, VarType type2);
    VarType getValueType(const std::string& value);
    
    // Semantic analysis methods
    void validateComparison(const std::string& left, const std::string& right, const std::string& operator_);
    bool varNameExists(const std::string& varName);
    Command::ReadType getReadTypeForVariable(const std::string& varName);

    void incrementFor(const std::string& loopVar, const std::string& incTemp);

    void consume(enum TokenType expected);

    // ------------------------------------
    //  main function
    // ------------------------------------
    void proc_function();
    // ------------------------------------
    //  variable declarations
    // ------------------------------------
    void proc_declarations();
    VarType proc_type();
    void proc_declaration();
    void proc_listIdent(std::vector<std::string>& varNames);
    void proc_restIdentList(std::vector<std::string>& varNames);
    void proc_restDeclaration();
    // ------------------------------------
    //  program statements
    // ------------------------------------
    void proc_block(
        const std::string& startLabel = "", const std::string& endLabel = "",
        const std::string& loopVar = "", const std::string& incTemp = ""
    );
    void proc_stmtList(
        const std::string& startLabel = "", const std::string& endLabel = "",
        const std::string& loopVar = "", const std::string& incTemp = ""
    );
    void proc_stmt(
        const std::string& startLabel = "", const std::string& endLabel = "",
        const std::string& loopVar = "", const std::string& incTemp = "");
    // ---------------------------
    //  statement descriptions
    // ---------------------------
    // for command
    void proc_forStmt();
    void proc_endFor();
    // IO commands
    void proc_ioStmt();
    void proc_outList(WriteNewLine new_line);
    void proc_restoOutList(WriteNewLine new_line);
    void proc_out(WriteNewLine new_line);
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
