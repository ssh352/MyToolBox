#include "CTP_API.h"
#include <boost\lexical_cast.hpp>

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

}
