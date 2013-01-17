#include "stdafx.h"


// which I want, but can not copmly (for reasion place template to another files
//template<typename type1>
//class MyTestTemplateClass
//{
//
//public:
//template<typename type2>
//void SetVal(type2 aVal);
//
//	type1 m_Val;
//};
//
//template<typename type1 ,typename type2>
//void MyTestTemplateClass<type1>::SetVal(type2 aVal)
//{
//	m_Val = aVal;
//}


//which now  I can use

template<typename type1>
class MyTestTemplateClass
{

public:
template<typename type2>
void SetVal(type2 aVal)
{
	m_Val = aVal;
}
	type1 m_Val;
};


//test whether we can define class using template argument like buildin
template<typename T1>
class MyTestTemplateClass2
{
	typedef T1 type1;
	class Testclassinclass
	{
		type1 m_type1;
	};



	Testclassinclass m_val;
};



//test template class typedef
template<typename T1>
class MyTestTemplateClass3
{

	T1 m_Val;
};
//===============can not pass
//typedef MyTestTemplateClass3<typename T1> test3;
//typedef MyTestTemplateClass3 test3;


//typedef with samename
template<typename T1>
class MyTestTemplateClass4
{
	typedef MyTestTemplateClass3<T1> MyTestTemplateClass3;
	T1 m_Val;
	MyTestTemplateClass3 m_Val2;
};



template<typename TObject>
class ListBaseHook
{
public:
	TObject* m_Pre;
	TObject* m_Next;
};



template<typename T1>
class MyTestTemplateClass5 
{
	MyTestTemplateClass5* m_Prve11;
	MyTestTemplateClass5* m_Next22;
};

//template<typename T1>
//class MyTestTemplateClass6 : public ListBaseHook<MyTestTemplateClass5>
//};



//template<typename T1>
//class MyTestTemplateClass6 
//{
//
//	typedef typename std::vector<T1>  VecType;
//	class VecIter
//	{
//	public:
//		VecIter(VecType::iterator  aiter)
//			:m_VecIter(aiter)
//		{
//
//		}
//		VecType::iterator m_VecIter;
//	};
//	VecType m_Vec;
//};



template<typename T1>
class MyTestTemplateClass6 
{
public:
	typedef typename std::vector<T1>  VecType;
	VecType m_Vec;
};

template<typename T1>
class VecIter
{

	typedef  typename MyTestTemplateClass6<T1>::VecType::iterator iterType;
public:
	VecIter(iterType  aiter)
		:m_VecIter(aiter)
	{

	}
	iterType m_VecIter;
};



template<typename T1>
class MyTestTemplateClass7_1
{
public:
	enum TestEnum
	{
		sdasd,
		dsadaf
	};

};
template<typename T1>
class MyTestTemplateClass7_2
{
public:
	//typename  myEnum;
	typedef typename MyTestTemplateClass7_1<T1>::TestEnum myEnum;
	MyTestTemplateClass7_2(myEnum  a)
	{

	}
};


TEST(template_class_with_template_method,DISABLED_test1)
{

	MyTestTemplateClass<int> aInst;
	double lSetVal = 3.5;
	aInst.SetVal(lSetVal);

	MyTestTemplateClass2<int> aInst2;

	MyTestTemplateClass4<int> aInst4;

	MyTestTemplateClass5<int> aInst5;

	MyTestTemplateClass6<int> aInst6;

	MyTestTemplateClass6<int>::VecType::iterator  aIter;
	VecIter<int> aInst6_2(aIter);

	MyTestTemplateClass7_2<int> ainst7_2(MyTestTemplateClass7_1<int>::sdasd);
}


