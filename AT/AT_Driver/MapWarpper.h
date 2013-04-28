#pragma once



#include "AT_Struct.h"
#include <map>
#include <string>
namespace AT
{
	//this class is used for warper STL map for cross DLL
	class MapWarpper
	{
	public:
		typedef std::map<uint32_t,MarketData>  MarketMapType;
		typedef std::map<uint32_t,MarketData>::iterator iterator;
	public:
		MapWarpper(	 MarketMapType*  aMap , std::string aID);
		virtual ~MapWarpper(void);
		
	public:
		std::string GetInstrumentID(){return m_InstrumentID;};
		const MarketData& operator[] (const uint32_t& k);
		iterator begin();
		iterator end();



	private:
		std::string  m_InstrumentID;
		MarketMapType*  m_pMap;

	};



}

