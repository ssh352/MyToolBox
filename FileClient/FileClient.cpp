


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fstream>
using boost::asio::ip::tcp;
const std::string TargetFileName = "yes_right.txt";

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query("127.0.0.1", "daytime");
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		
		std::fstream fileStream(TargetFileName,std::fstream::out|std::fstream::binary);
		
		boost::system::error_code error;
		do
		{
			boost::array<char, 1024> buf;
			

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			fileStream<<buf.data();
			//std::cout.write(buf.data(), len);
		}
		while(true);

		fileStream.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}