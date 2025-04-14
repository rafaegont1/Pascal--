#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

#include "pascal--/lexical/TokenType.hpp"

namespace SymbolTable {

/**
 *  @brief Checks if a token exists in the symbol table.
 *  @param token The string to search for in the symbol table.
 *  @return true if the token is a known symbol/keyword, false otherwise.
 */
bool contains(const std::string& token);

/**
 *  @brief Looks up a token in the symbol table.
 *  @param token The string to search for in the symbol table.
 *  @return The corresponding TokenType if found, TT_VAR_NAME otherwise.
 */
enum TokenType find(const std::string& token);

} // namespace SymbolTable

#endif // SYMBOLS_HPP
