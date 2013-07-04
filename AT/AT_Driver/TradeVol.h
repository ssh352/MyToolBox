
#include "ConstVarDefine.h"
#include <string>
#include <memory>
#include <boost\format.hpp>
#pragma  pack(push , 8)
//���������ݣ��糷����������
namespace AT
{
	struct TradeVolData
	{
		int						    SellDirectionVol;//���ֲ�����
		int						    BuyDirectionVol;//��ֲ�����
		int						    TotalOpenVol;//����ۼƿ�������
		int						    TotalCancleTime;//����������
		int						    AutoTradeTime;//�Գɽ�������
		std::string ToString() const
		{	
			return str(boost::format("[SellDirectionVol:%d] [BuyDirectionVol:%d %u] [TotalOpenVol:%d] [TotalCancleTime:%d] [AutoTradeTime%d]") 
				% SellDirectionVol %BuyDirectionVol % TotalOpenVol %TotalCancleTime %AutoTradeTime );
		}
	};
	typedef std::map<uint64_t,std::shared_ptr<TradeVolData> >  TradeVolMap;
}
#pragma  pack(pop)