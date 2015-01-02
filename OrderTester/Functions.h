#ifndef _Function_H_
#define _Function_H_

#define DLLImport _declspec(dllimport)


#define SK_SUCCESS										0
#define SK_ERROR_INITIALIZE_FAIL						1000
#define SK_ERROR_ACCOUNT_NOT_EXIST						1001
#define SK_ERROR_ACCOUNT_MARKET_NOT_MATCH				1002
#define SK_ERROR_PERIOD_OUT_OF_RANGE					1003
#define SK_ERROR_FLAG_OUT_OF_RANGE						1004
#define SK_ERROR_BUYSELL_OUT_OF_RANGE					1005
#define SK_ERROR_ORDER_SERVER_INVALID					1006
#define SK_ERROR_PERMISSION_DENIED						1007
#define SK_ERROR_TRADE_TYPR_OUT_OF_RANGE				1008
#define SK_ERROR_DAY_TRADE_OUT_OF_RANGE					1009
#define SK_ERROR_ORDER_SIGN_INVALID						1010
#define SK_ERROR_NEW_CLOSE_OUT_OF_RANGE					1011
#define SK_ERROR_PRODUCT_INVALID						1012
#define SK_ERROR_QTY_INVALID							1013
#define SK_ERROR_DAYTRADE_DENIED						1014
#define SK_ERROR_SPCIAL_TRADE_TYPE_INVALID				1015
#define SK_ERROR_PRICE_INVALID							1016
#define SK_ERROR_INDEX_OUT_OF_RANGE						1017
#define SK_ERROR_QUERY_IN_PROCESSING					1018
#define SK_ERROR_LOGIN_INVALID							1019
#define SK_ERROR_REGISTER_CALLBACK						1020
#define SK_ERROR_FUNCTION_PERMISSION_DENIED				1021	
#define	SK_ERROR_MARKET_OUT_OF_RANGE					1022
#define SK_ERROR_PERMISSION_TIMEOUT						1023
#define SK_ERROR_FOREIGNSTOCK_PRICE_OUT_OF_RANGE		1024
#define SK_ERROR_FOREIGNSTOCK_UNDEFINE_COINTYPE			1025
#define SK_ERROR_FOREIGNSTOCK_SAME_COINSTYPE			1026
#define SK_ERROR_FOREIGNSTOCK_SALE_SHOULD_ORIGINAL_COIN	1027	
#define	SK_ERROR_FOREIGNSTOCK_TRADE_UNIT_INVALID		1028
#define	SK_ERROR_FOREIGNSTOCK_STOCKNO_INVALID			1029
#define	SK_ERROR_FOREIGNSTOCK_ACCOUNTTYPE_INVALID		1030
#define	SK_ERROR_FOREIGNSTOCK_INITIALIZE_FAIL			1031
#define	SK_ERROR_TS_INITIALIZE_FAIL						1032
#define SK_ERROR_OVERSEA_TRADE_PRODUCT_FAIL				1033
#define SK_ERROR_OVERSEA_TRADE_DATA_NOT_COMPLETE		1034
#define SK_ERROR_CERT_VERIFY_CN_INVALID					1035
#define SK_ERROR_CERT_VERIFY_SERVER_REJECT				1036
#define SK_ERROR_CERT_NOT_VERIFIED						1037
#define SK_ERROR_OVERSEA_ACCOUNT_NOT_EXIST				1038
#define SK_ERROR_ORDER_LOCK								1039
#define SK_ERROR_DID_NOT_LOCK							1040

#define SK_WARNING_OF_COM_DATA_MISSING					2001
#define SK_WARNING_TS_READY								2002
#define SK_WARNING_LOGIN_ALREADY						2003
#define SK_WARNING_LOGIN_SPECIAL_ALREADY				2004

#define SK_FAIL											3001


struct SOfComProduct
{
   char m_strExchange[10]; ///< 交易所代號
   char m_strProductNo[10]; ///<商品代號
   char m_strYearMonth[10]; ///< 商品年月

   unsigned short m_sSpecialTradeType;///<特殊條件
   double m_dMinJump; ///< 跳動點
   unsigned short m_nDenominator; ///< 分母

   unsigned short m_sDayTrade; ///< 是否可當沖	
};

// import from SKOrderLib.dll

DLLImport int __stdcall SKOrderLib_Initialize(const char* pcUserName,const char* pcPassword);

DLLImport int __stdcall SKOrderLib_ReadCertByID( TCHAR* lpszUserID);

DLLImport int __stdcall SendStockOrder( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usPeriod, unsigned short usFlag, 
	unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall SendStockOrderAsync( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usPeriod, 
	unsigned short usFlag, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty);

DLLImport int __stdcall SendFutureOrder( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usDayTrade, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, TCHAR* lpszMessage, 
	int *pnMessageBufferSize);

DLLImport int __stdcall SendFutureOrderAsync( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usDayTrade, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty);

DLLImport int __stdcall SendOptionOrder( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usNewClose, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, TCHAR* lpszMessage, 
	int *pnMessageBufferSize);

DLLImport int __stdcall SendOptionOrderAsync( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usNewClose, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty);

DLLImport int __stdcall SendOverseaFutureOrder( const TCHAR* lpszAccount, const TCHAR* lpszTradeName, const TCHAR* lpszStockNo, const TCHAR* lpszYearMonth,
	unsigned short usBuySell, unsigned short usNewClose, unsigned short usDayTrade, unsigned short usTradeType, unsigned short usSpecialTradeType, int nQty,
	const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator,
    TCHAR* lpszMessage,	int *pnMessageBufferSize);

DLLImport int __stdcall SendOverseaFutureSpreadOrder( const TCHAR* lpszAccount, const TCHAR* lpszTradeName, const TCHAR* lpszStockNo, const TCHAR* lpszYearMonth, const TCHAR* lpszYearMonth2,
	unsigned short usBuySell, unsigned short usNewClose, unsigned short usDayTrade, unsigned short usTradeType, unsigned short usSpecialTradeType, int nQty,
	const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator,
    TCHAR* lpszMessage,	int *pnMessageBufferSize);

DLLImport int __stdcall SendOptionOrder( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usNewClose, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, TCHAR* lpszMessage, 
	int *pnMessageBufferSize);

DLLImport int __stdcall CancelOrderBySeqNo( const TCHAR* lpszAccount, const TCHAR* lpszSeqNo, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CancelOrderByStockNo( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall OverseaCancelOrderBySeqNo( const TCHAR* lpszAccount, const TCHAR* lpszSeqNo, TCHAR* lpszMessage, int* pnMessageBufferSize);
DLLImport int __stdcall OverseaCancelOrderByBookNo( const TCHAR* lpszAccount, const TCHAR* lpszBookNo, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall DecreaseOrderBySeqNo( const TCHAR* lpszAccount, const TCHAR* lpszSeqNo, int nDecreaseQty, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall OverseaDecreaseOrderBySeqNo( const TCHAR* lpszAccount, const TCHAR* lpszSeqNo, int nDecreaseQty, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall GetOverseaCount();
DLLImport int __stdcall GetOverseaProducts(int index,struct SOfComProduct* product);

DLLImport int __stdcall RegisterOnOpenInterestCallBack( long lCallBackFunction);
DLLImport int __stdcall GetOpenInterest(const TCHAR* lpszAccount);

DLLImport int __stdcall RegisterOnAccountCallBack( long lCallBackFunction);
DLLImport int __stdcall GetUserAccount();

DLLImport int __stdcall RegisterOnFutureRightCallBack( long lCallBackFunction);
DLLImport int __stdcall GetFutureRight(const TCHAR* lpszAccount);

DLLImport int __stdcall RegisterOnStopLossReportCallBack( long lCallBackFunction);
DLLImport int __stdcall GetStopLossReport(const TCHAR* lpszAccount,int nReportStatus,const TCHAR* lpszType);
	
DLLImport int __stdcall RegisterOnRealBalanceReportCallBack( long lCallBackFunction);
DLLImport int __stdcall GetRealBalanceReport(const TCHAR* lpszAccount);
					

DLLImport int __stdcall SendFutureStopLoss( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usDayTrade, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, const TCHAR* lpszTrigger, 
	TCHAR* lpszMessage, int *pnMessageBufferSize);

DLLImport int __stdcall SendOptionStopLoss( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usDayTrade, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, const TCHAR* lpszTrigger, 
	TCHAR* lpszMessage, int *pnMessageBufferSize);

DLLImport int __stdcall SendMovingStopLoss( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType,
	unsigned short usDayTrade, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty, const TCHAR* lpszMovingPoint, 
	TCHAR* lpszMessage, int *pnMessageBufferSize);

DLLImport int __stdcall SendQuoteOrder( const TCHAR* lpszAccount, short sMarket, const TCHAR* lpszStockNo, 
	unsigned short usTradeType, const TCHAR* lpszBidPrice, int nBidQty, const TCHAR* lpszAskPrice,
	int nAskQty, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CancelFutureStopLoss( const TCHAR* lpszAccount, const TCHAR* lpszBookNo,
   const TCHAR* lpszSymbol, const TCHAR* lpszBuySell, const TCHAR* lpszPrice,
   const TCHAR* lpszQty, const TCHAR* lpszTriggerPrice, const TCHAR* lpszTradeType, 
   const TCHAR* lpszDayTrade, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CancelMovingStopLoss( const TCHAR* lpszAccount, const TCHAR* lpszBookNo,
   const TCHAR* lpszSymbol, const TCHAR* lpszBuySell, const TCHAR* lpszPrice,
   const TCHAR* lpszQty, const TCHAR* lpszTriggerPrice, const TCHAR* lpszTradeType, 
   const TCHAR* lpszDayTrade, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CancelOptionStopLoss( const TCHAR* lpszAccount, const TCHAR* lpszBookNo,
   const TCHAR* lpszSymbol, const TCHAR* lpszBuySell, const TCHAR* lpszPrice,
   const TCHAR* lpszQty, const TCHAR* lpszTriggerPrice, const TCHAR* lpszTradeType, 
   const TCHAR* lpszDayTrade, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CheckForeignStockOrderPara( const TCHAR* lpszAccount, const TCHAR* lpszStockNo,const TCHAR* lpszExchangeNo, unsigned short usBuySell, const TCHAR* lpszPrice, 
													 int nQty, const TCHAR* lpszCurrency1, const TCHAR* lpszCurrency2, const TCHAR* lpszCurrency3, int iAccountType , TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall SendForeignStockOrder( const TCHAR* lpszAccount, const TCHAR* lpszStockNo,const TCHAR* lpszExchangeNo, 
			unsigned short usBuySell, const TCHAR* lpszPrice, int nQty,const TCHAR* lpszCurrency1, const TCHAR* lpszCurrency2, 
			const TCHAR* lpszCurrency3, int iAccountType, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall SendForeignStockOrderAsync( const TCHAR* lpszAccount, const TCHAR* lpszStockNo,const TCHAR* lpszExchangeNo, 
			unsigned short usBuySell, const TCHAR* lpszPrice, int nQty,const TCHAR* lpszCurrency1, const TCHAR* lpszCurrency2, const TCHAR* lpszCurrency3, int iAccountType, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CancelForeignStockOrderByBookNo( const TCHAR* lpszAccount, const TCHAR* lpszBookNo,const TCHAR* lpszExchangeNo, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall CancelForeignStockOrderBySeqNo( const TCHAR* lpszAccount, const TCHAR* lpszSeqNo,const  TCHAR* lpszExchangeNo, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall GetEfficientForeignStockForInitial(const TCHAR* lpszStockNo);

DLLImport int __stdcall RegisterOnForeignStocksCallBack( long lCallBackFunction);

DLLImport void __stdcall GetOSStockNoandName();

DLLImport void __stdcall ErrorResult(TCHAR* lpszResult, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall RegisterOnOverseaFutureOpenInterestCallBack( long lCallBackFunction);
DLLImport int __stdcall GetOverseaFutureOpenInterest( const TCHAR* lpszAccount);

DLLImport int __stdcall RegisterOnOrderAsyncReportCallBack( long lCallBackFunction);
DLLImport int __stdcall SendOverseaFutureOrderAsync( const TCHAR* lpszAccount, const TCHAR* lpszTradeName, const TCHAR* lpszStockNo, const TCHAR* lpszYearMonth,
	unsigned short usBuySell, unsigned short usNewClose, unsigned short usDayTrade, unsigned short usTradeType, unsigned short usSpecialTradeType, int nQty,
	const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator);

DLLImport int __stdcall SendOverseaFutureSpreadOrderAsync ( const TCHAR* lpszAccount, const TCHAR* lpszTradeName, const TCHAR* lpszStockNo, const TCHAR* lpszYearMonth, const TCHAR* lpszYearMonth2,
	unsigned short usBuySell, unsigned short usNewClose, unsigned short usDayTrade, unsigned short usTradeType, unsigned short usSpecialTradeType, int nQty,
	const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator);

DLLImport int __stdcall RegisterOnOverseaFuturesCallBack( long lCallBackFunction);
DLLImport int __stdcall GetOverseaFutures();

DLLImport int __stdcall UnlockOrder( TCHAR* lpszMarketType );

DLLImport int __stdcall RegisterOnExecutionReportCallBack( long lCallBackFunction);
DLLImport int __stdcall GetExecutionReport(TCHAR* lpszAccount,TCHAR* lpszStockNo,int nMarket, int nBuySell,int nDataNum, int nType);

DLLImport int __stdcall CorrectPriceBySeqNo( const TCHAR* lpszAccount, const TCHAR* lpszSeqNo, const TCHAR* lpszPrice, unsigned short usTradeType, TCHAR* lpszMessage, int* pnMessageBufferSize);
DLLImport int __stdcall CorrectPriceByBookNo( const TCHAR* lpszAccount, const TCHAR* lpszMarketSymbol, const TCHAR* lpszBookNo, const TCHAR* lpszPrice, unsigned short usTradeType, TCHAR* lpszMessage, int* pnMessageBufferSize);

DLLImport int __stdcall RegisterOnAccountWithLoginIDCallBack( long lCallBackFunction);


DLLImport int __stdcall RegisterOnOverseaOptionsCallBack( long lCallBackFunction);
DLLImport int __stdcall GetOverseaOptions();

DLLImport int __stdcall SendOverseaOptionOrder( const TCHAR* lpszAccount, const TCHAR* lpszTradeName, const TCHAR* lpszStockNo, const TCHAR* lpszYearMonth, const TCHAR* lpszStrikePrice, const TCHAR* lpszCallPut,
	unsigned short usBuySell, unsigned short usNewClose, unsigned short usDayTrade, unsigned short usTradeType, unsigned short usSpecialTradeType, int nQty,
	const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator,
    TCHAR* lpszMessage,	int *pnMessageBufferSize);

DLLImport int __stdcall SendOverseaOptionOrderAsync( const TCHAR* lpszAccount, const TCHAR* lpszTradeName, const TCHAR* lpszStockNo, const TCHAR* lpszYearMonth, const TCHAR* lpszStrikePrice, const TCHAR* lpszCallPut,
	unsigned short usBuySell, unsigned short usNewClose, unsigned short usDayTrade, unsigned short usTradeType, unsigned short usSpecialTradeType, int nQty,
	const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator);

DLLImport int __stdcall OverseaCorrectPriceByBookNo( const TCHAR* lpszAccount, const TCHAR* lpszBookNo, unsigned short usSpecialTradeType,const TCHAR* lpszOrder,const TCHAR* lpszOrderNumerator,const TCHAR* lpszTrigger,const TCHAR* lpszTriggerNumerator, TCHAR* lpszMessage, int* pnMessageBufferSize);


DLLImport int __stdcall RegisterOnCertCanceledCallBack( long lCallBackFunction);

#endif