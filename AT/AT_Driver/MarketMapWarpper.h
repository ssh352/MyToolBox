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
		class const_iterator
		{

		public:
			const_iterator( AT::MarketMap::const_iterator aMapIter)
			: m_MapIter(aMapIter)
			{

			}

			const  MarketData& operator* ()
			{
				return *m_MapIter->second;
			}
			const_iterator& operator++()
			{
				m_MapIter++;
				return *this;
			}
			bool operator==(const const_iterator& aLR)
			{
				return m_MapIter == aLR.m_MapIter;
			}
			bool operator!= (const const_iterator& aLR)
			{
				return !(*this == aLR);
			}
		private:
			 MarketMap::const_iterator  m_MapIter;
		};
	//	typedef MarketMap::const_iterator const_iterator ;
	public:
		MarketMapWarpper(MarketMap*  aMap , std::string aID);
		virtual ~MarketMapWarpper(void);
		
	public:
		std::string GetInstrumentID()const {return m_InstrumentID;} ;

		std::pair<const_iterator,const_iterator> lower_bound(const uint32_t k) const;
		const_iterator upper_bound (const uint32_t k) const;
		std::pair<const_iterator,const_iterator> TimeRange(const uint32_t low , const uint32_t aHigh) const;
		const_iterator begin() const;
		const_iterator end() const;


	private:
		const MarketData& operator[]  (const uint32_t k) const;
		const MarketData& at(const uint32_t k) const;
		std::string  m_InstrumentID;
		MarketMap*  m_pMap;

	};



}

