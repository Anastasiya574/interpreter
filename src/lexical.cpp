#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <cctype>

#include "const.hpp"
#include "lexem.hpp"
#include "lexical.hpp"

string ERRORTEXT[] = {
    "Entered unexisted operator",
    "Error with label",
    "Brackets are not balansed",
    "Function has not readble form",
    "Conditional or service operators are not balansed",
    "Bad function name declartion after def",
    "Error with declaration of start variables function",
    "Not correct use if/while/else",
    "Program need function main",
    "Work with undefined Variable",
    "Function was called not corect",
    "Assign problems",
    "No colons after condition while/if/else",
    "Error with collision between function and condition",
    "Invalid syntax",
    "Called non-void function in evaluation"
};

Lexem *is_oper(string codeline, int &i, int size) {
    Lexem *lex = nullptr;
    char tmp = codeline[i];
    while (i < size && (tmp == ' ' || tmp == '\t')) {
        ++i;
        tmp = codeline[i];
    }
    int n = OPER_NUMBER;
    for (int op = 0; op < n; op++) {
        bool flag_not_a_oper = false;
        string subcodeline = codeline.substr(i, OPERTEXT[op].size());
        for (int j = 0; j < 9; j++) {
            if (op == OPER_OR_VAR[j]) {
                int size_of_opertext = OPERTEXT[op].size();
                char next_symbol = ' ';
                if (i + size_of_opertext < size) {
                    next_symbol = codeline[i + size_of_opertext];
                }
                if (isdigit(next_symbol) || isalpha(next_symbol) || next_symbol == '_')
                    flag_not_a_oper = true;
                        break;
            }
		}
        if (OPERTEXT[op] == subcodeline && !flag_not_a_oper) {
            if ((OPERATOR)op == IF || (OPERATOR)op == ELSE || (OPERATOR)op == WHILE
            || (OPERATOR)op == ENDLOOP || (OPERATOR)op == RETURN) {
                lex = new Goto((OPERATOR)op);
            } else {
                lex = new Oper((OPERATOR)op);
            }
            i += OPERTEXT[op].size();
            break;
        }
    }
    return lex;
}

Lexem *is_number(string codeline, int &i, int size) {
    int p = i, number = 0;
    for ( ; p < size && codeline[p] >= '0' && codeline[p] <= '9'; p++) {
        number = number * 10 + codeline[p] - '0';
    }
    if (p == i) {
        return nullptr;
    }
    i = p;
    Lexem *lex = new Number(number);
    return lex;
}

Lexem *is_variable(string codeline, int &i, int size) {
    string variable;
    while (i < size && ((codeline[i] >= 'A' && codeline[i] <= 'Z') || (codeline[i] >= 'a' && codeline[i] <= 'z')
    || (codeline[i] >= '0' && codeline[i] <= '9') || codeline[i] == '_' )) {
        variable += codeline[i];
        i++;
    }
    if (variable.empty()) {
        return nullptr;
    }
    Lexem *lex = new Variable(variable);
    return lex;
}

vector<Lexem *> parse_lexem(string codeline) {
    codeline = codeline + ' ';
    int size = codeline.size();
    vector<Lexem *> infix;
    Lexem *lex;
    int i = 0;
    while (i < size) {
        lex = is_oper(codeline, i, size);
        if (lex != nullptr) {
            infix.push_back(lex);
            continue;
        }
        if (i == size) {
            break;
        }
        lex = is_number(codeline, i, size);
        if (lex != nullptr) {
            infix.push_back(lex);
            continue;
        }
        lex = is_variable(codeline, i, size);
        if (lex != nullptr) {
            infix.push_back(lex);
            continue;
        } else {
            throw (INVALID_SYNTAX);
        }
	}
    return infix;
}
