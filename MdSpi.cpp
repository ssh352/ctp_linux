#include "MdSpi.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
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
    PrintOut(pRspInfo, nRequestID, bIsLast);
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
    strcpy(req.BrokerID, "8070");
    strcpy(req.UserID, "067938");
    strcpy(req.Password, "6432281");
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
    CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    // copy from ctp documents demo
    if (pRspInfo->ErrorID != 0) {
	// printf("Failed to login,errorcode=%d, errormsg=%s,requestid=%d,chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
	return;
    }
    // printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    // printf("RequestID=[%d], chain=[%d]\n", nRequestID, bIsLast);

    printf("TradingDay=%s,LoginTime=%s,BrokerID=%s,UserId=%s,SystemName=%s,FrontID=%d,SessionID=%d\n", pRspUserLogin->TradingDay, pRspUserLogin->LoginTime, pRspUserLogin->BrokerID, pRspUserLogin->UserID, pRspUserLogin->SystemName, pRspUserLogin->FrontID, pRspUserLogin->SessionID);
    printf("MaxOrderRef=%s,SHFETime=%s,DCETime=%s,CZCETime=%s,FFEXTime=%s,INETime=%s\n", pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime, pRspUserLogin->DCETime, pRspUserLogin->CZCETime, pRspUserLogin->FFEXTime, pRspUserLogin->INETime);

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	cerr << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
    }
    SubscribeMarketData();
}

void CMdSpi::SubscribeMarketData()
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    //char* pp[] = { "IF1804", "IF1805" };
    char* pp[] = { "IF1804" };
    int iResult = pUserApi->SubscribeMarketData(pp, 2);
    cerr << "--->>> send SubscribeMarketData request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    printf("InstrumentID=%s\n", pSpecificInstrument->InstrumentID);
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    printf("InstrumentID=%s\n", pSpecificInstrument->InstrumentID);
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    cerr << "TradingDay=" << pDepthMarketData->TradingDay << endl;
    cerr << "InstrumentID=" << pDepthMarketData->InstrumentID << endl;
    cerr << "ExchangeID=" << pDepthMarketData->ExchangeID << endl;
    cerr << "ExchangeInstID=" << pDepthMarketData->ExchangeInstID << endl;
    cerr << "LastPrice=" << pDepthMarketData->LastPrice << endl;
    cerr << "PreSettlementPrice=" << pDepthMarketData->PreSettlementPrice << endl;
    cerr << "PreClosePrice=" << pDepthMarketData->PreClosePrice << endl;
    cerr << "PreOpenInterest=" << pDepthMarketData->PreOpenInterest << endl;
    cerr << "OpenPrice=" << pDepthMarketData->OpenPrice << endl;
    cerr << "HighestPrice=" << pDepthMarketData->HighestPrice << endl;
    cerr << "LowestPrice=" << pDepthMarketData->LowestPrice << endl;
    cerr << "Volume=" << pDepthMarketData->Volume << endl;
    cerr << "Turnover=" << pDepthMarketData->Turnover << endl;
    cerr << "OpenInterest=" << pDepthMarketData->OpenInterest << endl;
    cerr << "ClosePrice=" << pDepthMarketData->ClosePrice << endl;
    cerr << "SettlementPrice=" << pDepthMarketData->SettlementPrice << endl;
    cerr << "UpperLimitPrice=" << pDepthMarketData->UpperLimitPrice << endl;
    cerr << "LowerLimitPrice=" << pDepthMarketData->LowerLimitPrice << endl;
    cerr << "PreDelta=" << pDepthMarketData->PreDelta << endl;
    cerr << "CurrDelta=" << pDepthMarketData->CurrDelta << endl;
    cerr << "UpdateTime=" << pDepthMarketData->UpdateTime << endl;
    cerr << "UpdateMillisec=" << pDepthMarketData->UpdateMillisec << endl;
    cerr << "BidPrice1=" << pDepthMarketData->BidPrice1 << endl;
    cerr << "BidVolume1=" << pDepthMarketData->BidVolume1 << endl;
    cerr << "AskPrice1=" << pDepthMarketData->AskPrice1 << endl;
    cerr << "AskVolume1=" << pDepthMarketData->AskVolume1 << endl;
    cerr << "BidPrice2=" << pDepthMarketData->BidPrice2 << endl;
    cerr << "BidVolume2=" << pDepthMarketData->BidVolume2 << endl;
    cerr << "AskPrice2=" << pDepthMarketData->AskPrice2 << endl;
    cerr << "AskVolume2=" << pDepthMarketData->AskVolume2 << endl;
    cerr << "BidPrice3=" << pDepthMarketData->BidPrice3 << endl;
    cerr << "BidVolume3=" << pDepthMarketData->BidVolume3 << endl;
    cerr << "AskPrice3=" << pDepthMarketData->AskPrice3 << endl;
    cerr << "AskVolume3=" << pDepthMarketData->AskVolume3 << endl;
    cerr << "BidPrice4=" << pDepthMarketData->BidPrice4 << endl;
    cerr << "BidVolume4=" << pDepthMarketData->BidVolume4 << endl;
    cerr << "AskPrice4=" << pDepthMarketData->AskPrice4 << endl;
    cerr << "AskVolume4=" << pDepthMarketData->AskVolume4 << endl;
    cerr << "BidPrice5=" << pDepthMarketData->BidPrice5 << endl;
    cerr << "BidVolume5=" << pDepthMarketData->BidVolume5 << endl;
    cerr << "AskPrice5=" << pDepthMarketData->AskPrice5 << endl;
    cerr << "AskVolume5=" << pDepthMarketData->AskVolume5 << endl;
    cerr << "AveragePrice=" << pDepthMarketData->AveragePrice << endl;
    cerr << "ActionDay=" << pDepthMarketData->ActionDay << endl;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
    // 如果ErrorID != 0, 说明收到了错误的响应
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
	cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

