#pragma once

#include "ExchangeTable_CTP.h"
#include "ProductTable_CTP.h"
#include "InstrumentTable_CTP.h"
#include "MarketDepthCacheCTP.h"
#include <vector>
#include <string>
#include <set>
namespace CTP
{

	class FindByExchangeID
	{
	public:
		static bool FindByPropert(const std::string& aPreportVal, Instrument_Ptr apInstrument)
		{
			return ( strcmp(aPreportVal.c_str(),apInstrument->ExchangeID) == 0);
		}
	};

	class FindByProductID
	{
	public:
		static bool FindByPropert(const std::string& aPreportVal, Instrument_Ptr apInstrument)
		{
			return ( strcmp(aPreportVal.c_str(),apInstrument->ProductID) == 0);
		}
	};

	class FindAllInstrument
	{
	public:
		static bool FindByPropert(const std::string& aPreportVal, Instrument_Ptr apInstrument)
		{
			return true;
		}
	};


	class DataCacheCTP
	{
	public:
		DataCacheCTP();
		void  AddExchange(Exchange_Ptr apExchange);
		void  AddInstrument(Instrument_Ptr apInstrument);
		//void  UpdataInstrument(Instrument_Ptr apInstrument);
		void  AddMarketTick(MarketDataPtr aTick);

		std::vector<std::string> GetInstrumentListByExchange(const std::string& ExchangID);
		std::vector<std::string> GetInstrumentListByProduct(const std::string& ProductID);
		std::vector<std::string> GetInstrumentListAll();
		std::set<std::string>	 GetProductList();
		MarketDepthCacheCTP*	 GetMarketDepthCache();

		template<typename FindPropertTrait>
		std::vector<std::string> GetInstrumentListByPropert(const std::string& aPreportVal)
		{
			 std::vector<std::string> lRet;

			 for(InstrumentTable_CTP::iterator iter = m_InstrumentMap.begin();iter != m_InstrumentMap.end();iter++)
			 {
				 if(FindPropertTrait::FindByPropert(aPreportVal,iter->second) )
				 {
					 lRet.push_back(std::string(iter->second->InstrumentID));
				 }
			 }
			 return lRet;
		}

		// Why I can not using BOOST_FOREACH
		//{
		//	std::vector<std::string> lRet;
		//	//typedef std::map<std::string,Instrument_Ptr>::_Val_type  InstrumentMapVal;
		//	//BOOST_FOREACH(InstrumentMapVal lPair,m_InstrumentMap)
		//	//{
		//	//	//if(FindPropertTrait::FindByPropert(aPreportVal,lPair.second) )
		//	//	//{
		//	//	//	lRet.push_back(std::string(lPair.second->InstrumentID));
		//	//	//}
		//	//}

		//	return lRet;

		//};

	private:
		ExchangeTable_CTP m_ExchangeMap;
		//ProductTable_CTP  m_ProductMap;
		InstrumentTable_CTP m_InstrumentMap;

		//tick store
		MarketDepthCacheCTP m_MarketDepthCacheCTP;
	};

}
