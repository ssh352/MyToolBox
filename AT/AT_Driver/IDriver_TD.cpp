#include "IDriver_TD.h"
#include <boost\uuid\random_generator.hpp>
namespace AT
{


	AT::AT_Order_Key GenerateOrderKey()
	{
		static  boost::uuids::random_generator lInst;
		return lInst();
	}

}

