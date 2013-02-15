#include "stdafx.h"
#include <iostream>
#include <boost\asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost\bind.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\foreach.hpp>

#include <functional>


class printer
{
public:
	printer(boost::asio::io_service& io)
		: strand_(io),
		timer1_(io, boost::posix_time::seconds(1)),
		timer2_(io, boost::posix_time::seconds(1)),
		count_(0)
	{
		timer1_.async_wait(/*strand_.wrap*/(boost::bind(&printer::print1, this)));
		timer2_.async_wait(/*strand_.wrap*/(boost::bind(&printer::print2, this)));
	}

	~printer()
	{
		std::cout << "Final count is " << count_ << "\n";
	}

	void print1()
	{
		boost::mutex::scoped_lock lock1(m_lock);
		std::cout<<"print1 Call Thread "<<boost::this_thread::get_id();
		if (count_ < 10)
		{
			std::cout << "Timer 1: " << count_ << "\n";
			++count_;

			timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
			timer1_.async_wait(/*strand_.wrap*/(boost::bind(&printer::print1, this)));
		}
	}

	void print2()
	{
		boost::mutex::scoped_lock lock1(m_lock);


		std::cout<<"print2 Call Thread "<<boost::this_thread::get_id();
		if (count_ < 10)
		{
			std::cout << "Timer 2: " << count_ << "\n";
			++count_;

			timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
			timer2_.async_wait(/*strand_.wrap*/(boost::bind(&printer::print2, this)));
		}
	}

private:
	boost::asio::strand strand_;
	boost::asio::deadline_timer timer1_;
	boost::asio::deadline_timer timer2_;
	int count_;
	boost::mutex m_lock;
};


class asioTestFix  :public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_pwork = new boost::asio::io_service::work(io);
		m_pstrand = new boost::asio::strand(io);
		m_ThreadCount = 0;
	
		m_pPrinter = NULL;
	}

	virtual void TearDown()
	{

		delete m_pwork;
		delete m_pstrand;

		BOOST_FOREACH(auto lpThread,m_ThreadPool)
		{
			lpThread->join();
		}


		if(m_pPrinter!= NULL)
		{
			delete m_pPrinter;
			m_pPrinter = NULL;
		}
	}

	void InitThreadPool()
	{
		for(int i =0 ; i< m_ThreadCount; ++i)
		{
			boost::shared_ptr<boost::thread > lpThread(new boost::thread(boost::bind(&boost::asio::io_service::run,&io) ));
			m_ThreadPool.push_back(lpThread);
		}
	}

	 static void SetUpTestCase()
	{


	}

	 static void TearDownTestCase()
	{

	}

	 boost::asio::io_service io;
	 boost::asio::io_service::work* m_pwork;
	 boost::mutex m_lock;
	 boost::asio::strand* m_pstrand;


	 int		m_ThreadCount;
	 std::vector<boost::shared_ptr<boost::thread> > m_ThreadPool;


	 printer* m_pPrinter;
	//test function
public:

};

boost::mutex g_STDIOlock;

void print(const boost::system::error_code& , boost::shared_ptr< boost::asio::deadline_timer >  apTimer)
{
	//boost::mutex::scoped_lock lock(g_STDIOlock);
	std::cout<<"Call Thread "<<boost::this_thread::get_id()<<"\n";
	std::cout << "Hello, world! The async Print timer2 \n";
}



TEST_F(asioTestFix,DISABLED_timer2)
{
	m_ThreadCount = 1;
	InitThreadPool();
	 boost::shared_ptr< boost::asio::deadline_timer > lpTimer(new boost::asio::deadline_timer(io, boost::posix_time::seconds(5)));
	lpTimer->async_wait(boost::bind(&print,boost::asio::placeholders::error,lpTimer));
}

//
void print(const boost::system::error_code& /*e*/,
	boost::asio::deadline_timer* t, int* count)
{
	
	if (*count < 5)
	{
		//boost::mutex::scoped_lock lock(g_STDIOlock);
		std::cout<<"Call Thread "<<boost::this_thread::get_id();
		std::cout <<" "<< *count << "\n";
		++(*count);

		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
		t->async_wait(boost::bind(print,
			boost::asio::placeholders::error, t, count));
	}
	else
	{
		delete t;
		delete count;
	}
}

TEST_F(asioTestFix,DISABLED_timer3)
{
	m_ThreadCount = 5;
	InitThreadPool();

	int* lpcount = new int(0);
	boost::asio::deadline_timer* lpTimer = new boost::asio::deadline_timer(io, boost::posix_time::seconds(1));
	lpTimer->async_wait(boost::bind(print,boost::asio::placeholders::error,lpTimer, lpcount));
}


void wasterTime()
{
	boost::this_thread::sleep(boost::posix_time::seconds(1));
	{
		boost::mutex::scoped_lock lock(g_STDIOlock);
		std::cout<<"wasterTime Call Thread "<<boost::this_thread::get_id()<<'\n'<<std::endl;
	}
}


TEST_F(asioTestFix,DISABLED_strand_using)
{
	m_ThreadCount = 5;
	InitThreadPool();

	for(int i=0;i<10;++i)
	{
		io.post(m_pstrand->wrap(boost::bind(wasterTime)));
		//io.wrap((boost::bind(wasterTime)));
	}
}
TEST_F(asioTestFix,DISABLED_multiThread_con_call)
{
	m_ThreadCount = 5;
	InitThreadPool();

	for(int i=0;i<10;++i)
	{
		io.post((boost::bind(wasterTime)));
		//io.wrap((boost::bind(wasterTime)));
	}
}



int wasterTime_for_warp(int aTimeSecond)
{
	boost::this_thread::sleep(boost::posix_time::seconds(aTimeSecond));
	{
		boost::mutex::scoped_lock lock(g_STDIOlock);
		std::cout<<"wasterTime Call Thread "<<boost::this_thread::get_id()<<'\n'<<std::endl;
	}
	return aTimeSecond;
}

TEST_F(asioTestFix,DISABLED_multiThread_con_call_using_warp)
{
	m_ThreadCount = 5;
	InitThreadPool();

	for(int i=0;i<10;++i)
	{
		
		//io.post((boost::bind(wasterTime)));
		io.post(boost::bind(wasterTime_for_warp,1));
	}
}



TEST_F(asioTestFix,DISABLED_Printer)
{
	//boost::asio::io_service io;
	
	
	m_pPrinter = new printer(io);
	
	//boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
	//io.run();
	//t.join();
}

TEST_F(asioTestFix,DISABLED_whyonepost_multi_call)
{
	m_ThreadCount = 1;
	InitThreadPool();

	int count = 0;
	auto lLocalFUn = [&count](int x){std::cout<<x<<std::endl;count++;} ;
	std::function<void(int x)> lLocalFunt2 = [&count](int x){std::cout<<x<<std::endl;count++;} ;

	io.post(boost::bind<void>(lLocalFUn,1));
	io.post(boost::bind<void>(lLocalFUn,2));
	io.post(boost::bind<void>(lLocalFUn,3));
	io.post(boost::bind<void>(lLocalFUn,4));
	io.post(std::bind(lLocalFunt2,5));

	boost::this_thread::sleep(boost::posix_time::millisec(10));
	EXPECT_EQ(5,count);




}

