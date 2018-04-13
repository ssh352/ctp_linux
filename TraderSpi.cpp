// #include "ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
using namespace std;


#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcTraderApi* pUserApi;

// 配置参数
TThostFtdcBrokerIDType BROKER_ID = "9999";	     // 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "067938";       // 投资者代码
TThostFtdcPasswordType PASSWORD = "6432281";	   // 用户密码
TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu1810";   // 合约代码
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell; // 买卖方向
TThostFtdcPriceType LIMIT_PRICE = 51800;	       // 价格

// 请求编号
extern int iRequestID;

// 会话参数
TThostFtdcFrontIDType FRONT_ID;     //前置编号
TThostFtdcSessionIDType SESSION_ID; //会话编号
TThostFtdcOrderRefType ORDER_REF;   //报单引用

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
	// 保存会话参数
	FRONT_ID = pRspUserLogin->FrontID;
	SESSION_ID = pRspUserLogin->SessionID;
	int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
	iNextOrderRef++;
	sprintf(ORDER_REF, "%d", iNextOrderRef);
	///获取当前交易日
	cout << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
	///投资者结算结果确认
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
    ///经纪公司代码
    if  ( pSettlementInfoConfirm== NULL) {
	    cout << "pSettlementInfoConfirm is null!" << endl;
	    return;
    }
    cout << "BrokerID=" << pSettlementInfoConfirm->BrokerID << endl;
    ///投资者代码
    cout << "InvertorID=" << pSettlementInfoConfirm->InvestorID << endl;
    ///确认日期
    cout << "confirmdate=" << pSettlementInfoConfirm->ConfirmDate << endl;
    ///确认时间
    cout << "confirmtime=" << pSettlementInfoConfirm->ConfirmTime << endl;
    if (pRspInfo->ErrorID == 0) {
	///请求查询合约
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
    ///合约代码
    if  ( pInstrument== NULL) {
	    cout << "pInstrument is null!" << endl;
	    return;
    }
    cout << "InstrumentID=" << pInstrument->InstrumentID << endl;
    ///交易所代码
    cout << "ExchangeID=" << pInstrument->ExchangeID << endl;
    ///合约名称
    cout << "InstrumentName=" << pInstrument->InstrumentName << endl;
    ///合约在交易所的代码
    cout << "ExchangeInstID=" << pInstrument->ExchangeInstID << endl;
    ///产品代码
    cout << "ProductID=" << pInstrument->ProductID << endl;
    ///产品类型
    cout << "ProductClass=" << pInstrument->ProductClass << endl;
    ///交割年份
    cout << "DeliveryYear=" << pInstrument->DeliveryYear << endl;
    ///交割月
    cout << "DeliveryMonth=" << pInstrument->DeliveryMonth << endl;
    ///市价单最大下单量
    cout << "MaxMarketOrderVolume=" << pInstrument->MaxMarketOrderVolume << endl;
    ///市价单最小下单量
    cout << "MinMarketOrderVolume=" << pInstrument->MinMarketOrderVolume << endl;
    ///限价单最大下单量
    cout << "MaxLimitOrderVolume=" << pInstrument->MaxLimitOrderVolume << endl;
    ///限价单最小下单量
    cout << "MinLimitOrderVolume=" << pInstrument->MinLimitOrderVolume << endl;
    ///合约数量乘数
    cout << "VolumeMultiple=" << pInstrument->VolumeMultiple << endl;
    ///最小变动价位
    cout << "PriceTick=" << pInstrument->PriceTick << endl;
    ///创建日
    cout << "CreateDate=" << pInstrument->CreateDate << endl;
    ///上市日
    cout << "OpenDate=" << pInstrument->OpenDate << endl;
    ///到期日
    cout << "ExpireDate=" << pInstrument->ExpireDate << endl;
    ///开始交割日
    cout << "StartDelivDate=" << pInstrument->StartDelivDate << endl;
    ///结束交割日
    cout << "EndDelivDate=" << pInstrument->EndDelivDate << endl;
    ///合约生命周期状态
    cout << "InstLifePhase=" << pInstrument->InstLifePhase << endl;
    ///当前是否交易
    cout << "IsTrading=" << pInstrument->IsTrading << endl;
    ///持仓类型
    cout << "PositionType=" << pInstrument->PositionType << endl;
    ///持仓日期类型
    cout << "PositionDateType=" << pInstrument->PositionDateType << endl;
    ///多头保证金率
    cout << "LongMarginRatio=" << pInstrument->LongMarginRatio << endl;
    ///空头保证金率
    cout << "ShortMarginRatio=" << pInstrument->ShortMarginRatio << endl;
    ///是否使用大额单边保证金算法
    cout << "MaxMarginSideAlgorithm=" << pInstrument->MaxMarginSideAlgorithm << endl;
    ///基础商品代码
    cout << "UnderlyingInstrID=" << pInstrument->UnderlyingInstrID << endl;
    ///执行价
    cout << "StrikePrice=" << pInstrument->StrikePrice << endl;
    ///期权类型
    cout << "OptionsType=" << pInstrument->OptionsType << endl;
    ///合约基础商品乘数
    cout << "UnderlyingMultiple=" << pInstrument->UnderlyingMultiple << endl;
    ///组合类型
    cout << "CombinationType=" << pInstrument->CombinationType << endl;
    ///最小买下单单位
    cout << "MinBuyVolume=" << pInstrument->MinBuyVolume << endl;
    ///最小卖下单单位
    cout << "MinSellVolume=" << pInstrument->MinSellVolume << endl;
    ///合约标识码
    cout << "InstrumentCode=" << pInstrument->InstrumentCode << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	// 请求查询合约
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
    ///经纪公司代码
    if  ( pTradingAccount== NULL) {
	    cout << "pTradingAccount is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pTradingAccount->BrokerID << endl;
    ///投资者帐号
    cout << "AccountID=" << pTradingAccount->AccountID << endl;
    ///上次质押金额
    cout << "PreMortgage=" << pTradingAccount->PreMortgage << endl;
    ///上次信用额度
    cout << "PreCredit=" << pTradingAccount->PreCredit << endl;
    ///上次存款额
    cout << "PreDeposit=" << pTradingAccount->PreDeposit << endl;
    ///上次结算准备金
    cout << "PreBalance=" << pTradingAccount->PreBalance << endl;
    ///上次占用的保证金
    cout << "PreMargin=" << pTradingAccount->PreMargin << endl;
    ///利息基数
    cout << "InterestBase=" << pTradingAccount->InterestBase << endl;
    ///利息收入
    cout << "Interest=" << pTradingAccount->Interest << endl;
    ///入金金额
    cout << "Deposit=" << pTradingAccount->Deposit << endl;
    ///出金金额
    cout << "Withdraw=" << pTradingAccount->Withdraw << endl;
    ///冻结的保证金
    cout << "FrozenMargin=" << pTradingAccount->FrozenMargin << endl;
    ///冻结的资金
    cout << "FrozenCash=" << pTradingAccount->FrozenCash << endl;
    ///冻结的手续费
    cout << "FrozenCommission=" << pTradingAccount->FrozenCommission << endl;
    ///当前保证金总额
    cout << "CurrMargin=" << pTradingAccount->CurrMargin << endl;
    ///资金差额
    cout << "CashIn=" << pTradingAccount->CashIn << endl;
    ///手续费
    cout << "Commission=" << pTradingAccount->Commission << endl;
    ///平仓盈亏
    cout << "CloseProfit=" << pTradingAccount->CloseProfit << endl;
    ///持仓盈亏
    cout << "PositionProfit=" << pTradingAccount->PositionProfit << endl;
    ///期货结算准备金
    cout << "Balance=" << pTradingAccount->Balance << endl;
    ///可用资金
    cout << "Available=" << pTradingAccount->Available << endl;
    ///可取资金
    cout << "WithdrawQuota=" << pTradingAccount->WithdrawQuota << endl;
    ///基本准备金
    cout << "Reserve=" << pTradingAccount->Reserve << endl;
    ///交易日
    cout << "TradingDay=" << pTradingAccount->TradingDay << endl;
    ///结算编号
    cout << "SettlementID=" << pTradingAccount->SettlementID << endl;
    ///信用额度
    cout << "Credit=" << pTradingAccount->Credit << endl;
    ///质押金额
    cout << "Mortgage=" << pTradingAccount->Mortgage << endl;
    ///交易所保证金
    cout << "ExchangeMargin=" << pTradingAccount->ExchangeMargin << endl;
    ///投资者交割保证金
    cout << "DeliveryMargin=" << pTradingAccount->DeliveryMargin << endl;
    ///交易所交割保证金
    cout << "ExchangeDeliveryMargin=" << pTradingAccount->ExchangeDeliveryMargin << endl;
    ///保底期货结算准备金
    cout << "ReserveBalance=" << pTradingAccount->ReserveBalance << endl;
    ///币种代码
    cout << "CurrencyID=" << pTradingAccount->CurrencyID << endl;
    ///上次货币质入金额
    cout << "PreFundMortgageIn=" << pTradingAccount->PreFundMortgageIn << endl;
    ///上次货币质出金额
    cout << "PreFundMortgageOut=" << pTradingAccount->PreFundMortgageOut << endl;
    ///货币质入金额
    cout << "FundMortgageIn=" << pTradingAccount->FundMortgageIn << endl;
    ///货币质出金额
    cout << "FundMortgageOut=" << pTradingAccount->FundMortgageOut << endl;
    ///货币质押余额
    cout << "FundMortgageAvailable=" << pTradingAccount->FundMortgageAvailable << endl;
    ///可质押货币金额
    cout << "MortgageableFund=" << pTradingAccount->MortgageableFund << endl;
    ///特殊产品占用保证金
    cout << "SpecProductMargin=" << pTradingAccount->SpecProductMargin << endl;
    ///特殊产品冻结保证金
    cout << "SpecProductFrozenMargin=" << pTradingAccount->SpecProductFrozenMargin << endl;
    ///特殊产品手续费
    cout << "SpecProductCommission=" << pTradingAccount->SpecProductCommission << endl;
    ///特殊产品冻结手续费
    cout << "SpecProductFrozenCommission=" << pTradingAccount->SpecProductFrozenCommission << endl;
    ///特殊产品持仓盈亏
    cout << "SpecProductPositionProfit=" << pTradingAccount->SpecProductPositionProfit << endl;
    ///特殊产品平仓盈亏
    cout << "SpecProductCloseProfit=" << pTradingAccount->SpecProductCloseProfit << endl;
    ///根据持仓盈亏算法计算的特殊产品持仓盈亏
    cout << "SpecProductPositionProfitByAlg=" << pTradingAccount->SpecProductPositionProfitByAlg << endl;
    ///特殊产品交易所保证金
    cout << "SpecProductExchangeMargin=" << pTradingAccount->SpecProductExchangeMargin << endl;
    ///业务类型
    cout << "BizType=" << pTradingAccount->BizType << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///请求查询投资者持仓
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
    ///合约代码
    if  (pInvestorPosition == NULL) {
	    cout << "pInvestorPosition  is null!" << endl;
	    return;
    }
    cout << "InstrumentID=" << pInvestorPosition->InstrumentID << endl;
    ///经纪公司代码
    cout << "brokerID=" << pInvestorPosition->BrokerID << endl;
    /////投资者代码
    cout << "InvestorID=" << pInvestorPosition->InvestorID << endl;
    /////持仓多空方向
    cout << "PosiDirection=" << pInvestorPosition->PosiDirection << endl;
    /////投机套保标志
    cout << "HedgeFlag=" << pInvestorPosition->HedgeFlag << endl;
    /////持仓日期
    cout << "PositionDate=" << pInvestorPosition->PositionDate << endl;
    /////上日持仓
    cout << "YdPosition=" << pInvestorPosition->YdPosition << endl;
    /////今日持仓
    cout << "Position=" << pInvestorPosition->Position << endl;
    /////多头冻结
    cout << "LongFrozen=" << pInvestorPosition->LongFrozen << endl;
    /////空头冻结
    cout << "ShortFrozen=" << pInvestorPosition->ShortFrozen << endl;
    /////开仓冻结金额
    cout << "LongFrozenAmount=" << pInvestorPosition->LongFrozenAmount << endl;
    /////开仓冻结金额
    cout << "ShortFrozenAmount=" << pInvestorPosition->ShortFrozenAmount << endl;
    /////开仓量
    cout << "OpenVolume=" << pInvestorPosition->OpenVolume << endl;
    /////平仓量
    cout << "CloseVolume=" << pInvestorPosition->CloseVolume << endl;
    /////开仓金额
    cout << "OpenAmount=" << pInvestorPosition->OpenAmount << endl;
    /////平仓金额
    cout << "CloseAmount=" << pInvestorPosition->CloseAmount << endl;
    /////持仓成本
    cout << "PositionCost=" << pInvestorPosition->PositionCost << endl;
    /////上次占用的保证金
    cout << "PreMargin=" << pInvestorPosition->PreMargin << endl;
    /////占用的保证金
    cout << "UseMargin=" << pInvestorPosition->UseMargin << endl;
    /////冻结的保证金
    cout << "FrozenMargin=" << pInvestorPosition->FrozenMargin << endl;
    /////冻结的资金
    cout << "FrozenCash=" << pInvestorPosition->FrozenCash << endl;
    /////冻结的手续费
    cout << "FrozenCommission=" << pInvestorPosition->FrozenCommission << endl;
    /////资金差额
    cout << "CashIn=" << pInvestorPosition->CashIn << endl;
    /////手续费
    cout << "Commission=" << pInvestorPosition->Commission << endl;
    /////平仓盈亏
    cout << "CloseProfit=" << pInvestorPosition->CloseProfit << endl;
    /////持仓盈亏
    cout << "PositionProfit=" << pInvestorPosition->PositionProfit << endl;
    /////上次结算价
    cout << "PreSettlementPrice=" << pInvestorPosition->PreSettlementPrice << endl;
    /////本次结算价
    cout << "SettlementPrice=" << pInvestorPosition->SettlementPrice << endl;
    /////交易日
    cout << "TradingDay=" << pInvestorPosition->TradingDay << endl;
    /////结算编号
    cout << "SettlementID=" << pInvestorPosition->SettlementID << endl;
    /////开仓成本
    cout << "OpenCost=" << pInvestorPosition->OpenCost << endl;
    /////交易所保证金
    cout << "ExchangeMargin=" << pInvestorPosition->ExchangeMargin << endl;
    /////组合成交形成的持仓
    cout << "CombPosition=" << pInvestorPosition->CombPosition << endl;
    /////组合多头冻结
    cout << "CombLongFrozen=" << pInvestorPosition->CombLongFrozen << endl;
    /////组合空头冻结
    cout << "CombShortFrozen=" << pInvestorPosition->CombShortFrozen << endl;
    /////逐日盯市平仓盈亏
    cout << "CloseProfitByDate=" << pInvestorPosition->CloseProfitByDate << endl;
    /////逐笔对冲平仓盈亏
    cout << "CloseProfitByTrade=" << pInvestorPosition->CloseProfitByTrade << endl;
    /////今日持仓
    cout << "TodayPosition=" << pInvestorPosition->TodayPosition << endl;
    /////保证金率
    cout << "MarginRateByMoney=" << pInvestorPosition->MarginRateByMoney << endl;
    /////保证金率(按手数)
    cout << "MarginRateByVolume=" << pInvestorPosition->MarginRateByVolume << endl;
    /////执行冻结
    cout << "StrikeFrozen=" << pInvestorPosition->StrikeFrozen << endl;
    /////执行冻结金额
    cout << "StrikeFrozenAmount=" << pInvestorPosition->StrikeFrozenAmount << endl;
    /////放弃执行冻结
    cout << "AbandonFrozen=" << pInvestorPosition->AbandonFrozen << endl;
    /////交易所代码
    cout << "ExchangeID=" << pInvestorPosition->ExchangeID << endl;
    /////执行冻结的昨仓
    cout << "YdStrikeFrozen=" << pInvestorPosition->YdStrikeFrozen << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///报单录入请求
	//ReqOrderInsert();
    }
}

void CTraderSpi::ReqOrderInsert()
{
    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    ///经纪公司代码
    strcpy(req.BrokerID, BROKER_ID);
    ///投资者代码
    strcpy(req.InvestorID, INVESTOR_ID);
    ///合约代码
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    ///报单引用
    strcpy(req.OrderRef, ORDER_REF);
    ///用户代码
    //	TThostFtdcUserIDType	UserID;
    ///报单价格条件: 限价
    req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    ///买卖方向:
    req.Direction = DIRECTION;
    ///组合开平标志: 开仓
    req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    ///组合投机套保标志
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    ///价格
    req.LimitPrice = LIMIT_PRICE;
    ///数量: 1
    req.VolumeTotalOriginal = 1;
    ///有效期类型: 当日有效
    req.TimeCondition = THOST_FTDC_TC_GFD;
    ///GTD日期
    //	TThostFtdcDateType	GTDDate;
    ///成交量类型: 任何数量
    req.VolumeCondition = THOST_FTDC_VC_AV;
    ///最小成交量: 1
    req.MinVolume = 1;
    ///触发条件: 立即
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    ///止损价
    //	TThostFtdcPriceType	StopPrice;
    ///强平原因: 非强平
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    ///自动挂起标志: 否
    req.IsAutoSuspend = 0;
    ///业务单元
    //	TThostFtdcBusinessUnitType	BusinessUnit;
    ///请求编号
    //	TThostFtdcRequestIDType	RequestID;
    ///用户强评标志: 否
    req.UserForceClose = 0;

    int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
    cout << "--->>> sent orderInsert request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cout << "--->>> "
	 << "OnRspOrderInsert" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///经纪公司代码
    if  (pInputOrder== NULL) {
	    cout << "pInputOrder is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pInputOrder->BrokerID << endl;
    ///投资者代码
    cout << "InvestorID=" << pInputOrder->InvestorID << endl;
    ///合约代码
    cout << "InstrumentID=" << pInputOrder->InstrumentID << endl;
    ///报单引用
    cout << "OrderRef=" << pInputOrder->OrderRef << endl;
    ///用户代码
    cout << "UserID=" << pInputOrder->UserID << endl;
    ///报单价格条件
    cout << "OrderPriceType=" << pInputOrder->OrderPriceType << endl;
    ///买卖方向
    cout << "Direction=" << pInputOrder->Direction << endl;
    ///组合开平标志
    cout << "CombOffsetFlag=" << pInputOrder->CombOffsetFlag << endl;
    ///组合投机套保标志
    cout << "CombHedgeFlag=" << pInputOrder->CombHedgeFlag << endl;
    ///价格
    cout << "LimitPrice=" << pInputOrder->LimitPrice << endl;
    ///数量
    cout << "VolumeTotalOriginal=" << pInputOrder->VolumeTotalOriginal << endl;
    ///有效期类型
    cout << "TimeCondition=" << pInputOrder->TimeCondition << endl;
    ///GTD日期
    cout << "GTDDate=" << pInputOrder->GTDDate << endl;
    ///成交量类型
    cout << "VolumeCondition=" << pInputOrder->VolumeCondition << endl;
    ///最小成交量
    cout << "MinVolume=" << pInputOrder->MinVolume << endl;
    ///触发条件
    cout << "ContingentCondition=" << pInputOrder->ContingentCondition << endl;
    ///止损价
    cout << "StopPrice=" << pInputOrder->StopPrice << endl;
    ///强平原因
    cout << "ForceCloseReason=" << pInputOrder->ForceCloseReason << endl;
    ///自动挂起标志
    cout << "IsAutoSuspend=" << pInputOrder->IsAutoSuspend << endl;
    ///业务单元
    cout << "BusinessUnit=" << pInputOrder->BusinessUnit << endl;
    ///请求编号
    cout << "RequestID=" << pInputOrder->RequestID << endl;
    ///用户强评标志
    cout << "UserForceClose=" << pInputOrder->UserForceClose << endl;
    ///互换单标志
    cout << "IsSwapOrder=" << pInputOrder->IsSwapOrder << endl;
    ///交易所代码
    cout << "ExchangeID=" << pInputOrder->ExchangeID << endl;
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField* pOrder)
{
    static bool ORDER_ACTION_SENT = false; //是否发送了报单
    if (ORDER_ACTION_SENT)
	return;

    CThostFtdcInputOrderActionField req;
    memset(&req, 0, sizeof(req));
    ///经纪公司代码
    strcpy(req.BrokerID, pOrder->BrokerID);
    ///投资者代码
    strcpy(req.InvestorID, pOrder->InvestorID);
    ///报单操作引用
    //	TThostFtdcOrderActionRefType	OrderActionRef;
    ///报单引用
    strcpy(req.OrderRef, pOrder->OrderRef);
    ///请求编号
    //	TThostFtdcRequestIDType	RequestID;
    ///前置编号
    req.FrontID = FRONT_ID;
    ///会话编号
    req.SessionID = SESSION_ID;
    ///交易所代码
    //	TThostFtdcExchangeIDType	ExchangeID;
    ///报单编号
    //	TThostFtdcOrderSysIDType	OrderSysID;
    ///操作标志
    req.ActionFlag = THOST_FTDC_AF_Delete;
    ///价格
    //	TThostFtdcPriceType	LimitPrice;
    ///数量变化
    //	TThostFtdcVolumeType	VolumeChange;
    ///用户代码
    //	TThostFtdcUserIDType	UserID;
    ///合约代码
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
    ///经纪公司代码
    if  ( pInputOrderAction== NULL) {
	    cout << "pInputOrderAction is null!" << endl;
	    return;
    }
    cout << "brokerid" << pInputOrderAction->BrokerID << endl;
    ///投资者代码
    cout << "InvestorID" << pInputOrderAction->InvestorID << endl;
    ///报单操作引用
    cout << "OrderActionRef" << pInputOrderAction->OrderActionRef << endl;
    ///报单引用
    cout << "OrderRef" << pInputOrderAction->OrderRef << endl;
    ///请求编号
    cout << "RequestID" << pInputOrderAction->RequestID << endl;
    ///前置编号
    cout << "FrontID" << pInputOrderAction->FrontID << endl;
    ///会话编号
    cout << "SessionID" << pInputOrderAction->SessionID << endl;
    ///交易所代码
    cout << "brokerid" << pInputOrderAction->ExchangeID << endl;
    ///报单编号
    cout << "brokerid" << pInputOrderAction->OrderSysID << endl;
    ///操作标志
    cout << "brokerid" << pInputOrderAction->ActionFlag << endl;
    ///价格
    cout << "LimitPrice " << pInputOrderAction->LimitPrice << endl;
    ///数量变化
    cout << "VolumeChange " << pInputOrderAction->VolumeChange << endl;
    ///用户代码
    cout << "UserID " << pInputOrderAction->UserID << endl;
    ///合约代码
    cout << "InstrumentID " << pInputOrderAction->InstrumentID << endl;
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
    cout << "--->>> "
	 << "OnRtnOrder" << endl;
    ///经纪公司代码
    if  ( pOrder== NULL) {
	    cout << "pOrder is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pOrder->BrokerID << endl;
    ///投资者代码
    cout << "InvestorID=" << pOrder->InvestorID << endl;
    ///合约代码
    cout << "InstrumentID=" << pOrder->InstrumentID << endl;
    ///报单引用
    cout << "OrderRef=" << pOrder->OrderRef << endl;
    ///用户代码
    cout << "UserID=" << pOrder->UserID << endl;
    ///报单价格条件
    cout << "OrderPriceType=" << pOrder->OrderPriceType << endl;
    ///买卖方向
    cout << "Direction=" << pOrder->Direction << endl;
    ///组合开平标志
    cout << "CombOffsetFlag=" << pOrder->CombOffsetFlag << endl;
    ///组合投机套保标志
    cout << "CombHedgeFlag=" << pOrder->CombHedgeFlag << endl;
    ///价格
    cout << "LimitPrice=" << pOrder->LimitPrice << endl;
    ///数量
    cout << "VolumeTotalOriginal=" << pOrder->VolumeTotalOriginal << endl;
    ///有效期类型
    cout << "TimeCondition=" << pOrder->TimeCondition << endl;
    ///GTD日期
    cout << "GTDDate=" << pOrder->GTDDate << endl;
    ///成交量类型
    cout << "VolumeCondition=" << pOrder->VolumeCondition << endl;
    ///最小成交量
    cout << "MinVolume=" << pOrder->MinVolume << endl;
    ///触发条件
    cout << "ContingentCondition=" << pOrder->ContingentCondition << endl;
    ///止损价
    cout << "StopPrice=" << pOrder->StopPrice << endl;
    ///强平原因
    cout << "ForceCloseReason=" << pOrder->ForceCloseReason << endl;
    ///自动挂起标志
    cout << "IsAutoSuspend=" << pOrder->IsAutoSuspend << endl;
    ///业务单元
    cout << "BusinessUnit=" << pOrder->BusinessUnit << endl;
    ///请求编号
    cout << "RequestID=" << pOrder->RequestID << endl;
    ///本地报单编号
    cout << "OrderLocalID=" << pOrder->OrderLocalID << endl;
    ///交易所代码
    cout << "ExchangeID=" << pOrder->ExchangeID << endl;
    ///会员代码
    cout << "ParticipantID=" << pOrder->ParticipantID << endl;
    ///客户代码
    cout << "ClientID=" << pOrder->ClientID << endl;
    ///合约在交易所的代码
    cout << "ExchangeInstID=" << pOrder->ExchangeInstID << endl;
    ///交易所交易员代码
    cout << "TraderID=" << pOrder->TraderID << endl;
    ///安装编号
    cout << "InstallID=" << pOrder->InstallID << endl;
    ///报单提交状态
    cout << "OrderSubmitStatus=" << pOrder->OrderSubmitStatus << endl;
    ///报单提示序号
    cout << "NotifySequence=" << pOrder->NotifySequence << endl;
    ///交易日
    cout << "TradingDay=" << pOrder->TradingDay << endl;
    ///结算编号
    cout << "SettlementID=" << pOrder->SettlementID << endl;
    ///报单编号
    cout << "OrderSysID=" << pOrder->OrderSysID << endl;
    ///报单来源
    cout << "OrderSource=" << pOrder->OrderSource << endl;
    ///报单状态
    cout << "OrderStatus=" << pOrder->OrderStatus << endl;
    ///报单类型
    cout << "OrderType=" << pOrder->OrderType << endl;
    ///今成交数量
    cout << "VolumeTraded=" << pOrder->VolumeTraded << endl;
    ///剩余数量
    cout << "VolumeTotal=" << pOrder->VolumeTotal << endl;
    ///报单日期
    cout << "InsertDate=" << pOrder->InsertDate << endl;
    ///委托时间
    cout << "InsertTime=" << pOrder->InsertTime << endl;
    ///激活时间
    cout << "ActiveTime=" << pOrder->ActiveTime << endl;
    ///挂起时间
    cout << "SuspendTime=" << pOrder->SuspendTime << endl;
    ///最后修改时间
    cout << "UpdateTime=" << pOrder->UpdateTime << endl;
    ///撤销时间
    cout << "CancelTime=" << pOrder->CancelTime << endl;
    ///最后修改交易所交易员代码
    cout << "ActiveTraderID=" << pOrder->ActiveTraderID << endl;
    ///结算会员编号
    cout << "ClearingPartID=" << pOrder->ClearingPartID << endl;
    ///序号
    cout << "SequenceNo=" << pOrder->SequenceNo << endl;
    ///前置编号
    cout << "FrontID=" << pOrder->FrontID << endl;
    ///会话编号
    cout << "SessionID=" << pOrder->SessionID << endl;
    ///用户端产品信息
    cout << "UserProductInfo=" << pOrder->UserProductInfo << endl;
    ///状态信息
    cout << "StatusMsg=" << pOrder->StatusMsg << endl;
    ///用户强评标志
    cout << "UserForceClose=" << pOrder->UserForceClose << endl;
    ///操作用户代码
    cout << "ActiveUserID=" << pOrder->ActiveUserID << endl;
    ///经纪公司报单编号
    cout << "BrokerOrderSeq=" << pOrder->BrokerOrderSeq << endl;
    ///相关报单
    cout << "RelativeOrderSysID=" << pOrder->RelativeOrderSysID << endl;
    ///郑商所成交数量
    cout << "ZCETotalTradedVolume=" << pOrder->ZCETotalTradedVolume << endl;
    ///互换单标志
    cout << "IsSwapOrder=" << pOrder->IsSwapOrder << endl;
    ///营业部编号
    cout << "BranchID=" << pOrder->BranchID << endl;
    // if (IsMyOrder(pOrder)) {
    //     if (IsTradingOrder(pOrder))
    //         ReqOrderAction(pOrder);
    //     else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
    //         cout << "--->>> 撤单成功" << endl;
    // }
}

///成交通知
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField* pTrade)
{
    cout << "--->>> "
	 << "OnRtnTrade" << endl;
    ///经纪公司代码
    if  ( pTrade== NULL) {
	    cout << "pTrade is null!" << endl;
	    return;
    }
    cout << "brokerid=" << pTrade->BrokerID << endl;
    ///投资者代码
    cout << "InvestorID=" << pTrade->InvestorID << endl;
    ///合约代码
    cout << "InstrumentID=" << pTrade->InstrumentID << endl;
    ///报单引用
    cout << "OrderRef=" << pTrade->OrderRef << endl;
    ///用户代码
    cout << "UserID=" << pTrade->UserID << endl;
    ///交易所代码
    cout << "ExchangeID=" << pTrade->ExchangeID << endl;
    ///成交编号
    cout << "TradeID=" << pTrade->TradeID << endl;
    ///买卖方向
    cout << "Direction=" << pTrade->Direction << endl;
    ///报单编号
    cout << "OrderSysID=" << pTrade->OrderSysID << endl;
    ///会员代码
    cout << "ParticipantID=" << pTrade->ParticipantID << endl;
    ///客户代码
    cout << "ClientID=" << pTrade->ClientID << endl;
    ///交易角色
    cout << "TradingRole=" << pTrade->TradingRole << endl;
    ///合约在交易所的代码
    cout << "ExchangeInstID=" << pTrade->ExchangeInstID << endl;
    ///开平标志
    cout << "OffsetFlag=" << pTrade->OffsetFlag << endl;
    ///投机套保标志
    cout << "HedgeFlag=" << pTrade->HedgeFlag << endl;
    ///价格
    cout << "Price=" << pTrade->Price << endl;
    ///数量
    cout << "Volume=" << pTrade->Volume << endl;
    ///成交时期
    cout << "TradeDate=" << pTrade->TradeDate << endl;
    ///成交时间
    cout << "TradeTime=" << pTrade->TradeTime << endl;
    ///成交类型
    cout << "TradeType=" << pTrade->TradeType << endl;
    ///成交价来源
    cout << "PriceSource=" << pTrade->PriceSource << endl;
    ///交易所交易员代码
    cout << "TraderID=" << pTrade->TraderID << endl;
    ///本地报单编号
    cout << "OrderLocalID=" << pTrade->OrderLocalID << endl;
    ///结算会员编号
    cout << "ClearingPartID=" << pTrade->ClearingPartID << endl;
    ///业务单元
    cout << "BusinessUnit=" << pTrade->BusinessUnit << endl;
    ///序号
    cout << "SequenceNo=" << pTrade->SequenceNo << endl;
    ///交易日
    cout << "TradingDay=" << pTrade->TradingDay << endl;
    ///结算编号
    cout << "SettlementID=" << pTrade->SettlementID << endl;
    ///经纪公司报单编号
    cout << "BrokerOrderSeq=" << pTrade->BrokerOrderSeq << endl;
    ///成交来源
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
    // 如果ErrorID != 0, 说明收到了错误的响应
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

