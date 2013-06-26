#pragma once

#include "IDriver_TD.h"
#include <map>
#include <set>
#include <memory>
#include <string>
#include "ConstVarDefine.h"

namespace AT
{
	class ITradeSpi;
}
namespace CTP
{

	class CTP_TD;

	class CTP_MULTI_TD : public AT::IDriver_TD
	{
	public:
		CTP_MULTI_TD(const std::string& aConfigFile,AT::ITradeSpi* apTradeSpi);
		virtual ~CTP_MULTI_TD(void);

		virtual void UpdateParam(const AT::Param& apParam)override;
		virtual void Start()override;
		virtual void Stop() override;

		virtual void CreateOrder(const AT::InputOrder& aNewOrder) override;
		virtual void DeleteOrder(const  AT::CancelOrder& aDelOrderID) override;

		

		virtual	void ModifyOrder(const  AT::ModifyOrder& aRequest) override;

	private:
		std::string FindOrderInstByOrderKey( const AT::AT_Order_Key & aKey );
		void LoadSignalTD(const std::string& AccountName, const std::string aConfigFileName);
		std::map<std::string,std::shared_ptr<CTP_TD> > m_TradeInstMap;
		std::map<std::string, std::set<AT::AT_Order_Key> > m_OrderKeyMap;
		AT::ITradeSpi*			m_pTradeSpi;
	};

}

