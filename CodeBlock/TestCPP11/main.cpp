#include <iostream>
#include<vector>
#include<string>
#include <algorithm>
using namespace std;
#include<functional>
#include<thread>

class SomeType
{
private:
    SomeType(int aint):m_val(aint) {}
public:
    SomeType() : SomeType(1) {};
    virtual void fun1() {};
    virtual void fun2() final {};

private:
    int m_val;
};

class SomeType2 :public SomeType
{
    virtual void fun1() override
    {

    }
//    void fun2()
//    {
//
//    }
};

class SomeType3
{
    public:
SomeType3() = default;
void operator=(const SomeType3&) = delete;
int m_val;

void* operator new (std::size_t) = delete;
private:
    //void* operator new (std::size_t) {};
};




template <typename T1, typename T2>
class baseTemplateclass
{
    T1 m_T1;
    T2 m_T2;
};

template<typename T1>
using DrieveTemplateClass = baseTemplateclass<T1,int>;



int main()
{
    cout << "Hello world!" << endl;


//
    std::vector<std::string> lStrVec = {"123","23213"};
    std::vector<int> lVec {1,2,3,4,5};

    cout<<lVec[2];
    // system("pause");

// 中文
    for(std::string& lString: lStrVec)
    {
        std::cout<<lString<<'\n';
    }

    for(auto lString: lStrVec)
    {
        std::cout<<lString;
    }

    {
        char lcharArray[] = {"1234567890"};
        for(auto lchar: lcharArray)
        {
            std::cout<<lchar<<'\n';
        }
    }

    char lcharArray[] = {"1234567890"};


    int count = 0;
    auto lLocalFUn = [&count](int x)
    {
        std::cout<<x<<std::endl;
        count++;
    } ;
    std::function<void(int x)> lLocalFunt2 = [&count](int x)
    {
        std::cout<<x<<std::endl;
        count++;
    } ;

    //io.post(boost::bind<void>(lLocalFUn,1));
    //io.post(boost::bind<void>(lLocalFUn,2));
    //io.post(boost::bind<void>(lLocalFUn,3));
    //io.post(boost::bind<void>(lLocalFUn,4));
    std::bind(lLocalFunt2,5);

    std::bind(lLocalFUn,5);



    auto f = []()
    {
        std::cout<<"find";
    };
    auto f1 = [](int a1)
    {
        std::cout<<a1;
    };

    std::bind(f);
    std::bind(f1,2);

    //std::thread(f1,2);
    //std::thread t(f);

    std::cout<<"中文"<<std::endl;
    std::cout<<"中文"<<std::endl;

    SomeType testint;

SomeType3 lType3_1;
SomeType3 lType3_12;

	std::tuple<int,const char*,double> lstdtuple{123,"xxxxxx",31};
	int lent = sizeof(lstdtuple);
	int tupleVal = std::get<0>(lstdtuple);
    std::get<0>(lstdtuple) = 10;
    int tempdasdasd = std::get<0>(lstdtuple);




   // SomeType3* lpType3 = new SomeType3; can not new
   // lType3_12 = lType3_1; //delete operator=
    return 0;
}
