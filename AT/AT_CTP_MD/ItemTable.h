#pragma once

#include <boost\shared_ptr.hpp>
#include <map>
#include <string>
#include "leveldb/db.h"

template<typename ItemType,typename ItemTraits>
class ItemTable
{
public:
	ItemTable(): m_pDB(NULL){};
	typedef boost::shared_ptr<ItemType> ItemPtr;
	
	void InitWithDB(const std::string& aDbPath )
	{
		if(aDBPath.empty())
		{
			throw std::exception("Do not Set MarketDepthCacheCtp Db path");
		}

		leveldb::Options options;
		options.create_if_missing = true;
		leveldb::Status lstatus = leveldb::DB::Open(options, aDBPath.c_str(), &m_pDB);
		if(lstatus.ok())
		{
			LoadFromDB();
		}
		else
		{
			throw std::exception("Open  Db Failed");
		}
	}
	void LoadFromDB()
	{
		leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
		for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
		{
			ItemType lStoreInstPtr(new ItemType);
			memcpy(lStoreInstPtr.get(),liter->value().data(),liter->value().size());
			m_ItemMap[liter->key().ToString()] = lTickPtr;
		}
		delete liter;
	}

	void AddItem(ItemPtr apItem)
	{
		std::string lItemID =  ItemTraits::GetItemID(apItem);
		if(m_ItemMap.count(lItemID))
		{
			std::string Exception_info_str = "Add  ";
			Exception_info_str += ItemTraits::GetItemTypeName();
			Exception_info_str += " Already Exist ItemID=";
			Exception_info_str += lItemID;
			throw std::exception(Exception_info_str.c_str());
		}
		else
		{
			m_ItemMap.insert(make_pair(lItemID,apItem));
			if(m_pDB)
			{
				leveldb::Status lstatus  =m_pDB->Put(leveldb::WriteOptions(), lItemID, leveldb::Slice(((char*)apItem.get()), sizeof(ItemType)));
				if(!lstatus.ok())
				{
					std::cerr<<"Store "<<	ItemTraits::GetItemTypeName()<< " Failed" << std::endl;
				}
			}
		}
	}

	ItemPtr GetItem( const std::string& aItemID )
	{
		if(m_ItemMap.count(aItemID))
		{
			return m_ItemMap[aItemID];
		}
		else
		{
			return ItemPtr();
		}
	}

	//now it has the same interface with STL contain and can work with boost_for_each direct
public:
	typedef typename std::map<std::string, ItemPtr>::iterator iterator;
	typedef typename std::map<std::string, ItemPtr>::const_iterator const_iterator;
	iterator begin(){return m_ItemMap.begin();};
	iterator end(){return m_ItemMap.end();};

private:
	std::map<std::string, ItemPtr> m_ItemMap;
	leveldb::DB*							m_pDB;

};

