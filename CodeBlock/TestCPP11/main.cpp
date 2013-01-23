#include <iostream>
#include<vector>
#include<string>
#include <algorithm>
using namespace std;
#include<functional>

int main()
{
    cout << "Hello world!" << endl;


//  ³õÊ¼»¯ÁÐ±í
    std::vector<std::string> lStrVec = {"123","23213"};
    std::vector<int> lVec{1,2,3,4,5};

    cout<<lVec[2];
   // system("pause");

// ·¶Î§for
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
	auto lLocalFUn = [&count](int x){std::cout<<x<<std::endl;count++;} ;
	std::function<void(int x)> lLocalFunt2 = [&count](int x){std::cout<<x<<std::endl;count++;} ;

	//io.post(boost::bind<void>(lLocalFUn,1));
	//io.post(boost::bind<void>(lLocalFUn,2));
	//io.post(boost::bind<void>(lLocalFUn,3));
	//io.post(boost::bind<void>(lLocalFUn,4));
	std::bind(lLocalFunt2,5);

	std::bind(lLocalFUn,5);



	auto f = [](){std::cout<<"find";};
	auto f1 = [](int a1){std::cout<<a1;};

    std::bind(f);
    std::bind(f1,2);

    std::cout<<"中文"<<std::endl;
	std::cout<<u8"中文"<<std::endl;

    return 0;
}
