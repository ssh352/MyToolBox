#include "CTP_API.h"
#include <boost\lexical_cast.hpp>
#include <iostream>
namespace CTP
{

	std::string GenerateExchangeOrderID( OrderTypePtr aOrderPtr )
	{
		std::string lRet;
		lRet += boost::lexical_cast<std::string>(aOrderPtr->SessionID);
		lRet += '_';
		lRet += boost::lexical_cast<std::string>(aOrderPtr->FrontID);
		lRet += '_';
		lRet += boost::lexical_cast<std::string>(aOrderPtr->OrderRef);
		return lRet;
	}

	std::string GenerateClientOrderID( OrderTypePtr aOrderPtr )
	{
		std::string lRet;
		lRet += boost::lexical_cast<std::string>(aOrderPtr->OrderRef);
		return lRet;
	}

	std::string GenerateTradeID( TradeTypePtr aTradePtr )
	{
		std::string lRet;
		lRet += boost::lexical_cast<std::string>(aTradePtr->TraderID);
		return lRet;
	}

	bool IsErrorRspInfo( CThostFtdcRspInfoField *pRspInfo )
	{
		bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
		if(ret)
		{
			//TODO LOG
			std::cerr<<"Error Code="<<pRspInfo->ErrorID << "Ërror Message = "<<pRspInfo->ErrorMsg <<std::endl;
		}
		return ret;
	}

}
