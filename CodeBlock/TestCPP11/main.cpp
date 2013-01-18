#include <iostream>
#include<vector>
#include<string>
#include <algorithm>
using namespace std;


int main()
{
    cout << "Hello world!" << endl;


//  初始化列表
    std::vector<std::string> lStrVec = {"123","23213"};
    std::vector<int> lVec{1,2,3,4,5};

    cout<<lVec[2];
   // system("pause");

// 范围for
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

{
    char lcharArray[] = {"1234567890"};
    std::for_each(lcharArray+1,lcharArray +5,[](char aChar){cout<<aChar<<'\n';});
}
    return 0;
}
