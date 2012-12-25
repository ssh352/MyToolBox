#pragma once

#include <boost\shared_ptr.hpp>
#include <map>



template<typename ItemType,typename ItemTraits>
class ItemTable
{
public:
	typedef boost::shared_ptr<ItemType> ItemPtr;
	

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

};

