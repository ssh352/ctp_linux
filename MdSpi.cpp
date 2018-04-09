#include "MdSpi.h"
#include <iostream>
#include <string.h>
using namespace std;

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcMdApi* pUserApi;

// ���ò���
// extern char FRONT_ADDR[];
// extern TThostFtdcBrokerIDType BROKER_ID;
// extern TThostFtdcInvestorIDType INVESTOR_ID;
// extern TThostFtdcPasswordType PASSWORD;
// extern char* ppInstrumentID[];
// extern int iInstrumentID;

// ������
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
    ///�û���¼����
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
    // �ο�ctp�����ĵ�������demo
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
    //     ///��ȡ��ǰ������
    //     cerr << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
    //     // ����������
    //     // SubscribeMarketData();
    // }
}

void CMdSpi::SubscribeMarketData()
{
    cerr << __FUNCTION__ << endl;
    // int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
    // cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
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
    // ���ErrorID != 0, ˵���յ��˴������Ӧ
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
	cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

