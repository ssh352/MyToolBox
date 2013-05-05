#pragma once

#include <memory>
#include <map>
#include <string>
#include "leveldb/db.h"
#include "ATLogger.h"

template<typename ItemType,typename ItemTraits>
class ItemTable
{

	typedef typename ItemTraits::KeyType  KeyType;
	typedef std::shared_ptr<ItemType>	  ItemPtr;

public:
	explicit ItemTable(const char* apDBpath);
	virtual ~ItemTable();


private:
	void LoadFromDB();

protected:
	
	void PutItem(ItemPtr apItem);

	ItemPtr GetItem( const KeyType& aItemID );


	void DelItem(const std::string& aItemID );

	void Clear();

	//now it has the same interface with STL contain and can work with boost_for_each direct
public:
	typedef typename std::map<KeyType, ItemPtr>::iterator iterator;
	typedef typename std::map<KeyType, ItemPtr>::const_iterator const_iterator;
	iterator begin(){return m_ItemMap.begin();};
	iterator end(){return m_ItemMap.end();};
	const_iterator begin()const {return m_ItemMap.begin();};
	const_iterator end()const {return m_ItemMap.end();};

private:
	std::map<KeyType, ItemPtr> m_ItemMap;
	leveldb::DB*				m_pDB;
};

#include "ItemTable.inl"