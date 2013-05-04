#include "DataCache_CTP_TD.h"

namespace CTP
{
	DataCache_CTP_TD::DataCache_CTP_TD( const std::string& aDir )
	{
		m_OrderTable.InitWithDB(aDir+"OrderMapDB");
		m_TradeTable.InitWithDB(aDir+"TradeMapDB");
		m_InputOrderTable.InitWithDB(aDir+"InputOrderMapDB");
		m_PositionTable.InitWithDB(aDir+"PositionMapDB");
		RebuildOrderIDMap();
	}

	void DataCache_CTP_TD::RebuildOrderIDMap()
	{
		for( auto lPair: m_OrderTable)
		{
			UpdataOrderIDMap(lPair.second);
		}
	}

	DataCache_CTP_TD::~DataCache_CTP_TD(void)
	{
	}

	void DataCache_CTP_TD::UpdataOrder( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	{
		m_OrderTable.AddOrder(apOrder);
		UpdataOrderIDMap(apOrder);
	}

	void DataCache_CTP_TD::UpdataTrade( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
		m_TradeTable.AddTrade(apTrade);
		UpdatePositionWithTrade(apTrade);
	}

	void DataCache_CTP_TD::UpdataOrderIDMap( boost::shared_ptr<CThostFtdcOrderField> apOrder )
	{
		std::string lExchangeOrderID = GenerateExchangeOrderID(apOrder);
		std::string lThostOrderID  = GenerateThostOrderID(apOrder);
		m_ThostOrderID_ExchangOrderID[lThostOrderID] = lExchangeOrderID;
		m_ExchangOrderID_ThostOrderID[lExchangeOrderID] = lThostOrderID;
	}

	std::string DataCache_CTP_TD::GetThostOrderIDByExchangeOrderID( const std::string& lExchangeOrderID )
	{
		if(m_ExchangOrderID_ThostOrderID.count(lExchangeOrderID))
		{
			return m_ExchangOrderID_ThostOrderID[lExchangeOrderID];
		}
		else
		{
			throw std::logic_error("Can not GetThostOrderIDByExchangeOrderID " + lExchangeOrderID);
		}
	}

	std::string DataCache_CTP_TD::GetExchangeOrderIDByThostOrderID( const std::string& lThostOrderID )
	{
		if(m_ThostOrderID_ExchangOrderID.count(lThostOrderID))
		{
			return m_ThostOrderID_ExchangOrderID[lThostOrderID];
		}
		else
		{
			throw std::logic_error("Can not GetExchangeOrderIDByThostOrderID  "  + lThostOrderID );
		}
	}

	void DataCache_CTP_TD::UpdataInputOrder( InputOrderTypePtr apInputOrder )
	{
		m_InputOrderTable.AddOrder(apInputOrder);
	}

	InputOrderTypePtr DataCache_CTP_TD::FindInputOrderByThostID( const std::string& lThostOrderID )
	{
		return	m_InputOrderTable.GetOrder(lThostOrderID);
	}

	void DataCache_CTP_TD::ClearPosition()
	{
		m_PositionTable.ClearPosition();
	}

	void DataCache_CTP_TD::UpdatePosition( boost::shared_ptr<CThostFtdcInvestorPositionField> apPosition )
	{
		m_PositionTable.AddPostion(apPosition);
	}

	std::string DataCache_CTP_TD::GeneratorPositionString()
	{
		std::stringstream lbuf;
		lbuf<<"All Position Start"<<'\n';
		for (auto lPair: m_PositionTable)
		{
			lbuf << "==================\n"<< GeneratorOnePositionString(lPair.second);
		}
		lbuf<<"All Position End"<<'\n';
		std::string lRet(lbuf.str());
		return lRet;
	}

	std::string DataCache_CTP_TD::GetPositionID( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
		char lPositionDir;
		if(apTrade ->Direction == THOST_FTDC_D_Buy)
		{
			lPositionDir = THOST_FTDC_PD_Long;
		}
		else
		{
			lPositionDir = THOST_FTDC_PD_Short;
		}
		std::string lRet;
		lRet += apTrade->InstrumentID;
		lRet += "_";
		lRet += lPositionDir;
		return lRet;
	}

	void DataCache_CTP_TD::UpdatePositionWithTrade( boost::shared_ptr<CThostFtdcTradeField> apTrade )
	{
		std::string  lPositionID  = GetPositionID(apTrade);
		int			 lPosChange = apTrade->Volume;
		Postion_Ptr lAlreadyPostion = m_PositionTable.GetPosition(lPositionID);
		if(!lAlreadyPostion)
		{
			lAlreadyPostion.reset(new CThostFtdcInvestorPositionField);
			memset(lAlreadyPostion.get(),0,sizeof(CThostFtdcInvestorPositionField));
			memcpy(lAlreadyPostion->InstrumentID,apTrade->InstrumentID,31);
			char lPositionDir;
			if(apTrade ->Direction == THOST_FTDC_D_Buy)
			{
				lPositionDir = THOST_FTDC_PD_Long;
			}
			else
			{
				lPositionDir = THOST_FTDC_PD_Short;
			}
			lAlreadyPostion->PosiDirection = lPositionDir;
			m_PositionTable.AddPostion(lAlreadyPostion);
		}



		switch(apTrade->OffsetFlag)
		{
		case THOST_FTDC_OF_Open:
			lAlreadyPostion->Position += lPosChange;
			break;
		case THOST_FTDC_OF_Close:
		case THOST_FTDC_OF_ForceClose:
		case THOST_FTDC_OF_ForceOff:
		case THOST_FTDC_OF_LocalForceClose:
			{
				//todo clean when release
				assert(false);
				throw std::logic_error("Check Handle how to do");
			}
			break;
		case THOST_FTDC_OF_CloseToday:
			lAlreadyPostion->Position -= lPosChange;
			break;
		case THOST_FTDC_OF_CloseYesterday:
			lAlreadyPostion->YdPosition -= lPosChange;
			break;
		}
		if(lAlreadyPostion->YdPosition == 0 && lAlreadyPostion->Position == 0)
		{
			m_PositionTable.DelPosition(lAlreadyPostion);
		}
		
	}

	std::string DataCache_CTP_TD::GeneratorOnePositionString( boost::shared_ptr<CThostFtdcInvestorPositionField> apPosition )
	{
		std::stringstream lbuf;
		lbuf<< "one Position Start"<<'\n';
		lbuf<< "InstrumentID = "<<apPosition->InstrumentID<<'\n';
		lbuf<< "PosiDirection = "<<apPosition->PosiDirection<<" 123 = net long short"<<'\n';
		lbuf<< "YdPosition = "<<apPosition->YdPosition<<'\n';
		lbuf<< "Position = "<<apPosition->Position<<'\n';
		lbuf<< "one Position End"<<'\n';
		std::string lRet(lbuf.str());
		return lRet;
	}

	boost::shared_ptr<CThostFtdcOrderField> DataCache_CTP_TD::FindOrderByThostID( const std::string& lThostOrderID )
	{
		return m_OrderTable.GetOrder(lThostOrderID);
	}

}

