#include "stdafx.h"

#include "../AT_CTP_MD/StateReceiver.h"

#include <boost\thread.hpp>
#include <boost\date_time.hpp>
#include "../AT_CTP_MD/CTP_MD.h"

class StateReceiverTestBed  :public ::testing::Test
{
public:
	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{
		boost::this_thread::sleep(boost::posix_time::seconds(10));
	}

	static void SetUpTestCase()
	{


	}

	static void TearDownTestCase()
	{

	}
public:


};


class CTP_MD_Mock :public CTP::CTP_MD
{
public:
	MOCK_METHOD1(NotifyExchange,void(const std::string& aExchange));
	MOCK_METHOD1(NotifyProduct,void(const std::string& aProduct));
	MOCK_METHOD1(NotifyInstrument,void(const std::string& aInstrument));
	MOCK_METHOD1(NotifyMarketDepth,void(const std::string& aMarketDepth));
	MOCK_METHOD2(NotifySubModuleState,void(int aErrorCode,const std::string& aErrorMsg));
};

TEST_F(StateReceiverTestBed,DISABLE_login_test)
{
	using testing::_;
	CTP::StateReceiver lTestInst("");
	CTP_MD_Mock lMockInst;
	lTestInst.SetStateReceive(&lMockInst);
	lTestInst.Start();
	EXPECT_CALL(lMockInst,NotifySubModuleState(CTP::CTP_MD_StateReceiver_Connecting,_));
	EXPECT_CALL(lMockInst,NotifySubModuleState(CTP::CTP_MD_StateReceiver_Logining,_));
	EXPECT_CALL(lMockInst,NotifySubModuleState(CTP::CTP_MD_StateReceiver_Retrieving,_));


	boost::this_thread::sleep(boost::posix_time::seconds(100));
}