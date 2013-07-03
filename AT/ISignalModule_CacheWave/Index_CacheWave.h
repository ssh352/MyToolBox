#pragma once
#include <vector>
#include <string>
#include "ISignalModule.h"
#include <boost\property_tree\ptree.hpp>
namespace AT
{
	class Index_CacheWave :public AT::IIndexModule
	{
	public:
		Index_CacheWave(const char * configFile,const AT::IMarketCache * apMarketCache);
		virtual ~Index_CacheWave(void);

		virtual void UpdateParam(ESignalInputType errCode ,const Param& apParam) override {};
		virtual int OnMarketDepth(const MarketData& aMarketDepth) override;
		virtual void Start() override;
		virtual void Stop() override ;

	private: 
		std::pair<int32_t,MarketData>	m_LowPoint;
		std::pair<int32_t,MarketData>	m_HighPoint;


		enum  class Direction
		{
			BothSide,
			UpSide,
			DownSide
		};

		Direction m_CheckState;
		std::vector<MarketData>  m_WavePointVec;
		std::vector<MarketData> m_WaveChangeVec;
		bool m_isWaveStart;
		int32_t m_WaveVal;
		std::string m_InstrumentID;
		std::string m_StoreFile;
		std::string m_IndexName;
		const AT::IMarketCache * m_pMarketCache;
	
	};
}


