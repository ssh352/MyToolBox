#include "stdafx.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <algorithm>
TEST(test_cpp0x,lambda_auto_test)
{
	auto   fn = [](int a,int b){return a+b;};
	int c= fn(2,3);
	
	auto d =c;
}

TEST(test_cpp0x,lambda_test2)
{
	int count = 0;
	auto   fn = [&count](int a,int b)->bool {count++;return a+b;} ;
	bool a1 = fn(2,3);
	EXPECT_EQ(a1,true);
	bool a2 = fn(2,-2);
	EXPECT_EQ(false,a2);
}

TEST(test_cpp0x,std_for_each)
{
	std::vector<int> lVec;
	lVec.push_back(10);
	lVec.push_back(20);
	lVec.push_back(30);

	int count = 0;
	auto   fn = [&count](int a){count++;} ;
	BOOST_FOREACH(auto lvar,lVec)
	{

		fn(lvar);
	}

	EXPECT_EQ(3,count);

}

//no support of initializer_list
//
//class SequenceClass
//{
//public:
//	SequenceClass(std::initializer_list)
//};
//
//TEST(test_cpp0x,initializer_list)
//{
//
//}

TEST(test_cpp0x,lambda_test3)
{
	char lcharArray[] = {"1234567890"};
	std::for_each(lcharArray+1,lcharArray +5,[](char aChar){std::cout<<aChar<<'\n';});
}


