#pragma once


template<typename ItemType,typename ItemTraits>
ItemTable<ItemType,ItemTraits>::ItemTable(const char* apDBpath)
{
	if(strlen(apDBpath))
	{
		throw std::exception("DBpath Null");
	}

	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status lstatus = leveldb::DB::Open(options,apDBpath, &m_pDB);
	if(lstatus.ok())
	{
		LoadFromDB();
	}
	else
	{
		throw std::exception("Open  Db Failed");
	}
};


template<typename ItemType,typename ItemTraits>
ItemTable<ItemType,ItemTraits>::~ItemTable()
{
	delete m_pDB;
}


template<typename ItemType,typename ItemTraits>
void ItemTable<ItemType,ItemTraits>::LoadFromDB()
{
	leveldb::Iterator* liter = m_pDB->NewIterator(leveldb::ReadOptions());
	for (liter->SeekToFirst(); liter->Valid(); liter->Next()) 
	{
		std::shared_ptr<ItemType> lStoreInstPtr(new ItemType);
		memcpy(lStoreInstPtr.get(),liter->value().data(),liter->value().size());
		KeyType lKeyType;
		memcpy(&lKeyType,liter->key().data(),liter->key().size());
		m_ItemMap[lKeyType] = lStoreInstPtr;
	}
	delete liter;
}

template<typename ItemType,typename ItemTraits>
typename ItemTable<ItemType,ItemTraits>::ItemPtr	ItemTable<ItemType,ItemTraits>::GetItem( const KeyType& aItemID )
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

template<typename ItemType,typename ItemTraits>
void ItemTable<ItemType,ItemTraits>:: DelItem(const std::string& aItemID )
{
	if(m_ItemMap.count(aItemID))
	{
		m_ItemMap.erase(aItemID);
	}
}
template<typename ItemType,typename ItemTraits>
void ItemTable<ItemType,ItemTraits>::PutItem(ItemPtr apItem)
{
	KeyType lItemID =  ItemTraits::GetItemKey(apItem);
	m_ItemMap[lItemID] = apItem;
	leveldb::Status lstatus  =m_pDB->Put(leveldb::WriteOptions(), lItemID, leveldb::Slice(((char*)apItem.get()), sizeof(ItemType)));
	if(!lstatus.ok())
	{
		ATLOG(LogLevel::L_ERROR,"Can not Save DB");
	}
}

template<typename ItemType,typename ItemTraits>
void ItemTable<ItemType,ItemTraits>::Clear()
{
	m_ItemMap.clear();
}

