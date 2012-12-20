#pragma once

#include <string>

namespace CTP
{
	class CTP_MD;
	class DepthReceiver
	{
	public:
		DepthReceiver(const std::string aConfigStr);
		void SetStateReceive(CTP_MD* parent);
		void Start();
	};

}


