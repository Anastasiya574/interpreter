#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <vector>
#include <string>
#include <map>
#include "lexem.hpp"

void init_labels(vector<Lexem *> &infix, int row);

void init_loops(vector<vector<Lexem *>> &infix);

void init_function_start_vars(vector<Lexem *> &infix, int &i, map<string, set<string>> &func_var_table);

void init_functions(vector<vector<Lexem *>> &infix);

int evaluate_postfix(vector<vector<Lexem *>> &postfix, int row, Function **function_field, Number **res);

#endif
