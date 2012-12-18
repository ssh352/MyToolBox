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
#include <boost/filesystem.hpp>
using boost::asio::ip::tcp;

const std::string TargetFileName = "data";

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

		

		for (;;)
		{
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			boost::filesystem::path p(TargetFileName);
			uint64_t lfilesize = boost::filesystem::file_size(p);
			std::cout<<socket.remote_endpoint()<<std::endl;


			std::ifstream fileStream(p.string(),std::fstream::in|std::fstream::binary);

			const int buffsize = 1024;
			char lBuf[buffsize] = {0};
			uint64_t ltransferSize = 0;
			std::cout<<"Enter Trans with File root path="<< p.generic_string() <<std::endl;
			std::cout<<"Enter Trans with File Size="<<lfilesize<<std::endl;
			std::cout<<"Enter Trans with File Flag="<< fileStream.flags()<<std::endl;
			while(lfilesize>0 && fileStream)
			{
				if(lfilesize>buffsize)
				{
					fileStream.read(lBuf,buffsize);
					lfilesize -= buffsize;
					ltransferSize += buffsize;
					boost::system::error_code ignored_error;
					boost::asio::write(socket, boost::asio::buffer(lBuf,buffsize), ignored_error);

				}
				else
				{
					fileStream.read(lBuf,lfilesize);
					ltransferSize += lfilesize;
					boost::system::error_code ignored_error;
					boost::asio::write(socket, boost::asio::buffer(lBuf,lfilesize), ignored_error);
					lfilesize = 0;			
				}
				

			}
			std::cout<<"Exit Trans with File Flag="<< fileStream.flags()<<std::endl;
			socket.close();
			fileStream.close();
			std::cout<<"Send Files size="<<ltransferSize<<std::endl;
		}
		
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}