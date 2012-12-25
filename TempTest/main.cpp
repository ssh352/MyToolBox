

#include <string>
#include <iostream>
using namespace std;
#include <boost\foreach.hpp>
#include <map>
int main()
{


	string test ("xxxxxx" );
	const char * test_data_ptr = test.data();


	const char* lcBUf = "xxxxxxx";
	string test2 (lcBUf,6);
	const char * test_data_ptr2 = test2.data();
	//const char * test_cptr = test.c_str();


	std::map<string,int> testMap;
	typedef std::map<string,int>::_Val_type mapVal;
	BOOST_FOREACH(mapVal lval,testMap)
	{
		std::cout<< lval.second;
	}
	

}