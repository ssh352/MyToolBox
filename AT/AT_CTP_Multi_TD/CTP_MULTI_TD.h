#pragma once

#include "IDriver_TD.h"
#include <map>
#include <memory>
namespace CTP
{

	class CTP_TD;

	class CTP_MULTI_TD : public AT::IDriver_TD
	{
	public:
		CTP_MULTI_TD(void);
		virtual ~CTP_MULTI_TD(void);

	public:
		virtual void Init(const std::map<std::string,std::string>& aConfigMap, AT::ITradeSpi* apTradeSpi) ;
	public:
		virtual std::string CreateOrder(const std::string& aNewOrder);
		virtual void DeleteOrder(const std::string& aClientOrderID) ;
		virtual	void ModifyOrder(const std::string& aRequest) ;
		virtual void QueryPosition(const std::string& aRequest) ;

	private:
		std::map<std::string,std::shared_ptr<CTP_TD> > m_TradeInstMap;
		std::map<std::string, std::shared_ptr<CTP_TD> > mCLientID_TD_Map;
	};

}

