#include <stdexcept>

class CompilerError : public std::runtime_error {
public:
    CompilerError(const std::string& message, size_t line, size_t column)
    : std::runtime_error(message), m_line(line), m_column(column) {
    }

    size_t line() const { return m_line; }
    size_t column() const { return m_column; }

private:
    size_t m_line;
    size_t m_column;
};

class LexicalError : public CompilerError {
public:
    LexicalError(const std::string& message, size_t line, size_t column)
    : CompilerError(message, line, column) {
    }
};

class SyntaxError : public CompilerError {
public:
    SyntaxError(const std::string& message, size_t line, size_t column)
    : CompilerError(message, line, column) {
    }
};
