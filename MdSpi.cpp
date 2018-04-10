#include "MdSpi.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcMdApi* pUserApi;

// 配置参数
// extern char FRONT_ADDR[];
// extern TThostFtdcBrokerIDType BROKER_ID;
// extern TThostFtdcInvestorIDType INVESTOR_ID;
// extern TThostFtdcPasswordType PASSWORD;
// extern char* ppInstrumentID[];
// extern int iInstrumentID;

// 请求编号
extern int iRequestID;

void PrintOut(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	
    printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID=[%d], bIsLast=[%d]\n", nRequestID, bIsLast);
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo,nRequestID,bIsLast);
    // IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    cerr << "--->>> Reason = " << nReason << endl;
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    // user login request
    ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.UserID, "067938");
    strcpy(req.Password, "6432281");
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
    CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo,nRequestID,bIsLast);
    // copy from ctp documents demo
    if (pRspInfo->ErrorID != 0) {
	// printf("Failed to login,errorcode=%d, errormsg=%s,requestid=%d,chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
	return;
    }
    // printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    // printf("RequestID=[%d], chain=[%d]\n", nRequestID, bIsLast);

    printf("TradingDay=%s,LoginTime=%s,BrokerID=%s,UserId=%s,SystemName=%s,FrontID=%d,SessionID=%d\n", pRspUserLogin->TradingDay, pRspUserLogin->LoginTime, pRspUserLogin->BrokerID, pRspUserLogin->UserID, pRspUserLogin->SystemName, pRspUserLogin->FrontID, pRspUserLogin->SessionID);
    printf("MaxOrderRef=%s,SHFETime=%s,DCETime=%s,CZCETime=%s,FFEXTime=%s,INETime=%s\n", pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime, pRspUserLogin->DCETime, pRspUserLogin->CZCETime, pRspUserLogin->FFEXTime, pRspUserLogin->INETime);

    // char * Instrument[] = {"ru1810","rb1810"};
    // pUserApi->SubscribeMarketData(Instrument,2);
    // if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
    //     ///获取当前交易日
    //     cerr << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
    //     // 请求订阅行情
    //     // SubscribeMarketData();
    // }
}

void CMdSpi::SubscribeMarketData()
{
    cerr << __FUNCTION__ << endl;
    int iResult = pUserApi->SubscribeMarketData({"IF1804","RU1810"}, 2);
    cerr << "--->>> send SubscribeMarketData request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << __FUNCTION__ << endl;
    PrintOut(pRspInfo,nRequestID,bIsLast);
    printf("InstrumentID=%s\n",pSpecificInstrument->InstrumentID);

}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << __FUNCTION__ << endl;
    PrintOut(pRspInfo,nRequestID,bIsLast);
    printf("InstrumentID=%s\n",pSpecificInstrument->InstrumentID);

}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    cerr << __FUNCTION__ << endl;
	cerr << "TradingDay" <<pDepthMarketData->TradingDay;
	cerr << "InstrumentID" <<pDepthMarketData->InstrumentID;
	cerr << "ExchangeID" <<pDepthMarketData->ExchangeID;
	cerr << "ExchangeInstID" <<pDepthMarketData->ExchangeInstID;
	cerr << "LastPrice" <<pDepthMarketData->LastPrice;
	cerr << "PreSettlementPrice" <<pDepthMarketData->PreSettlementPrice;
	cerr << "PreClosePrice" <<pDepthMarketData->PreClosePrice;
	cerr << "PreOpenInterest" <<pDepthMarketData->PreOpenInterest;
	cerr << "OpenPrice" <<pDepthMarketData->OpenPrice;
	cerr << "HighestPrice" <<pDepthMarketData->HighestPrice;
	cerr << "LowestPrice" <<pDepthMarketData->LowestPrice;
	cerr << "Volume" <<pDepthMarketData->Volume;
	cerr << "Turnover" <<pDepthMarketData->Turnover;
	cerr << "OpenInterest" <<pDepthMarketData->OpenInterest;
	cerr << "ClosePrice" <<pDepthMarketData->ClosePrice;
	cerr << "SettlementPrice" <<pDepthMarketData->SettlementPrice;
	cerr << "UpperLimitPrice" <<pDepthMarketData->UpperLimitPrice;
	cerr << "LowerLimitPrice" <<pDepthMarketData->LowerLimitPrice;
	cerr << "PreDelta" <<pDepthMarketData->PreDelta;
	cerr << "CurrDelta" <<pDepthMarketData->CurrDelta;
	cerr << "UpdateTime" <<pDepthMarketData->UpdateTime;
	cerr << "UpdateMillisec" <<pDepthMarketData->UpdateMillisec;
	cerr << "BidPrice1" <<pDepthMarketData->BidPrice1;
	cerr << "BidVolume1" <<pDepthMarketData->BidVolume1;
	cerr << "AskPrice1" <<pDepthMarketData->AskPrice1;
	cerr << "AskVolume1" <<pDepthMarketData->AskVolume1;
	cerr << "BidPrice2" <<pDepthMarketData->BidPrice2;
	cerr << "BidVolume2" <<pDepthMarketData->BidVolume2;
	cerr << "AskPrice2" <<pDepthMarketData->AskPrice2;
	cerr << "AskVolume2" <<pDepthMarketData->AskVolume2;
	cerr << "BidPrice3" <<pDepthMarketData->BidPrice3;
	cerr << "BidVolume3" <<pDepthMarketData->BidVolume3;
	cerr << "AskPrice3" <<pDepthMarketData->AskPrice3;
	cerr << "AskVolume3" <<pDepthMarketData->AskVolume3;
	cerr << "BidPrice4" <<pDepthMarketData->BidPrice4;
	cerr << "BidVolume4" <<pDepthMarketData->BidVolume4;
	cerr << "AskPrice4" <<pDepthMarketData->AskPrice4;
	cerr << "AskVolume4" <<pDepthMarketData->AskVolume4;
	cerr << "BidPrice5" <<pDepthMarketData->BidPrice5;
	cerr << "BidVolume5" <<pDepthMarketData->BidVolume5;
	cerr << "AskPrice5" <<pDepthMarketData->AskPrice5;
	cerr << "AskVolume5" <<pDepthMarketData->AskVolume5;
	cerr << "AveragePrice" <<pDepthMarketData->AveragePrice;
	cerr << "ActionDay" <<pDepthMarketData->ActionDay;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
    // 如果ErrorID != 0, 说明收到了错误的响应
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
	cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

