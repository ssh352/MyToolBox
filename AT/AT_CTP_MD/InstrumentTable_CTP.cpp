#include "InstrumentTable_CTP.h"

namespace CTP
{

	void InstrumentTable_CTP::AddInstrument( Instrument_Ptr apInstrument )
	{
		AddItem(apInstrument);
	}

	Instrument_Ptr InstrumentTable_CTP::GetInstrument( const std::string& aInstrumentID )
	{
		return GetItem(aInstrumentID);
	}

}

