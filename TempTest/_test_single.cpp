#include <csignal>

#include <iostream>
#include <boost/function.hpp>
#include <boost/thread.hpp>

void singaleHandle(int parame)
{
	std::cerr<<"singaleHandle  "<<parame;
	boost::function< void(int)> lPrevHandler = signal(SIGINT,singaleHandle);
}


int main()
{
	boost::function< void(int)> lPrevHandler = signal(SIGINT,singaleHandle);
	boost::this_thread::sleep(boost::posix_time::seconds(100000));

}