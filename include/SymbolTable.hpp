#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <unordered_map>
#include "TokenType.hpp"

class SymbolTable {
public:
	SymbolTable();
	virtual ~SymbolTable();

	bool contains(const std::string& token) const;
	enum TokenType find(const std::string& token) const;

private:
	static const std::unordered_map<std::string, enum TokenType> ms_symbols;
};

#endif // SYMBOL_TABLE_HPP
