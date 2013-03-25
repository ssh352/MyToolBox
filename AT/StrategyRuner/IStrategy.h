#pragma once
#include <string>



namespace AT
{
	struct StrategyPram
	{
		int MessageType;
		int MessageLenth;
		char* m_Buffer;
	};

	enum  StrategyState
	{
		Str_Init,//include sync
		Str_Start,
		Str_Stop,
		Str_Reload
	};


	class IStrategy 
	{
	public:
		virtual ~IStrategy(void) {};

		
	public:
		virtual void NotifyState(int aErrorCode) = 0;
		virtual void SetStrategyPram(StrategyPram* apStrParam) = 0;
	public:
		virtual void OnMarketDepth(const std::string& aMarketDepth)= 0;
		virtual void OnRtnOrder(const std::string& apOrder) = 0;
		virtual void OnRtnTrade(const std::string& apTrade)=0;
		virtual void OnRtnPos(const std::string& aPos) = 0;
	};
}


