#pragma once



#include "AT_Struct.h"
#include <map>
#include <string>
namespace AT
{
	//this class is used for warper STL map for cross DLL
	
	class MarketMapWarpper
	{
	public:
		
		typedef MarketMap::const_iterator const_iterator ;
	public:
		MarketMapWarpper(MarketMap*  aMap , std::string aID);
		virtual ~MarketMapWarpper(void);
		
	public:
		std::string GetInstrumentID()const {return m_InstrumentID;} ;
		const MarketData& operator[]  (const uint32_t& k) const;
		const MarketData& at(const uint32_t& k) const;
		const_iterator begin() const;
		const_iterator end() const;


	private:
		std::string  m_InstrumentID;
		MarketMap*  m_pMap;

	};



}

