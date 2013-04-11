#include "CTP_MD_Replayer.h"
#include "IMarketSpi.h"
#include <leveldb/db.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
namespace CTP
{



	CTP_MD_Replayer::~CTP_MD_Replayer(void)
	{
	}

	void CTP_MD_Replayer::Init( const std::map<std::string,std::string>& aConfigMap,AT::IMarketSpi* apMarketSpi )
	{
		//TODO 
		//ֱ��һ�·���;
		//map��key���Ƕ����б� ֵΪ��Ӧ���ݿ��·��
		for (auto lPair: aConfigMap)
		{
			//std::string  lInstrumenID =;
			leveldb::DB*& lpDB = m_MarketDBMap[ lPair.first];
			leveldb::Options options;
			options.create_if_missing = true;
			leveldb::Status lstatus = leveldb::DB::Open(options, lPair.second, &lpDB);
			if(!lstatus.ok())
			{
				std::cerr<<"Open DB failed";
			}
		}

		for(auto lPair: m_MarketDBMap)
		{
			m_MarketTickMapStored;
			leveldb::DB* m_pDB = lPair.second;
			leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
			for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
			{
				std::string lKeyTime = liter->key().ToString();
				std::string lValMarket = liter->value().ToString();
				lKeyTime += lPair.first;
				m_MarketTickMapStored[lKeyTime] = lValMarket;
			}
			delete liter;
		}

		for(auto lPair:m_MarketTickMapStored)
		{
			apMarketSpi->NotifyMarketDepth(lPair.second);
		}


	}

	CTP_MD_Replayer::CTP_MD_Replayer(  )
	{

	}

}
