#include "ITradeSpi.h"
namespace AT
{
	std::string ToString( ETradeState aState )
	{
		switch (aState)
		{
		case AT::ETradeState::START:
			return std::string("ETradeState :Start");
			break;
		case AT::ETradeState::READY:
			return std::string("ETradeState :READY");
			break;
		case AT::ETradeState::DISCONNCT:
			return std::string("ETradeState :DISCONNCT");
			break;
		case AT::ETradeState::STOP:
			return std::string("ETradeState :STOP");
			break;
		default:
			return std::string("ETradeState :Unknown");
			break;
		}
	}
}