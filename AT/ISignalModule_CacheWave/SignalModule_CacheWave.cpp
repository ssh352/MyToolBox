#include "SignalModule_CacheWave.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\lexical_cast.hpp>
namespace AT
{
	SignalModule_CacheWave::SignalModule_CacheWave(const char* aConfigFileName)
	{
		std::cout<<"Signal Con";
		m_isWaveStart = false;
		//todo load from file
	//	m_WaveVal = boost::lexical_cast<double>(aConfigFileName);
	// m_StoreFile ;
		m_LowPoint.first = 100000;
		m_HighPoint.first = -100000;
		m_CheckState = Direction::BothSide;
	}


	SignalModule_CacheWave::~SignalModule_CacheWave(void)
	{
		std::cout<<"Signal deCon";

	}

	int SignalModule_CacheWave::OnMarketDepth( const AT::MarketData& aMarketDepth )
	{

		int32_t lLastPrice  = aMarketDepth.m_LastPrice;
		switch (m_CheckState)
		{
		case SignalModule_CacheWave::Direction::BothSide:
			{
				bool isNewHigh = false;

				if(lLastPrice < m_LowPoint.first)
				{
					isNewHigh = false;
					m_LowPoint.first = lLastPrice;
					m_LowPoint.second = aMarketDepth;
				}

				if(lLastPrice> m_HighPoint.first)
				{
					isNewHigh = true;
					m_HighPoint.first = lLastPrice;
					m_HighPoint.second = aMarketDepth;
				}

				if( (m_HighPoint.first - m_LowPoint.first) > (m_WaveVal - 0.001 ))
				{
					if(isNewHigh)
					{
						m_CheckState = Direction::UpSide;
						m_WavePointVec.push_back(m_LowPoint.second);
						return 1;
					}
					else
					{
						m_CheckState = Direction::DownSide;
						m_WavePointVec.push_back(m_HighPoint.second);
						return -1;
					}
				}
			}
			break;
		case SignalModule_CacheWave::Direction::UpSide:
			{
				if(lLastPrice> m_HighPoint.first)
				{
					m_HighPoint.first = lLastPrice;
					m_HighPoint.second = aMarketDepth;
				}

				if( m_HighPoint.first - lLastPrice  > m_WaveVal - 0.001)
				{
					m_CheckState = Direction::DownSide;
					m_WavePointVec.push_back(m_HighPoint.second);
					m_LowPoint.first = lLastPrice ;
					m_LowPoint.second = aMarketDepth;
					return -1;
				}
			}
			break;
		case SignalModule_CacheWave::Direction::DownSide:
			{
				if(lLastPrice < m_LowPoint.first)
				{
					m_LowPoint.first = lLastPrice;
					m_LowPoint.second = aMarketDepth;
				}

				if(lLastPrice - m_LowPoint.first > m_WaveVal - 0.001)
				{
					m_CheckState = Direction::UpSide;
					m_WavePointVec.push_back(m_LowPoint.second);
					m_HighPoint.first = lLastPrice ;
					m_HighPoint.second = aMarketDepth;
					return 1;
				}
			}
			break;
		default:
			break;
		}

		return 0;
	}

	void SignalModule_CacheWave::Stop()
	{
		for(auto lMarket :m_WavePointVec)
		{
			lPointTree.add("Wave.Point",lMarket);
		}
		write_xml("WaveSignal.xml",lPointTree);
	}

}
