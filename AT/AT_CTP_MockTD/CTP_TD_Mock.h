#pragma once
#include <thread>
#include "IDriver_TD.h"
#include <boost\asio.hpp>
namespace CTP
{
class CTP_TD_Mock :public AT::IDriver_TD
{
public:
	CTP_TD_Mock(const std::string& aConfigFile, AT::ITradeSpi* apSpi);
	virtual ~CTP_TD_Mock(void);

public:

	virtual void Start();
	virtual void Stop();

	virtual std::string CreateOrder(const std::string& aNewOrder);
	virtual void DeleteOrder(const std::string& aClientOrderID) ;
	virtual	void ModifyOrder(const std::string& aRequest) ;
	virtual void QueryPosition(const std::string& aRequest);

private:
	std::string BuildRtnOrder(const std::string& aNewOrder,const std::string& OrderID);
	std::string BuildRtnTrade(const std::string& aNewOrder,const std::string& OrderID);

	void OnRtnOrder(const std::string aRtnOrder);
	void OnRtnTrade(const std::string aRtnTrade);
	std::string MakeOrderID(const std::string& aNewOrder);

	std::thread m_MockIOThread;
	boost::asio::io_service m_IOService;
	boost::asio::io_service::work* m_pWorker;
	AT::ITradeSpi* m_pTradeSpi;
};

}