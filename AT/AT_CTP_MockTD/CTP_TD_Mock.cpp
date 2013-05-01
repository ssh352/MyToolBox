#include "CTP_TD_Mock.h"
#include "ITradeSpi.h"
#include <functional>
#include <thread>
#include <boost\bind.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\format.hpp>
namespace CTP
{


CTP_TD_Mock::CTP_TD_Mock(const char* aConfigFile,AT::ITradeSpi* apSpi)
	: m_pTradeSpi(apSpi)
	, m_SavePath(aConfigFile)
	, m_OrderNum(0)
	, m_Profit(0)
{

}


CTP_TD_Mock::~CTP_TD_Mock(void)
{

}

void CTP_TD_Mock::CreateOrder(const AT::InputOrder&  aNewOrder )
{

	
	std::string lInstrumen = aNewOrder.InstrumentID;

	if(aNewOrder.m_BuySellType == AT::BuySellType::BuyOrder)
	{
		m_Profit -= aNewOrder.m_Price;
	}
	else
	{
		m_Profit +=  aNewOrder.m_Price;
	}


	if(aNewOrder.m_OpenCloseType == AT::OpenCloseType::OpenOrder)
	{
		m_LastOpenPrice =  aNewOrder.m_Price;
	}
	else
	{
		m_LastOpenPrice = 0;
	}
	m_OrderNum ++;


	std::string SaveOrder = str(
		boost::format("%s  [BuySell:%s] [OpenClose:%s] [Price:%d] [Vol:%u] [Time:%s]")
		% lInstrumen
		% (aNewOrder.m_BuySellType == AT::BuySellType::BuyOrder ? "Buy":"Sell")
		% (aNewOrder.m_OpenCloseType == AT::OpenCloseType::OpenOrder? "Open":"Close" )
		% aNewOrder.m_Price % aNewOrder.m_Vol % aNewOrder.m_MoreInfo
		);


	m_SaveFilePt.add("OrderList.Order",SaveOrder);
	

	//std::string lRtnOrder = BuildRtnOrder( aNewOrder,lOrderID);
	//std::string lRtnTrade = BuildRtnTrade(aNewOrder,lOrderID);
	//m_IOService.post(std::bind( &AT::ITradeSpi::OnRtnOrder,m_pTradeSpi,lRtnOrder));
	//m_IOService.post(std::bind( &AT::ITradeSpi::OnRtnTrade,m_pTradeSpi,lRtnTrade));
}

void CTP_TD_Mock::DeleteOrder( const AT::CancelOrder& aDelOrderID)
{
	//todo not support yet
}

void CTP_TD_Mock::ModifyOrder( const AT::ModifyOrder& aRequest)
{
	//todo not support yet
}


void CTP_TD_Mock::Start()
{
	m_pWorker = new boost::asio::io_service::work(m_IOService);
	m_MockIOThread  = std::thread( boost::bind(&(boost::asio::io_service::run),&m_IOService));
	m_pTradeSpi->NotifyStateTD(AT::ETradeState::READY,"Mock TD Start and Ready");
}

void CTP_TD_Mock::Stop()
{
	delete m_pWorker;
	if(m_MockIOThread.joinable()) m_MockIOThread.join();

	boost::property_tree::ptree lTotal;
	lTotal.put("All.Static.TotalProfit",m_Profit );
	lTotal.put("All.Static.OperatorTimes",m_OrderNum);
	lTotal.put("All.Static.LastOpenPrice",m_LastOpenPrice);
	lTotal.put("All.Static.RemoveLastProfit",m_Profit +m_LastOpenPrice );

	if(!m_SavePath.empty())
	{
		std::string lSavePath = m_SavePath + ".xml";
		write_xml( lSavePath.c_str(),m_SaveFilePt);
		std::string TotalPath = m_SavePath + "Total" + ".xml";
		write_xml(TotalPath.c_str(),lTotal);
	}
	else
	{
		write_xml( "DefaultSaveFile.xml",m_SaveFilePt);
		write_xml("DefaultSaveFileTotal.xml",lTotal);
	}



	write_xml( "DefaultSaveFile",m_SaveFilePt);


	m_pTradeSpi->NotifyStateTD(AT::ETradeState::STOP,"Mock TD Stop");
}

void CTP_TD_Mock::UpdateParam( const AT::Param& apParam )
{

}


}