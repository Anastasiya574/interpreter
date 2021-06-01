#ifndef LEXEM_HPP
#define LEXEM_HPP
#include "const.hpp"
#include <map>
#include <string>
extern std::map <std::string, int* > ArrayTable;
class Lexem {
  public:
    Lexem();
    virtual ~Lexem();
    virtual int get_value() const { return 0; };
    virtual std::string get_name() { return ""; };
    virtual OPERATOR get_type() { return (OPERATOR)0; };
    virtual void set_value(int) {};
    virtual void print() {};
};

class Number: public Lexem {
    int value;
  public:
    Number();
    Number(int smthg);
    ~Number();
    void set_value(int smthg);
    int get_value() const;
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
    void print();
};

class Goto: public Oper {
    int row;
  public:
    Goto();
    Goto(OPERATOR optype);
    virtual ~Goto();
    void set_row(int row);
    int get_row();
    void print();
    static std::map<std::string, int> ltable;
};

class Function: public Goto {
    std::string f_name;
    std::map<std::string, int> vtable;
    int num_of_start_vars = 0;
  public:
    Function();
    Function(std::string name, int row, int n);
    ~Function();
    int get_num_of_start_vars();
    std::string get_name();
    void add_var(std::string var_name);
    void set_start_var(int answer, int i);
    void print_function_vars();
    int check_var(std::string var_name);
    void set_value(std::string var_name, int value);
    int get_value(std::string var_name);
    void print();
    static std::map<std::string, int> ftable;
};

class Variable: public Lexem {
    std::string v_name;
  public:
    Variable();
    Variable(std::string str);
    ~Variable();
    std::string get_name();
    int check_var(Function *function);
    void set_value(Function *function, int num);
    int get_value(Function *function);
    void print();
};

class Array: public Lexem {
    std::string arrayName;
    int elementNumber;
public:
    Array(Lexem *left, Lexem *right);
    void set_value(int number);
    int get_value() const;
};

#endif
