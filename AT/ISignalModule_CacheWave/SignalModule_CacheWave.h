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
		SignalModule_CacheWave(const std::string& aConfigFile);
		virtual ~SignalModule_CacheWave(void);

		virtual int OnMarketDepth(const std::string& aMarketDepth) override;
		virtual void Start() override {};
		virtual void Stop() override ;

		std::pair<double,std::string>	m_LowPoint;
		std::pair<double,std::string>	m_HighPoint;


		enum  class Direction
		{
			BothSide,
			UpSide,
			DownSide
		};

		Direction m_CheckState;
		std::vector<std::string>  m_WavePointVec;
		bool m_isWaveStart;
		double m_WaveVal;
		boost::property_tree::ptree lPointTree;
	};
}


