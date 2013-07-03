#include "SignalModule_CacheWave.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
namespace AT
{
	SignalModule_CacheWave::SignalModule_CacheWave(int iWave/*const char* aConfigFileName*/)
		:m_WaveVal(iWave)
	{
		
		/*boost::property_tree::ptree lConfig;
		read_xml(aConfigFileName,lConfig);
		double lrawWaveVal  = lConfig.get<double>("ISignalModule_CacheWave.Wave_value");
		m_WaveVal = AT::TranPriceToInt(lrawWaveVal);
		m_StoreFile = lConfig.get<std::string>("ISignalModule_CacheWave.stroe_file");*/
		m_StoreFile = "Wave5.xml";
	}


	SignalModule_CacheWave::~SignalModule_CacheWave(void)
	{
		std::cout<<"Signal deCon";

	}


	void SignalModule_CacheWave::Start()
	{
		m_isWaveStart = false;
		m_LowPoint.first = 1000000000;
		m_HighPoint.first = -1000000000;
		m_CheckState = Direction::BothSide;
	}

	void SignalModule_CacheWave::Stop()
	{
		boost::property_tree::ptree lPointTree;
		for(AT::MarketData& lMarket :m_WavePointVec)
		{
			lPointTree.add("Wave.m_WavePointVec",lMarket.ToString());
		}
		for(AT::MarketData& lMarket :m_WaveChangeVec)
		{
			lPointTree.add("Wave.m_WaveChangeVec",lMarket.ToString());
		}
		write_xml(m_StoreFile,lPointTree);
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
						m_WaveChangeVec.push_back(aMarketDepth);
						return 1;
					}
					else
					{
						m_CheckState = Direction::DownSide;
						m_WavePointVec.push_back(m_HighPoint.second);
						m_WaveChangeVec.push_back(aMarketDepth);
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
					m_WaveChangeVec.push_back(aMarketDepth);
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
					m_WaveChangeVec.push_back(aMarketDepth);
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

}
