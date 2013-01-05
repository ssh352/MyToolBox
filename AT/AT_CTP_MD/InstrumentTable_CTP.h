#pragma once

#include <boost/shared_ptr.hpp>
#include "ItemTable.h"
#include "CTP_API.h"
namespace CTP
{
	typedef boost::shared_ptr<CThostFtdcInstrumentField> Instrument_Ptr;

	class CThostFtdcInstrumentFieldTraits
	{
	public:
		static std::string GetItemID(Instrument_Ptr apInstrument){return std::string(apInstrument->InstrumentID,30);};
		static std::string GetItemTypeName(){return "CThostFtdcInstrumentField";};
		static const int IsDuplicateException = 1;
	};


	class InstrumentTable_CTP :public ItemTable<CThostFtdcInstrumentField,CThostFtdcInstrumentFieldTraits>
	{
	public:
		void AddInstrument(Instrument_Ptr apInstrument );
		Instrument_Ptr GetInstrument(const std::string& aInstrumentID);
	};
}


