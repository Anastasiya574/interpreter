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
#include "semantic.hpp"

void init_labels(vector<Lexem *> &infix, int row) {
    int size = infix.size();
    for (int i = 1; i < size; ++i) {
        if (infix[i - 1] == nullptr || infix[i] == nullptr) {
            continue;
        }
        if ((dynamic_cast<Variable *>(infix[i - 1])) && (dynamic_cast<Oper *>(infix[i]))) {
            Variable *lexemvar = (Variable *)infix[i - 1];
            Oper *lexemop = (Oper *)infix[i];
            if (lexemop -> get_type() == COLON) {
                Goto::ltable[lexemvar -> get_name()] = row;
                delete infix[i - 1];
                delete infix[i];
                infix[i - 1] = nullptr;
                infix[i] = nullptr;
                i++;
            }
        }
    }
}

void init_loops(vector<vector<Lexem *>> &infix) {
    stack<bool> is_it_if_condition;
    int size = infix.size();
    stack<Goto *> loop_stack;
    for (int row = 0; row < size; row++) {
        int length = infix[row].size();
        for (int i = 0; i < length; i++) {
            if (infix[row][i] == nullptr) {
                continue;
            }
            if (dynamic_cast<Oper *>(infix [row][i])) {
                Oper *lexemoper = (Oper *)infix[row][i];
                if (lexemoper -> get_type() == IF) {
                    delete infix[row].back();
                    infix[row].pop_back();
                    length--;
                    loop_stack.push((Goto *)lexemoper);
                    is_it_if_condition.push(true);
                }
                if (lexemoper -> get_type() == WHILE) {
                    delete infix[row].back();
                    infix[row].pop_back();
                    length--;
                    Goto *lexemgoto = (Goto *)lexemoper;
                    lexemgoto -> set_row(row);
                    loop_stack.push(lexemgoto);
                    is_it_if_condition.push(false);
                }
                if (lexemoper -> get_type() == ELSE) {
                    delete infix[row].back();
                    infix[row].pop_back();
                    length--;
                    loop_stack.top() -> set_row(row + 1);
                    loop_stack.pop();
                    loop_stack.push((Goto *)lexemoper);
                }
                if (lexemoper -> get_type() == ENDLOOP) {
                    if (!(is_it_if_condition.top())) {
                        Goto *lexemgoto = (Goto*)lexemoper;
                        lexemgoto -> set_row(loop_stack.top() -> get_row());
                    } else {
                    Goto *lexemgoto = (Goto*)lexemoper;
                        lexemgoto -> set_row(row + 1);
                    }
                    loop_stack.top() -> set_row(row + 1);
                    loop_stack.pop();
                    is_it_if_condition.pop();
                }
            }
        }
    }
    if (!loop_stack.empty()) {
        throw (ERR_NOT_BALANCED_CONDITIONAL_OPERATORS);
    }
}

void init_function_start_vars(vector<Lexem *> &infix, int &i, map<string, set<string>> &func_var_table) {
    int length = infix.size();
    Function *function = (Function *)infix[i];
    i += 2;
    bool correct_func = false;
    int start_var_counter = 0, point = i;
    while (i < length) {
        if (infix[i] && dynamic_cast<Oper *>(infix[i]) 
        && (((Oper *)infix[i]) -> get_type() == RBRACKET)) {
            correct_func = true;
            break;
        }
        if (dynamic_cast<Function *>(infix[i])) {
            init_function_start_vars(infix, i, func_var_table);
            continue;
        }
        if (dynamic_cast<Oper *>(infix[i])
        && (((Oper *)infix[i]) -> get_type() == COMMA)) {
            start_var_counter++;
        }
        i++;
    }
    if (start_var_counter || i > point) {
        start_var_counter++;
    }
    i++;
    if (!correct_func || start_var_counter != function -> get_num_of_start_vars()) {
        throw (ERR_UNDEFINED_FUNCTION);
    }
    for (auto &j: func_var_table[function -> get_name()]) {
        function -> add_var(j);
    }
}

void init_functions(vector<vector<Lexem *>> &infix) {
    stack<int> err_function_stack;
    map<string, int> f_st_var;
    map<string, set<string>> func_var_table;
    for (int row = 0; row < (int)infix.size(); row++) {
        int length = infix[row].size();
        for (int i = 0; i < length; i++) {
            if (infix[row][i] == nullptr) {
                continue;
            }
            if (dynamic_cast<Oper *>(infix[row][i])) { 
                if (((Oper *)infix[row][i]) -> get_type() == FUNCTION) {
                    string function_name = ((Variable *)infix[row][i + 1]) -> get_name();
                    Function::ftable[function_name] = row + 1;
                    delete infix[row][i];
                    delete infix[row][i + 1];
                    infix[row][i] = nullptr;
                    infix[row][i + 1] = nullptr;
                    i += 2;
                    while (i < length && !infix[row][i]) {
                        i++;
                    }
                    int start_point_to_del = i, start_var_counter = 0;
                    i++;
                    length--;
                    bool oper0_var1 = true;
                    while (i < length) {
                        if (oper0_var1 && dynamic_cast<Variable *>(infix[row][i])) {
                            func_var_table[function_name].insert(((Variable *)infix[row][i]) -> get_name());
                            start_var_counter++;
                            oper0_var1 = false;
                        } else if (!oper0_var1 && dynamic_cast<Oper *>(infix[row][i]) 
                        && (((Oper *)infix[row][i]) -> get_type() == COMMA)) {
                            oper0_var1 = true;
                        }
                        i++;
                    }
                    f_st_var[function_name] = start_var_counter;
                    length++;
                    for (int j = start_point_to_del; j < length; j++) {
                        delete infix[row].back();
                        infix[row].pop_back();
                    }
                    err_function_stack.push(row);
                } else if (((Oper *)infix[row][i]) -> get_type() == ENDFUNCTION) {
                    for (int j = i; j < length; j++) {
                        delete infix[row][i];
                        infix[row].pop_back();
                    }
                    err_function_stack.pop();
                } else if (((Oper *)infix[row][i]) -> get_type() == RETURN) {
                    ((Goto *)infix[row][i]) -> set_row(FUNCTION_RETURN);
                }
            } else if ((dynamic_cast<Variable *>(infix[row][i])) && (i + 1 < length)) {
                Variable *var = (Variable *)infix[row][i];
                string var_name = var -> get_name();
                if ((dynamic_cast<Oper *>(infix[row][i + 1])) && 
                (((Oper *) infix[row][i + 1]) -> get_type() == LBRACKET)) {
                    if (Function::ftable.find(var_name) != Function::ftable.end()) {
                        Function *function = new Function(var_name, Function::ftable[var_name], f_st_var[var_name]);
                        delete infix[row][i];
                        infix[row][i] = function;
                    }
                    i += 2;
                } 
            }
        }
    }
    if (!err_function_stack.empty()) {
        throw (ERR_NOT_BALANCED_FUNCTION);
    }
    for (int row = 0; row < (int)infix.size(); row++) {
        int length = infix[row].size();
        for (int i = 0 ; i < length; ) {
            if (infix[row][i] == nullptr) {
                i++;
                continue;
            }
            if (dynamic_cast<Function *>(infix[row][i])) {	
                try {
                    init_function_start_vars(infix[row], i, func_var_table);
                } catch (ERRORS e) {
                    cerr << "problems in " << row + 1 << " string:\n";
                    throw (e);
                }
                continue;
            } else {
                i++;
            }
        }
    }
}

int get_value(Lexem *leftlex, Lexem *rightlex, Function **function, OPERATOR opertype) {
    int left, right;
    if (Number *ptr = dynamic_cast<Number *>(leftlex)) {
        left = ((Number *)leftlex) -> get_value();
    }
    if (Number *ptr = dynamic_cast<Number *>(rightlex)) {
        right = ((Number *)rightlex) -> get_value();
    }
    if (Variable *ptr = dynamic_cast<Variable *>(leftlex)) {
        left = ((Variable *)leftlex) -> get_value(*function);
    }
    if (Variable *ptr = dynamic_cast<Variable *>(rightlex)) {
        right = ((Variable *)rightlex) -> get_value(*function);
    }
    switch (opertype) {
        case ASSIGN:
            if (Variable *ptr = dynamic_cast<Variable *>(leftlex)) {
                int tmp;
                if (Number *ptr = dynamic_cast<Number *>(rightlex)) {
                    tmp = ((Number *)rightlex) -> get_value();
                } else {
                    tmp = ((Variable *)rightlex) -> get_value(*function);
                }
                    ((Variable *)leftlex) -> set_value(*function, tmp);
                    return tmp;
                } else {
                    throw(ERR_ASSIGN_PROBLEMS);
                }
        case PLUS:
            return left + right;
        case MINUS:
            return left - right;
        case MULT:
            return left * right;
        case DIV:
            return left / right;
        case MOD:
            return left % right;
        case OR:
            return left || right;
        case AND:
            return left && right;
        case BITOR:
            return left | right;
        case XOR:
            return left ^ right;
        case BITAND:
            return left & right;
        case EQ:
            return left == right;
        case NEQ:
            return left != right;
        case LEQ:
            return left <= right;
        case LT:
            return left < right;
        case GEQ:
            return left >= right;
        case GT:
            return left > right;
        case SHL:
            return left << right;
        case SHR:
            return left >> right;
    }
    return 0;
}

int evaluate_postfix(vector<vector<Lexem *>> &postfix, int row, Function **function_field, Number **result) {
    vector<Lexem *> stack;
    vector<Number *> need_to_clear;
    Lexem *res;
    int i = 0;
    for (auto &element: postfix[row]) {
        if (element == nullptr) {
            continue;
        }
        if (dynamic_cast<Number *>(element)) {
            stack.push_back(element);
            continue;
        }
        if (dynamic_cast<Variable *>(element)) {
            int random;
            if (((Variable *)element) -> check_var(*function_field))
                ((Variable *)element) -> set_value(*function_field, random);
            stack.push_back(element);
            continue;
        }
        if (dynamic_cast<Function *>(element)) {
            Function *function_elem = (Function *)element;
            int num = function_elem -> get_num_of_start_vars();
            while (num--) {
                if (stack.back() == nullptr) {
                    for (auto &cl: need_to_clear) {
                        if (cl)
                            delete cl;
                    }
                    throw (ERR_VOID_FUNCTION_IN_EVALUATION);
                }
                if (dynamic_cast<Variable *>(stack.back())) {
                    function_elem -> set_start_var(((Variable *)stack.back()) -> get_value(*function_field), num);
                } else {
                    function_elem -> set_start_var(((Number *)stack.back()) -> get_value(), num);
                }
                stack.pop_back();
            }
            int func_row = ((Goto *)element) -> get_row();
            Number *func_result = nullptr;
			while (0 <= func_row && func_row < (int)postfix.size()) {
				try {
                    func_row = evaluate_postfix(postfix, func_row, &function_elem, &func_result);
                } catch (ERRORS e) {
                    for (auto &cl: need_to_clear) {
                        if (cl)
                            delete cl;
                    }
                    cerr << "function " << function_elem -> get_name() << " problems in " << func_row + 1 << " string:\n";
                    throw (e);	
                }
            }
            stack.push_back(func_result);
            if (func_result) {
                need_to_clear.push_back(func_result);
            }
        } else {
            OPERATOR operatortype = ((Oper *)element) -> get_type();
            if (operatortype == GOTO) {
                Variable *label = (Variable *)stack.back();
                stack.pop_back();
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                if (!stack.empty()) {
                    throw (ERR_NOT_BALANCED_BRACKETS);
                }
                *result = nullptr;
                return Goto::ltable[label -> get_name()];
            }
            if (operatortype == ELSE || operatortype == ENDLOOP) {
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                if (!stack.empty()) {
                    throw (ERR_NOT_BALANCED_BRACKETS);
                }
                *result = nullptr;
                return ((Goto *)element) -> get_row();
            }
            if (operatortype == IF || operatortype == WHILE) {
                if (stack.empty()) {
                    for (auto &cl: need_to_clear) {
                        if (cl)
                        delete cl;
                    }
                throw (ERR_NOT_BALANCED_BRACKETS);
                }
                int jump;
                if (dynamic_cast<Number *>(stack.back())) {
                    jump = ((Number *)stack.back()) -> get_value();
                } else if (dynamic_cast<Variable *>(stack.back())){
                    jump = ((Variable *)stack.back()) -> get_value(*function_field);
                } else {
                    for (auto &cl: need_to_clear) {
                        if (cl)
                            delete cl;
                    }
                    throw (ERR_NOT_BALANCED_BRACKETS);
                }   
                stack.pop_back();
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                if (!stack.empty()) {
                    throw (ERR_NOT_BALANCED_BRACKETS);
                }
                if (!jump) {
                    return ((Goto *)element) -> get_row();
                }
                *result = nullptr;
                return row + 1;
            }
            if (operatortype == RETURN) {
                if (stack.empty()) {        
                    (*result) = nullptr;
                } else {
                    int tmp;
                    if (dynamic_cast<Number *>(stack.back())) {
                        tmp = ((Number *)stack.back()) -> get_value();
                    } else if (dynamic_cast<Variable *>(stack.back())){
                        tmp = ((Variable *)stack.back()) -> get_value(*function_field);
                    } else {
                        for (auto &cl: need_to_clear) {
                            if (cl)
                                delete cl;
                        }
                        throw (ERR_NOT_BALANCED_BRACKETS);
                    }
                    *result = new Number(tmp);
                    stack.pop_back();
                    if (!stack.empty()) {
                        for (auto &cl: need_to_clear) {
                            if (cl)
                                delete cl;
                        }
                        throw (ERR_NOT_BALANCED_BRACKETS);
                    }
                }
                return ((Goto *)element) -> get_row(); 
            }
            if (stack.empty()) {
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                throw (ERR_NOT_BALANCED_BRACKETS);
            }
            res = stack.back();
            stack.pop_back();   
            if (stack.empty()) {
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                throw (ERR_NOT_BALANCED_BRACKETS);
            }
            if (res == nullptr || stack.back() == nullptr) {
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                throw(ERR_VOID_FUNCTION_IN_EVALUATION);
            }
            res = new Number(get_value(stack.back(), res, function_field, operatortype));
            stack.pop_back();
            stack.push_back(res);
            need_to_clear.push_back((Number *)res);
        }
    }
        if (!stack.empty()) {
        int tmp;
        if (!stack.back()) {
            *result = nullptr;
        } else {
            if (dynamic_cast<Number *>(stack.back())) {
                tmp = ((Number *)stack.back()) -> get_value();
            } else if (dynamic_cast<Variable *>(stack.back())){
                tmp = ((Variable *)stack.back()) -> get_value(*function_field);
            } else {
                for (auto &cl: need_to_clear) {
                    if (cl)
                        delete cl;
                }
                throw (ERR_NOT_BALANCED_BRACKETS);
            }
            *result = new Number(tmp);
        }
        stack.pop_back();
    } else {
        throw (ERR_NOT_BALANCED_BRACKETS);
    }
    for (auto &cl: need_to_clear) {
        if (cl)
            delete cl;
    }
    if (!stack.empty()) {
        throw (ERR_NOT_BALANCED_BRACKETS);
    }
    return row + 1;
}
