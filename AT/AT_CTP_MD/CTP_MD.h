#pragma once
#include "IDriver_MD.h"
#include "IMarketSpi.h"
#include <boost\shared_ptr.hpp>

namespace CTP
{
	class StateReceiver;
	class DepthReceiver;
	class DataCacheCTP;

	enum CTP_MD_CODE
	{
		CTP_MD_StateReceiver_Connecting,
		//CTP_MD_StateReceiver_Connected,
		CTP_MD_StateReceiver_Logining,
		CTP_MD_StateReceiver_Login_Failed,
		//CTP_MD_StateReceiver_Logined,
		CTP_MD_StateReceiver_Retrieving,
		CTP_MD_StateReceiver_Retrieve_Failed,
	};


	enum MD_SubMoudle_State
	{
		StateReceiver_UNINITIALIZE_STATE,
		StateReceiver_CONNECTING_STATE,
		StateReceiver_LOGINING_STATE,
		StateReceiver_RETRIEVE_EXCHANGE_STATE,
		StateReceiver_RETRIEVE_PRODUCT_STATE,
		StateReceiver_RETRIEVE_INSTRUMENT_STATE,
		StateReceiver_RETRIEVE_DYNAMIC_STATE,
		StateReceiver_ERROR_DISCONNECT_STATE,
		StateReceiver_ERROR_STOP_STATE,

		DepthReceiver_UNINITIALIZE_STATE,
		DepthReceiver_CONNECTING_STATE,
		DepthReceiver_LOGINING_STATE,
		DepthReceiver_RECEIVE_STATE,
		DepthReceiver_ERROR_DISCONNECT_STATE,
		DepthReceiver_ERROR_STOP_STATE,

		DepthReceiver_ERROR_SUBSCRIBE_EVENT,
		DepthReceiver_ERROR_UNSUBSCRIBE_EVENT

	};

	class CTP_MD :
		public AT::IDriver_MD
	{
	friend class StateReceiver;
	friend class DepthReceiver;
	public:
		CTP_MD(void);
		virtual ~CTP_MD(void);

	virtual void Init(const std::string&  aConfigStr,AT::IMarketSpi* apMarketSpi);


	//this block is using for member class direct post out
	private:
		 void NotifyExchange(const std::string& aExchange);
		 void NotifyProduct(const std::string& aProduct);
		 void NotifyInstrument(const std::string& aInstrument);
		 void NotifyMarketDepth(const std::string& aMarketDepth);

		//collect submodule info and notify state out
	private:
		void NotifySubModuleState(int aErrorCode,const std::string& aErrorMsg = "");

	private:
		AT::IMarketSpi*	m_MarketSpi;
		boost::shared_ptr<StateReceiver>  m_pStateReceiver;
		boost::shared_ptr<DepthReceiver>  m_pDepthReceiver;
		boost::shared_ptr<DataCacheCTP> m_pDataCache;

	};
}
