#include "CTP_TD_Mock.h"
#include "ITradeSpi.h"
#include <functional>
#include <thread>
#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace CTP
{


CTP_TD_Mock::CTP_TD_Mock(void)
{
	m_pWorker = new boost::asio::io_service::work(m_IOService);
	m_MockIOThread  = std::thread( boost::bind(&(boost::asio::io_service::run),&m_IOService));
}


CTP_TD_Mock::~CTP_TD_Mock(void)
{
	delete m_pWorker;
	if(m_MockIOThread.joinable()) m_MockIOThread.join();
}

std::string CTP_TD_Mock::CreateOrder( const std::string& aNewOrder )
{
	std::string lOrderID = MakeOrderID(aNewOrder);
	std::string lRtnOrder = BuildRtnOrder( aNewOrder,lOrderID);
	std::string lRtnTrade = BuildRtnTrade(aNewOrder,lOrderID);
	m_IOService.post(std::bind( &CTP_TD_Mock::OnRtnOrder,this,lRtnOrder));
	m_IOService.post(std::bind( &CTP_TD_Mock::OnRtnTrade,this,lRtnTrade));
	return lOrderID;
}

void CTP_TD_Mock::DeleteOrder( const std::string& aClientOrderID )
{
	//todo not support yet
}

void CTP_TD_Mock::ModifyOrder( const std::string& aRequest )
{
	//todo not support yet
}

void CTP_TD_Mock::QueryPosition( const std::string& aRequest )
{
	//todo not support yet
}

void CTP_TD_Mock::OnRtnOrder( const std::string aRtnOrder )
{
	m_pTradeSpi->OnRtnOrder(aRtnOrder);
}

void CTP_TD_Mock::OnRtnTrade( const std::string aRtnTrade )
{
	m_pTradeSpi->OnRtnTrade(aRtnTrade);
}

std::string CTP_TD_Mock::BuildRtnTrade( const std::string& aNewOrder,const std::string& OrderID )
{
	using boost::property_tree::ptree;
	ptree pt;
	pt.put("Trade.ThostOrderID",OrderID);
	std::stringstream lStringStream;
	write_xml(lStringStream,pt);
	return lStringStream.str();
	
}

std::string CTP_TD_Mock::BuildRtnOrder( const std::string& aNewOrder,const std::string& OrderID )
{
	using boost::property_tree::ptree;
	ptree pt;
	pt.put("Order.ThostOrderID",OrderID);
	std::stringstream lStringStream;
	write_xml(lStringStream,pt);
	return lStringStream.str(); 
}

void CTP_TD_Mock::Init( const std::map<std::string,std::string>& aConfigMap, AT::ITradeSpi* apTradeSpi )
{
	//todo 可以设置滑点率 设置最新价成交率等
	m_pTradeSpi = apTradeSpi;
}
std::string CTP_TD_Mock::MakeOrderID(const std::string& aNewOrder)
{
	static int i = 0;
	++i;
	std::string lret;
	lret += i;
	return lret;
}


}