//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;
boost::array<char, 1024> buf;

bool endReceiv = false;


void read_handler( const boost::system::error_code& error, std::size_t bytes_transferred,boost::asio::ip::tcp::socket* apSocket)
{
	static int count = 0;
	count++;
	std::cout<<"\nread state "<<error<<" read_size = "<<bytes_transferred<<" count ="<<count<<std::endl;
	if (error == boost::asio::error::eof)
	{
		std::cout<<"Connnetion close"<<std::endl;

	}
	else if (error)
	{
		throw boost::system::system_error(error); // Some other error.
	}
	std::cout.write(buf.data(), bytes_transferred);

	endReceiv =true;
	apSocket->async_read_some(boost::asio::buffer(buf),  boost::bind(read_handler, boost::asio::placeholders::error,_2,apSocket));
}

void connect_handler(
	const boost::system::error_code& ec,
	boost::asio::ip::tcp::resolver::iterator iterator,boost::asio::ip::tcp::socket* apSocket)
{
	std::cout<<"Connected "<<ec<<std::endl;
	
}


int main(int argc, char* argv[])
{
	try
	{


		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query("127.0.0.1", "daytime");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket* lpsocket= new tcp::socket(io_service);
		boost::asio::async_connect(*lpsocket, endpoint_iterator,boost::bind(connect_handler, boost::asio::placeholders::error,boost::asio::placeholders::iterator,lpsocket));
		lpsocket->async_read_some(boost::asio::buffer(buf),boost::bind(read_handler, boost::asio::placeholders::error,_2,lpsocket));

		io_service.run();
		//while(!endReceiv)
		//{
		//	boost::this_thread::sleep(boost::posix_time::millisec(100));
		//}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}