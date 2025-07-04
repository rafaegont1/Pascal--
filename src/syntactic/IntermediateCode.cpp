#include "Pascal--/syntactic/IntermediateCode.hpp"
#include <iostream>
#include <sstream>

void IntermediateCode::addInstruction(OpCode opcode, const std::string& op1, 
                                     const std::string& op2, const std::string& op3) {
    instructions.emplace_back(opcode, op1, op2, op3);
}

std::string IntermediateCode::generateTemp() {
    return "Temp" + std::to_string(++tempCounter);
}

std::string IntermediateCode::generateLabel() {
    return "L_" + std::to_string(++labelCounter);
}

std::string IntermediateCode::opcodeToString(OpCode opcode) const {
    switch (opcode) {
        case OpCode::MOV: return "MOV";
        case OpCode::ASSIGN: return "ASSIGN";
        case OpCode::ADD: return "ADD";
        case OpCode::SUB: return "SUB";
        case OpCode::MUL: return "MUL";
        case OpCode::DIV: return "DIV";
        case OpCode::MOD: return "MOD";
        case OpCode::EQL: return "EQL";
        case OpCode::NEQ: return "NEQ";
        case OpCode::GRT: return "GRT";
        case OpCode::GEQ: return "GEQ";
        case OpCode::LSS: return "LSS";
        case OpCode::LEQ: return "LEQ";
        case OpCode::AND: return "AND";
        case OpCode::OR: return "OR";
        case OpCode::NOT: return "NOT";
        case OpCode::IF: return "IF";
        case OpCode::JUMP: return "JUMP";
        case OpCode::LABEL: return "LABEL";
        case OpCode::READ: return "READ";
        case OpCode::WRITE: return "WRITE";
        case OpCode::WRITELN: return "WRITELN";
        case OpCode::READLN: return "READLN";
        case OpCode::CALL: return "CALL";
        case OpCode::NOP: return "NOP";
        default: return "UNKNOWN";
    }
}

void IntermediateCode::printCode() const {
    std::cout << "# Código Intermediário Gerado" << std::endl;
    std::cout << "# ==========================" << std::endl;
    
    for (size_t i = 0; i < instructions.size(); ++i) {
        const auto& instr = instructions[i];
        
    
        std::cout << "(" << opcodeToString(instr.opcode);
        
        std::cout << ", '" << (instr.operand1.empty() ? "None" : instr.operand1) << "'";
        std::cout << ", '" << (instr.operand2.empty() ? "None" : instr.operand2) << "'";
        std::cout << ", '" << (instr.operand3.empty() ? "None" : instr.operand3) << "'";
        
        
        std::cout << ")";
        
        std::cout << std::endl;
    }
} 