#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "Pascal--/interpreter/Command.hpp"
#include "Pascal--/syntactic/Parser.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <variant>
#include <functional>
#include <limits>

class Interpreter {
public:
    using VarValue = std::variant<int64_t, double, std::string>;

    Interpreter();
    ~Interpreter() = default;

    // Execute commands
    void execute(const std::vector<Command>& commands);
    void execute(const std::vector<Command>& commands,
        const std::unordered_map<std::string, VariableInfo>& variableTypes);

    // Get execution results
    const std::unordered_map<std::string, VarValue>& getVariables() const { return m_variables; }

    // Print current state
    void printState() const;

private:
    std::unordered_map<std::string, VarValue> m_variables;
    std::unordered_map<std::string, std::vector<Command>::const_iterator> m_labels;
    std::unordered_map<std::string, VariableInfo> m_variableTypes;

    // Helper methods
    void buildLabelMap(const std::vector<Command>& commands);
    VarValue resolveOperand(const Command::Source& operand);
    void validateAssignment(const std::string& varName, const VarValue& value);
    bool isTypeCompatible(VarType expectedType, const VarValue& value);

    // Command execution methods
    void executeAssignment(const Command& cmd);
    void executeArithmetic(const Command& cmd);
    void executeComparison(const Command& cmd);
    void executeLogical(const Command& cmd);
    void executeControlFlow(const Command& cmd, std::vector<Command>::const_iterator& current);
    void executeIO(const Command& cmd);

    // Simple arithmetic operations - no templates
    VarValue addValues(const VarValue& left, const VarValue& right);
    VarValue subtractValues(const VarValue& left, const VarValue& right);
    VarValue multiplyValues(const VarValue& left, const VarValue& right);
    VarValue divideValues(const VarValue& left, const VarValue& right);
    VarValue moduloValues(const VarValue& left, const VarValue& right);
    VarValue integerDivideValues(const VarValue& left, const VarValue& right);

    // Simple comparison operations
    bool compareLess(const VarValue& left, const VarValue& right);
    bool compareLessEqual(const VarValue& left, const VarValue& right);
    bool compareGreater(const VarValue& left, const VarValue& right);
    bool compareGreaterEqual(const VarValue& left, const VarValue& right);

    // Simple type conversion methods
    bool toBool(const VarValue& value);
    int64_t toInt(const VarValue& value);
    double toDouble(const VarValue& value);
    std::string toString(const VarValue& value);

    // Simple utility methods
    void printValue(const VarValue& value) const;
};

#endif // INTERPRETER_HPP
