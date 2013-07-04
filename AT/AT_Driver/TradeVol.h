
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
		int						    SellDirectionVol;//卖持仓手数
		int						    BuyDirectionVol;//买持仓手数
		int						    TotalOpenVol;//最大累计开仓手数
		int						    TotalCancleTime;//撤单最多次数
		int						    AutoTradeTime;//自成交最大次数
		std::string ToString() const
		{	
			return str(boost::format("[SellDirectionVol:%d] [BuyDirectionVol:%d %u] [TotalOpenVol:%d] [TotalCancleTime:%d] [AutoTradeTime%d]") 
				% SellDirectionVol %BuyDirectionVol % TotalOpenVol %TotalCancleTime %AutoTradeTime );
		}
	};
	typedef std::map<uint64_t,std::shared_ptr<TradeVolData> >  TradeVolMap;
}
#pragma  pack(pop)