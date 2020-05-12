#include "Calculator.h"

using namespace constants;

//-------------------------------------------------------------
int factorial(int n)
{
	if (n > 1)
		return n * factorial(n - 1);
	else
		return 1;
}

//--------------------------------------------------------------
// gramer for math expresions
double expression()
{
	

	double left = term();
	Token t = ts.get();
	
	while (true)
	{
		switch (t.kind)
		{

		case'+':
			left += term();
			t = ts.get();
			break;

		case'-':
			left -= term();
			t = ts.get();
			break;

		default:
			ts.putback(t);
			return left;
		}
	}
}

double term()
{
	double left = fact();
	Token t = ts.get();

	while (true)
	{
		switch (t.kind)
		{

		case '*':
			left *= fact();
			t = ts.get();
			break;

		case'/':
		{
			double d = fact();
			if (d == 0) throw("Divide By Zero!!");
			left /= d;
			t = ts.get();
			break;
		}

		case'%':
		{
			double d = fact();
			if (d == 0)  throw runtime_error("divide by thero");
			left = fmod(left, d);
			t = ts.get();
			break;
		}

		default:
			ts.putback(t);
			return left;

		}
	}
}

int fact() {
	int left = primary();
	Token t = ts.get();

	while (true)
	{

		switch (t.kind)
		{
		
		case '!':

			if (left == 0)
				left = 1;
			else
		    left = factorial(left);
			t = ts.get();
			break;


		case '^':
			left = pow(left, primary());
			t = ts.get();
			break;

		/*case'√':
			if (left == 0)
				throw runtime_error("negative value for sqrt");
			left = sqrt(left);
			t = ts.get();
			break;*/

		default:
			ts.putback(t);
			return left;
		}


	}
}

double primary()
{
	Token t = ts.get();

	switch (t.kind)
	{

	case '{':
	{
		double d = expression();
		t = ts.get();
		if (t.kind != '}')throw runtime_error("'}' expected");
		return d;
	}

	case'(':
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')')throw runtime_error("')' expected!");
		return d;
	}


	case number:
		return t.value;

	case '-':
		return -primary();
	case '+':
		return primary();
	case name:
	{
		Token next = ts.get();
		if (next.kind == '=')
		{
			double d = expression();
			st.set_value(t.name, d);
			return d;
		}
		else
		{
			ts.putback(next);
			return st.get_value(t.name);
		}


	}
	default:
		throw runtime_error("primary expected!!!");
	}
}

//-------------------------------------------------------------------------------------------

//getting tokens from the cin
Token Token_stream::get()
{
	if (full) {       // do we already have a Token ready?
		// remove token from buffer
		full = false;
		return buffer;
	}
	char ch;

	cin>>ch;    // note that >> skips whitespace (space, newline, tab, etc.)
	
	switch (ch) 
	{
	case help:   // for instruqtions 
	case uperHelp:
	case ';':    // for "print"
	case 'q':    // for "quit"
	case '(':
	case ')':
	case '+':
	case '-': 
	case '*': 
	case '/': 
	case '!':
	case '{':
	case '}': 
	case '%':
	case '√': //square root
	case '^': //exponant
	case '=':
		return Token(ch);        // let each character represent itself

	case '.':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);         // put digit back into the input stream
		double val;
		cin >> val;              // read a floating-point number
		return Token(number, val);   // let '8' represent "a number"
	}

	default:
		if (isalpha(ch))
		{
			string s;
			s += ch;
			while (cin.get(ch) && isalpha(ch) || isdigit(ch) || ch == '_' )
			{
				
				s += ch;
			}
			cin.putback(ch);
			if (s == decklkey) 
				return Token(let);
			if (s == constkey)
				return Token(con);
			
			return Token(name, s);
			
		}
		throw runtime_error("Bad token");
	}
}


// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) throw runtime_error("Buffer is full");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}


// clean up the bufer till ';'
void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind)
	{
		full = false;
		return;
	}
	full = false;

	char ch = 0;
	while (cin >> ch)
	{
		if (ch == c)
			return;
	}
}

//recovering from errors 
void clean_up_mess()
{
	ts.ignore(print);
}

//---------------------------------------------------------------------------------------------

//getting and setting value in the vector
double Symbol_table::get_value(string s)
{
	for (const Variables& v : var_batle)
		if (v.name == s)
			return v.value;
	throw runtime_error("get() undefined variable ");
}

void Symbol_table::set_value(string s, double d)
{
	for (Variables& v : var_batle)
		if (v.name == s) {
			if (v.var == false) throw runtime_error("variable is const");
			v.value = d;
			return;
		}
		
	throw runtime_error("set() undefined variable");
}

//-----------------------------------------------------------------------------------------------


// is variable already in the cevtor? 
bool Symbol_table::is_declared(string s)
{
	for (const Variables& i : var_batle)
		if (i.name == s)
			return true;
	return false;
}

//adding to vector
double Symbol_table::define_name(string s, double d,bool var)
{
	if (is_declared(s))
		throw runtime_error("twice declaration");
	var_batle.push_back(Variables(s, d,var));
	return d;
}

//--------------------------------------------------------------------------------------------------

//declares the variable 
double declaration(Token k)
{
	Token t = ts.get();
	if (t.kind != name)
		throw runtime_error("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=')
		throw runtime_error("= missing");

	double d = expression();
	st.define_name(var_name, d, k.kind == let);
	return d;
}

//deals with declaration and expresion
double statemant()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case let:
	case con:
		return declaration(t.kind);
	default:
		ts.putback(t);
		return expression();
	}
}

//main loop
void calculate()
{
	cout << "For instruqtion enter H or h character\n";
	while (cin)try
	{
		cout << prompt;

		Token t = ts.get();

		if (t.kind == help || t.kind == uperHelp)
		{
			instruction_to_user();
			cout << prompt;
			t = ts.get();
		}

		while (t.kind == print)
			t = ts.get();

		if (t.kind == quit)
			return;
		
		
		
		ts.putback(t);
		cout << result << statemant() << endl;
	}

	catch (exception e)
	{
		cerr << e.what() << endl;
		clean_up_mess();
	}
}


void instruction_to_user()
{
	cout << "--------------------------------------------------------\n"
		<< "It is a simple calculator handling mos of the operations\n"
		<< "( ) + - * / % ^ !!!\n"
		<< "It also has variables , plain and consts ... \n"
		<< "To declare variable u have to specifie let or const!!!!\n"
		<< "For print enter \";\" for quit \"q\"\n";
}
		
		