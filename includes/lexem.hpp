#ifndef LEXEM_HPP
#define LEXEM_HPP
#include "const.hpp"
#include <map>
#include <string>

class Lexem {
  public:
    Lexem();
    virtual ~Lexem();
};

class Number: public Lexem {
    int value;
  public:
    Number();
    Number(int smthg);
    ~Number();
    void set_value(int smthg);
    int get_value();
    void print();
};

class Oper: public Lexem {
    OPERATOR opertype;
  public:
    Oper();
    Oper(OPERATOR opertype);
    virtual ~Oper();
    void set_type(OPERATOR optype);
    OPERATOR get_type();
    int get_priority();
    virtual void print();
};

class Goto: public Oper {
    int row;
  public:
    Goto();
    Goto(OPERATOR optype);
    virtual ~Goto();
    void set_row(int row);
    int get_row();
    virtual void print();
    static map<string, int> ltable;
};

class Function: public Goto {
    string f_name;
    map<string, int> vtable;
    int num_of_start_vars = 0;
  public:
    Function();
    Function(string name, int row, int n);
    ~Function();
    int get_num_of_start_vars();
    string get_name();
    void add_var(string var_name);
    void set_start_var(int answer, int i);
    void print_function_vars();
    int check_var(string var_name);
    void set_value(string var_name, int value);
    int get_value(string var_name);
    virtual void print();
    static map<string, int> ftable;
};

class Variable: public Lexem {
    string v_name;
  public:
    Variable();
    Variable(string str);
    ~Variable();
    string get_name();
    int check_var(Function *function);
    void set_value(Function *function, int num);
    int get_value(Function *function);
    void print();
};

class Array: public Lexem {
    string arrayName;
    int elementNumber;
public:
    Array(Lexem *left, Lexem *right);
    void set_value(int number);
    int get_value() const;
    void print();
};

#endif
