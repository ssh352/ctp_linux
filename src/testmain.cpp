#include "../include/ThostFtdcTraderApi.h"
#include "../include/TraderSpi.h"
#include "../include/MdSpi.h"
#include "../include/ThostFtdcMdApi.h"

CThostFtdcMdApi* pUserMdApi;

CThostFtdcTraderApi* pUserTraderApi;
// ÅäÖÃ²ÎÊý
// char  FRONT_ADDR[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// Ç°ÖÃµØÖ·
// TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ¾­¼Í¹«Ë¾´úÂë
// TThostFtdcInvestorIDType INVESTOR_ID = "00069";			// Í¶×ÊÕß´úÂë
// TThostFtdcPasswordType  PASSWORD = "888888";			// ÓÃ»§ÃÜÂë
// TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu0909";	// ºÏÔ¼´úÂë
// TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ÂòÂô·½Ïò
// TThostFtdcPriceType	LIMIT_PRICE = 38850;				// ¼Û¸ñ

// ÇëÇó±àºÅ
int iRequestID = 0;

//int iRequestID = 0;

int main(void)
{
    //char FRONT_ADDR[] = "tcp://180.168.146.187:10011"; // Ç°ÖÃµØÖ·
    char FRONT_ADDR[] = "tcp://180.168.214.246:41213"; // Ç°ÖÃµØÖ·
    pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./log/md/"); // ´´½¨UserApi
    CThostFtdcMdSpi* pUserMdSpi = new CMdSpi();
    pUserMdApi->RegisterSpi(pUserMdSpi);     // ×¢²áÊÂ¼þÀà
    pUserMdApi->RegisterFront(FRONT_ADDR); // connect
    pUserMdApi->Init();
    //pUserMdApi->Join();
    //	pUserApi->Release();

    //FRONT_ADDR = "tcp://180.168.146.187:10001"; // Ç°ÖÃµØÖ·
    char trader_front_addr[] = "tcp://180.168.146.187:10001";
    pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./log/trader/"); // set log path 
    CTraderSpi* pUserTraderSpi = new CTraderSpi();
    pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserTraderSpi); // ×¢²áÊÂ¼þÀà
    pUserTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ×¢²á¹«ÓÐÁ÷
    pUserTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ×¢²áË½ÓÐÁ÷
    pUserTraderApi->RegisterFront(trader_front_addr);		   // connect
    pUserTraderApi->Init();

    pUserMdApi->Join();
    pUserTraderApi->Join();
    //	pUserApi->Release();
}

