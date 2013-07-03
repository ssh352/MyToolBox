
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
		int						    m_SellDirectionVol;//���ֲ�����
		int						    m_BuyDirectionVol;//��ֲ�����
		int						    m_TotalOpenVol;//����ۼƿ�������
		int						    m_TotalCancleTime;//����������
		int						    m_AutoTradeTime;//�Գɽ�������
		std::string ToString() const
		{	
			return str(boost::format("[SellDirectionVol:%d] [BuyDirectionVol:%d %u] [TotalOpenVol:%d] [TotalCancleTime:%d] [AutoTradeTime%d]") 
				% m_SellDirectionVol %m_BuyDirectionVol % m_TotalOpenVol %m_TotalCancleTime %m_AutoTradeTime );
		}
	};

}
#pragma  pack(pop)