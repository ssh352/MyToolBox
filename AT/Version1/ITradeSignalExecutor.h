#pragma  once
#include "AT_Struct.h"
#include "TradeCommand.h"
#include <boost\smart_ptr.hpp>
#include <boost\function.hpp>
namespace AT
{


	typedef boost::function<void(int32_t Price,int32_t vol,bool IsBuy, bool isFinish)> FinishExecuteCallbackType;
	class ITradeSignalExecutor 
	{
	
	public:

		//����1 �������ϲ�Ľ����ź�
		virtual boost::shared_ptr<TradeCommand> SetupTarget(int targetQuantity,bool IsBuy, const AT::MarketData& aMarket) =0;
		virtual boost::shared_ptr<TradeCommand> AddTarget(int addTargetQuantity, bool isBuy, const AT::MarketData& aMarket) =0;

		//����2 ������ִ�в���
		virtual  boost::shared_ptr<TradeCommand> OnMarketDepth(const AT::MarketData& aMarketDepth) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnOrder(const  AT::OrderUpdate& apOrder) =0;
		virtual  boost::shared_ptr<TradeCommand> OnRtnTrade(const  AT::TradeUpdate& apTrade) =0;

		//����3 ������������callback
		//
		virtual void SetFinishedCallback(FinishExecuteCallbackType aFinishCallback) {m_FinishehNotfiy = aFinishCallback;};
		virtual std::string GetExecutorID() = 0;

		//��ʼ���õ��ֲ߳��޶�
		virtual void InitSignalDirectionVol(){m_BuyDirectionVol = 0;m_SellDirectionVol = 0;}
		//���õ��ֲ߳��޶�
		virtual void SetSignalDirectionVol(BuySellType type,int Vol)
		{
			if(type == BuySellType::BuyOrder)
				m_BuyDirectionVol += Vol;
			else
				m_SellDirectionVol += Vol;
		}
		//��ȡ���ֲ߳��޶�
		virtual int GetSignalDirectionVol(BuySellType type)
		{
			if(type == BuySellType::BuyOrder)
			{
				return m_BuyDirectionVol;
			}
			return m_SellDirectionVol;
		}
		//��ʼ���ܵĽ��״���
		virtual void InitTotalTime(){m_TotalTime = 0;}
		//���ý��״���
		virtual void SetTotalTime(){m_TotalTime++;}
		//��ȡ���״���
		virtual int  GetTotalTime(){return m_TotalTime;}

		//��ʼ���ܵĳ������״���
		virtual void InitTotalCancleTime(){m_TotalCancleTime = 0;}
		//���ó������״���
		virtual void SetTotalCancleTime(){m_TotalCancleTime++;}
		//��ȡ�������״���
		virtual int GetTotalCancleTime(){return m_TotalCancleTime;}
	protected:
		FinishExecuteCallbackType  m_FinishehNotfiy;
		int						   m_SellDirectionVol;
		int						   m_BuyDirectionVol;
		int						   m_TotalTime;
		int						   m_TotalCancleTime;
	};


}