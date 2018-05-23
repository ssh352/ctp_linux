//

#include "../include/MdSpi.h"
#include "../include/ThostFtdcMdApi.h"

// UserApi对象
CThostFtdcMdApi* pUserMdApi;

// 配置参数
// TThostFtdcBrokerIDType BROKER_ID = "6000";	// 经纪公司代码
// TThostFtdcInvestorIDType INVESTOR_ID = "";	// 投资者代码
// TThostFtdcPasswordType PASSWORD = "";		  // 用户密码
// char* ppInstrumentID[] = { "rb1602", "rb1603" };  // 行情订阅列表
// int iInstrumentID = 2; // 行情订阅数量

// 请求编号
int iRequestID = 0;

int main(void)
{
    //char FRONT_ADDR[] = "tcp://180.168.146.187:10011"; // 前置地址
    char FRONT_ADDR[] = "tcp://180.168.214.246:41213"; // 前置地址
    // 初始化UserApi
    pUserMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./log_md/"); // 创建UserApi
    CThostFtdcMdSpi* pUserSpi = new CMdSpi();
    pUserMdApi->RegisterSpi(pUserSpi);     // 注册事件类
    pUserMdApi->RegisterFront(FRONT_ADDR); // connect
    pUserMdApi->Init();

     pUserMdApi->Join();
    //	pUserMdApi->Release();
}

