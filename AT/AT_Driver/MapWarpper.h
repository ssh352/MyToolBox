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
		typedef std::map<uint32_t,MarketData>::const_iterator const_iterator ;
	public:
		MapWarpper(	 MarketMapType*  aMap , std::string aID);
		virtual ~MapWarpper(void);
		
	public:
		std::string GetInstrumentID()const {return m_InstrumentID;} ;
		const MarketData& operator[]  (const uint32_t& k) const;
		const MarketData& at(const uint32_t& k) const;
		const_iterator begin() const;
		const_iterator end() const;
	private:
		std::string  m_InstrumentID;
		MarketMapType*  m_pMap;

	};



}

