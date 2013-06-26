#pragma once

#include "ISignalModule.h"
#include <string>
#include <set>
class SignalPrintHistory :public AT::ISignalModule
{
public:
	SignalPrintHistory(const char* aConfigFile,const AT::IMarketCache* apMarketCache);
	virtual ~SignalPrintHistory(void);
	virtual void UpdateParam(AT::ESignalInputType errCode ,const AT::Param& apParam) override {};
	virtual int OnMarketDepth(const AT::MarketData& aMarketDepth) override;
	virtual void Start() override;
	virtual void Stop() override ;
	virtual std::string GetIndexName() override{return "";}

private:
	std::string m_Configfile;
	const AT::IMarketCache* m_pConstMarketCache;
	std::set<std::string> m_PrintList;
};
