#include "OrderRefGenerator.h"
#include <boost\format.hpp>
#include <iomanip>

OrderRefGenerator::OrderRefGenerator(void)
{
}


OrderRefGenerator::~OrderRefGenerator(void)
{
}

void OrderRefGenerator::SetMaxOrderRef( int orderRef )
{
	m_MaxRef = orderRef;
}

std::string OrderRefGenerator::GenerateOrderRef()
{
	boost::format fmt("%1%");
	fmt % boost::io::group(std::setfill('0'),std::setw(12),++m_MaxRef);
	std::string lRet = str(fmt);
	return lRet;
}
