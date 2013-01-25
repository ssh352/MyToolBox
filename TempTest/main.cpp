

#include <string>
#include <iostream>
using namespace std;
#include <boost\foreach.hpp>
#include <map>
#include <boost\thread.hpp>

class TestA
{
	public:
	TestA(int * ap)
		:m_pInt(ap)
	{

	}
	int* m_pInt;
};
class TestB :public TestA
{
	public:
	TestB()
		:TestA(&m_autoPint)
		, m_autoPint(123)
		
	{

	}
	int m_autoPint;
};
int main()
{

	//TestB inst;
	//string test ("xxxxxx" );
	//const char * test_data_ptr = test.data();


	//const char* lcBUf = "xxxxxxx";
	//string test2 (lcBUf,6);
	//const char * test_data_ptr2 = test2.data();
	////const char * test_cptr = test.c_str();


	//std::map<string,int> testMap;
	//typedef std::map<string,int>::_Val_type mapVal;
	//BOOST_FOREACH(auto lval,testMap)
	//{
	//	std::cout<< lval.second;
	//}

	//std::cout<<"中文"<<std::endl;
	//std::cout<<u8"中文"<<std::endl;


	//std::tuple<int,const char*,double> lstdtuple;
	//int lent = sizeof(lstdtuple);


	return 2;



}