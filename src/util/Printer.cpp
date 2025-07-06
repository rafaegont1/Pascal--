#include "Pascal--/util/Printer.hpp"
#include "Pascal--/syntactic/Parser.hpp"
#include <iostream>

void Printer::printLexemes(const std::vector<Lexeme>& lexemes) {
    std::cout << "LEXEME LIST" << std::endl;
    std::cout << "-----------" << std::endl;
    for (const auto& lexeme : lexemes) {
        std::cout << lexeme.str() << std::endl;
    }
    std::cout << std::endl;
}

void Printer::printCommands(const std::vector<Command>& commands) {
    std::cout << "COMMANDS" << std::endl;
    std::cout << "========" << std::endl;

    for (size_t i = 0; i < commands.size(); ++i) {
        const auto& cmd = commands[i];
        std::cout << i << ": ";

        // Print mnemonic
        switch (cmd.mnemonic) {
            case Command::Mnemonic::ADD: std::cout << "ADD"; break;
            case Command::Mnemonic::SUB: std::cout << "SUB"; break;
            case Command::Mnemonic::MUL: std::cout << "MUL"; break;
            case Command::Mnemonic::DIV: std::cout << "DIV"; break;
            case Command::Mnemonic::MOD: std::cout << "MOD"; break;
            case Command::Mnemonic::IDIV: std::cout << "IDIV"; break;
            case Command::Mnemonic::OR: std::cout << "OR"; break;
            case Command::Mnemonic::AND: std::cout << "AND"; break;
            case Command::Mnemonic::NOT: std::cout << "NOT"; break;
            case Command::Mnemonic::EQL: std::cout << "EQL"; break;
            case Command::Mnemonic::NEQ: std::cout << "NEQ"; break;
            case Command::Mnemonic::GTR: std::cout << "GTR"; break;
            case Command::Mnemonic::GEQ: std::cout << "GEQ"; break;
            case Command::Mnemonic::LSS: std::cout << "LSS"; break;
            case Command::Mnemonic::LEQ: std::cout << "LEQ"; break;
            case Command::Mnemonic::ASSIGN: std::cout << "ASSIGN"; break;
            case Command::Mnemonic::IF: std::cout << "IF"; break;
            case Command::Mnemonic::JMP: std::cout << "JMP"; break;
            case Command::Mnemonic::CALL: std::cout << "CALL"; break;
            case Command::Mnemonic::LABEL: std::cout << "LABEL"; break;
        }

        // Print operands
        std::cout << " ";

        // Print dst
        try {
            std::visit([](const auto& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, Command::CallType>) {
                    switch (arg) {
                        case Command::CallType::READ: std::cout << "READ"; break;
                        case Command::CallType::WRITE: std::cout << "WRITE"; break;
                        case Command::CallType::READLN: std::cout << "READLN"; break;
                        case Command::CallType::WRITELN: std::cout << "WRITELN"; break;
                    }
                }
            }, cmd.dst);
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what();
        }

        std::cout << ", ";

        // Print src1
        try {
            std::visit([](const auto& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, int64_t>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, double>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, Command::WriteType>) {
                    std::cout << (arg == Command::WriteType::STRING ? "STRING" : "VARIABLE");
                } else if constexpr (std::is_same_v<T, Command::ReadType>) {
                    switch (arg) {
                        case Command::ReadType::INTEGER: std::cout << "INTEGER"; break;
                        case Command::ReadType::REAL: std::cout << "REAL"; break;
                        case Command::ReadType::STRING: std::cout << "STRING"; break;
                    }
                } else if constexpr (std::is_same_v<T, std::monostate>) {
                    std::cout << "NONE";
                }
            }, cmd.src1);
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what();
        }

        std::cout << ", ";

        // Print src2
        try {
            std::visit([](const auto& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, int64_t>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, double>) {
                    std::cout << arg;
                } else if constexpr (std::is_same_v<T, Command::WriteType>) {
                    std::cout << (arg == Command::WriteType::STRING ? "STRING" : "VARIABLE");
                } else if constexpr (std::is_same_v<T, Command::ReadType>) {
                    switch (arg) {
                        case Command::ReadType::INTEGER: std::cout << "INTEGER"; break;
                        case Command::ReadType::REAL: std::cout << "REAL"; break;
                        case Command::ReadType::STRING: std::cout << "STRING"; break;
                    }
                } else if constexpr (std::is_same_v<T, std::monostate>) {
                    std::cout << "NONE";
                }
            }, cmd.src2);
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what();
        }

        std::cout << std::endl;
    }
}

void Printer::printVariableTypes(const std::unordered_map<std::string, VariableInfo>& variableTypes) {
    std::cout << "VARIABLE TYPES" << std::endl;
    std::cout << "==============" << std::endl;
    for (const auto& [varName, varInfo] : variableTypes) {
        std::cout << varName << ": " << varTypeToString(varInfo.type) << std::endl;
    }
    std::cout << std::endl;
}

void Printer::printTypeErrors(const std::vector<std::string>& typeErrors) {
    if (!typeErrors.empty()) {
        std::cout << std::endl;
        std::cout << "TYPE ERRORS DETECTED:" << std::endl;
        std::cout << "====================" << std::endl;
        for (const auto& error : typeErrors) {
            std::cout << error << std::endl;
        }
        std::cout << std::endl;
    }
}

std::string Printer::varTypeToString(VarType type) {
    switch (type) {
        case VarType::INTEGER: return "integer";
        case VarType::REAL: return "real";
        case VarType::STRING: return "string";
        default: return "unknown";
    }
}
