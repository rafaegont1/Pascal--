#include <iostream>
#include <functional>
#include <stdexcept>
#include "Pascal--/interpreter/Interpreter.hpp"
#include "Pascal--/syntactic/Parser.hpp"

Interpreter::Interpreter() {
}

void Interpreter::execute(const std::vector<Command>& commands) {
    execute(commands, std::unordered_map<std::string, VariableInfo>{});
}

void Interpreter::execute(
    const std::vector<Command>& commands,
    const std::unordered_map<std::string, VariableInfo>& variableTypes
) {
    m_variables.clear();
    m_labels.clear();
    m_variableTypes = variableTypes;

    // Build label map for jumps
    buildLabelMap(commands);

    // Execute commands
    for (auto current = commands.begin(); current != commands.end(); ++current) {
        const Command& cmd = *current;

        switch (cmd.mnemonic) {
            case Command::Mnemonic::ASSIGN:
                executeAssignment(cmd);
                break;

            case Command::Mnemonic::ADD:
            case Command::Mnemonic::SUB:
            case Command::Mnemonic::MUL:
            case Command::Mnemonic::DIV:
            case Command::Mnemonic::MOD:
            case Command::Mnemonic::IDIV:
                executeArithmetic(cmd);
                break;

            case Command::Mnemonic::EQL:
            case Command::Mnemonic::NEQ:
            case Command::Mnemonic::LSS:
            case Command::Mnemonic::LEQ:
            case Command::Mnemonic::GTR:
            case Command::Mnemonic::GEQ:
                executeComparison(cmd);
                break;

            case Command::Mnemonic::AND:
            case Command::Mnemonic::OR:
            case Command::Mnemonic::NOT:
                executeLogical(cmd);
                break;

            case Command::Mnemonic::IF:
            case Command::Mnemonic::JMP:
            case Command::Mnemonic::LABEL:
                executeControlFlow(cmd, current);
                break;

            case Command::Mnemonic::CALL:
                executeIO(cmd);
                break;
        }
    }
}

void Interpreter::buildLabelMap(const std::vector<Command>& commands) {
    for (auto it = commands.begin(); it != commands.end(); ++it) {
        if (it->mnemonic == Command::Mnemonic::LABEL) {
            const std::string& labelName = std::get<std::string>(it->dst);
            m_labels[labelName] = it;
        }
    }
}

Interpreter::VarValue Interpreter::resolveOperand(const Command::Source& operand) {
    return std::visit([this](const auto& arg) -> VarValue {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::string>) {
            // Check if it's a variable name
            auto it = m_variables.find(arg);
            if (it != m_variables.end()) {
                return it->second;
            }

            // Try to parse as number with different bases
            try {
                // Check for hexadecimal (0x prefix)
                if (arg.size() > 2 && arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X')) {
                    return static_cast<int64_t>(std::stoul(arg, nullptr, 16));
                }
                // Check for octal (leading 0)
                else if (arg.size() > 1 && arg[0] == '0' && arg.find('.') == std::string::npos) {
                    return static_cast<int64_t>(std::stoul(arg, nullptr, 8));
                }
                // Regular decimal number
                else {
                    size_t pos;
                    double d = std::stod(arg, &pos);
                    // If entire string was consumed, it's a pure number
                    if (pos == arg.size()) {
                        // Check if it's integer or real
                        if (arg.find('.') != std::string::npos || 
                            arg.find('e') != std::string::npos ||
                            arg.find('E') != std::string::npos) {
                            return d;
                        } else {
                            return static_cast<int64_t>(d);
                        }
                    }
                }
            } catch (...) {
                // Parsing failed, treat as string
            }

            // Not a number format, treat as string literal
            return arg;
        } else if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, double>) {
            return arg;
        } else if constexpr (std::is_same_v<T, std::monostate>) {
            throw std::runtime_error("cannot resolve monostate operand");
        } else {
            throw std::runtime_error("unsupported operand type");
        }
    }, operand);
}

void Interpreter::validateAssignment(const std::string& varName, const VarValue& value) {
    auto it = m_variableTypes.find(varName);
    if (it != m_variableTypes.end()) {
        if (!isTypeCompatible(it->second.type, value)) {
            throw std::runtime_error("type mismatch in assignment for '"
                + varName + "' variable");
        }
    }
}

bool Interpreter::isTypeCompatible(VarType expectedType, const VarValue& value) {
    return std::visit([expectedType](const auto& val) -> bool {
        using T = std::decay_t<decltype(val)>;

        if constexpr (std::is_arithmetic_v<T>) {
            return expectedType == VarType::INTEGER
                || expectedType == VarType::REAL;
        } else if constexpr (std::is_same_v<T, std::string>) {
            return expectedType == VarType::STRING;
        } else {
            return false;
        }
    }, value);
}

void Interpreter::executeAssignment(const Command& cmd) {
    const std::string& varName = std::get<std::string>(cmd.dst);
    VarValue value = resolveOperand(cmd.src1);

    validateAssignment(varName, value);
    m_variables[varName] = value;
}

void Interpreter::executeArithmetic(const Command& cmd) {
    const std::string& resultVar = std::get<std::string>(cmd.dst);
    VarValue left = resolveOperand(cmd.src1);
    VarValue right = resolveOperand(cmd.src2);

    VarValue result;
    switch (cmd.mnemonic) {
        case Command::Mnemonic::ADD:
            result = addValues(left, right);
            break;
        case Command::Mnemonic::SUB:
            result = subtractValues(left, right);
            break;
        case Command::Mnemonic::MUL:
            result = multiplyValues(left, right);
            break;
        case Command::Mnemonic::DIV:
            result = divideValues(left, right);
            break;
        case Command::Mnemonic::MOD:
            result = moduloValues(left, right);
            break;
        case Command::Mnemonic::IDIV:
            result = integerDivideValues(left, right);
            break;
        default:
            throw std::runtime_error("unknown arithmetic operation (mnemonic "
                + std::to_string((uint8_t)cmd.mnemonic) + ")");
    }

    m_variables[resultVar] = result;
}

void Interpreter::executeComparison(const Command& cmd) {
    const std::string& resultVar = std::get<std::string>(cmd.dst);
    VarValue left = resolveOperand(cmd.src1);
    VarValue right = resolveOperand(cmd.src2);

    bool result = false;
    switch (cmd.mnemonic) {
        case Command::Mnemonic::EQL:
            result = (left == right);
            break;
        case Command::Mnemonic::NEQ:
            result = (left != right);
            break;
        case Command::Mnemonic::LSS:
            result = compareLess(left, right);
            break;
        case Command::Mnemonic::LEQ:
            result = compareLessEqual(left, right);
            break;
        case Command::Mnemonic::GTR:
            result = compareGreater(left, right);
            break;
        case Command::Mnemonic::GEQ:
            result = compareGreaterEqual(left, right);
            break;
        default:
            throw std::runtime_error("unknown comparison operation (mnemonic "
                + std::to_string((uint8_t)cmd.mnemonic) + ")");
    }

    m_variables[resultVar] = static_cast<int64_t>(result);
}

void Interpreter::executeLogical(const Command& cmd) {
    const std::string& resultVar = std::get<std::string>(cmd.dst);

    if (cmd.mnemonic == Command::Mnemonic::NOT) {
        VarValue operand = resolveOperand(cmd.src1);
        bool value = toBool(operand);
        m_variables[resultVar] = static_cast<int64_t>(!value);
    } else {
        VarValue left = resolveOperand(cmd.src1);
        VarValue right = resolveOperand(cmd.src2);

        bool leftBool = toBool(left);
        bool rightBool = toBool(right);

        bool result = false;
        if (cmd.mnemonic == Command::Mnemonic::AND) {
            result = leftBool && rightBool;
        } else if (cmd.mnemonic == Command::Mnemonic::OR) {
            result = leftBool || rightBool;
        }

        m_variables[resultVar] = static_cast<int64_t>(result);
    }
}

void Interpreter::executeControlFlow(
    const Command& cmd,
    std::vector<Command>::const_iterator& current
) {
    if (cmd.mnemonic == Command::Mnemonic::LABEL) {
        // Labels are just markers, no action needed
        return;
    }

    if (cmd.mnemonic == Command::Mnemonic::JMP) {
        const std::string& labelName = std::get<std::string>(cmd.dst);
        auto it = m_labels.find(labelName);
        if (it != m_labels.end()) {
            current = it->second;
        } else {
            throw std::runtime_error("label '" + labelName + "' not found");
        }
        return;
    }

    if (cmd.mnemonic == Command::Mnemonic::IF) {
        const std::string& conditionVar = std::get<std::string>(cmd.dst);
        const std::string& trueLabel = std::get<std::string>(cmd.src1);
        const std::string& falseLabel = std::get<std::string>(cmd.src2);

        auto it = m_variables.find(conditionVar);
        if (it == m_variables.end()) {
            throw std::runtime_error("condition variable '" + conditionVar
                + "' not found");
        }

        bool condition = toBool(it->second);
        const std::string& targetLabel = condition ? trueLabel : falseLabel;

        auto labelIt = m_labels.find(targetLabel);
        if (labelIt != m_labels.end()) {
            current = labelIt->second;
        } else {
            throw std::runtime_error("label '" + targetLabel + "' not found");
        }
    }
}

void Interpreter::executeIO(const Command& cmd) {
    auto callType = std::get<Command::CallType>(cmd.dst);
    const std::string& operand = std::get<std::string>(cmd.src1);

    switch (callType) {
        case Command::CallType::READ: {
            switch (m_variableTypes[operand].type) {
                case VarType::INTEGER: {
                    int64_t value;
                    std::cin >> value;
                    m_variables[operand] = value;
                    break;
                }
                case VarType::REAL: {
                    double value;
                    std::cin >> value;
                    m_variables[operand] = value;
                    break;
                }
                case VarType::STRING: {
                    std::string value;
                    std::cin >> value;
                    m_variables[operand] = value;
                    break;
                }
            }
            break;
        }
        case Command::CallType::WRITE: {
            auto it = m_variables.find(operand);
            if (it != m_variables.end()) {
                printValue(it->second);
            } else {
                std::cout << operand;
            }
            break;
        }
        case Command::CallType::READLN: {
            switch (m_variableTypes[operand].type) {
                case VarType::INTEGER: {
                    int64_t value;
                    std::cin >> value;
                    m_variables[operand] = value;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                case VarType::REAL: {
                    double value;
                    std::cin >> value;
                    m_variables[operand] = value;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                case VarType::STRING: {
                    std::string value;
                    std::getline(std::cin, value);
                    m_variables[operand] = value;
                    break;
                }
            }
            break;
        }
        case Command::CallType::WRITELN: {
            auto it = m_variables.find(operand);
            if (it != m_variables.end()) {
                printValue(it->second);
            } else {
                std::cout << operand;
            }
            std::cout << std::endl;
            break;
        }
    }
}

// Simple arithmetic operations - much more readable
Interpreter::VarValue Interpreter::addValues(const VarValue& left, const VarValue& right) {
    // Try integer addition first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l + r;
    }
    // Try string concatenation
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        std::string l = std::get<std::string>(left);
        std::string r = std::get<std::string>(right);
        return l + r;
    }
    // Fall back to double addition
    return toDouble(left) + toDouble(right);
}

Interpreter::VarValue Interpreter::subtractValues(const VarValue& left, const VarValue& right) {
    // Try integer subtraction first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l - r;
    }
    // Fall back to double subtraction
    return toDouble(left) - toDouble(right);
}

Interpreter::VarValue Interpreter::multiplyValues(const VarValue& left, const VarValue& right) {
    // Try integer multiplication first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l * r;
    }
    // Fall back to double multiplication
    return toDouble(left) * toDouble(right);
}

Interpreter::VarValue Interpreter::divideValues(const VarValue& left, const VarValue& right) {
    double l = toDouble(left);
    double r = toDouble(right);
    if (r == 0.0) {
        throw std::runtime_error("division by zero");
    }
    return l / r;
}

Interpreter::VarValue Interpreter::moduloValues(const VarValue& left, const VarValue& right) {
    int64_t l = toInt(left);
    int64_t r = toInt(right);
    if (r == 0) {
        throw std::runtime_error("modulo by zero");
    }
    return l % r;
}

Interpreter::VarValue Interpreter::integerDivideValues(const VarValue& left, const VarValue& right) {
    int64_t l = toInt(left);
    int64_t r = toInt(right);
    if (r == 0) {
        throw std::runtime_error("integer division by zero");
    }
    return l / r;
}

// Simple comparison operations - much more readable
bool Interpreter::compareLess(const VarValue& left, const VarValue& right) {
    // Try integer comparison first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l < r;
    }
    // Try string comparison
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        std::string l = std::get<std::string>(left);
        std::string r = std::get<std::string>(right);
        return l < r;
    }
    // Fall back to double comparison
    return toDouble(left) < toDouble(right);
}

bool Interpreter::compareLessEqual(const VarValue& left, const VarValue& right) {
    // Try integer comparison first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l <= r;
    }
    // Try string comparison
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        std::string l = std::get<std::string>(left);
        std::string r = std::get<std::string>(right);
        return l <= r;
    }
    // Fall back to double comparison
    return toDouble(left) <= toDouble(right);
}

bool Interpreter::compareGreater(const VarValue& left, const VarValue& right) {
    // Try integer comparison first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l > r;
    }
    // Try string comparison
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        std::string l = std::get<std::string>(left);
        std::string r = std::get<std::string>(right);
        return l > r;
    }
    // Fall back to double comparison
    return toDouble(left) > toDouble(right);
}

bool Interpreter::compareGreaterEqual(const VarValue& left, const VarValue& right) {
    // Try integer comparison first
    if (std::holds_alternative<int64_t>(left) && std::holds_alternative<int64_t>(right)) {
        int64_t l = std::get<int64_t>(left);
        int64_t r = std::get<int64_t>(right);
        return l >= r;
    }
    // Try string comparison
    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        std::string l = std::get<std::string>(left);
        std::string r = std::get<std::string>(right);
        return l <= r;
    }
    // Fall back to double comparison
    return toDouble(left) >= toDouble(right);
}

// Simple type conversion methods - much more readable
bool Interpreter::toBool(const VarValue& value) {
    if (std::holds_alternative<int64_t>(value)) {
        return std::get<int64_t>(value) != 0;
    }
    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value) != 0.0;
    }
    if (std::holds_alternative<std::string>(value)) {
        return !std::get<std::string>(value).empty();
    }
    return false;
}

int64_t Interpreter::toInt(const VarValue& value) {
    if (std::holds_alternative<int64_t>(value)) {
        return std::get<int64_t>(value);
    }
    if (std::holds_alternative<double>(value)) {
        return static_cast<int64_t>(std::get<double>(value));
    }
    if (std::holds_alternative<std::string>(value)) {
        try {
            return std::stoll(std::get<std::string>(value));
        } catch (...) {
            return 0;
        }
    }
    return 0;
}

double Interpreter::toDouble(const VarValue& value) {
    if (std::holds_alternative<int64_t>(value)) {
        return static_cast<double>(std::get<int64_t>(value));
    }
    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value);
    }
    if (std::holds_alternative<std::string>(value)) {
        try {
            return std::stod(std::get<std::string>(value));
        } catch (...) {
            return 0.0;
        }
    }
    return 0.0;
}

std::string Interpreter::toString(const VarValue& value) {
    if (std::holds_alternative<int64_t>(value)) {
        return std::to_string(std::get<int64_t>(value));
    }
    if (std::holds_alternative<double>(value)) {
        return std::to_string(std::get<double>(value));
    }
    if (std::holds_alternative<std::string>(value)) {
        return std::get<std::string>(value);
    }
    return "";
}

void Interpreter::printValue(const VarValue& value) const {
    if (std::holds_alternative<int64_t>(value)) {
        std::cout << std::get<int64_t>(value);
    } else if (std::holds_alternative<double>(value)) {
        std::cout << std::get<double>(value);
    } else if (std::holds_alternative<std::string>(value)) {
        std::cout << std::get<std::string>(value);
    } else {
        std::cout << "unknown";
    }
}

void Interpreter::printState() const {
    std::cout << "\nINTERPRETER STATE" << std::endl;
    std::cout << "=================" << std::endl;
    for (const auto& [name, value] : m_variables) {
        std::cout << name << " = ";
        printValue(value);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
