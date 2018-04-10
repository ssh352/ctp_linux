// #include "ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
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
	// 保存会话参数
	FRONT_ID = pRspUserLogin->FrontID;
	SESSION_ID = pRspUserLogin->SessionID;
	int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
	iNextOrderRef++;
	sprintf(ORDER_REF, "%d", iNextOrderRef);
	///获取当前交易日
	cerr << "--->>> call api function GetTradingDay = " << pUserApi->GetTradingDay() << endl;
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
    cerr << "--->>> investor confirm settlement: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspSettlementInfoConfirm" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///经纪公司代码
    cerr << "BrokerID" << pSettlementInfoConfirm->BrokerID;
    ///投资者代码
    cerr << "InvertorID" << pSettlementInfoConfirm->InvestorID;
    ///确认日期
    cerr << "confirmdate" << pSettlementInfoConfirm->ConfirmDate;
    ///确认时间
    cerr << "confirmtime" << pSettlementInfoConfirm->ConfirmTime;
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
    cerr << "--->>> send ReqQryInstrument request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspQryInstrument" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///合约代码
    cerr << "InstrumentID=" << pInstrument->InstrumentID << endl;
    ///交易所代码
    cerr << "ExchangeID=" << pInstrument->ExchangeID << endl;
    ///合约名称
    cerr << "InstrumentName=" << pInstrument->InstrumentName << endl;
    ///合约在交易所的代码
    cerr << "ExchangeInstID=" << pInstrument->ExchangeInstID << endl;
    ///产品代码
    cerr << "ProductID=" << pInstrument->ProductID << endl;
    ///产品类型
    cerr << "ProductClass=" << pInstrument->ProductClass << endl;
    ///交割年份
    cerr << "DeliveryYear=" << pInstrument->DeliveryYear << endl;
    ///交割月
    cerr << "DeliveryMonth=" << pInstrument->DeliveryMonth << endl;
    ///市价单最大下单量
    cerr << "MaxMarketOrderVolume=" << pInstrument->MaxMarketOrderVolume << endl;
    ///市价单最小下单量
    cerr << "MinMarketOrderVolume=" << pInstrument->MinMarketOrderVolume << endl;
    ///限价单最大下单量
    cerr << "MaxLimitOrderVolume=" << pInstrument->MaxLimitOrderVolume << endl;
    ///限价单最小下单量
    cerr << "MinLimitOrderVolume=" << pInstrument->MinLimitOrderVolume << endl;
    ///合约数量乘数
    cerr << "VolumeMultiple=" << pInstrument->VolumeMultiple << endl;
    ///最小变动价位
    cerr << "PriceTick=" << pInstrument->PriceTick << endl;
    ///创建日
    cerr << "CreateDate=" << pInstrument->CreateDate << endl;
    ///上市日
    cerr << "OpenDate=" << pInstrument->OpenDate << endl;
    ///到期日
    cerr << "ExpireDate=" << pInstrument->ExpireDate << endl;
    ///开始交割日
    cerr << "StartDelivDate=" << pInstrument->StartDelivDate << endl;
    ///结束交割日
    cerr << "EndDelivDate=" << pInstrument->EndDelivDate << endl;
    ///合约生命周期状态
    cerr << "InstLifePhase=" << pInstrument->InstLifePhase << endl;
    ///当前是否交易
    cerr << "IsTrading=" << pInstrument->IsTrading << endl;
    ///持仓类型
    cerr << "PositionType=" << pInstrument->PositionType << endl;
    ///持仓日期类型
    cerr << "PositionDateType=" << pInstrument->PositionDateType << endl;
    ///多头保证金率
    cerr << "LongMarginRatio=" << pInstrument->LongMarginRatio << endl;
    ///空头保证金率
    cerr << "ShortMarginRatio=" << pInstrument->ShortMarginRatio << endl;
    ///是否使用大额单边保证金算法
    cerr << "MaxMarginSideAlgorithm=" << pInstrument->MaxMarginSideAlgorithm << endl;
    ///基础商品代码
    cerr << "UnderlyingInstrID=" << pInstrument->UnderlyingInstrID << endl;
    ///执行价
    cerr << "StrikePrice=" << pInstrument->StrikePrice << endl;
    ///期权类型
    cerr << "OptionsType=" << pInstrument->OptionsType << endl;
    ///合约基础商品乘数
    cerr << "UnderlyingMultiple=" << pInstrument->UnderlyingMultiple << endl;
    ///组合类型
    cerr << "CombinationType=" << pInstrument->CombinationType << endl;
    ///最小买下单单位
    cerr << "MinBuyVolume=" << pInstrument->MinBuyVolume << endl;
    ///最小卖下单单位
    cerr << "MinSellVolume=" << pInstrument->MinSellVolume << endl;
    ///合约标识码
    cerr << "InstrumentCode=" << pInstrument->InstrumentCode << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	// 请求查询合约
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
    ///经纪公司代码
    cerr << "brokerid=" << pTradingAccount->BrokerID;
    ///投资者帐号
    cerr << "AccountID=" << pTradingAccount->AccountID << endl;
    ///上次质押金额
    cerr << "PreMortgage=" << pTradingAccount->PreMortgage << endl;
    ///上次信用额度
    cerr << "PreCredit=" << pTradingAccount->PreCredit << endl;
    ///上次存款额
    cerr << "PreDeposit=" << pTradingAccount->PreDeposit << endl;
    ///上次结算准备金
    cerr << "PreBalance=" << pTradingAccount->PreBalance << endl;
    ///上次占用的保证金
    cerr << "PreMargin=" << pTradingAccount->PreMargin << endl;
    ///利息基数
    cerr << "InterestBase=" << pTradingAccount->InterestBase << endl;
    ///利息收入
    cerr << "Interest=" << pTradingAccount->Interest << endl;
    ///入金金额
    cerr << "Deposit=" << pTradingAccount->Deposit << endl;
    ///出金金额
    cerr << "Withdraw=" << pTradingAccount->Withdraw << endl;
    ///冻结的保证金
    cerr << "FrozenMargin=" << pTradingAccount->FrozenMargin << endl;
    ///冻结的资金
    cerr << "FrozenCash=" << pTradingAccount->FrozenCash << endl;
    ///冻结的手续费
    cerr << "FrozenCommission=" << pTradingAccount->FrozenCommission << endl;
    ///当前保证金总额
    cerr << "CurrMargin=" << pTradingAccount->CurrMargin << endl;
    ///资金差额
    cerr << "CashIn=" << pTradingAccount->CashIn << endl;
    ///手续费
    cerr << "Commission=" << pTradingAccount->Commission << endl;
    ///平仓盈亏
    cerr << "CloseProfit=" << pTradingAccount->CloseProfit << endl;
    ///持仓盈亏
    cerr << "PositionProfit=" << pTradingAccount->PositionProfit << endl;
    ///期货结算准备金
    cerr << "Balance=" << pTradingAccount->Balance << endl;
    ///可用资金
    cerr << "Available=" << pTradingAccount->Available << endl;
    ///可取资金
    cerr << "WithdrawQuota=" << pTradingAccount->WithdrawQuota << endl;
    ///基本准备金
    cerr << "Reserve=" << pTradingAccount->Reserve << endl;
    ///交易日
    cerr << "TradingDay=" << pTradingAccount->TradingDay << endl;
    ///结算编号
    cerr << "SettlementID=" << pTradingAccount->SettlementID << endl;
    ///信用额度
    cerr << "Credit=" << pTradingAccount->Credit << endl;
    ///质押金额
    cerr << "Mortgage=" << pTradingAccount->Mortgage << endl;
    ///交易所保证金
    cerr << "ExchangeMargin=" << pTradingAccount->ExchangeMargin << endl;
    ///投资者交割保证金
    cerr << "DeliveryMargin=" << pTradingAccount->DeliveryMargin << endl;
    ///交易所交割保证金
    cerr << "ExchangeDeliveryMargin=" << pTradingAccount->ExchangeDeliveryMargin << endl;
    ///保底期货结算准备金
    cerr << "ReserveBalance=" << pTradingAccount->ReserveBalance << endl;
    ///币种代码
    cerr << "CurrencyID=" << pTradingAccount->CurrencyID << endl;
    ///上次货币质入金额
    cerr << "PreFundMortgageIn=" << pTradingAccount->PreFundMortgageIn << endl;
    ///上次货币质出金额
    cerr << "PreFundMortgageOut=" << pTradingAccount->PreFundMortgageOut << endl;
    ///货币质入金额
    cerr << "FundMortgageIn=" << pTradingAccount->FundMortgageIn << endl;
    ///货币质出金额
    cerr << "FundMortgageOut=" << pTradingAccount->FundMortgageOut << endl;
    ///货币质押余额
    cerr << "FundMortgageAvailable=" << pTradingAccount->FundMortgageAvailable << endl;
    ///可质押货币金额
    cerr << "MortgageableFund=" << pTradingAccount->MortgageableFund << endl;
    ///特殊产品占用保证金
    cerr << "SpecProductMargin=" << pTradingAccount->SpecProductMargin << endl;
    ///特殊产品冻结保证金
    cerr << "SpecProductFrozenMargin=" << pTradingAccount->SpecProductFrozenMargin << endl;
    ///特殊产品手续费
    cerr << "SpecProductCommission=" << pTradingAccount->SpecProductCommission << endl;
    ///特殊产品冻结手续费
    cerr << "SpecProductFrozenCommission=" << pTradingAccount->SpecProductFrozenCommission << endl;
    ///特殊产品持仓盈亏
    cerr << "SpecProductPositionProfit=" << pTradingAccount->SpecProductPositionProfit << endl;
    ///特殊产品平仓盈亏
    cerr << "SpecProductCloseProfit=" << pTradingAccount->SpecProductCloseProfit << endl;
    ///根据持仓盈亏算法计算的特殊产品持仓盈亏
    cerr << "SpecProductPositionProfitByAlg=" << pTradingAccount->SpecProductPositionProfitByAlg << endl;
    ///特殊产品交易所保证金
    cerr << "SpecProductExchangeMargin=" << pTradingAccount->SpecProductExchangeMargin << endl;
    ///业务类型
    cerr << "BizType=" << pTradingAccount->BizType << endl;

    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///请求查询投资者持仓
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
    ///合约代码
    cerr << "InstrumentID=" << pInvestorPosition->InstrumentID << endl;
    ///经纪公司代码
    cerr << "brokerID=" << pInvestorPosition->BrokerID << endl;
    ///投资者代码
    cerr << "InvestorID=" << pInvestorPosition->InvestorID << endl;
    ///持仓多空方向
    cerr << "PosiDirection=" << pInvestorPosition->PosiDirection << endl;
    ///投机套保标志
    cerr << "HedgeFlag=" << pInvestorPosition->HedgeFlag << endl;
    ///持仓日期
    cerr << "PositionDate=" << pInvestorPosition->PositionDate << endl;
    ///上日持仓
    cerr << "YdPosition=" << pInvestorPosition->YdPosition << endl;
    ///今日持仓
    cerr << "Position=" << pInvestorPosition->Position << endl;
    ///多头冻结
    cerr << "LongFrozen=" << pInvestorPosition->LongFrozen << endl;
    ///空头冻结
    cerr << "ShortFrozen=" << pInvestorPosition->ShortFrozen << endl;
    ///开仓冻结金额
    cerr << "LongFrozenAmount=" << pInvestorPosition->LongFrozenAmount << endl;
    ///开仓冻结金额
    cerr << "ShortFrozenAmount=" << pInvestorPosition->ShortFrozenAmount << endl;
    ///开仓量
    cerr << "OpenVolume=" << pInvestorPosition->OpenVolume << endl;
    ///平仓量
    cerr << "CloseVolume=" << pInvestorPosition->CloseVolume << endl;
    ///开仓金额
    cerr << "OpenAmount=" << pInvestorPosition->OpenAmount << endl;
    ///平仓金额
    cerr << "CloseAmount=" << pInvestorPosition->CloseAmount << endl;
    ///持仓成本
    cerr << "PositionCost=" << pInvestorPosition->PositionCost << endl;
    ///上次占用的保证金
    cerr << "PreMargin=" << pInvestorPosition->PreMargin << endl;
    ///占用的保证金
    cerr << "UseMargin=" << pInvestorPosition->UseMargin << endl;
    ///冻结的保证金
    cerr << "FrozenMargin=" << pInvestorPosition->FrozenMargin << endl;
    ///冻结的资金
    cerr << "FrozenCash=" << pInvestorPosition->FrozenCash << endl;
    ///冻结的手续费
    cerr << "FrozenCommission=" << pInvestorPosition->FrozenCommission << endl;
    ///资金差额
    cerr << "CashIn=" << pInvestorPosition->CashIn << endl;
    ///手续费
    cerr << "Commission=" << pInvestorPosition->Commission << endl;
    ///平仓盈亏
    cerr << "CloseProfit=" << pInvestorPosition->CloseProfit << endl;
    ///持仓盈亏
    cerr << "PositionProfit=" << pInvestorPosition->PositionProfit << endl;
    ///上次结算价
    cerr << "PreSettlementPrice=" << pInvestorPosition->PreSettlementPrice << endl;
    ///本次结算价
    cerr << "SettlementPrice=" << pInvestorPosition->SettlementPrice << endl;
    ///交易日
    cerr << "TradingDay=" << pInvestorPosition->TradingDay << endl;
    ///结算编号
    cerr << "SettlementID=" << pInvestorPosition->SettlementID << endl;
    ///开仓成本
    cerr << "OpenCost=" << pInvestorPosition->OpenCost << endl;
    ///交易所保证金
    cerr << "ExchangeMargin=" << pInvestorPosition->ExchangeMargin << endl;
    ///组合成交形成的持仓
    cerr << "CombPosition=" << pInvestorPosition->CombPosition << endl;
    ///组合多头冻结
    cerr << "CombLongFrozen=" << pInvestorPosition->CombLongFrozen << endl;
    ///组合空头冻结
    cerr << "CombShortFrozen=" << pInvestorPosition->CombShortFrozen << endl;
    ///逐日盯市平仓盈亏
    cerr << "CloseProfitByDate=" << pInvestorPosition->CloseProfitByDate << endl;
    ///逐笔对冲平仓盈亏
    cerr << "CloseProfitByTrade=" << pInvestorPosition->CloseProfitByTrade << endl;
    ///今日持仓
    cerr << "TodayPosition=" << pInvestorPosition->TodayPosition << endl;
    ///保证金率
    cerr << "MarginRateByMoney=" << pInvestorPosition->MarginRateByMoney << endl;
    ///保证金率(按手数)
    cerr << "MarginRateByVolume=" << pInvestorPosition->MarginRateByVolume << endl;
    ///执行冻结
    cerr << "StrikeFrozen=" << pInvestorPosition->StrikeFrozen << endl;
    ///执行冻结金额
    cerr << "StrikeFrozenAmount=" << pInvestorPosition->StrikeFrozenAmount << endl;
    ///放弃执行冻结
    cerr << "AbandonFrozen=" << pInvestorPosition->AbandonFrozen << endl;
    ///交易所代码
    cerr << "ExchangeID=" << pInvestorPosition->ExchangeID << endl;
    ///执行冻结的昨仓
    cerr << "YdStrikeFrozen=" << pInvestorPosition->YdStrikeFrozen << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo)) {
	///报单录入请求
	ReqOrderInsert();
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
    cerr << "--->>> sent orderInsert request: " << ((iResult == 0) ? "success" : "failed") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspOrderInsert" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///经纪公司代码
    cerr << "brokerid=" << pInputOrder->BrokerID << endl;
    ///投资者代码
    cerr << "InvestorID=" << pInputOrder->InvestorID << endl;
    ///合约代码
    cerr << "InstrumentID=" << pInputOrder->InstrumentID << endl;
    ///报单引用
    cerr << "OrderRef=" << pInputOrder->OrderRef << endl;
    ///用户代码
    cerr << "UserID=" << pInputOrder->UserID << endl;
    ///报单价格条件
    cerr << "OrderPriceType=" << pInputOrder->OrderPriceType << endl;
    ///买卖方向
    cerr << "Direction=" << pInputOrder->Direction << endl;
    ///组合开平标志
    cerr << "CombOffsetFlag=" << pInputOrder->CombOffsetFlag << endl;
    ///组合投机套保标志
    cerr << "CombHedgeFlag=" << pInputOrder->CombHedgeFlag << endl;
    ///价格
    cerr << "LimitPrice=" << pInputOrder->LimitPrice << endl;
    ///数量
    cerr << "VolumeTotalOriginal=" << pInputOrder->VolumeTotalOriginal << endl;
    ///有效期类型
    cerr << "TimeCondition=" << pInputOrder->TimeCondition << endl;
    ///GTD日期
    cerr << "GTDDate=" << pInputOrder->GTDDate << endl;
    ///成交量类型
    cerr << "VolumeCondition=" << pInputOrder->VolumeCondition << endl;
    ///最小成交量
    cerr << "MinVolume=" << pInputOrder->MinVolume << endl;
    ///触发条件
    cerr << "ContingentCondition=" << pInputOrder->ContingentCondition << endl;
    ///止损价
    cerr << "StopPrice=" << pInputOrder->StopPrice << endl;
    ///强平原因
    cerr << "ForceCloseReason=" << pInputOrder->ForceCloseReason << endl;
    ///自动挂起标志
    cerr << "IsAutoSuspend=" << pInputOrder->IsAutoSuspend << endl;
    ///业务单元
    cerr << "BusinessUnit=" << pInputOrder->BusinessUnit << endl;
    ///请求编号
    cerr << "RequestID=" << pInputOrder->RequestID << endl;
    ///用户强评标志
    cerr << "UserForceClose=" << pInputOrder->UserForceClose << endl;
    ///互换单标志
    cerr << "IsSwapOrder=" << pInputOrder->IsSwapOrder << endl;
    ///交易所代码
    cerr << "ExchangeID=" << pInputOrder->ExchangeID << endl;
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
    cerr << "--->>> sent orderaction request: " << ((iResult == 0) ? "success" : "failed") << endl;
    ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> "
	 << "OnRspOrderAction" << endl;
    PrintOut(pRspInfo, nRequestID, bIsLast);
    ///经纪公司代码
    cerr << "brokerid" << pInputOrderAction->BrokerID << endl;
    ///投资者代码
    cerr << "InvestorID" << pInputOrderAction->InvestorID << endl;
    ///报单操作引用
    cerr << "OrderActionRef" << pInputOrderAction->OrderActionRef << endl;
    ///报单引用
    cerr << "OrderRef" << pInputOrderAction->OrderRef << endl;
    ///请求编号
    cerr << "RequestID" << pInputOrderAction->RequestID << endl;
    ///前置编号
    cerr << "FrontID" << pInputOrderAction->FrontID << endl;
    ///会话编号
    cerr << "SessionID" << pInputOrderAction->SessionID << endl;
    ///交易所代码
    cerr << "brokerid" << pInputOrderAction->ExchangeID << endl;
    ///报单编号
    cerr << "brokerid" << pInputOrderAction->OrderSysID << endl;
    ///操作标志
    cerr << "brokerid" << pInputOrderAction->ActionFlag << endl;
    ///价格
    cerr << "LimitPrice " << pInputOrderAction->LimitPrice << endl;
    ///数量变化
    cerr << "VolumeChange " << pInputOrderAction->VolumeChange << endl;
    ///用户代码
    cerr << "UserID " << pInputOrderAction->UserID << endl;
    ///合约代码
    cerr << "InstrumentID " << pInputOrderAction->InstrumentID << endl;
}

///报单通知
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
    cerr << "--->>> "
	 << "OnRtnOrder" << endl;
    ///经纪公司代码
    cerr << "brokerid=" << pOrder->BrokerID << endl;
    ///投资者代码
    cerr << "InvestorID=" << pOrder->InvestorID << endl;
    ///合约代码
    cerr << "InstrumentID=" << pOrder->InstrumentID << endl;
    ///报单引用
    cerr << "OrderRef=" << pOrder->OrderRef << endl;
    ///用户代码
    cerr << "UserID=" << pOrder->UserID << endl;
    ///报单价格条件
    cerr << "OrderPriceType=" << pOrder->OrderPriceType << endl;
    ///买卖方向
    cerr << "Direction=" << pOrder->Direction << endl;
    ///组合开平标志
    cerr << "CombOffsetFlag=" << pOrder->CombOffsetFlag << endl;
    ///组合投机套保标志
    cerr << "CombHedgeFlag=" << pOrder->CombHedgeFlag << endl;
    ///价格
    cerr << "LimitPrice=" << pOrder->LimitPrice << endl;
    ///数量
    cerr << "VolumeTotalOriginal=" << pOrder->VolumeTotalOriginal << endl;
    ///有效期类型
    cerr << "TimeCondition=" << pOrder->TimeCondition << endl;
    ///GTD日期
    cerr << "GTDDate=" << pOrder->GTDDate << endl;
    ///成交量类型
    cerr << "VolumeCondition=" << pOrder->VolumeCondition << endl;
    ///最小成交量
    cerr << "MinVolume=" << pOrder->MinVolume << endl;
    ///触发条件
    cerr << "ContingentCondition=" << pOrder->ContingentCondition << endl;
    ///止损价
    cerr << "StopPrice=" << pOrder->StopPrice << endl;
    ///强平原因
    cerr << "ForceCloseReason=" << pOrder->ForceCloseReason << endl;
    ///自动挂起标志
    cerr << "IsAutoSuspend=" << pOrder->IsAutoSuspend << endl;
    ///业务单元
    cerr << "BusinessUnit=" << pOrder->BusinessUnit << endl;
    ///请求编号
    cerr << "RequestID=" << pOrder->RequestID << endl;
    ///本地报单编号
    cerr << "OrderLocalID=" << pOrder->OrderLocalID << endl;
    ///交易所代码
    cerr << "ExchangeID=" << pOrder->ExchangeID << endl;
    ///会员代码
    cerr << "ParticipantID=" << pOrder->ParticipantID << endl;
    ///客户代码
    cerr << "ClientID=" << pOrder->ClientID << endl;
    ///合约在交易所的代码
    cerr << "ExchangeInstID=" << pOrder->ExchangeInstID << endl;
    ///交易所交易员代码
    cerr << "TraderID=" << pOrder->TraderID << endl;
    ///安装编号
    cerr << "InstallID=" << pOrder->InstallID << endl;
    ///报单提交状态
    cerr << "OrderSubmitStatus=" << pOrder->OrderSubmitStatus << endl;
    ///报单提示序号
    cerr << "NotifySequence=" << pOrder->NotifySequence << endl;
    ///交易日
    cerr << "TradingDay=" << pOrder->TradingDay << endl;
    ///结算编号
    cerr << "SettlementID=" << pOrder->SettlementID << endl;
    ///报单编号
    cerr << "OrderSysID=" << pOrder->OrderSysID << endl;
    ///报单来源
    cerr << "OrderSource=" << pOrder->OrderSource << endl;
    ///报单状态
    cerr << "OrderStatus=" << pOrder->OrderStatus << endl;
    ///报单类型
    cerr << "OrderType=" << pOrder->OrderType << endl;
    ///今成交数量
    cerr << "VolumeTraded=" << pOrder->VolumeTraded << endl;
    ///剩余数量
    cerr << "VolumeTotal=" << pOrder->VolumeTotal << endl;
    ///报单日期
    cerr << "InsertDate=" << pOrder->InsertDate << endl;
    ///委托时间
    cerr << "InsertTime=" << pOrder->InsertTime << endl;
    ///激活时间
    cerr << "ActiveTime=" << pOrder->ActiveTime << endl;
    ///挂起时间
    cerr << "SuspendTime=" << pOrder->SuspendTime << endl;
    ///最后修改时间
    cerr << "UpdateTime=" << pOrder->UpdateTime << endl;
    ///撤销时间
    cerr << "CancelTime=" << pOrder->CancelTime << endl;
    ///最后修改交易所交易员代码
    cerr << "ActiveTraderID=" << pOrder->ActiveTraderID << endl;
    ///结算会员编号
    cerr << "ClearingPartID=" << pOrder->ClearingPartID << endl;
    ///序号
    cerr << "SequenceNo=" << pOrder->SequenceNo << endl;
    ///前置编号
    cerr << "FrontID=" << pOrder->FrontID << endl;
    ///会话编号
    cerr << "SessionID=" << pOrder->SessionID << endl;
    ///用户端产品信息
    cerr << "UserProductInfo=" << pOrder->UserProductInfo << endl;
    ///状态信息
    cerr << "StatusMsg=" << pOrder->StatusMsg << endl;
    ///用户强评标志
    cerr << "UserForceClose=" << pOrder->UserForceClose << endl;
    ///操作用户代码
    cerr << "ActiveUserID=" << pOrder->ActiveUserID << endl;
    ///经纪公司报单编号
    cerr << "BrokerOrderSeq=" << pOrder->BrokerOrderSeq << endl;
    ///相关报单
    cerr << "RelativeOrderSysID=" << pOrder->RelativeOrderSysID << endl;
    ///郑商所成交数量
    cerr << "ZCETotalTradedVolume=" << pOrder->ZCETotalTradedVolume << endl;
    ///互换单标志
    cerr << "IsSwapOrder=" << pOrder->IsSwapOrder << endl;
    ///营业部编号
    cerr << "BranchID=" << pOrder->BranchID << endl;
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
    cerr << "--->>> "
	 << "OnRtnTrade" << endl;
    ///经纪公司代码
    cerr << "brokerid=" << pTrade->BrokerID << endl;
    ///投资者代码
    cerr << "InvestorID=" << pTrade->InvestorID << endl;
    ///合约代码
    cerr << "InstrumentID=" << pTrade->InstrumentID << endl;
    ///报单引用
    cerr << "OrderRef=" << pTrade->OrderRef << endl;
    ///用户代码
    cerr << "UserID=" << pTrade->UserID << endl;
    ///交易所代码
    cerr << "ExchangeID=" << pTrade->ExchangeID << endl;
    ///成交编号
    cerr << "TradeID=" << pTrade->TradeID << endl;
    ///买卖方向
    cerr << "Direction=" << pTrade->Direction << endl;
    ///报单编号
    cerr << "OrderSysID=" << pTrade->OrderSysID << endl;
    ///会员代码
    cerr << "ParticipantID=" << pTrade->ParticipantID << endl;
    ///客户代码
    cerr << "ClientID=" << pTrade->ClientID << endl;
    ///交易角色
    cerr << "TradingRole=" << pTrade->TradingRole << endl;
    ///合约在交易所的代码
    cerr << "ExchangeInstID=" << pTrade->ExchangeInstID << endl;
    ///开平标志
    cerr << "OffsetFlag=" << pTrade->OffsetFlag << endl;
    ///投机套保标志
    cerr << "HedgeFlag=" << pTrade->HedgeFlag << endl;
    ///价格
    cerr << "Price=" << pTrade->Price << endl;
    ///数量
    cerr << "Volume=" << pTrade->Volume << endl;
    ///成交时期
    cerr << "TradeDate=" << pTrade->TradeDate << endl;
    ///成交时间
    cerr << "TradeTime=" << pTrade->TradeTime << endl;
    ///成交类型
    cerr << "TradeType=" << pTrade->TradeType << endl;
    ///成交价来源
    cerr << "PriceSource=" << pTrade->PriceSource << endl;
    ///交易所交易员代码
    cerr << "TraderID=" << pTrade->TraderID << endl;
    ///本地报单编号
    cerr << "OrderLocalID=" << pTrade->OrderLocalID << endl;
    ///结算会员编号
    cerr << "ClearingPartID=" << pTrade->ClearingPartID << endl;
    ///业务单元
    cerr << "BusinessUnit=" << pTrade->BusinessUnit << endl;
    ///序号
    cerr << "SequenceNo=" << pTrade->SequenceNo << endl;
    ///交易日
    cerr << "TradingDay=" << pTrade->TradingDay << endl;
    ///结算编号
    cerr << "SettlementID=" << pTrade->SettlementID << endl;
    ///经纪公司报单编号
    cerr << "BrokerOrderSeq=" << pTrade->BrokerOrderSeq << endl;
    ///成交来源
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
    // 如果ErrorID != 0, 说明收到了错误的响应
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

