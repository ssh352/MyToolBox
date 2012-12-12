//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <fstream>
#include <boost\array.hpp>
using boost::asio::ip::tcp;

const std::string TargetFileName = "test.txt";

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

int main()
{
	try
	{
		boost::asio::io_service io_service;

		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

		//for (;;)
		{
			//tcp::socket socket(io_service);
			//acceptor.accept(socket);
			//std::cout<<socket.remote_endpoint()<<std::endl;


			std::ifstream fileStream(TargetFileName,std::fstream::out|std::fstream::binary);
			const int buffsize = 1024;
			char lBuf[buffsize] = {0};
			while(fileStream.eof())
			{
				fileStream.get(lBuf,buffsize);
				std::cout<< lBuf;
				//boost::system::error_code ignored_error;
				//boost::asio::write(socket, boost::asio::buffer(lBuf), ignored_error);
			}

	
			//while(fileStream)
			//{
			//	fileStream.get(lBuf,buffsize);
			//	std::cout<< lBuf;
			//	boost::system::error_code ignored_error;
			//	boost::asio::write(socket, boost::asio::buffer(lBuf), ignored_error);
			//}
			//socket.close();
			//fileStream.close();
		}
		
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}