#pragma once
#include <string>
#include "IStrategy.h"
#include <boost\property_tree\ptree.hpp>
#include <vector>
class WaveCacher :public AT::IStrategy
{
public:
	WaveCacher(const std::string& aConfigFile);
	virtual ~WaveCacher(void);


	virtual void UpdateParam(EStrInputState errCode ,const std::string& apParam) override;
	virtual void Start() override;
	virtual void Stop()override;
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth)override;
	virtual void OnRtnOrder(const std::string& apOrder)override;
	virtual void OnRtnTrade(const std::string& apTrade)override;
	virtual void OnRtnPos(const std::string& aPos)override;



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

