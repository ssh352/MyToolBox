#include "DepthReceiver.h"
#include "CTP_API.h"

namespace CTP
{

	DepthReceiver::DepthReceiver( const std::string aConfigStr )
	{

	}

	void DepthReceiver::SetStateReceive( CTP_MD* parent )
	{

	}

	void DepthReceiver::Start()
	{
		char lConPath[128] = "./MD_DepthReceiver/";
		m_pMDAPI = CThostFtdcTraderApi::CreateFtdcTraderApi();
		m_pMDAPI->RegisterSpi(this);		 
		m_pMDAPI->RegisterFront("tcp://asp-sim2-front1.financial-trading-platform.com:26205");
		m_pMDAPI->Init();
		m_RuningState = CONNECTING_STATE;
		m_pCTP_MD->NotifySubModuleState(CTP_MD_StateReceiver_Connecting);	
	}

}
