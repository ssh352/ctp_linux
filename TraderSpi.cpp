// #include "ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;


#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcTraderApi* pUserApi;

// ���ò���
TThostFtdcBrokerIDType BROKER_ID = "9999";	     // ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "067938";       // Ͷ���ߴ���
TThostFtdcPasswordType PASSWORD = "6432281";	   // �û�����
TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu1810";   // ��Լ����
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell; // ��������
TThostFtdcPriceType LIMIT_PRICE = 51800;	       // �۸�

// ������
extern int iRequestID;

// �Ự����
TThostFtdcFrontIDType FRONT_ID;     //ǰ�ñ��
TThostFtdcSessionIDType SESSION_ID; //�Ự���
TThostFtdcOrderRefType ORDER_REF;   //��������

void PrintOut(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

    printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID=[%d], bIsLast=[%d]\n", nRequestID, bIsLast);
}

void CTraderSpi::OnFrontConnected()
{
    cerr << "--->>> "
	 << "OnFrontConnected" << endl;
    // user login request
    ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.UserID, "067938");
    strcpy(req.Password, "6432281");
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    cerr << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    // copy from ctp documents demo
    if (pRspInfo->ErrorID != 0) {
	return;
    }

    printf("TradingDay=%s,LoginTime=%s,BrokerID=%s,UserId=%s,SystemName=%s,FrontID=%d,SessionID=%d\n", pRspUserLogin->TradingDay, pRspUserLogin->LoginTime, pRspUserLogin->BrokerID, pRspUserLogin->UserID, pRspUserLogin->SystemName, pRspUserLogin->FrontID, pRspUserLogin->SessionID);
    printf("MaxOrderRef=%s,SHFETime=%s,DCETime=%s,CZCETime=%s,FFEXTime=%s,INETime=%s\n", pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime, pRspUserLogin->DCETime, pRspUserLogin->CZCETime, pRspUserLogin->FFEXTime, pRspUserLogin->INETime);
    // cerr << "TradingDay=" << pRspUserLogin->TradingDay << endl;
    // cerr << "TBrokerID=" << pRspUserLogin->BrokerID << endl;
    // cerr << "TUserID=" << pRspUserLogin->UserID << endl;
    // cerr << "TPassword=" << pRspUserLogin->Password << endl;
    // cerr << "TUserProductInfo=" << pRspUserLogin->UserProductInfo << endl;
    // cerr << "TInterfaceProductInfo=" << pRspUserLogin->InterfaceProductInfo << endl;
    // cerr << "TProtocolInfo=" << pRspUserLogin->ProtocolInfo << endl;
    // cerr << "TMacAddress=" << pRspUserLogin->MacAddress << endl;
    // cerr << "TOneTimePassword=" << pRspUserLogin->OneTimePassword << endl;
    // cerr << "TClientIPAddress=" << pRspUserLogin->ClientIPAddress << endl;
    //
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	// ����Ự����
	FRONT_ID = pRspUserLogin->FrontID;
	SESSION_ID = pRspUserLogin->SessionID;
	int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
	iNextOrderRef++;
	sprintf(ORDER_REF, "%d", iNextOrderRef);
	///��ȡ��ǰ������
	cerr << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
	///Ͷ���߽�����ȷ��
	ReqSettlementInfoConfirm();
    }
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.InvestorID, "067938");
    int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
    cerr << "--->>> investor confirm settlement: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspSettlementInfoConfirm" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    cerr << "BrokerID" << pSettlementInfoConfirm->BrokerID;
    ///Ͷ���ߴ���
    cerr << "InvertorID" << pSettlementInfoConfirm->InvestorID;
    ///ȷ������
    cerr << "confirmdate" << pSettlementInfoConfirm->ConfirmDate;
    ///ȷ��ʱ��
    cerr << "confirmtime" << pSettlementInfoConfirm->ConfirmTime;
    if (pRspInfo->ErrorID == 0) {
	///�����ѯ��Լ
	ReqQryInstrument();
    }
}

void CTraderSpi::ReqQryInstrument()
{
    CThostFtdcQryInstrumentField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
    cerr << "--->>> send ReqQryInstrument request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspQryInstrument" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///��Լ����
    cerr << "InstrumentID=" << pInstrument->InstrumentID << endl;
    ///����������
    cerr << "ExchangeID=" << pInstrument->ExchangeID << endl;
    ///��Լ����
    cerr << "InstrumentName=" << pInstrument->InstrumentName << endl;
    ///��Լ�ڽ������Ĵ���
    cerr << "ExchangeInstID=" << pInstrument->ExchangeInstID << endl;
    ///��Ʒ����
    cerr << "ProductID=" << pInstrument->ProductID << endl;
    ///��Ʒ����
    cerr << "ProductClass=" << pInstrument->ProductClass << endl;
    ///�������
    cerr << "DeliveryYear=" << pInstrument->DeliveryYear << endl;
    ///������
    cerr << "DeliveryMonth=" << pInstrument->DeliveryMonth << endl;
    ///�м۵�����µ���
    cerr << "MaxMarketOrderVolume=" << pInstrument->MaxMarketOrderVolume << endl;
    ///�м۵���С�µ���
    cerr << "MinMarketOrderVolume=" << pInstrument->MinMarketOrderVolume << endl;
    ///�޼۵�����µ���
    cerr << "MaxLimitOrderVolume=" << pInstrument->MaxLimitOrderVolume << endl;
    ///�޼۵���С�µ���
    cerr << "MinLimitOrderVolume=" << pInstrument->MinLimitOrderVolume << endl;
    ///��Լ��������
    cerr << "VolumeMultiple=" << pInstrument->VolumeMultiple << endl;
    ///��С�䶯��λ
    cerr << "PriceTick=" << pInstrument->PriceTick << endl;
    ///������
    cerr << "CreateDate=" << pInstrument->CreateDate << endl;
    ///������
    cerr << "OpenDate=" << pInstrument->OpenDate << endl;
    ///������
    cerr << "ExpireDate=" << pInstrument->ExpireDate << endl;
    ///��ʼ������
    cerr << "StartDelivDate=" << pInstrument->StartDelivDate << endl;
    ///����������
    cerr << "EndDelivDate=" << pInstrument->EndDelivDate << endl;
    ///��Լ��������״̬
    cerr << "InstLifePhase=" << pInstrument->InstLifePhase << endl;
    ///��ǰ�Ƿ���
    cerr << "IsTrading=" << pInstrument->IsTrading << endl;
    ///�ֲ�����
    cerr << "PositionType=" << pInstrument->PositionType << endl;
    ///�ֲ���������
    cerr << "PositionDateType=" << pInstrument->PositionDateType << endl;
    ///��ͷ��֤����
    cerr << "LongMarginRatio=" << pInstrument->LongMarginRatio << endl;
    ///��ͷ��֤����
    cerr << "ShortMarginRatio=" << pInstrument->ShortMarginRatio << endl;
    ///�Ƿ�ʹ�ô��߱�֤���㷨
    cerr << "MaxMarginSideAlgorithm=" << pInstrument->MaxMarginSideAlgorithm << endl;
    ///������Ʒ����
    cerr << "UnderlyingInstrID=" << pInstrument->UnderlyingInstrID << endl;
    ///ִ�м�
    cerr << "StrikePrice=" << pInstrument->StrikePrice << endl;
    ///��Ȩ����
    cerr << "OptionsType=" << pInstrument->OptionsType << endl;
    ///��Լ������Ʒ����
    cerr << "UnderlyingMultiple=" << pInstrument->UnderlyingMultiple << endl;
    ///�������
    cerr << "CombinationType=" << pInstrument->CombinationType << endl;
    ///��С���µ���λ
    cerr << "MinBuyVolume=" << pInstrument->MinBuyVolume << endl;
    ///��С���µ���λ
    cerr << "MinSellVolume=" << pInstrument->MinSellVolume << endl;
    ///��Լ��ʶ��
    cerr << "InstrumentCode=" << pInstrument->InstrumentCode << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	// �����ѯ��Լ
	ReqQryTradingAccount();
    }
}

void CTraderSpi::ReqQryTradingAccount()
{
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.InvestorID, "067938");
    int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
    cerr << "--->>> sent ReqQryTradingAccount request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspQryTradingAccount" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    cerr << "brokerid=" << pTradingAccount->BrokerID;
    ///Ͷ�����ʺ�
    cerr << "AccountID=" << pTradingAccount->AccountID << endl;
    ///�ϴ���Ѻ���
    cerr << "PreMortgage=" << pTradingAccount->PreMortgage << endl;
    ///�ϴ����ö��
    cerr << "PreCredit=" << pTradingAccount->PreCredit << endl;
    ///�ϴδ���
    cerr << "PreDeposit=" << pTradingAccount->PreDeposit << endl;
    ///�ϴν���׼����
    cerr << "PreBalance=" << pTradingAccount->PreBalance << endl;
    ///�ϴ�ռ�õı�֤��
    cerr << "PreMargin=" << pTradingAccount->PreMargin << endl;
    ///��Ϣ����
    cerr << "InterestBase=" << pTradingAccount->InterestBase << endl;
    ///��Ϣ����
    cerr << "Interest=" << pTradingAccount->Interest << endl;
    ///�����
    cerr << "Deposit=" << pTradingAccount->Deposit << endl;
    ///������
    cerr << "Withdraw=" << pTradingAccount->Withdraw << endl;
    ///����ı�֤��
    cerr << "FrozenMargin=" << pTradingAccount->FrozenMargin << endl;
    ///������ʽ�
    cerr << "FrozenCash=" << pTradingAccount->FrozenCash << endl;
    ///�����������
    cerr << "FrozenCommission=" << pTradingAccount->FrozenCommission << endl;
    ///��ǰ��֤���ܶ�
    cerr << "CurrMargin=" << pTradingAccount->CurrMargin << endl;
    ///�ʽ���
    cerr << "CashIn=" << pTradingAccount->CashIn << endl;
    ///������
    cerr << "Commission=" << pTradingAccount->Commission << endl;
    ///ƽ��ӯ��
    cerr << "CloseProfit=" << pTradingAccount->CloseProfit << endl;
    ///�ֲ�ӯ��
    cerr << "PositionProfit=" << pTradingAccount->PositionProfit << endl;
    ///�ڻ�����׼����
    cerr << "Balance=" << pTradingAccount->Balance << endl;
    ///�����ʽ�
    cerr << "Available=" << pTradingAccount->Available << endl;
    ///��ȡ�ʽ�
    cerr << "WithdrawQuota=" << pTradingAccount->WithdrawQuota << endl;
    ///����׼����
    cerr << "Reserve=" << pTradingAccount->Reserve << endl;
    ///������
    cerr << "TradingDay=" << pTradingAccount->TradingDay << endl;
    ///������
    cerr << "SettlementID=" << pTradingAccount->SettlementID << endl;
    ///���ö��
    cerr << "Credit=" << pTradingAccount->Credit << endl;
    ///��Ѻ���
    cerr << "Mortgage=" << pTradingAccount->Mortgage << endl;
    ///��������֤��
    cerr << "ExchangeMargin=" << pTradingAccount->ExchangeMargin << endl;
    ///Ͷ���߽��֤��
    cerr << "DeliveryMargin=" << pTradingAccount->DeliveryMargin << endl;
    ///���������֤��
    cerr << "ExchangeDeliveryMargin=" << pTradingAccount->ExchangeDeliveryMargin << endl;
    ///�����ڻ�����׼����
    cerr << "ReserveBalance=" << pTradingAccount->ReserveBalance << endl;
    ///���ִ���
    cerr << "CurrencyID=" << pTradingAccount->CurrencyID << endl;
    ///�ϴλ���������
    cerr << "PreFundMortgageIn=" << pTradingAccount->PreFundMortgageIn << endl;
    ///�ϴλ����ʳ����
    cerr << "PreFundMortgageOut=" << pTradingAccount->PreFundMortgageOut << endl;
    ///����������
    cerr << "FundMortgageIn=" << pTradingAccount->FundMortgageIn << endl;
    ///�����ʳ����
    cerr << "FundMortgageOut=" << pTradingAccount->FundMortgageOut << endl;
    ///������Ѻ���
    cerr << "FundMortgageAvailable=" << pTradingAccount->FundMortgageAvailable << endl;
    ///����Ѻ���ҽ��
    cerr << "MortgageableFund=" << pTradingAccount->MortgageableFund << endl;
    ///�����Ʒռ�ñ�֤��
    cerr << "SpecProductMargin=" << pTradingAccount->SpecProductMargin << endl;
    ///�����Ʒ���ᱣ֤��
    cerr << "SpecProductFrozenMargin=" << pTradingAccount->SpecProductFrozenMargin << endl;
    ///�����Ʒ������
    cerr << "SpecProductCommission=" << pTradingAccount->SpecProductCommission << endl;
    ///�����Ʒ����������
    cerr << "SpecProductFrozenCommission=" << pTradingAccount->SpecProductFrozenCommission << endl;
    ///�����Ʒ�ֲ�ӯ��
    cerr << "SpecProductPositionProfit=" << pTradingAccount->SpecProductPositionProfit << endl;
    ///�����Ʒƽ��ӯ��
    cerr << "SpecProductCloseProfit=" << pTradingAccount->SpecProductCloseProfit << endl;
    ///���ݳֲ�ӯ���㷨����������Ʒ�ֲ�ӯ��
    cerr << "SpecProductPositionProfitByAlg=" << pTradingAccount->SpecProductPositionProfitByAlg << endl;
    ///�����Ʒ��������֤��
    cerr << "SpecProductExchangeMargin=" << pTradingAccount->SpecProductExchangeMargin << endl;
    ///ҵ������
    cerr << "BizType=" << pTradingAccount->BizType << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///�����ѯͶ���ֲ߳�
	ReqQryInvestorPosition();
    }
}

void CTraderSpi::ReqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
    cerr << "--->>> sent ReqQryInvestorPosition request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspQryInvestorPosition" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///��Լ����
    cerr << "InstrumentID=" << pInvestorPosition->InstrumentID << endl;
    ///���͹�˾����
    cerr << "brokerID=" << pInvestorPosition->BrokerID << endl;
    ///Ͷ���ߴ���
    cerr << "InvestorID=" << pInvestorPosition->InvestorID << endl;
    ///�ֲֶ�շ���
    cerr << "PosiDirection=" << pInvestorPosition->PosiDirection << endl;
    ///Ͷ���ױ���־
    cerr << "HedgeFlag=" << pInvestorPosition->HedgeFlag << endl;
    ///�ֲ�����
    cerr << "PositionDate=" << pInvestorPosition->PositionDate << endl;
    ///���ճֲ�
    cerr << "YdPosition=" << pInvestorPosition->YdPosition << endl;
    ///���ճֲ�
    cerr << "Position=" << pInvestorPosition->Position << endl;
    ///��ͷ����
    cerr << "LongFrozen=" << pInvestorPosition->LongFrozen << endl;
    ///��ͷ����
    cerr << "ShortFrozen=" << pInvestorPosition->ShortFrozen << endl;
    ///���ֶ�����
    cerr << "LongFrozenAmount=" << pInvestorPosition->LongFrozenAmount << endl;
    ///���ֶ�����
    cerr << "ShortFrozenAmount=" << pInvestorPosition->ShortFrozenAmount << endl;
    ///������
    cerr << "OpenVolume=" << pInvestorPosition->OpenVolume << endl;
    ///ƽ����
    cerr << "CloseVolume=" << pInvestorPosition->CloseVolume << endl;
    ///���ֽ��
    cerr << "OpenAmount=" << pInvestorPosition->OpenAmount << endl;
    ///ƽ�ֽ��
    cerr << "CloseAmount=" << pInvestorPosition->CloseAmount << endl;
    ///�ֲֳɱ�
    cerr << "PositionCost=" << pInvestorPosition->PositionCost << endl;
    ///�ϴ�ռ�õı�֤��
    cerr << "PreMargin=" << pInvestorPosition->PreMargin << endl;
    ///ռ�õı�֤��
    cerr << "UseMargin=" << pInvestorPosition->UseMargin << endl;
    ///����ı�֤��
    cerr << "FrozenMargin=" << pInvestorPosition->FrozenMargin << endl;
    ///������ʽ�
    cerr << "FrozenCash=" << pInvestorPosition->FrozenCash << endl;
    ///�����������
    cerr << "FrozenCommission=" << pInvestorPosition->FrozenCommission << endl;
    ///�ʽ���
    cerr << "CashIn=" << pInvestorPosition->CashIn << endl;
    ///������
    cerr << "Commission=" << pInvestorPosition->Commission << endl;
    ///ƽ��ӯ��
    cerr << "CloseProfit=" << pInvestorPosition->CloseProfit << endl;
    ///�ֲ�ӯ��
    cerr << "PositionProfit=" << pInvestorPosition->PositionProfit << endl;
    ///�ϴν����
    cerr << "PreSettlementPrice=" << pInvestorPosition->PreSettlementPrice << endl;
    ///���ν����
    cerr << "SettlementPrice=" << pInvestorPosition->SettlementPrice << endl;
    ///������
    cerr << "TradingDay=" << pInvestorPosition->TradingDay << endl;
    ///������
    cerr << "SettlementID=" << pInvestorPosition->SettlementID << endl;
    ///���ֳɱ�
    cerr << "OpenCost=" << pInvestorPosition->OpenCost << endl;
    ///��������֤��
    cerr << "ExchangeMargin=" << pInvestorPosition->ExchangeMargin << endl;
    ///��ϳɽ��γɵĳֲ�
    cerr << "CombPosition=" << pInvestorPosition->CombPosition << endl;
    ///��϶�ͷ����
    cerr << "CombLongFrozen=" << pInvestorPosition->CombLongFrozen << endl;
    ///��Ͽ�ͷ����
    cerr << "CombShortFrozen=" << pInvestorPosition->CombShortFrozen << endl;
    ///���ն���ƽ��ӯ��
    cerr << "CloseProfitByDate=" << pInvestorPosition->CloseProfitByDate << endl;
    ///��ʶԳ�ƽ��ӯ��
    cerr << "CloseProfitByTrade=" << pInvestorPosition->CloseProfitByTrade << endl;
    ///���ճֲ�
    cerr << "TodayPosition=" << pInvestorPosition->TodayPosition << endl;
    ///��֤����
    cerr << "MarginRateByMoney=" << pInvestorPosition->MarginRateByMoney << endl;
    ///��֤����(������)
    cerr << "MarginRateByVolume=" << pInvestorPosition->MarginRateByVolume << endl;
    ///ִ�ж���
    cerr << "StrikeFrozen=" << pInvestorPosition->StrikeFrozen << endl;
    ///ִ�ж�����
    cerr << "StrikeFrozenAmount=" << pInvestorPosition->StrikeFrozenAmount << endl;
    ///����ִ�ж���
    cerr << "AbandonFrozen=" << pInvestorPosition->AbandonFrozen << endl;
    ///����������
    cerr << "ExchangeID=" << pInvestorPosition->ExchangeID << endl;
    ///ִ�ж�������
    cerr << "YdStrikeFrozen=" << pInvestorPosition->YdStrikeFrozen << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///����¼������
	ReqOrderInsert();
    }
}

void CTraderSpi::ReqOrderInsert()
{
    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    ///���͹�˾����
    strcpy(req.BrokerID, BROKER_ID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, INVESTOR_ID);
    ///��Լ����
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    ///��������
    strcpy(req.OrderRef, ORDER_REF);
    ///�û�����
    //	TThostFtdcUserIDType	UserID;
    ///�����۸�����: �޼�
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ///��������:
    req.Direction = DIRECTION;
    ///��Ͽ�ƽ��־: ����
    req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ///���Ͷ���ױ���־
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    ///�۸�
    req.LimitPrice = LIMIT_PRICE;
    ///����: 1
    req.VolumeTotalOriginal = 1;
    ///��Ч������: ������Ч
    req.TimeCondition = THOST_FTDC_TC_GFD;
    ///GTD����
    //	TThostFtdcDateType	GTDDate;
    ///�ɽ�������: �κ�����
    req.VolumeCondition = THOST_FTDC_VC_AV;
    ///��С�ɽ���: 1
    req.MinVolume = 1;
    ///��������: ����
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    ///ֹ���
    //	TThostFtdcPriceType	StopPrice;
    ///ǿƽԭ��: ��ǿƽ
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ///�Զ������־: ��
    req.IsAutoSuspend = 0;
    ///ҵ��Ԫ
    //	TThostFtdcBusinessUnitType	BusinessUnit;
    ///������
    //	TThostFtdcRequestIDType	RequestID;
    ///�û�ǿ����־: ��
    req.UserForceClose = 0;

    int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
    cerr << "--->>> sent orderInsert request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspOrderInsert" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    cerr << "brokerid=" << pInputOrder->BrokerID << endl;
    ///Ͷ���ߴ���
    cerr << "InvestorID=" << pInputOrder->InvestorID << endl;
    ///��Լ����
    cerr << "InstrumentID=" << pInputOrder->InstrumentID << endl;
    ///��������
    cerr << "OrderRef=" << pInputOrder->OrderRef << endl;
    ///�û�����
    cerr << "UserID=" << pInputOrder->UserID << endl;
    ///�����۸�����
    cerr << "OrderPriceType=" << pInputOrder->OrderPriceType << endl;
    ///��������
    cerr << "Direction=" << pInputOrder->Direction << endl;
    ///��Ͽ�ƽ��־
    cerr << "CombOffsetFlag=" << pInputOrder->CombOffsetFlag << endl;
    ///���Ͷ���ױ���־
    cerr << "CombHedgeFlag=" << pInputOrder->CombHedgeFlag << endl;
    ///�۸�
    cerr << "LimitPrice=" << pInputOrder->LimitPrice << endl;
    ///����
    cerr << "VolumeTotalOriginal=" << pInputOrder->VolumeTotalOriginal << endl;
    ///��Ч������
    cerr << "TimeCondition=" << pInputOrder->TimeCondition << endl;
    ///GTD����
    cerr << "GTDDate=" << pInputOrder->GTDDate << endl;
    ///�ɽ�������
    cerr << "VolumeCondition=" << pInputOrder->VolumeCondition << endl;
    ///��С�ɽ���
    cerr << "MinVolume=" << pInputOrder->MinVolume << endl;
    ///��������
    cerr << "ContingentCondition=" << pInputOrder->ContingentCondition << endl;
    ///ֹ���
    cerr << "StopPrice=" << pInputOrder->StopPrice << endl;
    ///ǿƽԭ��
    cerr << "ForceCloseReason=" << pInputOrder->ForceCloseReason << endl;
    ///�Զ������־
    cerr << "IsAutoSuspend=" << pInputOrder->IsAutoSuspend << endl;
    ///ҵ��Ԫ
    cerr << "BusinessUnit=" << pInputOrder->BusinessUnit << endl;
    ///������
    cerr << "RequestID=" << pInputOrder->RequestID << endl;
    ///�û�ǿ����־
    cerr << "UserForceClose=" << pInputOrder->UserForceClose << endl;
    ///��������־
    cerr << "IsSwapOrder=" << pInputOrder->IsSwapOrder << endl;
    ///����������
    cerr << "ExchangeID=" << pInputOrder->ExchangeID << endl;
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField* pOrder)
{
    static bool ORDER_ACTION_SENT = false; //�Ƿ����˱���
    if (ORDER_ACTION_SENT)
	return;

    CThostFtdcInputOrderActionField req;
    memset(&req, 0, sizeof(req));
    ///���͹�˾����
    strcpy(req.BrokerID, pOrder->BrokerID);
    ///Ͷ���ߴ���
    strcpy(req.InvestorID, pOrder->InvestorID);
    ///������������
    //	TThostFtdcOrderActionRefType	OrderActionRef;
    ///��������
    strcpy(req.OrderRef, pOrder->OrderRef);
    ///������
    //	TThostFtdcRequestIDType	RequestID;
    ///ǰ�ñ��
    req.FrontID = FRONT_ID;
    ///�Ự���
    req.SessionID = SESSION_ID;
    ///����������
    //	TThostFtdcExchangeIDType	ExchangeID;
    ///�������
    //	TThostFtdcOrderSysIDType	OrderSysID;
    ///������־
    req.ActionFlag = THOST_FTDC_AF_Delete;
    ///�۸�
    //	TThostFtdcPriceType	LimitPrice;
    ///�����仯
    //	TThostFtdcVolumeType	VolumeChange;
    ///�û�����
    //	TThostFtdcUserIDType	UserID;
    ///��Լ����
    strcpy(req.InstrumentID, pOrder->InstrumentID);

    int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
    cerr << "--->>> sent orderaction request: " << ((iResult == 0) ? "success" : "failed") << endl;
    ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspOrderAction" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    cerr << "brokerid" << pInputOrderAction->BrokerID << endl;
    ///Ͷ���ߴ���
    cerr << "InvestorID" << pInputOrderAction->InvestorID << endl;
    ///������������
    cerr << "OrderActionRef" << pInputOrderAction->OrderActionRef << endl;
    ///��������
    cerr << "OrderRef" << pInputOrderAction->OrderRef << endl;
    ///������
    cerr << "RequestID" << pInputOrderAction->RequestID << endl;
    ///ǰ�ñ��
    cerr << "FrontID" << pInputOrderAction->FrontID << endl;
    ///�Ự���
    cerr << "SessionID" << pInputOrderAction->SessionID << endl;
    ///����������
    cerr << "brokerid" << pInputOrderAction->ExchangeID << endl;
    ///�������
    cerr << "brokerid" << pInputOrderAction->OrderSysID << endl;
    ///������־
    cerr << "brokerid" << pInputOrderAction->ActionFlag << endl;
    ///�۸�
    cerr << "LimitPrice " << pInputOrderAction->LimitPrice << endl;
    ///�����仯
    cerr << "VolumeChange " << pInputOrderAction->VolumeChange << endl;
    ///�û�����
    cerr << "UserID " << pInputOrderAction->UserID << endl;
    ///��Լ����
    cerr << "InstrumentID " << pInputOrderAction->InstrumentID << endl;
}

///����֪ͨ
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
    cerr << "--->>> "
	 << "OnRtnOrder" << endl;
    ///���͹�˾����
    cerr << "brokerid=" << pOrder->BrokerID << endl;
    ///Ͷ���ߴ���
    cerr << "InvestorID=" << pOrder->InvestorID << endl;
    ///��Լ����
    cerr << "InstrumentID=" << pOrder->InstrumentID << endl;
    ///��������
    cerr << "OrderRef=" << pOrder->OrderRef << endl;
    ///�û�����
    cerr << "UserID=" << pOrder->UserID << endl;
    ///�����۸�����
    cerr << "OrderPriceType=" << pOrder->OrderPriceType << endl;
    ///��������
    cerr << "Direction=" << pOrder->Direction << endl;
    ///��Ͽ�ƽ��־
    cerr << "CombOffsetFlag=" << pOrder->CombOffsetFlag << endl;
    ///���Ͷ���ױ���־
    cerr << "CombHedgeFlag=" << pOrder->CombHedgeFlag << endl;
    ///�۸�
    cerr << "LimitPrice=" << pOrder->LimitPrice << endl;
    ///����
    cerr << "VolumeTotalOriginal=" << pOrder->VolumeTotalOriginal << endl;
    ///��Ч������
    cerr << "TimeCondition=" << pOrder->TimeCondition << endl;
    ///GTD����
    cerr << "GTDDate=" << pOrder->GTDDate << endl;
    ///�ɽ�������
    cerr << "VolumeCondition=" << pOrder->VolumeCondition << endl;
    ///��С�ɽ���
    cerr << "MinVolume=" << pOrder->MinVolume << endl;
    ///��������
    cerr << "ContingentCondition=" << pOrder->ContingentCondition << endl;
    ///ֹ���
    cerr << "StopPrice=" << pOrder->StopPrice << endl;
    ///ǿƽԭ��
    cerr << "ForceCloseReason=" << pOrder->ForceCloseReason << endl;
    ///�Զ������־
    cerr << "IsAutoSuspend=" << pOrder->IsAutoSuspend << endl;
    ///ҵ��Ԫ
    cerr << "BusinessUnit=" << pOrder->BusinessUnit << endl;
    ///������
    cerr << "RequestID=" << pOrder->RequestID << endl;
    ///���ر������
    cerr << "OrderLocalID=" << pOrder->OrderLocalID << endl;
    ///����������
    cerr << "ExchangeID=" << pOrder->ExchangeID << endl;
    ///��Ա����
    cerr << "ParticipantID=" << pOrder->ParticipantID << endl;
    ///�ͻ�����
    cerr << "ClientID=" << pOrder->ClientID << endl;
    ///��Լ�ڽ������Ĵ���
    cerr << "ExchangeInstID=" << pOrder->ExchangeInstID << endl;
    ///����������Ա����
    cerr << "TraderID=" << pOrder->TraderID << endl;
    ///��װ���
    cerr << "InstallID=" << pOrder->InstallID << endl;
    ///�����ύ״̬
    cerr << "OrderSubmitStatus=" << pOrder->OrderSubmitStatus << endl;
    ///������ʾ���
    cerr << "NotifySequence=" << pOrder->NotifySequence << endl;
    ///������
    cerr << "TradingDay=" << pOrder->TradingDay << endl;
    ///������
    cerr << "SettlementID=" << pOrder->SettlementID << endl;
    ///�������
    cerr << "OrderSysID=" << pOrder->OrderSysID << endl;
    ///������Դ
    cerr << "OrderSource=" << pOrder->OrderSource << endl;
    ///����״̬
    cerr << "OrderStatus=" << pOrder->OrderStatus << endl;
    ///��������
    cerr << "OrderType=" << pOrder->OrderType << endl;
    ///��ɽ�����
    cerr << "VolumeTraded=" << pOrder->VolumeTraded << endl;
    ///ʣ������
    cerr << "VolumeTotal=" << pOrder->VolumeTotal << endl;
    ///��������
    cerr << "InsertDate=" << pOrder->InsertDate << endl;
    ///ί��ʱ��
    cerr << "InsertTime=" << pOrder->InsertTime << endl;
    ///����ʱ��
    cerr << "ActiveTime=" << pOrder->ActiveTime << endl;
    ///����ʱ��
    cerr << "SuspendTime=" << pOrder->SuspendTime << endl;
    ///����޸�ʱ��
    cerr << "UpdateTime=" << pOrder->UpdateTime << endl;
    ///����ʱ��
    cerr << "CancelTime=" << pOrder->CancelTime << endl;
    ///����޸Ľ���������Ա����
    cerr << "ActiveTraderID=" << pOrder->ActiveTraderID << endl;
    ///�����Ա���
    cerr << "ClearingPartID=" << pOrder->ClearingPartID << endl;
    ///���
    cerr << "SequenceNo=" << pOrder->SequenceNo << endl;
    ///ǰ�ñ��
    cerr << "FrontID=" << pOrder->FrontID << endl;
    ///�Ự���
    cerr << "SessionID=" << pOrder->SessionID << endl;
    ///�û��˲�Ʒ��Ϣ
    cerr << "UserProductInfo=" << pOrder->UserProductInfo << endl;
    ///״̬��Ϣ
    cerr << "StatusMsg=" << pOrder->StatusMsg << endl;
    ///�û�ǿ����־
    cerr << "UserForceClose=" << pOrder->UserForceClose << endl;
    ///�����û�����
    cerr << "ActiveUserID=" << pOrder->ActiveUserID << endl;
    ///���͹�˾�������
    cerr << "BrokerOrderSeq=" << pOrder->BrokerOrderSeq << endl;
    ///��ر���
    cerr << "RelativeOrderSysID=" << pOrder->RelativeOrderSysID << endl;
    ///֣�����ɽ�����
    cerr << "ZCETotalTradedVolume=" << pOrder->ZCETotalTradedVolume << endl;
    ///��������־
    cerr << "IsSwapOrder=" << pOrder->IsSwapOrder << endl;
    ///Ӫҵ�����
    cerr << "BranchID=" << pOrder->BranchID << endl;
    // if (IsMyOrder(pOrder)) {
    //     if (IsTradingOrder(pOrder))
    //         ReqOrderAction(pOrder);
    //     else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
    //         cout << "--->>> �����ɹ�" << endl;
    // }
}

///�ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField* pTrade)
{
    cerr << "--->>> "
	 << "OnRtnTrade" << endl;
    ///���͹�˾����
    cerr << "brokerid=" << pTrade->BrokerID << endl;
    ///Ͷ���ߴ���
    cerr << "InvestorID=" << pTrade->InvestorID << endl;
    ///��Լ����
    cerr << "InstrumentID=" << pTrade->InstrumentID << endl;
    ///��������
    cerr << "OrderRef=" << pTrade->OrderRef << endl;
    ///�û�����
    cerr << "UserID=" << pTrade->UserID << endl;
    ///����������
    cerr << "ExchangeID=" << pTrade->ExchangeID << endl;
    ///�ɽ����
    cerr << "TradeID=" << pTrade->TradeID << endl;
    ///��������
    cerr << "Direction=" << pTrade->Direction << endl;
    ///�������
    cerr << "OrderSysID=" << pTrade->OrderSysID << endl;
    ///��Ա����
    cerr << "ParticipantID=" << pTrade->ParticipantID << endl;
    ///�ͻ�����
    cerr << "ClientID=" << pTrade->ClientID << endl;
    ///���׽�ɫ
    cerr << "TradingRole=" << pTrade->TradingRole << endl;
    ///��Լ�ڽ������Ĵ���
    cerr << "ExchangeInstID=" << pTrade->ExchangeInstID << endl;
    ///��ƽ��־
    cerr << "OffsetFlag=" << pTrade->OffsetFlag << endl;
    ///Ͷ���ױ���־
    cerr << "HedgeFlag=" << pTrade->HedgeFlag << endl;
    ///�۸�
    cerr << "Price=" << pTrade->Price << endl;
    ///����
    cerr << "Volume=" << pTrade->Volume << endl;
    ///�ɽ�ʱ��
    cerr << "TradeDate=" << pTrade->TradeDate << endl;
    ///�ɽ�ʱ��
    cerr << "TradeTime=" << pTrade->TradeTime << endl;
    ///�ɽ�����
    cerr << "TradeType=" << pTrade->TradeType << endl;
    ///�ɽ�����Դ
    cerr << "PriceSource=" << pTrade->PriceSource << endl;
    ///����������Ա����
    cerr << "TraderID=" << pTrade->TraderID << endl;
    ///���ر������
    cerr << "OrderLocalID=" << pTrade->OrderLocalID << endl;
    ///�����Ա���
    cerr << "ClearingPartID=" << pTrade->ClearingPartID << endl;
    ///ҵ��Ԫ
    cerr << "BusinessUnit=" << pTrade->BusinessUnit << endl;
    ///���
    cerr << "SequenceNo=" << pTrade->SequenceNo << endl;
    ///������
    cerr << "TradingDay=" << pTrade->TradingDay << endl;
    ///������
    cerr << "SettlementID=" << pTrade->SettlementID << endl;
    ///���͹�˾�������
    cerr << "BrokerOrderSeq=" << pTrade->BrokerOrderSeq << endl;
    ///�ɽ���Դ
    cerr << "TradeSource=" << pTrade->TradeSource << endl;
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
    cerr << "--->>> "
	 << "OnFrontDisconnected" << endl;
    cerr << "--->>> Reason = " << nReason << endl;
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    cerr << "--->>> "
	 << "OnHeartBeatWarning" << endl;
    cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspError" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
    // ���ErrorID != 0, ˵���յ��˴������Ӧ
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
	cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField* pOrder)
{
    return ((pOrder->FrontID == FRONT_ID) && (pOrder->SessionID == SESSION_ID) && (strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField* pOrder)
{
    return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) && (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) && (pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

