#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <string>

boost::random::mt19937 gen;


int GenRandom( int start = 10000000, int end = 90000000 )
{
	boost::random::uniform_int_distribution<> dist(start, end);
	return dist(gen);
}

const char RandStrCharSet [] = {"01234567890qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM`!@#$%^&*()+[]{};':\",.<>/\\|?"};


std::string GenRandomString( int len )
{

	const int lCharSetsize = strlen(RandStrCharSet);

	std::string lRet(len,' ');
	for(int i = 0; i<len; ++i)
	{
		int lseed = GenRandom();
		lRet[i] = RandStrCharSet[lseed%lCharSetsize];
	}
	return lRet;

}