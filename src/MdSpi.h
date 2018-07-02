#pragma once
#include "../api/ThostFtdcMdApi.h"
#include "file_utils.h"

class CMdSpi : public CThostFtdcMdSpi
{
	private:
		CThostFtdcMdApi *userapi;
		string broker_id;
		string user_id;
		string passwd;
		string front_id;
		int reqid;

	public:
		void ReqUserLogin();
		void logout();
		void SubscribeMarketData(const vector<string> &instruments,const vector<string> &markets);
		bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
		void PrintOut(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
		CMdSpi();
		~CMdSpi();

		void load_config(const Document &d);
		virtual void connect();
	public:
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
				int nRequestID, bool bIsLast);

		virtual void OnFrontDisconnected(int nReason);

		virtual void OnHeartBeatWarning(int nTimeLapse);

		virtual void OnFrontConnected();

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

};
