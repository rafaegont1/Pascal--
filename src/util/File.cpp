#include "pascal--/util/File.hpp"

#include <fstream>

File::File() : m_start(nullptr), m_current(nullptr), m_line(0), m_column(0) {
}

File::File(const char* filename) {
    open(filename);
}

File::~File() {
    close();
}

static const char* getText(const char* filename) {
    std::ifstream text(filename, std::ios::binary);

    // Throw exception if couldn't open file
    if (!text.is_open()) {
        throw std::string("couldn't open file " + std::string(filename));
    }

    // Determine the size of the file in bytes
    text.seekg(0, std::ios::end);
    std::streamsize textSize = text.tellg();
    text.seekg(0, std::ios::beg);

    // Allocate memory for the wchar_t buffer
    char* buffer = new char[textSize];

    // Read the file contents into the buffer
    text.read(buffer, textSize);

    // Set the EOF as null character '\0'
    buffer[textSize-1] = '\0';

    // Close the file
    text.close();

    return buffer;
}

void File::open(const char* filename) {
    const char* buffer = getText(filename);

    m_start = buffer;
    m_current = buffer;
    m_line = 1;
    m_column = 1;
}

void File::close() {
    if (isOpen()) {
        delete[] m_start;
        m_start = nullptr;
        m_current = nullptr;
        m_line = 0;
        m_column = 0;
    }
}

void File::rewind() {
    m_current = m_start;
    m_line = 1;
    m_column = 1;
}

bool File::isOpen() const {
    return m_start != nullptr;
}

bool File::isAtEOF() const {
    return *m_current == '\0';
}

char File::advance() {
    char c;

    if (isAtEOF()) {
        c = '\0';
    } else {
        c = *(++m_current);

        if (peekPrev() == '\n') {
            m_line++;
            m_column = 1;
        } else {
            m_column++;
        }
    }

    return c;
}

char File::peek() const {
    return *m_current;
}

char File::peekNext() const {
    return !isAtEOF() ? *(m_current + 1) : '\0';
}

char File::peekPrev() const {
    return (m_current != m_start) ? *(m_current - 1) : '\0';
}
