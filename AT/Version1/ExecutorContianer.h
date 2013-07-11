#pragma once
#include "IExecutor.h"
#include <boost\shared_ptr.hpp>
#include <map>
#include <vector>
#include <string>
namespace AT
{
	class ExecutorContianer
	{
	public:
		ExecutorContianer(const std::string& aConfig,CommandHandler apCommandHandle,TradeReportFun aTradeReportFun);
		~ExecutorContianer(void);

		boost::shared_ptr<IExecutor>		GetExecutorByID(const std::string& aExecutorID);

		void	OnMarketDepth(const AT::MarketData& aMarketDepth) ;
		void	OnRtnOrder(const  AT::OrderUpdate& apOrder) ;
		void	OnRtnTrade(const  AT::TradeUpdate& apTrade) ;

	private:
		std::map<std::string,boost::shared_ptr<IExecutor>>	m_ExecutorGruop;
		void InitFromConfig(const std::string& aConfigFile);

	public:
		typedef std::map<std::string,boost::shared_ptr<IExecutor>>::iterator iterator;
		typedef std::map<std::string,boost::shared_ptr<IExecutor>>::const_iterator const_iterator;
		iterator	begin(){return m_ExecutorGruop.begin();};
		iterator	end(){return m_ExecutorGruop.end();};;
		const_iterator begin()const {return m_ExecutorGruop.begin();} ;
		const_iterator end() const{return m_ExecutorGruop.end();};

	private:
		CommandHandler			m_CommandHandler;
		TradeReportFun			m_ReportFun;

	};
}


