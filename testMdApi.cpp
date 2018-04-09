// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "ThostFtdcMdApi.h"
#include "MdSpi.h"

// UserApi����
CThostFtdcMdApi* pUserApi;

// ���ò���
char FRONT_ADDR[] = "tcp://180.168.146.187:10011"; // ǰ�õ�ַ
// TThostFtdcBrokerIDType BROKER_ID = "6000";	// ���͹�˾����
// TThostFtdcInvestorIDType INVESTOR_ID = "";	// Ͷ���ߴ���
// TThostFtdcPasswordType PASSWORD = "";		  // �û�����
// char* ppInstrumentID[] = { "rb1602", "rb1603" };  // ���鶩���б�
// int iInstrumentID = 2; // ���鶩������

// ������
 int iRequestID = 0;

int main(void)
{
    // ��ʼ��UserApi
    pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(); // ����UserApi
    CThostFtdcMdSpi* pUserSpi = new CMdSpi();
    pUserApi->RegisterSpi(pUserSpi);     // ע���¼���
    pUserApi->RegisterFront(FRONT_ADDR); // connect
    pUserApi->Init();

    pUserApi->Join();
    //	pUserApi->Release();
}

