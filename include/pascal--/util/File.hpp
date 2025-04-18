#ifndef FILE_HPP
#define FILE_HPP

class File {
public:
    File();
    File(const char* filename);
    virtual ~File();

    void open(const char* filename);
    void close();
    void rewind();
    [[nodiscard]] bool isOpen() const;
    [[nodiscard]] bool isAtEOF() const;
    char advance();
    [[nodiscard]] char peek() const;
    [[nodiscard]] char peekNext() const;
    [[nodiscard]] char peekPrev() const;

    [[nodiscard]] int line() const { return m_line; }
    [[nodiscard]] int column() const { return m_column; }

private:
    const char* m_start;
    const char* m_current;
    int m_line;
    int m_column;
};

#endif // FILE_HPP
