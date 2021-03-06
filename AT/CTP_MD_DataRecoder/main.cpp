
#include <boost\date_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "DepthReceiveV2.h"
#include "MarketDBWriter.h"
int main()
{
	//load config
	using boost::property_tree::ptree;
	ptree pt;
	read_xml("MDDataRecoder.xml",pt);

	std::vector<std::string> lVec;
	for(auto lVale: pt.get_child("DataRecorderConfig.RecordList"))
	{
		lVec.push_back(lVale.second.data());
	}
	std::string lDBpath = pt.get<std::string>("DataRecorderConfig.StoreDBPath");
	

	lDBpath += to_iso_string(boost::gregorian::day_clock::local_day());
	MarketDBWriter writeInst(lVec,lDBpath);


	std::string lFront = pt.get<std::string>("DataRecorderConfig.Front");
	pt.clear();

	//MDConfig.Front
	//MDConfig.InstrumentList
	//MDConfig.WorkFlowDir

	pt.put("MDConfig.Front",lFront);
	pt.put("MDConfig.WorkFlowDir","DaRecoderFlow");
	for(std::string lInstrument : lVec)
	{
		pt.add("MDConfig.InstrumentList.Item",lInstrument);
	}

	std::stringstream lStringStream;
	write_xml(lStringStream,pt);
	std::string  lConfigString = lStringStream.str();

	auto lLamb = 
		[&writeInst](std::shared_ptr< AT::MarketData> lpDataTick) 
	{
		std::cout<<lpDataTick->ToString()<<"\n";
		writeInst.StroeMarketTick(lpDataTick);
	};

	auto lStatHandle = [ ](CTP::CTP_Market_Status_Enum aStatus,std::string aErrorMsg)
	{
		std::cout<<aErrorMsg;
	};

	CTP::DepthReceiveV2 lMarketInst(lConfigString,lLamb ,lStatHandle);

	lMarketInst.Start();

	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}


}