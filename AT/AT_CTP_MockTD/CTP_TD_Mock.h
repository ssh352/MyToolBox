#pragma once
#include <thread>
#include "IDriver_TD.h"
#include <boost\asio.hpp>
#include <boost\property_tree\ptree.hpp>


namespace CTP
{
class CTP_TD_Mock :public AT::IDriver_TD
{
public:
	CTP_TD_Mock(const std::string& aConfigFile, AT::ITradeSpi* apSpi);
	virtual ~CTP_TD_Mock(void);

public:

	virtual void Start() override;
	virtual void Stop() override;

	virtual std::string CreateOrder(const std::string& aNewOrder) override;
	virtual void DeleteOrder(const std::string& aClientOrderID) override ;
	virtual	void ModifyOrder(const std::string& aRequest) override;
	virtual void QueryPosition(const std::string& aRequest) override;

private:
	std::string BuildRtnOrder(const std::string& aNewOrder,const std::string& OrderID);
	std::string BuildRtnTrade(const std::string& aNewOrder,const std::string& OrderID);

	std::string MakeOrderID(const std::string& aNewOrder);

	std::thread m_MockIOThread;
	boost::asio::io_service m_IOService;
	boost::asio::io_service::work* m_pWorker;
	AT::ITradeSpi* m_pTradeSpi;
	boost::property_tree::ptree m_SaveFilePt; 
	boost::property_tree::ptree m_Total;
	std::string m_SavePath;
	int m_OrderNum;
	double m_Profit;
	double m_LastOpenPrice;
};

}