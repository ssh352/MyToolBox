#include "FileWriterMDSpi.h"
#include <iostream>

void FileWriterMDSpi::NotifyState( int aErrorCode,const std::string& aErrorMsg )
{

}

void FileWriterMDSpi::NotifyExchange( const std::string& aExchange )
{

}

void FileWriterMDSpi::NotifyProduct( const std::string& aProduct )
{

}

void FileWriterMDSpi::NotifyInstrument( const std::string& aInstrument )
{

}

void FileWriterMDSpi::NotifyMarketDepth( const std::string& aMarketDepth )
{
	m_SnapShot[aMarketDepth] = aMarketDepth;
	std::cout<<aMarketDepth;
	WriteSnapShot();
}

FileWriterMDSpi::FileWriterMDSpi( const std::string& fieName )
{
	if(m_output_file.is_open())	 m_output_file.close();

	m_output_file.open ("test.log", std::fstream::out);

}


FileWriterMDSpi::~FileWriterMDSpi()
{
	m_output_file.close();
}

void FileWriterMDSpi::WriteSnapShot()
{
	m_output_file.clear();
	for(auto lPari :m_SnapShot)
	{
		m_output_file<<lPari.second<<"\n";
	}
}

