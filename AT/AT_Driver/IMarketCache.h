#pragma  once
#include <string>
#include <boost\shared_ptr.hpp>
#include <map>
#include <vector>

namespace AT
{
	class IMarketCache
	{
	public:
		//virtual const IMarketCache* GetMarketSubCacheByName(const char* aInstrument)  const = 0;
		//virtual const IMarketCache* GetMarketSubCacheByTime(const std::string& aTimeStart, const std::string& aTimeStop)  const =0;

		//class const_iterator
		//{
		//	 boost::shared_ptr<const std::string> operator()*;
		//};


		////typedef  const boost::shared_ptr<std::string> const_iterator ;
		////virtual const_iterator begin() const = 0;
		////virtual const_iterator end() const = 0;

		//std::map<std::string, std::vector<const boost::shared_ptr<std::string> > > m_Cache;

	};
}