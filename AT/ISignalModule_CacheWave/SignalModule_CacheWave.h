#pragma once
#include <vector>
#include <string>
#include "ISignalModule.h"
#include <boost\property_tree\ptree.hpp>
namespace AT
{
	class SignalModule_CacheWave :public AT::ISignalModule
	{
	public:
		SignalModule_CacheWave(const char* aConfigFile);
		virtual ~SignalModule_CacheWave(void);

		virtual void UpdateParam(ESignalInputType errCode ,const Param& apParam) override {};
		virtual int OnMarketDepth(const MarketData& aMarketDepth) override;
		virtual void Start() override;
		virtual void Stop() override ;
		virtual std::string GetIndexName() override{return "HKY006";}

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
		std::string m_StoreFile;
	
	};
}


