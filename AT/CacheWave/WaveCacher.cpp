#include "WaveCacher.h"
#include <boost\lexical_cast.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <sstream>
WaveCacher::WaveCacher(const std::string& aConfigFile)
	: m_isWaveStart(false)
{
	m_WaveVal = boost::lexical_cast<double>(aConfigFile);
	m_LowPoint.first = 100000;
	m_HighPoint.first = -100000;

	m_CheckState = Direction::BothSide;
}


WaveCacher::~WaveCacher(void)
{
}


void WaveCacher::UpdateParam( EStrInputState errCode ,const std::string& apParam )
{

}

void WaveCacher::Start()
{

}

void WaveCacher::Stop()
{
	for(auto lMarket :m_WavePointVec)
	{
		lPointTree.add("Wave.Point",lMarket);
	}
	write_xml("Wave.xml",lPointTree);
}

void WaveCacher::OnMarketDepth( const std::string& aMarketDepth )
{
	std::stringstream lbuf(aMarketDepth);
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(lbuf,pt);
	double lLastPrice  = pt.get<double>("market.LastPx");


	switch (m_CheckState)
	{
	case WaveCacher::Direction::BothSide:
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
				}
				else
				{
					m_CheckState = Direction::DownSide;
					m_WavePointVec.push_back(m_HighPoint.second);
				}
			}
		}
		break;
	case WaveCacher::Direction::UpSide:
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
			}
		}
		break;
	case WaveCacher::Direction::DownSide:
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
			}
		}
		break;
	default:
		break;
	}


}


void WaveCacher::OnRtnOrder( const std::string& apOrder )
{

}

void WaveCacher::OnRtnTrade( const std::string& apTrade )
{

}

void WaveCacher::OnRtnPos( const std::string& aPos )
{

}
