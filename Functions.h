#ifndef _Function_H_
#define _Function_H_


#define DLLImport _declspec(dllimport)

#define SK_SUCCESS								0
#define SK_FAIL									-1
#define SK_ERROR_STRING_LENGTH_NOT_ENOUGH		-2
#define SK_ERROR_SERVER_NOT_CONNECTED			-3
#define SK_ERROR_INITIALIZE_FAIL				-4
#define SK_ERROR_ACCOUNT_NOT_EXIST				1
#define SK_ERROR_ACCOUNT_MARKET_NOT_MATCH		2
#define SK_ERROR_PERIOD_OUT_OF_RANGE			3
#define SK_ERROR_FLAG_OUT_OF_RANGE				4
#define SK_ERROR_BUYSELL_OUT_OF_RANGE			5
#define SK_ERROR_ORDER_SERVER_INVALID			6
#define SK_ERROR_PERMISSION_DENIED				7
#define SK_KLINE_DATA_TYPE_NOT_FOUND			8
#define SK_ERROR_PERMISSION_TIMEOUT			    9


#define SK_SUBJECT_CONNECTION_CONNECTED			100
#define SK_SUBJECT_CONNECTION_DISCONNECT		101
#define SK_SUBJECT_QUOTE_PAGE_EXCEED			200
#define SK_SUBJECT_QUOTE_PAGE_INCORRECT			201
#define SK_SUBJECT_TICK_PAGE_EXCEED				210
#define SK_SUBJECT_TICK_PAGE_INCORRECT			211
#define SK_SUBJECT_TICK_STOCK_NOT_FOUND			212
#define SK_SUBJECT_BEST5_DATA_NOT_FOUND			213

struct STOCK
{
	short m_sStockidx;
	short m_sDecimal;
	short m_sTypeNo;

	char m_cMarketNo;
	char m_caStockNo[20];
	char m_caName[20];

	int m_nOpen;
	int m_nHigh;
	int m_nLow;
	int m_nClose;
	
	int m_nTickQty;	// 單量

	int m_nRef;		// 昨收、參考價

	int m_nBid;		// 買價
	int m_nBc;		// 買量
	int m_nAsk;		// 賣價
	int m_nAc;		// 賣量

	int m_nTBc;		// 買盤量
	int m_nTAc;		// 賣盤量

	int m_nFutureOI;	// 期貨未平倉 OI

	int m_nTQty;
	int m_nYQty;

	int m_nUp;
	int m_nDown;
};

struct BEST5
{
	int m_nBid1;
	int m_nBidQty1;
	int m_nBid2;
	int m_nBidQty2;
	int m_nBid3;
	int m_nBidQty3;
	int m_nBid4;
	int m_nBidQty4;
	int m_nBid5;
	int m_nBidQty5;

	int m_nExtendBid;
	int m_nExtendBidQty;

	int m_nAsk1;
	int m_nAskQty1;
	int m_nAsk2;
	int m_nAskQty2;
	int m_nAsk3;
	int m_nAskQty3;
	int m_nAsk4;
	int m_nAskQty4;
	int m_nAsk5;
	int m_nAskQty5;

	int m_nExtendAsk;
	int m_nExtendAskQty;

};

struct TICK
{
	int m_nPtr;
	int m_nTime;
	int m_nBid;
	int m_nAsk;
	int m_nClose;
	int m_nQty;

	const bool operator == (const int &a) {
		if ( m_nPtr == a)
			return true;
		else
			return false;
	}

	const bool operator == (const TICK &a)  {
		if ( a.m_nPtr == m_nPtr && a.m_nTime == m_nTime )
			return true;
		else
			return false;
	}
};

typedef STOCK TStock;
typedef BEST5 TBest5;
typedef TICK  TTick;


DLLImport int __stdcall SKQuoteLib_Initialize( const char* pcUserName, const char* pcPassword);


DLLImport int __stdcall SKQuoteLib_EnterMonitor();
DLLImport int __stdcall SKQuoteLib_LeaveMonitor();
DLLImport int __stdcall SKQuoteLib_RequestServerTime();
DLLImport int __stdcall SKQuoteLib_GetVersion( char* lpszVersion, int* pnLen);	
DLLImport int __stdcall SKQuoteLib_AttachConnectionCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachQuoteCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachBest5CallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachTicksCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachStrikePricesCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachKLineDataCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachTicksGetCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachBest5GetCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttchServerTimeCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachHistoryTicksGetCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachProductsReadyCallBack( UINT_PTR lCallBack);



DLLImport int __stdcall SKQuoteLib_AttachMarketTotCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachMarketBuySellCallBack( UINT_PTR lCallBack);
DLLImport int __stdcall SKQuoteLib_AttachMarketHighLowCallBack( UINT_PTR lCallBack);

DLLImport int __stdcall SKQuoteLib_RequestStocks( short* pnPageNo, char* pStockNos);
DLLImport int __stdcall SKQuoteLib_GetStockByIndex( short sMarketNo, short sIndex, TStock* pStock);
DLLImport int __stdcall SKQuoteLib_GetStockByNo( const char* lpszStockNo, TStock* pStock);
DLLImport int __stdcall SKQuoteLib_RequestTicks( short* pnPageNo, char* pStockNo);
DLLImport int __stdcall SKQuoteLib_GetTick( short sMarketNo, short sStockidx, int nPtr, TTick* pTick);					
DLLImport int __stdcall SKQuoteLib_GetBest5( short sMarketNo, short sStockidx, TBest5* pBest5); 
DLLImport int __stdcall SKQuoteLib_GetKLine( char * caStockNo,int KLineType);
DLLImport int __stdcall SKQuoteLib_GetStrikePrices();

DLLImport int __stdcall SKQuoteLib_GetMarketTot( int index, char cMarketNo, short* sPrt, long* lTime, long* lTotv, long* lTots, long* lTotc );
DLLImport int __stdcall SKQuoteLib_GetMarketBuySell( int index, char cMarketNo, short* sPrt, long* lTime, long* lBc, long* lSc, long* lBs, long* lSs );
DLLImport int __stdcall SKQuoteLib_GetMarketHighLow( int index, char cMarketNo, short* sPrt, long* lTime, short* sUp, short* sDown, short* sHigh, short* sLow, short* sNoChange );


DLLImport int __stdcall SKQuoteLib_Delta( int nCallPut, double S, double K, double R, double T, double sigma, double* dDelta);
DLLImport int __stdcall SKQuoteLib_Gamma( double S, double K, double R, double T, double sigma,double* dGamma);
DLLImport int __stdcall SKQuoteLib_Theta( int nCallPut, double S, double K, double R, double T, double sigma, double* dTheta);
DLLImport int __stdcall SKQuoteLib_Vega( double S, double K, double R, double T, double sigma, double* dVega);
DLLImport int __stdcall SKQuoteLib_Rho( int nCallPut, double S, double K, double R, double T, double sigma, double* dRho);



#endif