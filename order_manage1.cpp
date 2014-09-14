#include "stdafx.h"
#include "order_manage1.h"
#include "Functions.h"

CCaOrderArray pCaOrders;

int Order::order_count = 0;
Order::Order()
{
	order_id = 0;
	state = Pending;
	pFirst_record = pLast_record = NULL;
	pStock_data = NULL;
	int datetime_index = -1;
}

Order::~Order()
{
	struct Order_OP_Record *next_record = NULL, *current_record = NULL;

	current_record = next_record = pFirst_record;
	while (current_record)
	{
		next_record = current_record->next_record;
		delete current_record;
		current_record = next_record;
	}
}

BOOL Order::IsLong()
{
	if (order_type==OP_BUY || order_type==OP_BUYLIMIT || order_type==OP_BUYSTOP)
		return true;
	else
		return false;
}

BOOL Order::IsShort()
{
	if (order_type==OP_SHORT || order_type==OP_SHORTLIMIT || order_type==OP_SHORTSTOP)
		return true;
	else
		return false;
}

BOOL Order::IsIOC()
{
	if (state==Pending && (order_type==OP_BUY || order_type==OP_SHORT))
		return true;
	else
		return false;
}

BOOL Order::IsPending()
{
	if (state==Pending && (order_type==OP_BUYLIMIT || order_type==OP_SHORTLIMIT || order_type==OP_BUYSTOP || order_type==OP_SHORTSTOP))
		return true;
	else
		return false;
}

BOOL Order::IsOpened()
{
	if (state==Opened)
		return true;
	else
		return false;
}

int Order::RefreshOrder(float date, float time)
{
	int current_datetime_index = 0;

	if ((date==pStock_data->pDate_num[datetime_index] && time > pStock_data->pTime_num[datetime_index]) || (date > pStock_data->pDate_num[datetime_index]))
	{
		current_datetime_index = pStock_data->find_datetime_index(date, time);
	}
	else
		AfxMessageBox("RefreshOrder: quote data too old");

	if (current_datetime_index >= 0)
	{
		if (state==Pending)
		{
//order manage rule for pending orders
			if (IsIOC())
			{
				if (IsLong() && pStock_data->pClose[current_datetime_index] < pLast_record->info.open_price)
				{
					OrderTrade(pLast_record->info.open_price, pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
				}
				else if (IsShort() && pStock_data->pClose[current_datetime_index] > pLast_record->info.open_price)
				{
					OrderTrade(pLast_record->info.open_price, pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
				}
				else
					OrderDelete(pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
			}
			else if (IsPending())
			{
				if (order_type==OP_BUYLIMIT && pStock_data->pClose[current_datetime_index] < pLast_record->info.open_price)
					OrderTrade(pLast_record->info.open_price, pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
				else if (order_type==OP_SHORTLIMIT && pStock_data->pClose[current_datetime_index] > pLast_record->info.open_price)
					OrderTrade(pLast_record->info.open_price, pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
				else if (order_type==OP_BUYSTOP && pStock_data->pClose[current_datetime_index] > pLast_record->info.open_price)
					OrderTrade(pLast_record->info.open_price, pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
				else if (order_type==OP_SHORTSTOP && pStock_data->pClose[current_datetime_index] < pLast_record->info.open_price)
					OrderTrade(pLast_record->info.open_price, pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
				else
					OrderDelete(pStock_data->pDate_num[current_datetime_index], pStock_data->pTime_num[current_datetime_index]);
			}
			else
				AfxMessageBox("RefreshOrder: can't refresh this order status");
		}
		else if (state==Opened)
		{
//order manage rule for opened orders
			OrderClose();
			//OrderModify();
		}
		else if (state==Exited)
		{

		}

		datetime_index = current_datetime_index;
	}
	else
		AfxMessageBox("RefreshOrder: can't find correct date&time index");
	return 0;
}

bool Order::AddOrderRecord(struct Order_OP_Record *new_record)
{
	if (!pFirst_record)
	{
		if (new_record->op==Create)
			pFirst_record = pLast_record = new_record;
		else
		{
			AfxMessageBox("The order record error: expected is a \"Creating\" record");
			return false;
		}
	}
	else
	{
		pLast_record->next_record = new_record;
		pLast_record = new_record;
	}
	state = pLast_record->next_state;
	return true;
}

int Order::OrderSend(char *symbol, int order_t, struct Order_Info *pinfo)
{
	strcpy(pszTicker, symbol);
	order_id = ++order_count;
	order_type = order_t;
	struct Order_OP_Record *pNewRec = NULL;
	pNewRec = new Order_OP_Record;
	if (!pNewRec)
	{
		AfxMessageBox("OrderSend: Allocate order record memory fail");
		return 1;
	}
	else
	{
		pNewRec->next_state = Pending;
		pNewRec->op = Create;
		pNewRec->next_record = NULL;
		memcpy(&pNewRec->info, pinfo, sizeof(struct Order_Info));
		AddOrderRecord(pNewRec);
		return 0;
	}
}

void Order::OrderModify()
{

}

int  Order::OrderTrade(float trade_price, float trade_date, float trade_time)
{
	struct Order_OP_Record *pNewRec = NULL;
	pNewRec = new Order_OP_Record;
	if (!pNewRec)
	{
		AfxMessageBox("OrderTrade: Allocate order record memory fail");
		return 1;
	}
	else
	{
		memcpy(pNewRec, pLast_record, sizeof(struct Order_OP_Record));
		pNewRec->op = Deal;
		pNewRec->info.deal_price = trade_price;
		pNewRec->info.deal_date = trade_date;
		pNewRec->info.deal_time = trade_time;
		pNewRec->next_state = Opened;
		pNewRec->next_record = NULL;
		AddOrderRecord(pNewRec);
		return 0;
	}
}

int Order::OrderDelete(float trade_date, float trade_time)
{
	struct Order_OP_Record *pNewRec = NULL;
	pNewRec = new Order_OP_Record;
	if (!pNewRec)
	{
		AfxMessageBox("OrderDelete: Allocate order record mem error");
		return 1;
	}
	else
	{
		memcpy(pNewRec, pLast_record, sizeof(struct Order_OP_Record));
		pNewRec->op = Cancel;
		pNewRec->info.close_date = trade_date;
		pNewRec->info.close_time = trade_time;
		pNewRec->next_state = Exited;
		pNewRec->next_record = NULL;
		AddOrderRecord(pNewRec);
		return 0;
	}
}

void Order::OrderClose()
{

}

char *Order::OrderSymbol()
{
	return pszTicker;
}

float Order::OrderOpenTime()
{
	return pFirst_record->info.open_time;
}

float Order::OrderOpenDate()
{
	return pLast_record->info.open_date;
}

float  Order::OrderOpenPrice()
{
	return pLast_record->info.open_price;
}

float Order::OrderLots()
{
	return pLast_record->info.volumn;
}

int Order::OrderType()
{
	return order_type;
}


/*
typedef int (*DLL_SKQuoteLib_IsConnected)();
typedef int (*DLL_SKQuoteLib_Connect)(char *caID, char *caPass);
DLL_SKQuoteLib_IsConnected ProcAdd_SKQuoteLib_IsConnected;
DLL_SKQuoteLib_Connect ProcAdd_SKQuoteLib_Connect;
HINSTANCE hinstLib = NULL;
BOOL fFreeResult;
extern void __stdcall OnConnect( int nErrorCode);
extern void __stdcall OnDisconnect( int nErrorCode);
extern void __stdcall OnData( BSTR bstrData);

int AT_Trade::CheckConnection()
{
	Quote_Connect();
	Reply_Connect();
	return  0;
}

//capital future quote&order&reply specific code
Capital_AT_Trade::Capital_AT_Trade()
{
	strcpy(caID, "S122811334");
	strcpy(caPass, "swat9110");
	hinstLib = LoadLibrary(TEXT(".\\Plugins\\Test.dll"));
	if (hinstLib != NULL)
	{
		ProcAdd_SKQuoteLib_IsConnected = (DLL_SKQuoteLib_IsConnected) GetProcAddress(hinstLib, "SKQuoteLib_IsConnected");
		ProcAdd_SKQuoteLib_Connect = (DLL_SKQuoteLib_Connect) GetProcAddress(hinstLib, "SKQuoteLib_Connect");

	}
}

Capital_AT_Trade::~Capital_AT_Trade()
{
	fFreeResult = FreeLibrary(hinstLib);
}

int Capital_AT_Trade::CheckConnection()
{
	return 0;
}

int Capital_AT_Trade::Quote_Connect()
{
	int nCode = 0;

	nCode = (ProcAdd_SKQuoteLib_IsConnected)();
	if (nCode==SK_Quote_Connect)
		return 0;
	else if (nCode==SK_Quote_Disconnect)
		nCode = ProcAdd_SKQuoteLib_Connect(caID, caPass);
	return nCode;
}

int Capital_AT_Trade::Reply_Init()
{
	int nCode = 0;
	TCHAR caBuf[512] = {0};
	try
	{
		nCode = SKReplyLib_Initialize( caID, caPass);

		if ( nCode == 0)
		{
			AfxGetMainWnd()->MessageBox( "Initialize successful", "Message", MB_ICONASTERISK|MB_TOPMOST);
		}
		else
		{
			sprintf( caBuf, "%s (%d)", "Initialize fail", nCode);
			AfxGetMainWnd()->MessageBox( caBuf, "Message", MB_ICONERROR|MB_TOPMOST);
		}
	}
	catch ( char* pMsg)
	{
		AfxGetMainWnd()->MessageBox( pMsg, "Initialize Error", MB_ICONASTERISK|MB_TOPMOST);
		nCode = 1;
	}
	catch (...)
	{
		nCode = 1;
	}

	if (nCode==0)
	{
		nCode += RegisterOnConnectCallBack( (long)OnConnect);
		nCode += RegisterOnDisconnectCallBack( (long)OnDisconnect);
		nCode += RegisterOnDataCallBack( (long)OnData);
		if (nCode==3)
			return 0;
		else
			return 1;
	}
	return 1;
}

int Capital_AT_Trade::Reply_Connect()
{
	// TODO: Add your control notification handler code here
	int nCode = 0;
	TCHAR caBuf[512] = {0};

	if ( SKReplyLib_IsConnected() == SK_Reply_NoneInit)
	{
		nCode = Reply_Init();
	}
	if ( nCode==0 && SKReplyLib_IsConnected() == SK_Reply_Disconnect)
	{
		nCode = SKReplyLib_Connect();

		if ( nCode != 1)
		{
			sprintf( caBuf, "%s (%d)", "connect fail", nCode);
			AfxGetMainWnd()->MessageBox( caBuf, "Message", MB_ICONERROR|MB_TOPMOST);
			return 1;
		}
		else
			return 0;
	}
}
*/