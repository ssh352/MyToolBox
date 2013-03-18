#include "FileWriterMDSpi.h"
#include "myForeach.h"
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
	//size_t lpos = aMarketDepth.find_first_of(' ',0);
	//m_SnapShot[aMarketDepth.substr(0,lpos)] = aMarketDepth;
	std::cout<<aMarketDepth;
	m_output_file<<aMarketDepth;
	//WriteSnapShot();
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
	m_output_file.seekg(0,std::ios_base::beg);
	MYFOREACH(lPari ,m_SnapShot)
	{
		m_output_file<<lPari.second<<"\n";
	}
}

