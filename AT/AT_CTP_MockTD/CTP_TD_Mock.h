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
	CTP_TD_Mock(const char*  aConfigFile, AT::ITradeSpi* apSpi);
	virtual ~CTP_TD_Mock(void);

public:

	virtual void UpdateParam(const AT::Param& apParam) override;
	virtual void Start() override;
	virtual void Stop() override;

	virtual void CreateOrder(const AT::InputOrder& aNewOrder) override;
	virtual void DeleteOrder(const AT::CancelOrder& aDelOrderID) override;
	virtual	void ModifyOrder(const AT::ModifyOrder& aRequest) override;


private:
	std::string BuildRtnOrder(const std::string& aNewOrder,const std::string& OrderID);
	std::string BuildRtnTrade(const std::string& aNewOrder,const std::string& OrderID);

	std::thread m_MockIOThread;
	boost::asio::io_service m_IOService;
	boost::asio::io_service::work* m_pWorker;
	AT::ITradeSpi* m_pTradeSpi;
	boost::property_tree::ptree m_SaveFilePt; 
	boost::property_tree::ptree m_Total;
	std::string m_SavePath;
	int m_OrderNum;
	int32_t m_Profit;
	int32_t m_LastOpenPrice;
};

}