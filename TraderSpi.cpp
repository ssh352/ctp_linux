// #include "ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
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
	if (pRspInfo == NULL){
		cout << "pRspInfo is null!" << endl;
		return;
	}
    //cout << "ErrorCode=" << pRspInfo->ErrorID <<  "ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
    printf("RequestID=[%d], bIsLast=[%d]\n", nRequestID, bIsLast);
}

void CTraderSpi::OnFrontConnected()
{
    cout << "--->>> "
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
    cout << "--->>> sent login request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> " << __FUNCTION__ << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    // copy from ctp documents demo
    if (pRspInfo->ErrorID != 0) {
	return;
    }

   // printf("TradingDay=%s,LoginTime=%s,BrokerID=%s,UserId=%s,SystemName=%s,FrontID=%d,SessionID=%d\n", pRspUserLogin->TradingDay, pRspUserLogin->LoginTime, pRspUserLogin->BrokerID, pRspUserLogin->UserID, pRspUserLogin->SystemName, pRspUserLogin->FrontID, pRspUserLogin->SessionID);
   // printf("MaxOrderRef=%s,SHFETime=%s,DCETime=%s,CZCETime=%s,FFEXTime=%s,INETime=%s\n", pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime, pRspUserLogin->DCETime, pRspUserLogin->CZCETime, pRspUserLogin->FFEXTime, pRspUserLogin->INETime);
    if  ( pRspUserLogin== NULL) {
	    cout << "pRspUserLogin is null!" << endl;
	    return;
    }
    // cout << "TradingDay=" << pRspUserLogin->TradingDay << endl;
    // cout << "TBrokerID=" << pRspUserLogin->BrokerID << endl;
    // cout << "TUserID=" << pRspUserLogin->UserID << endl;
    // cout << "TPassword=" << pRspUserLogin->Password << endl;
    // cout << "TUserProductInfo=" << pRspUserLogin->UserProductInfo << endl;
    // cout << "TInterfaceProductInfo=" << pRspUserLogin->InterfaceProductInfo << endl;
    // cout << "TProtocolInfo=" << pRspUserLogin->ProtocolInfo << endl;
    // cout << "TMacAddress=" << pRspUserLogin->MacAddress << endl;
    // cout << "TOneTimePassword=" << pRspUserLogin->OneTimePassword << endl;
    // cout << "TClientIPAddress=" << pRspUserLogin->ClientIPAddress << endl;
    //
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	// ����Ự����
	FRONT_ID = pRspUserLogin->FrontID;
	SESSION_ID = pRspUserLogin->SessionID;
	int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
	iNextOrderRef++;
	sprintf(ORDER_REF, "%d", iNextOrderRef);
	///��ȡ��ǰ������
	cout << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
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
    cout << "--->>> investor confirm settlement: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspSettlementInfoConfirm" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    if  ( pSettlementInfoConfirm== NULL) {
	    cout << "pSettlementInfoConfirm is null!" << endl;
	    return;
    }
    cout << "BrokerID=" << pSettlementInfoConfirm->BrokerID << endl;
    ///Ͷ���ߴ���
    cout << "InvertorID=" << pSettlementInfoConfirm->InvestorID << endl;
    ///ȷ������
    cout << "confirmdate=" << pSettlementInfoConfirm->ConfirmDate << endl;
    ///ȷ��ʱ��
    cout << "confirmtime=" << pSettlementInfoConfirm->ConfirmTime << endl;
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
    cout << "--->>> send ReqQryInstrument request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> " << "OnRspQryInstrument" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///��Լ����
    if  ( pInstrument== NULL) {
	    cout << "pInstrument is null!" << endl;
	    return;
    }
    cout << "InstrumentID=" << pInstrument->InstrumentID << endl;
    ///����������
    cout << "ExchangeID=" << pInstrument->ExchangeID << endl;
    ///��Լ����
    cout << "InstrumentName=" << pInstrument->InstrumentName << endl;
    ///��Լ�ڽ������Ĵ���
    cout << "ExchangeInstID=" << pInstrument->ExchangeInstID << endl;
    ///��Ʒ����
    cout << "ProductID=" << pInstrument->ProductID << endl;
    ///��Ʒ����
    cout << "ProductClass=" << pInstrument->ProductClass << endl;
    ///�������
    cout << "DeliveryYear=" << pInstrument->DeliveryYear << endl;
    ///������
    cout << "DeliveryMonth=" << pInstrument->DeliveryMonth << endl;
    ///�м۵�����µ���
    cout << "MaxMarketOrderVolume=" << pInstrument->MaxMarketOrderVolume << endl;
    ///�м۵���С�µ���
    cout << "MinMarketOrderVolume=" << pInstrument->MinMarketOrderVolume << endl;
    ///�޼۵�����µ���
    cout << "MaxLimitOrderVolume=" << pInstrument->MaxLimitOrderVolume << endl;
    ///�޼۵���С�µ���
    cout << "MinLimitOrderVolume=" << pInstrument->MinLimitOrderVolume << endl;
    ///��Լ��������
    cout << "VolumeMultiple=" << pInstrument->VolumeMultiple << endl;
    ///��С�䶯��λ
    cout << "PriceTick=" << pInstrument->PriceTick << endl;
    ///������
    cout << "CreateDate=" << pInstrument->CreateDate << endl;
    ///������
    cout << "OpenDate=" << pInstrument->OpenDate << endl;
    ///������
    cout << "ExpireDate=" << pInstrument->ExpireDate << endl;
    ///��ʼ������
    cout << "StartDelivDate=" << pInstrument->StartDelivDate << endl;
    ///����������
    cout << "EndDelivDate=" << pInstrument->EndDelivDate << endl;
    ///��Լ��������״̬
    cout << "InstLifePhase=" << pInstrument->InstLifePhase << endl;
    ///��ǰ�Ƿ���
    cout << "IsTrading=" << pInstrument->IsTrading << endl;
    ///�ֲ�����
    cout << "PositionType=" << pInstrument->PositionType << endl;
    ///�ֲ���������
    cout << "PositionDateType=" << pInstrument->PositionDateType << endl;
    ///��ͷ��֤����
    cout << "LongMarginRatio=" << pInstrument->LongMarginRatio << endl;
    ///��ͷ��֤����
    cout << "ShortMarginRatio=" << pInstrument->ShortMarginRatio << endl;
    ///�Ƿ�ʹ�ô��߱�֤���㷨
    cout << "MaxMarginSideAlgorithm=" << pInstrument->MaxMarginSideAlgorithm << endl;
    ///������Ʒ����
    cout << "UnderlyingInstrID=" << pInstrument->UnderlyingInstrID << endl;
    ///ִ�м�
    cout << "StrikePrice=" << pInstrument->StrikePrice << endl;
    ///��Ȩ����
    cout << "OptionsType=" << pInstrument->OptionsType << endl;
    ///��Լ������Ʒ����
    cout << "UnderlyingMultiple=" << pInstrument->UnderlyingMultiple << endl;
    ///�������
    cout << "CombinationType=" << pInstrument->CombinationType << endl;
    ///��С���µ���λ
    cout << "MinBuyVolume=" << pInstrument->MinBuyVolume << endl;
    ///��С���µ���λ
    cout << "MinSellVolume=" << pInstrument->MinSellVolume << endl;
    ///��Լ��ʶ��
    cout << "InstrumentCode=" << pInstrument->InstrumentCode << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	// �����ѯ��Լ
	ReqQryTradingAccount();
    }
}

void CTraderSpi::ReqQryTradingAccount()
{
	sleep(1);
    CThostFtdcQryTradingAccountField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.InvestorID, "067938");
    int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
    cout << "--->>> sent ReqQryTradingAccount request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspQryTradingAccount" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    if  ( pTradingAccount== NULL) {
	    cout << "pTradingAccount is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pTradingAccount->BrokerID << endl;
    ///Ͷ�����ʺ�
    cout << "AccountID=" << pTradingAccount->AccountID << endl;
    ///�ϴ���Ѻ���
    cout << "PreMortgage=" << pTradingAccount->PreMortgage << endl;
    ///�ϴ����ö��
    cout << "PreCredit=" << pTradingAccount->PreCredit << endl;
    ///�ϴδ���
    cout << "PreDeposit=" << pTradingAccount->PreDeposit << endl;
    ///�ϴν���׼����
    cout << "PreBalance=" << pTradingAccount->PreBalance << endl;
    ///�ϴ�ռ�õı�֤��
    cout << "PreMargin=" << pTradingAccount->PreMargin << endl;
    ///��Ϣ����
    cout << "InterestBase=" << pTradingAccount->InterestBase << endl;
    ///��Ϣ����
    cout << "Interest=" << pTradingAccount->Interest << endl;
    ///�����
    cout << "Deposit=" << pTradingAccount->Deposit << endl;
    ///������
    cout << "Withdraw=" << pTradingAccount->Withdraw << endl;
    ///����ı�֤��
    cout << "FrozenMargin=" << pTradingAccount->FrozenMargin << endl;
    ///������ʽ�
    cout << "FrozenCash=" << pTradingAccount->FrozenCash << endl;
    ///�����������
    cout << "FrozenCommission=" << pTradingAccount->FrozenCommission << endl;
    ///��ǰ��֤���ܶ�
    cout << "CurrMargin=" << pTradingAccount->CurrMargin << endl;
    ///�ʽ���
    cout << "CashIn=" << pTradingAccount->CashIn << endl;
    ///������
    cout << "Commission=" << pTradingAccount->Commission << endl;
    ///ƽ��ӯ��
    cout << "CloseProfit=" << pTradingAccount->CloseProfit << endl;
    ///�ֲ�ӯ��
    cout << "PositionProfit=" << pTradingAccount->PositionProfit << endl;
    ///�ڻ�����׼����
    cout << "Balance=" << pTradingAccount->Balance << endl;
    ///�����ʽ�
    cout << "Available=" << pTradingAccount->Available << endl;
    ///��ȡ�ʽ�
    cout << "WithdrawQuota=" << pTradingAccount->WithdrawQuota << endl;
    ///����׼����
    cout << "Reserve=" << pTradingAccount->Reserve << endl;
    ///������
    cout << "TradingDay=" << pTradingAccount->TradingDay << endl;
    ///������
    cout << "SettlementID=" << pTradingAccount->SettlementID << endl;
    ///���ö��
    cout << "Credit=" << pTradingAccount->Credit << endl;
    ///��Ѻ���
    cout << "Mortgage=" << pTradingAccount->Mortgage << endl;
    ///��������֤��
    cout << "ExchangeMargin=" << pTradingAccount->ExchangeMargin << endl;
    ///Ͷ���߽��֤��
    cout << "DeliveryMargin=" << pTradingAccount->DeliveryMargin << endl;
    ///���������֤��
    cout << "ExchangeDeliveryMargin=" << pTradingAccount->ExchangeDeliveryMargin << endl;
    ///�����ڻ�����׼����
    cout << "ReserveBalance=" << pTradingAccount->ReserveBalance << endl;
    ///���ִ���
    cout << "CurrencyID=" << pTradingAccount->CurrencyID << endl;
    ///�ϴλ���������
    cout << "PreFundMortgageIn=" << pTradingAccount->PreFundMortgageIn << endl;
    ///�ϴλ����ʳ����
    cout << "PreFundMortgageOut=" << pTradingAccount->PreFundMortgageOut << endl;
    ///����������
    cout << "FundMortgageIn=" << pTradingAccount->FundMortgageIn << endl;
    ///�����ʳ����
    cout << "FundMortgageOut=" << pTradingAccount->FundMortgageOut << endl;
    ///������Ѻ���
    cout << "FundMortgageAvailable=" << pTradingAccount->FundMortgageAvailable << endl;
    ///����Ѻ���ҽ��
    cout << "MortgageableFund=" << pTradingAccount->MortgageableFund << endl;
    ///�����Ʒռ�ñ�֤��
    cout << "SpecProductMargin=" << pTradingAccount->SpecProductMargin << endl;
    ///�����Ʒ���ᱣ֤��
    cout << "SpecProductFrozenMargin=" << pTradingAccount->SpecProductFrozenMargin << endl;
    ///�����Ʒ������
    cout << "SpecProductCommission=" << pTradingAccount->SpecProductCommission << endl;
    ///�����Ʒ����������
    cout << "SpecProductFrozenCommission=" << pTradingAccount->SpecProductFrozenCommission << endl;
    ///�����Ʒ�ֲ�ӯ��
    cout << "SpecProductPositionProfit=" << pTradingAccount->SpecProductPositionProfit << endl;
    ///�����Ʒƽ��ӯ��
    cout << "SpecProductCloseProfit=" << pTradingAccount->SpecProductCloseProfit << endl;
    ///���ݳֲ�ӯ���㷨����������Ʒ�ֲ�ӯ��
    cout << "SpecProductPositionProfitByAlg=" << pTradingAccount->SpecProductPositionProfitByAlg << endl;
    ///�����Ʒ��������֤��
    cout << "SpecProductExchangeMargin=" << pTradingAccount->SpecProductExchangeMargin << endl;
    ///ҵ������
    cout << "BizType=" << pTradingAccount->BizType << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///�����ѯͶ���ֲ߳�
	ReqQryInvestorPosition();
    }
}

void CTraderSpi::ReqQryInvestorPosition()
{
	sleep(1);
    CThostFtdcQryInvestorPositionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
    cout << "--->>> sent ReqQryInvestorPosition request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspQryInvestorPosition" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///��Լ����
    if  (pInvestorPosition == NULL) {
	    cout << "pInvestorPosition  is null!" << endl;
	    return;
    }
    cout << "InstrumentID=" << pInvestorPosition->InstrumentID << endl;
    ///���͹�˾����
    cout << "brokerID=" << pInvestorPosition->BrokerID << endl;
    /////Ͷ���ߴ���
    cout << "InvestorID=" << pInvestorPosition->InvestorID << endl;
    /////�ֲֶ�շ���
    cout << "PosiDirection=" << pInvestorPosition->PosiDirection << endl;
    /////Ͷ���ױ���־
    cout << "HedgeFlag=" << pInvestorPosition->HedgeFlag << endl;
    /////�ֲ�����
    cout << "PositionDate=" << pInvestorPosition->PositionDate << endl;
    /////���ճֲ�
    cout << "YdPosition=" << pInvestorPosition->YdPosition << endl;
    /////���ճֲ�
    cout << "Position=" << pInvestorPosition->Position << endl;
    /////��ͷ����
    cout << "LongFrozen=" << pInvestorPosition->LongFrozen << endl;
    /////��ͷ����
    cout << "ShortFrozen=" << pInvestorPosition->ShortFrozen << endl;
    /////���ֶ�����
    cout << "LongFrozenAmount=" << pInvestorPosition->LongFrozenAmount << endl;
    /////���ֶ�����
    cout << "ShortFrozenAmount=" << pInvestorPosition->ShortFrozenAmount << endl;
    /////������
    cout << "OpenVolume=" << pInvestorPosition->OpenVolume << endl;
    /////ƽ����
    cout << "CloseVolume=" << pInvestorPosition->CloseVolume << endl;
    /////���ֽ��
    cout << "OpenAmount=" << pInvestorPosition->OpenAmount << endl;
    /////ƽ�ֽ��
    cout << "CloseAmount=" << pInvestorPosition->CloseAmount << endl;
    /////�ֲֳɱ�
    cout << "PositionCost=" << pInvestorPosition->PositionCost << endl;
    /////�ϴ�ռ�õı�֤��
    cout << "PreMargin=" << pInvestorPosition->PreMargin << endl;
    /////ռ�õı�֤��
    cout << "UseMargin=" << pInvestorPosition->UseMargin << endl;
    /////����ı�֤��
    cout << "FrozenMargin=" << pInvestorPosition->FrozenMargin << endl;
    /////������ʽ�
    cout << "FrozenCash=" << pInvestorPosition->FrozenCash << endl;
    /////�����������
    cout << "FrozenCommission=" << pInvestorPosition->FrozenCommission << endl;
    /////�ʽ���
    cout << "CashIn=" << pInvestorPosition->CashIn << endl;
    /////������
    cout << "Commission=" << pInvestorPosition->Commission << endl;
    /////ƽ��ӯ��
    cout << "CloseProfit=" << pInvestorPosition->CloseProfit << endl;
    /////�ֲ�ӯ��
    cout << "PositionProfit=" << pInvestorPosition->PositionProfit << endl;
    /////�ϴν����
    cout << "PreSettlementPrice=" << pInvestorPosition->PreSettlementPrice << endl;
    /////���ν����
    cout << "SettlementPrice=" << pInvestorPosition->SettlementPrice << endl;
    /////������
    cout << "TradingDay=" << pInvestorPosition->TradingDay << endl;
    /////������
    cout << "SettlementID=" << pInvestorPosition->SettlementID << endl;
    /////���ֳɱ�
    cout << "OpenCost=" << pInvestorPosition->OpenCost << endl;
    /////��������֤��
    cout << "ExchangeMargin=" << pInvestorPosition->ExchangeMargin << endl;
    /////��ϳɽ��γɵĳֲ�
    cout << "CombPosition=" << pInvestorPosition->CombPosition << endl;
    /////��϶�ͷ����
    cout << "CombLongFrozen=" << pInvestorPosition->CombLongFrozen << endl;
    /////��Ͽ�ͷ����
    cout << "CombShortFrozen=" << pInvestorPosition->CombShortFrozen << endl;
    /////���ն���ƽ��ӯ��
    cout << "CloseProfitByDate=" << pInvestorPosition->CloseProfitByDate << endl;
    /////��ʶԳ�ƽ��ӯ��
    cout << "CloseProfitByTrade=" << pInvestorPosition->CloseProfitByTrade << endl;
    /////���ճֲ�
    cout << "TodayPosition=" << pInvestorPosition->TodayPosition << endl;
    /////��֤����
    cout << "MarginRateByMoney=" << pInvestorPosition->MarginRateByMoney << endl;
    /////��֤����(������)
    cout << "MarginRateByVolume=" << pInvestorPosition->MarginRateByVolume << endl;
    /////ִ�ж���
    cout << "StrikeFrozen=" << pInvestorPosition->StrikeFrozen << endl;
    /////ִ�ж�����
    cout << "StrikeFrozenAmount=" << pInvestorPosition->StrikeFrozenAmount << endl;
    /////����ִ�ж���
    cout << "AbandonFrozen=" << pInvestorPosition->AbandonFrozen << endl;
    /////����������
    cout << "ExchangeID=" << pInvestorPosition->ExchangeID << endl;
    /////ִ�ж�������
    cout << "YdStrikeFrozen=" << pInvestorPosition->YdStrikeFrozen << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///����¼������
	//ReqOrderInsert();
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
    cout << "--->>> sent orderInsert request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspOrderInsert" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    if  (pInputOrder== NULL) {
	    cout << "pInputOrder is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pInputOrder->BrokerID << endl;
    ///Ͷ���ߴ���
    cout << "InvestorID=" << pInputOrder->InvestorID << endl;
    ///��Լ����
    cout << "InstrumentID=" << pInputOrder->InstrumentID << endl;
    ///��������
    cout << "OrderRef=" << pInputOrder->OrderRef << endl;
    ///�û�����
    cout << "UserID=" << pInputOrder->UserID << endl;
    ///�����۸�����
    cout << "OrderPriceType=" << pInputOrder->OrderPriceType << endl;
    ///��������
    cout << "Direction=" << pInputOrder->Direction << endl;
    ///��Ͽ�ƽ��־
    cout << "CombOffsetFlag=" << pInputOrder->CombOffsetFlag << endl;
    ///���Ͷ���ױ���־
    cout << "CombHedgeFlag=" << pInputOrder->CombHedgeFlag << endl;
    ///�۸�
    cout << "LimitPrice=" << pInputOrder->LimitPrice << endl;
    ///����
    cout << "VolumeTotalOriginal=" << pInputOrder->VolumeTotalOriginal << endl;
    ///��Ч������
    cout << "TimeCondition=" << pInputOrder->TimeCondition << endl;
    ///GTD����
    cout << "GTDDate=" << pInputOrder->GTDDate << endl;
    ///�ɽ�������
    cout << "VolumeCondition=" << pInputOrder->VolumeCondition << endl;
    ///��С�ɽ���
    cout << "MinVolume=" << pInputOrder->MinVolume << endl;
    ///��������
    cout << "ContingentCondition=" << pInputOrder->ContingentCondition << endl;
    ///ֹ���
    cout << "StopPrice=" << pInputOrder->StopPrice << endl;
    ///ǿƽԭ��
    cout << "ForceCloseReason=" << pInputOrder->ForceCloseReason << endl;
    ///�Զ������־
    cout << "IsAutoSuspend=" << pInputOrder->IsAutoSuspend << endl;
    ///ҵ��Ԫ
    cout << "BusinessUnit=" << pInputOrder->BusinessUnit << endl;
    ///������
    cout << "RequestID=" << pInputOrder->RequestID << endl;
    ///�û�ǿ����־
    cout << "UserForceClose=" << pInputOrder->UserForceClose << endl;
    ///��������־
    cout << "IsSwapOrder=" << pInputOrder->IsSwapOrder << endl;
    ///����������
    cout << "ExchangeID=" << pInputOrder->ExchangeID << endl;
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
    cout << "--->>> sent orderaction request: " << ((iResult == 0) ? "success" : "failed") << endl;
    ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspOrderAction" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///���͹�˾����
    if  ( pInputOrderAction== NULL) {
	    cout << "pInputOrderAction is null!" << endl;
	    return;
    }
    cout << "brokerid" << pInputOrderAction->BrokerID << endl;
    ///Ͷ���ߴ���
    cout << "InvestorID" << pInputOrderAction->InvestorID << endl;
    ///������������
    cout << "OrderActionRef" << pInputOrderAction->OrderActionRef << endl;
    ///��������
    cout << "OrderRef" << pInputOrderAction->OrderRef << endl;
    ///������
    cout << "RequestID" << pInputOrderAction->RequestID << endl;
    ///ǰ�ñ��
    cout << "FrontID" << pInputOrderAction->FrontID << endl;
    ///�Ự���
    cout << "SessionID" << pInputOrderAction->SessionID << endl;
    ///����������
    cout << "brokerid" << pInputOrderAction->ExchangeID << endl;
    ///�������
    cout << "brokerid" << pInputOrderAction->OrderSysID << endl;
    ///������־
    cout << "brokerid" << pInputOrderAction->ActionFlag << endl;
    ///�۸�
    cout << "LimitPrice " << pInputOrderAction->LimitPrice << endl;
    ///�����仯
    cout << "VolumeChange " << pInputOrderAction->VolumeChange << endl;
    ///�û�����
    cout << "UserID " << pInputOrderAction->UserID << endl;
    ///��Լ����
    cout << "InstrumentID " << pInputOrderAction->InstrumentID << endl;
}

///����֪ͨ
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
    cout << "--->>> "
	 << "OnRtnOrder" << endl;
    ///���͹�˾����
    if  ( pOrder== NULL) {
	    cout << "pOrder is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pOrder->BrokerID << endl;
    ///Ͷ���ߴ���
    cout << "InvestorID=" << pOrder->InvestorID << endl;
    ///��Լ����
    cout << "InstrumentID=" << pOrder->InstrumentID << endl;
    ///��������
    cout << "OrderRef=" << pOrder->OrderRef << endl;
    ///�û�����
    cout << "UserID=" << pOrder->UserID << endl;
    ///�����۸�����
    cout << "OrderPriceType=" << pOrder->OrderPriceType << endl;
    ///��������
    cout << "Direction=" << pOrder->Direction << endl;
    ///��Ͽ�ƽ��־
    cout << "CombOffsetFlag=" << pOrder->CombOffsetFlag << endl;
    ///���Ͷ���ױ���־
    cout << "CombHedgeFlag=" << pOrder->CombHedgeFlag << endl;
    ///�۸�
    cout << "LimitPrice=" << pOrder->LimitPrice << endl;
    ///����
    cout << "VolumeTotalOriginal=" << pOrder->VolumeTotalOriginal << endl;
    ///��Ч������
    cout << "TimeCondition=" << pOrder->TimeCondition << endl;
    ///GTD����
    cout << "GTDDate=" << pOrder->GTDDate << endl;
    ///�ɽ�������
    cout << "VolumeCondition=" << pOrder->VolumeCondition << endl;
    ///��С�ɽ���
    cout << "MinVolume=" << pOrder->MinVolume << endl;
    ///��������
    cout << "ContingentCondition=" << pOrder->ContingentCondition << endl;
    ///ֹ���
    cout << "StopPrice=" << pOrder->StopPrice << endl;
    ///ǿƽԭ��
    cout << "ForceCloseReason=" << pOrder->ForceCloseReason << endl;
    ///�Զ������־
    cout << "IsAutoSuspend=" << pOrder->IsAutoSuspend << endl;
    ///ҵ��Ԫ
    cout << "BusinessUnit=" << pOrder->BusinessUnit << endl;
    ///������
    cout << "RequestID=" << pOrder->RequestID << endl;
    ///���ر������
    cout << "OrderLocalID=" << pOrder->OrderLocalID << endl;
    ///����������
    cout << "ExchangeID=" << pOrder->ExchangeID << endl;
    ///��Ա����
    cout << "ParticipantID=" << pOrder->ParticipantID << endl;
    ///�ͻ�����
    cout << "ClientID=" << pOrder->ClientID << endl;
    ///��Լ�ڽ������Ĵ���
    cout << "ExchangeInstID=" << pOrder->ExchangeInstID << endl;
    ///����������Ա����
    cout << "TraderID=" << pOrder->TraderID << endl;
    ///��װ���
    cout << "InstallID=" << pOrder->InstallID << endl;
    ///�����ύ״̬
    cout << "OrderSubmitStatus=" << pOrder->OrderSubmitStatus << endl;
    ///������ʾ���
    cout << "NotifySequence=" << pOrder->NotifySequence << endl;
    ///������
    cout << "TradingDay=" << pOrder->TradingDay << endl;
    ///������
    cout << "SettlementID=" << pOrder->SettlementID << endl;
    ///�������
    cout << "OrderSysID=" << pOrder->OrderSysID << endl;
    ///������Դ
    cout << "OrderSource=" << pOrder->OrderSource << endl;
    ///����״̬
    cout << "OrderStatus=" << pOrder->OrderStatus << endl;
    ///��������
    cout << "OrderType=" << pOrder->OrderType << endl;
    ///��ɽ�����
    cout << "VolumeTraded=" << pOrder->VolumeTraded << endl;
    ///ʣ������
    cout << "VolumeTotal=" << pOrder->VolumeTotal << endl;
    ///��������
    cout << "InsertDate=" << pOrder->InsertDate << endl;
    ///ί��ʱ��
    cout << "InsertTime=" << pOrder->InsertTime << endl;
    ///����ʱ��
    cout << "ActiveTime=" << pOrder->ActiveTime << endl;
    ///����ʱ��
    cout << "SuspendTime=" << pOrder->SuspendTime << endl;
    ///����޸�ʱ��
    cout << "UpdateTime=" << pOrder->UpdateTime << endl;
    ///����ʱ��
    cout << "CancelTime=" << pOrder->CancelTime << endl;
    ///����޸Ľ���������Ա����
    cout << "ActiveTraderID=" << pOrder->ActiveTraderID << endl;
    ///�����Ա���
    cout << "ClearingPartID=" << pOrder->ClearingPartID << endl;
    ///���
    cout << "SequenceNo=" << pOrder->SequenceNo << endl;
    ///ǰ�ñ��
    cout << "FrontID=" << pOrder->FrontID << endl;
    ///�Ự���
    cout << "SessionID=" << pOrder->SessionID << endl;
    ///�û��˲�Ʒ��Ϣ
    cout << "UserProductInfo=" << pOrder->UserProductInfo << endl;
    ///״̬��Ϣ
    cout << "StatusMsg=" << pOrder->StatusMsg << endl;
    ///�û�ǿ����־
    cout << "UserForceClose=" << pOrder->UserForceClose << endl;
    ///�����û�����
    cout << "ActiveUserID=" << pOrder->ActiveUserID << endl;
    ///���͹�˾�������
    cout << "BrokerOrderSeq=" << pOrder->BrokerOrderSeq << endl;
    ///��ر���
    cout << "RelativeOrderSysID=" << pOrder->RelativeOrderSysID << endl;
    ///֣�����ɽ�����
    cout << "ZCETotalTradedVolume=" << pOrder->ZCETotalTradedVolume << endl;
    ///��������־
    cout << "IsSwapOrder=" << pOrder->IsSwapOrder << endl;
    ///Ӫҵ�����
    cout << "BranchID=" << pOrder->BranchID << endl;
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
    cout << "--->>> "
	 << "OnRtnTrade" << endl;
    ///���͹�˾����
    if  ( pTrade== NULL) {
	    cout << "pTrade is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pTrade->BrokerID << endl;
    ///Ͷ���ߴ���
    cout << "InvestorID=" << pTrade->InvestorID << endl;
    ///��Լ����
    cout << "InstrumentID=" << pTrade->InstrumentID << endl;
    ///��������
    cout << "OrderRef=" << pTrade->OrderRef << endl;
    ///�û�����
    cout << "UserID=" << pTrade->UserID << endl;
    ///����������
    cout << "ExchangeID=" << pTrade->ExchangeID << endl;
    ///�ɽ����
    cout << "TradeID=" << pTrade->TradeID << endl;
    ///��������
    cout << "Direction=" << pTrade->Direction << endl;
    ///�������
    cout << "OrderSysID=" << pTrade->OrderSysID << endl;
    ///��Ա����
    cout << "ParticipantID=" << pTrade->ParticipantID << endl;
    ///�ͻ�����
    cout << "ClientID=" << pTrade->ClientID << endl;
    ///���׽�ɫ
    cout << "TradingRole=" << pTrade->TradingRole << endl;
    ///��Լ�ڽ������Ĵ���
    cout << "ExchangeInstID=" << pTrade->ExchangeInstID << endl;
    ///��ƽ��־
    cout << "OffsetFlag=" << pTrade->OffsetFlag << endl;
    ///Ͷ���ױ���־
    cout << "HedgeFlag=" << pTrade->HedgeFlag << endl;
    ///�۸�
    cout << "Price=" << pTrade->Price << endl;
    ///����
    cout << "Volume=" << pTrade->Volume << endl;
    ///�ɽ�ʱ��
    cout << "TradeDate=" << pTrade->TradeDate << endl;
    ///�ɽ�ʱ��
    cout << "TradeTime=" << pTrade->TradeTime << endl;
    ///�ɽ�����
    cout << "TradeType=" << pTrade->TradeType << endl;
    ///�ɽ�����Դ
    cout << "PriceSource=" << pTrade->PriceSource << endl;
    ///����������Ա����
    cout << "TraderID=" << pTrade->TraderID << endl;
    ///���ر������
    cout << "OrderLocalID=" << pTrade->OrderLocalID << endl;
    ///�����Ա���
    cout << "ClearingPartID=" << pTrade->ClearingPartID << endl;
    ///ҵ��Ԫ
    cout << "BusinessUnit=" << pTrade->BusinessUnit << endl;
    ///���
    cout << "SequenceNo=" << pTrade->SequenceNo << endl;
    ///������
    cout << "TradingDay=" << pTrade->TradingDay << endl;
    ///������
    cout << "SettlementID=" << pTrade->SettlementID << endl;
    ///���͹�˾�������
    cout << "BrokerOrderSeq=" << pTrade->BrokerOrderSeq << endl;
    ///�ɽ���Դ
    cout << "TradeSource=" << pTrade->TradeSource << endl;
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
    cout << "--->>> "
	 << "OnFrontDisconnected" << endl;
    cout << "--->>> Reason = " << nReason << endl;
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    cout << "--->>> "
	 << "OnHeartBeatWarning" << endl;
    cout << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspError" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
    // ���ErrorID != 0, ˵���յ��˴������Ӧ
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
	cout << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
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

