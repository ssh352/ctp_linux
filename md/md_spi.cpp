#include "md_spi.h"

extern Document d;
extern zmq::context_t context;
extern zmq::socket_t publisher;

CMdSpi::CMdSpi() : userapi(nullptr) , reqid(0) { }

void CMdSpi::load_config(const Document& d)
{
	broker_id = d["broker_id"].GetString();
	user_id = d["user_id"].GetString();
	passwd = d["passwd"].GetString();
	front_id = d["md_address"].GetString();
}

void CMdSpi::connect()
{
	if (userapi == nullptr) {
		userapi = CThostFtdcMdApi::CreateFtdcMdApi("./log/md/"); // ����UserApi

		if (!userapi) {
			// throw runtime_error("ctp_md failed to create api");
			throw "ctp_md failed to create api";
		}
		userapi->RegisterSpi(this);
	}

	userapi->RegisterFront(const_cast<char*>(front_id.c_str())); // connect
	userapi->Init();
	userapi->Join();
	LOG(FATAL) << "CTP exit!";
}

void CMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	// IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// cerr << "--->>> Reason = " << nReason << endl;
	LOG(ERROR) << "Disconect from ctp front."
		<< " Reason code " << nReason;
	// publisher.send("OnFrontDisconnected");
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
	LOG(WARNING) << "Receive ctp Heartbeat timeout.";
}

void CMdSpi::OnFrontConnected()
{

	LOG(INFO) << "Connect to ctp sucessfully.";
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// user login request
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, broker_id.c_str());
	strcpy(req.UserID, user_id.c_str());
	strcpy(req.Password, passwd.c_str());
	int iResult = userapi->ReqUserLogin(&req, ++reqid);
	// FIXME: should reconnect later instead of ASSERT
	// LOG(INFO) << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
	std::cout << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CMdSpi::logout()
{
	CThostFtdcUserLogoutField req = {};
	strcpy(req.BrokerID, broker_id.c_str());
	strcpy(req.UserID, user_id.c_str());
	if (userapi->ReqUserLogout(&req, ++reqid)) {
		LOG(ERROR) << "request logout failed"
			<< "(bid)" << req.BrokerID << "(uid)" << req.UserID;
	}
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	// if (pRspInfo != nullptr && pRspInfo->ErrorID != 0) {
	//         // LOG(INFO) << "Login to ctp failed." << "(errid)" << pRspInfo->ErrorID << "(errmsg)" << pRspInfo->ErrorMsg;
	//         std::cout << "Login to ctp failed." << "(errid)" << pRspInfo->ErrorID << "(errmsg)" << pRspInfo->ErrorMsg;
	// }
	// else
	// {
	//         std::cout << "Login to ctp successed." << "(Bid)" << pRspUserLogin->BrokerID << "(Uid)" << pRspUserLogin->UserID;
	//         // LOG(INFO) << "Login to ctp successed." << "(Bid)" << pRspUserLogin->BrokerID << "(Uid)" << pRspUserLogin->UserID;
	// }
	//
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (pRspInfo == NULL) {
		std::cout << "pRspInfo is null" << std::endl;
	}

	if (pRspUserLogin == NULL) {
		std::cout << "pRspUserLogin is null" << std::endl;
	}
	cout << pRspUserLogin->TradingDay << endl;
	cout << pRspUserLogin->LoginTime << endl;
	cout << pRspUserLogin->BrokerID << endl;
	cout << pRspUserLogin->UserID << endl;
	cout << pRspUserLogin->SystemName << endl;
	cout << pRspUserLogin->FrontID << endl;
	cout << pRspUserLogin->SessionID << endl;
	cout << pRspUserLogin->MaxOrderRef << endl;
	cout << pRspUserLogin->SHFETime << endl;
	cout << pRspUserLogin->DCETime << endl;
	cout << pRspUserLogin->CZCETime << endl;
	cout << pRspUserLogin->FFEXTime << endl;
	cout << pRspUserLogin->INETime << endl;
	// if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	//     cerr << "--->>> call api function GetTradingDay = " << pUserMdApi->GetTradingDay() << endl;
	// }
	SubscribeMarketData();	
}
void CMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0) {
		LOG(INFO) << "logout to ctp failed."
			<< "(errid)" << pRspInfo->ErrorID << "(errmsg)" << pRspInfo->ErrorMsg;
	} else {
		LOG(INFO) << "Login to ctp successed."
			<< "(Bid)" << pUserLogout->BrokerID << "(Uid)" << pUserLogout->UserID;
	}
}


void CMdSpi::SubscribeMarketData(){
	char *insts[2];
	insts[0] = const_cast<char*>("IF1807");
	insts[1] = const_cast<char*>("IH1807");
	int ret = userapi->SubscribeMarketData(insts, 2);
	cerr << "--->>> send SubscribeMarketData request: " << ((ret == 0) ? "success" : "failed") << endl;
}

void CMdSpi::SubscribeMarketData(const vector<string>& instruments, const vector<string>& markets)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// //char* pp[] = { "IF1804", "IF1805" };
	// char* pp[] = { "IF1807" };
	// int iResult = pUserMdApi->SubscribeMarketData(pp, 2);
	// cerr << "--->>> send SubscribeMarketData request: " << ((iResult == 0) ? "success" : "failed") << endl;
	//
	int ncount = instruments.size();
	char* insts[ncount];
	for (int i = 0; i < ncount; i++)
		insts[i] = const_cast<char*>(instruments[i].c_str());

	userapi->SubscribeMarketData(insts, ncount);
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0) {
		LOG(ERROR) << "[OnRspSubMarketData]"
			<< "(errID)" << pRspInfo->ErrorID << "(errMsg)" << pRspInfo->ErrorMsg;
	}
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	// cerr << "--->>> " << __FUNCTION__ << endl;
	// printf("InstrumentID=%s\n", pSpecificInstrument->InstrumentID);
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{

	ctp::TDF_FUTURE_DATA td;
	// #TODO: ���ֶ�tdf��δʹ��
	// cerr << "ExchangeID=" << pDepthMarketData->ExchangeID << endl;
	// cerr << "ExchangeInstID=" << pDepthMarketData->ExchangeInstID << endl;
	// cerr << "UpdateTime=" << pDepthMarketData->UpdateTime << endl;

	td.set_szcode(pDepthMarketData->InstrumentID);		  //ԭʼCode
	// td.set_nactionday(pDepthMarketData->ActionDay);		  //ҵ������(��Ȼ��)
	// td.set_ntradingday(pDepthMarketData->TradingDay);		  //������
	td.set_ntime(pDepthMarketData->UpdateMillisec);		  //ʱ��(HHMMSSmmm)
	td.set_ipreopeninterest(pDepthMarketData->PreOpenInterest);   //��ֲ�
	td.set_npreclose(pDepthMarketData->PreClosePrice);		  //�����̼�
	td.set_npresettleprice(pDepthMarketData->PreSettlementPrice); //�����
	td.set_nopen(pDepthMarketData->OpenPrice);			  //���̼�
	td.set_nhigh(pDepthMarketData->HighestPrice);		  //��߼�
	td.set_nlow(pDepthMarketData->LowestPrice);			  //��ͼ�
	td.set_nmatch(pDepthMarketData->LastPrice);			  //���¼�
	td.set_ivolume(pDepthMarketData->Volume);			  //�ɽ�����
	td.set_iturnover(pDepthMarketData->Turnover);		  //�ɽ��ܽ��
	td.set_iopeninterest(pDepthMarketData->OpenInterest);	 //�ֲ�����
	td.set_nclose(pDepthMarketData->ClosePrice);		  //������
	td.set_nsettleprice(pDepthMarketData->SettlementPrice);       //�����
	td.set_nhighlimited(pDepthMarketData->UpperLimitPrice);       //��ͣ��
	td.set_nlowlimited(pDepthMarketData->LowerLimitPrice);	//��ͣ��
	td.set_npredelta(pDepthMarketData->PreDelta);		  //����ʵ��
	td.set_ncurrdelta(pDepthMarketData->CurrDelta);		  //����ʵ��

	td.add_naskprice(pDepthMarketData->AskPrice1); //������
	td.add_naskprice(pDepthMarketData->AskPrice2); //������
	td.add_naskprice(pDepthMarketData->AskPrice3); //������
	td.add_naskprice(pDepthMarketData->AskPrice4); //������
	td.add_naskprice(pDepthMarketData->AskPrice5); //������

	td.add_naskvol(pDepthMarketData->AskVolume1); //������
	td.add_naskvol(pDepthMarketData->AskVolume2); //������
	td.add_naskvol(pDepthMarketData->AskVolume3); //������
	td.add_naskvol(pDepthMarketData->AskVolume4); //������
	td.add_naskvol(pDepthMarketData->AskVolume5); //������

	td.add_nbidprice(pDepthMarketData->BidPrice1); //�����
	td.add_nbidprice(pDepthMarketData->BidPrice2); //�����
	td.add_nbidprice(pDepthMarketData->BidPrice3); //�����
	td.add_nbidprice(pDepthMarketData->BidPrice4); //�����
	td.add_nbidprice(pDepthMarketData->BidPrice5); //�����

	td.add_nbidvol(pDepthMarketData->BidVolume1);     //������
	td.add_nbidvol(pDepthMarketData->BidVolume2);     //������
	td.add_nbidvol(pDepthMarketData->BidVolume3);     //������
	td.add_nbidvol(pDepthMarketData->BidVolume4);     //������
	td.add_nbidvol(pDepthMarketData->BidVolume5);     //������
	td.set_navgprice(pDepthMarketData->AveragePrice); //֣�����ڻ�����

	//TODO: �ֶ�ctp������û��
	td.set_szwindcode("");   //600001.SH
	td.set_nauctionprice(0); //�������жϲο���
	td.set_nauctionqty(0);   //�������жϼ��Ͼ�������ƥ����
	td.set_nstatus(0);       //״̬

	auto msg_size = td.ByteSize();
	zmq::message_t message(msg_size);
	// td.SerializerToString(message.data(),msg_size);
	td.SerializeToArray(message.data(),msg_size);
	publisher.send(message);
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

