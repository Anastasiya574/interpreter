#ifndef LEXICAL_HPP
#define LEXICAL_HPP

#include <vector>
#include <string>
#include <map>
#include "lexem.hpp"

Lexem *is_oper(string codeline, int &i, int size);

Lexem *is_number(string codeline, int &i, int size);

Lexem *is_variable(string codeline, int &i, int size);

vector<Lexem *> parse_lexem(string codeline);

#endif
