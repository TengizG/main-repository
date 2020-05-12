#include "Calculator.h"
#include <Windows.h>

int main()
try
{
	

	st.define_name("pi", 3.1415926535);
	st.define_name("e", 2.7182818284);
	st.define_name("k", 1000);

	calculate();
	return 0;
	
}

catch (runtime_error & e)
{
	cerr << e.what() << endl;
	cout << "Please enter \"`\" to close the window\n";
	for (char ch; cin >> ch;)
		if (ch == '`')
			return -1;
}