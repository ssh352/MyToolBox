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


CTP_TD_Mock::CTP_TD_Mock(const std::string& aConfigFile,AT::ITradeSpi* apSpi)
	: m_pTradeSpi(apSpi)
	, m_SavePath(aConfigFile)
	, m_OrderNum(0)
	, m_Profit(0)
{

}


CTP_TD_Mock::~CTP_TD_Mock(void)
{

}

std::string CTP_TD_Mock::CreateOrder( const std::string& aNewOrder )
{
	std::stringstream lBuf(aNewOrder);
	boost::property_tree::ptree lOrderPt;
	read_xml(lBuf,lOrderPt);


	std::string lInstrumen = lOrderPt.get<std::string>("Order.ID");
	std::string lBuySell = lOrderPt.get<std::string>("Order.BuyCode");
	std::string lOpenClose = lOrderPt.get<std::string>("Order.OpenCode");
	std::string lPrice = lOrderPt.get<std::string>("Order.Price");
	std::string lVol = lOrderPt.get<std::string>("Order.Vol");
	std::string lTime =lOrderPt.get<std::string>("Order.Time");



	if(lBuySell == "Buy")
	{
		m_Profit -= std::stod(lPrice);
	}
	else
	{
		m_Profit += std::stod(lPrice);
	}


	if(lOpenClose == "Open")
	{
		m_LastOpenPrice = std::stod(lPrice);
	}
	else
	{
		m_LastOpenPrice = 0;
	}
	m_OrderNum ++;

	
	//std::string lOrderKey = "All.Order";
	//lOrderKey  += '.' ;
	//lOrderKey  += boost::lexical_cast<std::string>(m_OrderNum);
	//lOrderKey  += '.';
	//m_SaveFilePt.put(lOrderKey + "ID",lInstrumen);
	//m_SaveFilePt.put(lOrderKey + "BuyCode",lBuySell);
	//m_SaveFilePt.put(lOrderKey + "OpenCode",lOpenClose);
	//m_SaveFilePt.put(lOrderKey + "Price",lPrice);
	//m_SaveFilePt.put(lOrderKey + "Vol",lVol);


	std::string SaveOrder = str(boost::format("%s %s %s %s %s %s")%lInstrumen%lBuySell%lOpenClose%lPrice%lVol%lTime);


	m_SaveFilePt.add("OrderList.Order",SaveOrder);
	



	std::string lOrderID = MakeOrderID(aNewOrder);
	std::string lRtnOrder = BuildRtnOrder( aNewOrder,lOrderID);
	std::string lRtnTrade = BuildRtnTrade(aNewOrder,lOrderID);
	m_IOService.post(std::bind( &AT::ITradeSpi::OnRtnOrder,m_pTradeSpi,lRtnOrder));
	m_IOService.post(std::bind( &AT::ITradeSpi::OnRtnTrade,m_pTradeSpi,lRtnTrade));
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

std::string CTP_TD_Mock::MakeOrderID(const std::string& aNewOrder)
{
	static int i = 0;
	++i;

	return boost::lexical_cast<std::string>(i);
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


}