#include <iostream>
#include <exception>
#include <cmath>
#include <vector>

using namespace std;

#ifndef CALCULATOR_H
#define CALCULATOR_H

//constans for identifieng tokens 
//--------------------------------------------------------------
namespace constants
{
    const char help = 'h'; const char uperHelp = 'H';
    const char number = '8';
    const char quit = 'q';
    const char print = ';';
    const string prompt = "> ";
    const string result = "= ";
    const char name = 'a';          //name token
    const char let = 'L';           //variable declaration token
    const char con = 'C';           //const variable declaration
    const string decklkey = "let";  //key word for declaration
    const string constkey = "const"; // const variable in calculator
}
//---------------------------------------------------------------

//name value pairs for variable 
//---------------------------------------------------------------
struct Variables
{
    string name;
    double value;
    bool var;  //cheking if variable is a const
    Variables(string s,double d,bool va=true): name(s),value(d),var(va){}
};

// vector of variables 
class Symbol_table
{
public:
    //cheking if the name is declared alreadu
    bool is_declared(string s);

    //puts vector in the vector
    double define_name(string s, double d, bool var = true);

    //getting value from vector
    double get_value(string s);

    //setting value of variable in vector 
    void set_value(string s, double d);

private:
    vector<Variables> var_batle;
};

//----------------------------------------------------------------------------

//kind value pairs for tokens (number,val) is a number
class Token
{
public:
	char kind;
	double value;
    string name;
	Token(char ch) :kind(ch), value(0) {}
	Token(char ch, double val) : kind(ch), value(val) {}
    Token(char ch, string n) : kind(ch), name(n) {}
};

//token_stream class for geting tokens from cin and puting into buffer 
class Token_stream {
public:
    Token_stream() :full(false), buffer(0) {}   // no Token in buffer;   // make a Token_stream that reads from cin
    Token get();      // get a Token (get() is defined elsewhere)
    void putback(Token t);    // put a Token back
    void ignore(char c);
private:
    bool full;        // is there a Token in the buffer?
    Token buffer;     // here is where we keep a Token put back using putback()
};

//-------------------------------------------------------------------------

_declspec (selectany) Token_stream ts;   //for get() and putback()
_declspec (selectany) Symbol_table st;  //for vector and variable declaration


//-------------------------------------------------------------------------------

//getting factorial of number 
int factorial(int );

//-----------------------------------------------------------------------------------

//gramer for tokens prymary works directly with tokkens
double primary();

//factorial gramer with high priority
int fact();

//term deals with * / %
double term();

//expression deals with + -
double expression();

//-----------------------------------------------------------------------------------------

//declares the variable 
double declaration(Token);

//statemant deals with declaration of variable or expresion
double statemant();

//main loop
void calculate();


//recowering from errors 
void clean_up_mess();


void instruction_to_user();

#endif // !CALCULATOR_H














