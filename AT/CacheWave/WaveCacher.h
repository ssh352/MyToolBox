#pragma once

#include "IStrategy.h"
class WaveCacher :public AT::IStrategy
{
public:
	WaveCacher(void);
	virtual ~WaveCacher(void);


	virtual void UpdateParam(EStrInputState errCode ,const std::string& apParam) override;
	virtual void Start() override;
	virtual void Stop()override;
public:
	virtual void OnMarketDepth(const std::string& aMarketDepth)override;
	virtual void OnRtnOrder(const std::string& apOrder)override;
	virtual void OnRtnTrade(const std::string& apTrade)override;
	virtual void OnRtnPos(const std::string& aPos)override;

};

