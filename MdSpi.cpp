#include "MdSpi.h"
#include <iostream>
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

void CMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo,
    int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    IsErrorRspInfo(pRspInfo);
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
    ///用户登录请求
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
    // 参考ctp开发文档的行情demo
    if (pRspInfo->ErrorID != 0) {
	printf("Failed to login,errorcode=%d, errormsg=%s,requestid=%d,chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
	exit(-1);
    }
    printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID=[%d], chain=[%d]\n", nRequestID, bIsLast);

    printf("TradingDay=%s,LoginTime=%s,BrokerID=%s,UserId=%s,SystemName=%s,FrontID=%d,SessionID=%d", pRspUserLogin->TradingDay, pRspUserLogin->LoginTime, pRspUserLogin->BrokerID, pRspUserLogin->UserID, pRspUserLogin->SystemName, pRspUserLogin->FrontID, pRspUserLogin->SessionID);
    printf("MaxOrderRef=%s,SHFETime=%s,DCETime=%s,CZCETime=%s,FFEXTime=%s,INETime=%s", pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime, pRspUserLogin->DCETime, pRspUserLogin->CZCETime, pRspUserLogin->FFEXTime, pRspUserLogin->INETime);

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
    // int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
    // cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
    cerr << __FUNCTION__ << endl;
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
    // 如果ErrorID != 0, 说明收到了错误的响应
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
	cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

