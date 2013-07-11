#pragma once
#include "Signal.h"
#include "IExecutor.h"
#include <string>
#include <map>
namespace AT
{
class ExecutorContianer;


struct SignalExecutInfo
{
	std::string		 ExecutorID;
	std::string  InstrumentID;
	BuySellType BuySellCode;
	OpenCloseType OpenCloseCode;
	int			vol;
};

class AccountSignalDispach
{
public:
	AccountSignalDispach(const std::string& aConfigFile,ExecutorContianer* apExecutorContainer,int AccountVol);
	~AccountSignalDispach(void);

	void HandleSignal(Signal aSignal);


private:
	void LoadExecuteInfoFromFile(const std::string& aConfigfile);

	int			m_Vol;
	ExecutorInput BuildExecutorInput(const SignalExecutInfo& lInfo,const MarketData& aTriggerMarket );

	std::map<std::string,SignalExecutInfo>		m_Signal_Executor_Map;

	ExecutorContianer* m_pExecutorContianer;
};

}