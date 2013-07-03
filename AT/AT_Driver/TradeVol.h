
#include "ConstVarDefine.h"
#include <string>
#include <memory>
#include <boost\format.hpp>
#pragma  pack(push , 8)
//交易量数据，如撤单最多次数等
namespace AT
{
	struct TradeVolData
	{
		int						    m_SellDirectionVol;//卖持仓手数
		int						    m_BuyDirectionVol;//买持仓手数
		int						    m_TotalOpenVol;//最大累计开仓手数
		int						    m_TotalCancleTime;//撤单最多次数
		int						    m_AutoTradeTime;//自成交最大次数
		std::string ToString() const
		{	
			return str(boost::format("[SellDirectionVol:%d] [BuyDirectionVol:%d %u] [TotalOpenVol:%d] [TotalCancleTime:%d] [AutoTradeTime%d]") 
				% m_SellDirectionVol %m_BuyDirectionVol % m_TotalOpenVol %m_TotalCancleTime %m_AutoTradeTime );
		}
	};

}
#pragma  pack(pop)