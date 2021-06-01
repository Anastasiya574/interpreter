#include "const.hpp"
#include "lexem.hpp"
#include <iostream>
#include <string>
#include <map>
Lexem::Lexem() {}
Lexem::~Lexem() {}

//////////////////////////////////////////////////

Number::Number() {}

Number::Number(int smthg) {
	value = smthg; 
}

Number::~Number () {}

void Number::set_value(int smthg) {
	value = smthg;
}

int Number::get_value() const {
	return value;
}

void Number::print() {
		std::cout << '[' << value << "] ";
}

//////////////////////////////////////////////////

Oper::Oper() {}

Oper::Oper(OPERATOR opertype) {
	Oper::opertype = opertype;
}

Oper::~Oper() {}

void Oper::set_type(OPERATOR optype) {
	opertype = optype;
}

OPERATOR Oper::get_type() {
	return opertype;
};

int Oper::get_priority() {
	return PRIORITY[(int)opertype];
}

void Oper::print() {
	std::cout << '|' << OPERTEXT[(int)opertype] << "| ";
}

//////////////////////////////////////////////////

Goto::Goto() {}
    
Goto::Goto(OPERATOR optype): Oper (optype) { 
	row = UNDEFINED; 
}

Goto::~Goto() {}

void Goto::set_row(int row) {
	Goto::row = row;
}

int Goto::get_row() {
	return row;
}

void Goto::print() {
	std::cout << "<row " << row << '>';
	Oper::print(); 
}

std::map<std::string, int> Goto::ltable;

//////////////////////////////////////////////////

Function::Function() {}

Function::Function(std::string name, int row, int n) {
	((Oper *)this) -> set_type(FUNCTION);
	((Goto *)this) -> set_row(row);
	num_of_start_vars = n;
	f_name = name;
}

Function::~Function() {}

int Function::get_num_of_start_vars() {
	return num_of_start_vars;
}

std::string Function::get_name(){
	return f_name;
}

void Function::add_var(std::string var_name) {
	vtable[var_name];
}

void Function::set_start_var(int answer, int i) {
	int j = 0;
	for	(auto &q: vtable) {
		if (j == i) {
			q.second = answer;
		}
		j++;	
	}
}

void Function::print_function_vars() {
	for	(auto &q: vtable) {
		std::cout << q.first << " = " << q.second << std::endl;
	}
}

int Function::check_var(std::string var_name) {
	if (vtable.find(var_name) == vtable.end()) {
		return 1;
	}
	return 0;
}

void Function::set_value(std::string var_name, int value) {
	vtable[var_name] = value;
}

int Function::get_value(std::string var_name) {
	return vtable[var_name];
}

void Function::print() {
	Goto::print();
	std::cout << '@' << f_name << " %" << num_of_start_vars << "%";
	for (auto &var: vtable) {
		std::cout << "_" << var.first;
	}
	std::cout << "@ ";
}

std::map<std::string, int> Function::ftable;

//////////////////////////////////////////////////

Variable::Variable() {}

Variable::Variable(std::string str) {
	v_name = str; 
}

Variable::~Variable() {}

std::string Variable::get_name() {
	return v_name;
}

int Variable::check_var(Function *function) {
	return function -> check_var(v_name);
}

void Variable::set_value(Function *function, int num) {
	function -> set_value(v_name, num);
}

int Variable::get_value(Function *function) {
	return (function -> get_value(v_name));
}

void Variable::print() {
	std::cout << '[' << v_name << "] ";
}

//////////////////////////////////////////////////

std::map <std::string, int* > ArrayTable; 

Array::Array(Lexem *left, Lexem *right) {
    arrayName = left->get_name();
    elementNumber = right->get_value();
}

int Array::get_value() const{
    return ArrayTable[arrayName][elementNumber];
} 

void Array::set_value(int number) {
    ArrayTable[arrayName][elementNumber] = number;
}

