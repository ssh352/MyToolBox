#include "PrintTDSpi.h"
#include "ATLogger.h"


using namespace  AT;
void PrintTDSpi::OnRtnOrder( const AT::OrderUpdate& apOrder )
{
	ATLOG(LogLevel::L_INFO,ToString(apOrder));
}

void PrintTDSpi::OnRtnTrade( const AT::TradeUpdate& apTrade )
{
	ATLOG(LogLevel::L_INFO,ToString(apTrade));
}

void PrintTDSpi::NotifyStateTD( AT::ETradeState aErrorCode,const char* aErrorMsg )
{
	std::string lRet = ToString(aErrorCode);
	lRet += aErrorMsg;
	ATLOG(LogLevel::L_INFO,lRet);
}

