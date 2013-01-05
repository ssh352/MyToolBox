#pragma once
#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"

struct CThostFtdcDepthMarketDataField;
namespace leveldb
{
	class DB;
}

namespace CTP
{
	typedef 	boost::shared_ptr<CThostFtdcDepthMarketDataField> MarketDataPtr;
//	typedef		std::vector<MarketDataPtr>						MarketDataPtrVec;
	class CThostFtdcDepthMarketDataFieldTraits
	{
	public:
		static std::string GetItemID(MarketDataPtr aTickData)
		{
			std::string lInstrumentIDKey (aTickData->InstrumentID,30);
			std::string	lTime(aTickData->UpdateTime,8);
			//std::string	lTime(boost::lexical_cast<std::string>(aTickData->UpdateMillisec));
			lInstrumentIDKey += lTime;
			return lInstrumentIDKey ;
		};
		static std::string GetItemTypeName(){return "CThostFtdcDepthMarketDataField";};
		static const int IsDuplicateException = 0;
	};

	class MarketDepthCacheCTP :public ItemTable<CThostFtdcDepthMarketDataField,CThostFtdcDepthMarketDataFieldTraits>
	{
	public:
		void InsertMarketTick( MarketDataPtr  aTickData);
	};
}



