#include "Index_CacheWave.h"
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include "IMarketCache.h"
#include "MarketMapWarpper.h"
namespace AT
{
	Index_CacheWave::Index_CacheWave(const char* aConfigFileName,const AT::IMarketCache * apMarketCache)
		:m_pMarketCache(apMarketCache)
	{
		
		boost::property_tree::ptree lConfig;
		read_xml(aConfigFileName,lConfig);
		double lrawWaveVal  = lConfig.get<double>("IIndexMoudle_CacheWave.Wave_value");
		m_WaveVal = AT::TranPriceToInt(lrawWaveVal);
		m_StoreFile = lConfig.get<std::string>("IIndexMoudle_CacheWave.stroe_file");

		m_InstrumentID = lConfig.get<std::string>("IIndexMoudle_CacheWave.Instrument");

		std::string lIndexName =  lConfig.get<std::string>("IIndexMoudle_CacheWave.IndexName");
		SetIndexName(lIndexName);

	}


	Index_CacheWave::~Index_CacheWave(void)
	{
		std::cout<<"Index CacheWave deCon";

	}


	void Index_CacheWave::Start()
	{
		m_isWaveStart = false;
		m_LowPoint.first = 1000000000;
		m_HighPoint.first = -1000000000;
		m_CheckState = Direction::BothSide;

		const_MapWarpper_ptr lpMarketCacheByID = m_pMarketCache->GetMarketMapByName(m_InstrumentID.c_str());
		for(MarketMapWarpper::const_iterator iter =  lpMarketCacheByID->begin(); iter != lpMarketCacheByID->end();++iter)
		{
			OnMarketDepth(*iter);
		}
	}

	void Index_CacheWave::Stop()
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


	int Index_CacheWave::OnMarketDepth( const AT::MarketData& aMarketDepth )
	{

		int32_t lLastPrice  = aMarketDepth.m_LastPrice;
		switch (m_CheckState)
		{
		case Index_CacheWave::Direction::BothSide:
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
		case Index_CacheWave::Direction::UpSide:
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
		case Index_CacheWave::Direction::DownSide:
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
