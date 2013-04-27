﻿#pragma once

#include "AT_API_HEAD.h"

#include "AT_Struct.h"
namespace AT
{
	class ITradeSpi;

	class IDriver_TD
	{
	public:

		virtual void UpdateParam(const Param& apParam) =0;
		virtual void Start() =0;
		virtual void Stop() =0;

		virtual void CreateOrder(const NewOrder& aNewOrder) =0;
		virtual void DeleteOrder(const CancelOrder& aDelOrderID) =0;
		virtual	void ModifyOrder(const ModifyOrder& aRequest) =0;
	
	};

	AT_Order_Key GenerateOrderKey(); 

}

typedef AT::IDriver_TD* (*CreatTDInstFun)(const char*  aConfigFile, AT::ITradeSpi* apTradeSpi);
DLL_API AT::IDriver_TD* CreateTD(const char* aConfigFile, AT::ITradeSpi* apTradeSpi);

