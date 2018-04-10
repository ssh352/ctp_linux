// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "MdSpi.h"
#include "ThostFtdcMdApi.h"

// UserApi����
CThostFtdcMdApi* pUserApi;

// ���ò���
// TThostFtdcBrokerIDType BROKER_ID = "6000";	// ���͹�˾����
// TThostFtdcInvestorIDType INVESTOR_ID = "";	// Ͷ���ߴ���
// TThostFtdcPasswordType PASSWORD = "";		  // �û�����
// char* ppInstrumentID[] = { "rb1602", "rb1603" };  // ���鶩���б�
// int iInstrumentID = 2; // ���鶩������

// ������
int iRequestID = 0;

int main(void)
{
    //char FRONT_ADDR[] = "tcp://180.168.146.187:10011"; // ǰ�õ�ַ
    char FRONT_ADDR[] = "tcp://180.168.214.246:41213"; // ǰ�õ�ַ
    // ��ʼ��UserApi
    pUserApi = CThostFtdcMdApi::CreateFtdcMdApi("./ctp_linux_md/"); // ����UserApi
    CThostFtdcMdSpi* pUserSpi = new CMdSpi();
    pUserApi->RegisterSpi(pUserSpi);     // ע���¼���
    pUserApi->RegisterFront(FRONT_ADDR); // connect
    pUserApi->Init();

     pUserApi->Join();
    //	pUserApi->Release();
}

