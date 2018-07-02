//
#include "../include/ThostFtdcTraderApi.h"
#include "../include/TraderSpi.h"

// UserApi����
CThostFtdcTraderApi* pUserTraderApi;

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
    pUserTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./log_trader/"); // set log path 
    CTraderSpi* pUserSpi = new CTraderSpi();
    pUserTraderApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi); // ע���¼���
    pUserTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);    // ע�ṫ����
    pUserTraderApi->SubscribePrivateTopic(THOST_TERT_RESTART);   // ע��˽����
    pUserTraderApi->RegisterFront(FRONT_ADDR);		   // connect
    pUserTraderApi->Init();

    pUserTraderApi->Join();
    //	pUserTraderApi->Release();
}

