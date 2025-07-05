#ifndef INTERMEDIATE_CODE_HPP
#define INTERMEDIATE_CODE_HPP

#include <string>
#include <vector>
#include <memory>

// Enumeração das operações do código intermediário
enum class OpCode {
    // Operações de movimento e atribuição
    MOV,        // Movimento de valor
    ASSIGN,     // Atribuição

    // Operações aritméticas
    ADD,        // Adição
    SUB,        // Subtração
    MUL,        // Multiplicação
    DIV,        // Divisão
    MOD,        // Módulo

    // Operações de comparação
    EQL,        // Igual
    NEQ,        // Diferente
    GRT,        // Maior que
    GEQ,        // Maior ou igual
    LSS,        // Menor que
    LEQ,        // Menor ou igual

    // Operações lógicas
    AND,        // E lógico
    OR,         // OU lógico
    NOT,        // NÃO lógico

    // Controle de fluxo
    IF,         // Condicional
    JUMP,       // Salto incondicional
    LABEL,      // Rótulo

    // Entrada/Saída
    READ,       // Leitura
    WRITE,      // Escrita
    WRITELN,    // Escrita com quebra de linha
    READLN,     // Leitura com quebra de linha

    // Chamadas de função
    CALL,       // Chamada de função

    // Operações especiais
    NOP         // Operação vazia
};

// Estrutura para representar uma instrução intermediária
struct Instruction {
    OpCode opcode;
    std::string operand1;
    std::string operand2;
    std::string operand3;

    Instruction(OpCode op, const std::string& op1 = "",
                const std::string& op2 = "", const std::string& op3 = "")
        : opcode(op), operand1(op1), operand2(op2), operand3(op3) {}
};

// Classe para gerenciar o código intermediário
class IntermediateCode {
private:
    std::vector<Instruction> instructions;
    int tempCounter = 0;
    int labelCounter = 0;

public:
    IntermediateCode() = default;
    ~IntermediateCode() = default;

    void addInstruction(OpCode opcode, const std::string& op1 = "",
                       const std::string& op2 = "", const std::string& op3 = "");

    std::string generateTemp();
    std::string generateLabel();


    void printCode() const;
    std::string opcodeToString(OpCode opcode) const;

    const std::vector<Instruction>& getInstructions() const { return instructions; }
    std::vector<Instruction>& getInstructions() { return instructions; }

    void clear() { instructions.clear(); tempCounter = 0; labelCounter = 0; }
};

#endif // INTERMEDIATE_CODE_HPP