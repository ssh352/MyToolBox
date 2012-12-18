#include "stdafx.h"
#include <iostream>
#include <boost\asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost\bind.hpp>

class printer;

class DISABLED_asioTestFix  :public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_pwork = new boost::asio::io_service::work(io);
		m_pstrand = new boost::asio::strand(io);
		m_pThread1.swap(boost::thread(boost::bind(&boost::asio::io_service::run,&io) ));
		m_pThread2.swap(boost::thread(boost::bind(&boost::asio::io_service::run,&io) ));
		m_pThread3.swap(boost::thread(boost::bind(&boost::asio::io_service::run,&io) ));
		m_pThread4.swap(boost::thread(boost::bind(&boost::asio::io_service::run,&io) ));
		m_pPrinter = NULL;
	}

	virtual void TearDown()
	{
		delete m_pwork;
		m_pThread1.join();
		m_pThread2.join();
		m_pThread3.join();
		m_pThread4.join();
		delete m_pstrand;
		if(m_pPrinter!= NULL)
		{
			delete m_pPrinter;
			m_pPrinter = NULL;
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

	 boost::thread m_pThread1;
	 boost::thread m_pThread2;
	 boost::thread m_pThread3;
	 boost::thread m_pThread4;


	 printer* m_pPrinter;

	//test function
public:

};


void print(const boost::system::error_code& /*e*/)
{
	boost::mutex::scoped_lock(stdIOlock);
	std::cout<<"Call Thread "<<boost::this_thread::get_id();
	std::cout << "\nHello, world! The sync Print\n";
}



TEST_F(DISABLED_asioTestFix,timer2)
{
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	t.async_wait(&print);
}

//
void print(const boost::system::error_code& /*e*/,
	boost::asio::deadline_timer* t, int* count)
{
	
	if (*count < 2)
	{
		boost::mutex::scoped_lock(stdIOlock);
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

TEST_F(DISABLED_asioTestFix,timer3)
{
	int* lpcount = new int(0);
	boost::asio::deadline_timer* lpTimer = new boost::asio::deadline_timer(io, boost::posix_time::seconds(1));
	lpTimer->async_wait(boost::bind(print,boost::asio::placeholders::error,lpTimer, lpcount));
}


void wasterTime()
{
	boost::this_thread::sleep(boost::posix_time::seconds(0));
	{
		std::cout<<"wasterTime Call Thread "<<boost::this_thread::get_id()<<std::endl;
	}


}


TEST_F(DISABLED_asioTestFix,conCAll)
{

	for(int i=0;i<2;++i)
	{
		io.post(m_pstrand->wrap(boost::bind(wasterTime)));
		//io.wrap((boost::bind(wasterTime)));
	}
}


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

TEST_F(DISABLED_asioTestFix,Printer)
{
	//boost::asio::io_service io;
	
	
	m_pPrinter = new printer(io);
	
	//boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
	//io.run();
	//t.join();
}

