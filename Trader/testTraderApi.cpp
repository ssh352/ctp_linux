//
#include "../include/ThostFtdcTraderApi.h"
#include "../include/TraderSpi.h"

// UserApi对象
CThostFtdcTraderApi* pUserTraderApi;

// 配置参数
// char  FRONT_ADDR[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// 前置地址
// TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码
// TThostFtdcInvestorIDType INVESTOR_ID = "00069";			// 投资者代码
// TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码
// TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu0909";	// 合约代码
// TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// 买卖方向
// TThostFtdcPriceType	LIMIT_PRICE = 38850;				// 价格

// 请求编号
int iRequestID = 0;

int main(void)
{
    char FRONT_ADDR[] = "tcp://180.168.146.187:10001"; // 前置地址
    //  init UserApi
    pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./log_trader/"); // set log path 
    CTraderSpi* pUserSpi = new CTraderSpi();
    pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi); // 注册事件类
    pUserTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);    // 注册公有流
    pUserTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // 注册私有流
    pUserTraderApi->RegisterFront(FRONT_ADDR);		   // connect
    pUserTraderApi->Init();

    pUserTraderApi->Join();
    //	pUserTraderApi->Release();
}

