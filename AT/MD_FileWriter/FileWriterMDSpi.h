#pragma once
#include <fstream>
#include <map>
#include "IMarketSpi.h"
class FileWriterMDSpi :public AT::IMarketSpi
{
public:
	FileWriterMDSpi(const std::string& fieName);
	virtual ~FileWriterMDSpi();
public:
	virtual void NotifyState(int aErrorCode,const std::string& aErrorMsg) ;
	virtual void NotifyExchange(const std::string& aExchange) ;
	virtual void NotifyProduct(const std::string& aProduct) ;
	virtual void NotifyInstrument(const std::string& aInstrument);
	virtual void NotifyMarketDepth(const std::string& aMarketDepth);
private:
	std::fstream m_output_file;
	std::map<std::string , std::string> m_SnapShot;
	void WriteSnapShot();

};