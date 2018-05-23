//

#include "../include/MdSpi.h"
#include "../include/ThostFtdcMdApi.h"

// UserApi����
CThostFtdcMdApi* pUserMdApi;

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
    pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./log_md/"); // ����UserApi
    CThostFtdcMdSpi* pUserSpi = new CMdSpi();
    pUserMdApi->RegisterSpi(pUserSpi);     // ע���¼���
    pUserMdApi->RegisterFront(FRONT_ADDR); // connect
    pUserMdApi->Init();

     pUserMdApi->Join();
    //	pUserMdApi->Release();
}

