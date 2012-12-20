#pragma once

#include <string>



namespace AT
{
	class IMarketSpi;


	//accept xml string for question

	//thread safe Driver promise it
	class IDriver_MD
	{
	public:
		//config 中可以带一个订阅列表,或者是否全部订阅等等设置
		virtual void Init(const std::string&  aConfigStr,AT::IMarketSpi* apTradeSpi) = 0;

		//todo 基于节约带宽的考虑 还是应该提供这两个选项 目前全推
		//virtual Subscibe(const std::string& aSubScribe);
		//virtual UnSubscribe(const std::string& aUnSubScribe);
	};

}

