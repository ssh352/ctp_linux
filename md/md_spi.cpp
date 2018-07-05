#include "md_spi.h"

extern Document d;
extern zmq::context_t context;
extern zmq::socket_t publisher;

CMdSpi::CMdSpi(): userapi(nullptr), reqid(0) { }

void CMdSpi::load_config(const Document &d){
	broker_id = d["broker_id"].GetString();
	user_id = d["user_id"].GetString();
	passwd = d["passwd"].GetString();
	front_id = d["md_address"].GetString();
}

void CMdSpi::connect()
{
	if (userapi == nullptr)
	{
		userapi = CThostFtdcMdApi::CreateFtdcMdApi("./log/md/"); // ´´½¨UserApi

		if (!userapi)
		{
			// throw runtime_error("ctp_md failed to create api");
			throw "ctp_md failed to create api";
		}
		userapi->RegisterSpi(this);
	}

	userapi->RegisterFront(const_cast<char*>(front_id.c_str())); // connect
	userapi->Init();
	userapi->Join();
	LOG(FATAL) << "CTP exit!";
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	// IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// cerr << "--->>> Reason = " << nReason << endl;
	LOG(ERROR) << "Disconect from ctp front." << " Reason code "<<nReason ;
	// publisher.send("OnFrontDisconnected");
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
	LOG(WARNING) << "Receive ctp Heartbeat timeout.";
}

void CMdSpi::OnFrontConnected()
{

	LOG(INFO) << "Connect to ctp sucessfully.";
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// user login request
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, broker_id.c_str());
	strcpy(req.UserID, user_id.c_str());
	strcpy(req.Password, passwd.c_str());
	int iResult = userapi->ReqUserLogin(&req, ++reqid);
	// FIXME: should reconnect later instead of ASSERT
	// LOG(INFO) << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
	std::cout << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
	

}

void CMdSpi::logout()
{
	CThostFtdcUserLogoutField req = {};
	strcpy(req.BrokerID, broker_id.c_str());
	strcpy(req.UserID, user_id.c_str());
	if (userapi->ReqUserLogout(&req, ++reqid))
	{
		LOG(ERROR) << "request logout failed" << "(bid)" << req.BrokerID << "(uid)" << req.UserID;
	}

}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	// if (pRspInfo != nullptr && pRspInfo->ErrorID != 0) {
	//         // LOG(INFO) << "Login to ctp failed." << "(errid)" << pRspInfo->ErrorID << "(errmsg)" << pRspInfo->ErrorMsg;
	//         std::cout << "Login to ctp failed." << "(errid)" << pRspInfo->ErrorID << "(errmsg)" << pRspInfo->ErrorMsg;
	// }
	// else
	// {
	//         std::cout << "Login to ctp successed." << "(Bid)" << pRspUserLogin->BrokerID << "(Uid)" << pRspUserLogin->UserID;
	//         // LOG(INFO) << "Login to ctp successed." << "(Bid)" << pRspUserLogin->BrokerID << "(Uid)" << pRspUserLogin->UserID;
	// }
        //
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (pRspInfo == NULL){
		std::cout << "pRspInfo is null" << std::endl;
	}

	if (pRspUserLogin == NULL ){
		std::cout << "pRspUserLogin is null" << std::endl;
	}
	cout << pRspUserLogin->TradingDay << endl;
	cout << pRspUserLogin->LoginTime<< endl;
	cout << pRspUserLogin->BrokerID<< endl;
	cout << pRspUserLogin->UserID<< endl;
	cout << pRspUserLogin->SystemName<< endl;
	cout << pRspUserLogin->FrontID<< endl;
	cout << pRspUserLogin->SessionID<<endl;
	cout << pRspUserLogin->MaxOrderRef << endl;
	cout << pRspUserLogin->SHFETime << endl;
	cout << pRspUserLogin->DCETime << endl;
	cout << pRspUserLogin->CZCETime << endl;
	cout << pRspUserLogin->FFEXTime << endl;
	cout << pRspUserLogin->INETime << endl;
	// if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	//     cerr << "--->>> call api function GetTradingDay = " << pUserMdApi->GetTradingDay() << endl;
	// }
}
void CMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
	{
		LOG(INFO) << "logout to ctp failed." << "(errid)" << pRspInfo->ErrorID << "(errmsg)" << pRspInfo->ErrorMsg;
	}
	else
	{
		LOG(INFO) << "Login to ctp successed." << "(Bid)" << pUserLogout->BrokerID << "(Uid)" << pUserLogout->UserID;
	}
}


void CMdSpi::SubscribeMarketData(const vector<string> &instruments,const vector<string> &markets)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// //char* pp[] = { "IF1804", "IF1805" };
	// char* pp[] = { "IF1807" };
	// int iResult = pUserMdApi->SubscribeMarketData(pp, 2);
	// cerr << "--->>> send SubscribeMarketData request: " << ((iResult == 0) ? "success" : "failed") << endl;
	//
	int ncount = instruments.size();
	char *insts[ncount];
	for (int i=0; i < ncount; i++)
		insts[i] = const_cast<char*>(instruments[i].c_str());

	userapi->SubscribeMarketData(insts,ncount);
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
	{
		LOG(ERROR) << "[OnRspSubMarketData]" << "(errID)" << pRspInfo->ErrorID << "(errMsg)" << pRspInfo->ErrorMsg;
	}

}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// printf("InstrumentID=%s\n", pSpecificInstrument->InstrumentID);
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// cerr << "TradingDay=" << pDepthMarketData->TradingDay << endl;
	// cerr << "InstrumentID=" << pDepthMarketData->InstrumentID << endl;
	// cerr << "ExchangeID=" << pDepthMarketData->ExchangeID << endl;
	// cerr << "ExchangeInstID=" << pDepthMarketData->ExchangeInstID << endl;
	// cerr << "LastPrice=" << pDepthMarketData->LastPrice << endl;
	// cerr << "PreSettlementPrice=" << pDepthMarketData->PreSettlementPrice << endl;
	// cerr << "PreClosePrice=" << pDepthMarketData->PreClosePrice << endl;
	// cerr << "PreOpenInterest=" << pDepthMarketData->PreOpenInterest << endl;
	// cerr << "OpenPrice=" << pDepthMarketData->OpenPrice << endl;
	// cerr << "HighestPrice=" << pDepthMarketData->HighestPrice << endl;
	// cerr << "LowestPrice=" << pDepthMarketData->LowestPrice << endl;
	// cerr << "Volume=" << pDepthMarketData->Volume << endl;
	// cerr << "Turnover=" << pDepthMarketData->Turnover << endl;
	// cerr << "OpenInterest=" << pDepthMarketData->OpenInterest << endl;
	// cerr << "ClosePrice=" << pDepthMarketData->ClosePrice << endl;
	// cerr << "SettlementPrice=" << pDepthMarketData->SettlementPrice << endl;
	// cerr << "UpperLimitPrice=" << pDepthMarketData->UpperLimitPrice << endl;
	// cerr << "LowerLimitPrice=" << pDepthMarketData->LowerLimitPrice << endl;
	// cerr << "PreDelta=" << pDepthMarketData->PreDelta << endl;
	// cerr << "CurrDelta=" << pDepthMarketData->CurrDelta << endl;
	// cerr << "UpdateTime=" << pDepthMarketData->UpdateTime << endl;
	// cerr << "UpdateMillisec=" << pDepthMarketData->UpdateMillisec << endl;
	// cerr << "BidPrice1=" << pDepthMarketData->BidPrice1 << endl;
	// cerr << "BidVolume1=" << pDepthMarketData->BidVolume1 << endl;
	// cerr << "AskPrice1=" << pDepthMarketData->AskPrice1 << endl;
	// cerr << "AskVolume1=" << pDepthMarketData->AskVolume1 << endl;
	// cerr << "BidPrice2=" << pDepthMarketData->BidPrice2 << endl;
	// cerr << "BidVolume2=" << pDepthMarketData->BidVolume2 << endl;
	// cerr << "AskPrice2=" << pDepthMarketData->AskPrice2 << endl;
	// cerr << "AskVolume2=" << pDepthMarketData->AskVolume2 << endl;
	// cerr << "BidPrice3=" << pDepthMarketData->BidPrice3 << endl;
	// cerr << "BidVolume3=" << pDepthMarketData->BidVolume3 << endl;
	// cerr << "AskPrice3=" << pDepthMarketData->AskPrice3 << endl;
	// cerr << "AskVolume3=" << pDepthMarketData->AskVolume3 << endl;
	// cerr << "BidPrice4=" << pDepthMarketData->BidPrice4 << endl;
	// cerr << "BidVolume4=" << pDepthMarketData->BidVolume4 << endl;
	// cerr << "AskPrice4=" << pDepthMarketData->AskPrice4 << endl;
	// cerr << "AskVolume4=" << pDepthMarketData->AskVolume4 << endl;
	// cerr << "BidPrice5=" << pDepthMarketData->BidPrice5 << endl;
	// cerr << "BidVolume5=" << pDepthMarketData->BidVolume5 << endl;
	// cerr << "AskPrice5=" << pDepthMarketData->AskPrice5 << endl;
	// cerr << "AskVolume5=" << pDepthMarketData->AskVolume5 << endl;
	// cerr << "AveragePrice=" << pDepthMarketData->AveragePrice << endl;
	// cerr << "ActionDay=" << pDepthMarketData->ActionDay << endl;
	LOG(INFO) << "OnRspSubMarketData" << endl;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

