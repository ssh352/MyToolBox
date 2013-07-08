#pragma  once
#include "AT_API_HEAD.h"
#include "AT_Struct.h"
#include <string>
namespace AT
{
	class IMarketCache;
	//Store All The History Market

	enum class ESignalInputType
	{
		PARAM
	};

	static const int Ignore_Market_result = -9999;	//Ëæ±ãÑ¡µÄÖµ
	class IIndexModule
	{
	public:
		virtual void Start() =0;
		virtual void Stop() = 0;
		virtual void UpdateParam(AT::ESignalInputType aInputCode ,const AT::Param& apParam)  = 0;
		virtual int OnMarketDepth(const  AT::MarketData& aMarketPtr) = 0;
		std::string GetIndexName(){return m_IndexName;};
		void SetIndexName(const std::string& aIndexName){m_IndexName = aIndexName;};

		virtual std::map<AT_Time,int> GetHistoryResult() = 0;

	private:
		std::string m_IndexName;

		
	};

}
typedef  AT::IIndexModule* (*CreateSignalInstFun) (const char* aConfigFile, const AT::IMarketCache * apMarketCache);
DLL_API  AT::IIndexModule* CreateSignal(const char* aConfigFile, const AT::IMarketCache * apMarketCache);
