// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "ThostFtdcTraderApi.h"
#include "TraderSpi.h"

// UserApi����
CThostFtdcTraderApi* pUserApi;

// ���ò���
// char  FRONT_ADDR[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// ǰ�õ�ַ
// TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
// TThostFtdcInvestorIDType INVESTOR_ID = "00069";			// Ͷ���ߴ���
// TThostFtdcPasswordType  PASSWORD = "888888";			// �û�����
// TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu0909";	// ��Լ����
// TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ��������
// TThostFtdcPriceType	LIMIT_PRICE = 38850;				// �۸�

// ������
int iRequestID = 0;

int main(void)
{
    char FRONT_ADDR[] = "tcp://180.168.146.187:10001"; // ǰ�õ�ַ
    //  init UserApi
    pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./ctp_linux_trader/"); // ����UserApi
    CTraderSpi* pUserSpi = new CTraderSpi();
    pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi); // ע���¼���
    pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ע�ṫ����
    pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ע��˽����
    pUserApi->RegisterFront(FRONT_ADDR);		   // connect
    pUserApi->Init();

    pUserApi->Join();
    //	pUserApi->Release();
}

