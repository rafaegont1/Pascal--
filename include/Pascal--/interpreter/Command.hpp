#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <cstdint>
// #include <stdfloat>
#include <string>
#include <variant>

struct Command {
    enum class Mnemonic : uint8_t {
        // Arithmetic operators
        ADD,    // DST,        OP1,        OP2
        SUB,    // DST,        OP1,        OP2
        MUL,    // DST,        OP1,        OP2
        DIV,    // DST,        OP1,        OP2
        MOD,    // DST,        OP1,        OP2
        IDIV,   // DST,        OP1,        OP2

        // Logical, relational operators and assignments
        OR,     // DST,        OP1,        OP2
        AND,    // DST',       OP1,        OP2
        NOT,    // DST',       OP1,        NONE
        EQL,    // DST,        OP1,        OP2
        NEQ,    // DST,        OP1,        OP2
        GTR,    // DST,        OP1,        OP2
        GEQ,    // DST,        OP1,        OP2
        LSS,    // DST,        OP1,        OP2
        LEQ,    // DST,        OP1,        OP2
        ASSIGN, // DST,        VALOR,      NONE

        // Flow change functions
        IF,     // COND,       LABEL-TRUE, LABEL-FALSE
        JMP,    // LABEL-NAME, NONE,       NONE

        // System calls
        CALL,   // READ/WRITE, DST,        NONE

        // Labels
        LABEL,  // LABEL-NAME, NONE,       NONE
    };
    enum class CallType : uint8_t { READ, WRITE, READLN, WRITELN };
    enum class WriteType : uint8_t { STRING, VARIABLE };
    enum class ReadType : uint8_t { INTEGER, REAL, STRING };

    using Target = std::variant<std::string, CallType>;
    using Source = std::variant<
        std::monostate, // NONE
        std::int64_t, double, std::string, // Variable
        WriteType, ReadType // Call modifiers
    >;

    Mnemonic mnemonic;
    Target dst;
    Source src1;
    Source src2;
};

#endif // COMMAND_HPP
