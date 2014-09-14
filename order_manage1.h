#ifndef ORDER_MANAGE_H
#define ORDER_MANAGE_H 1



enum Order_Type { OP_BUY = 0, OP_SHORT, OP_BUYLIMIT, OP_SHORTLIMIT, OP_BUYSTOP, OP_SHORTSTOP };
enum Order_State { Pending, Opened, Exited };
enum Order_Operation { Create, Deal, Modify, Cancel, Close };

struct Order_Info
{
	int order_type;
	double volumn;
	float open_price, open_date, open_time;
	float deal_price, deal_date, deal_time;
	float close_price, close_date, close_time;
	int slippage;
	double stoploss;
	double takeprofit;
	int open_bar_index, deal_bar_index, close_bar_index;
};

struct Order_OP_Record
{
	Order_Operation op;
	Order_Info info;
	Order_State next_state;
	struct Order_OP_Record *next_record;
};

class Order
{
private:
	struct stock_data *pStock_data;
	int datetime_index;
	int order_id;
	char pszTicker[30];
	int order_type;
	Order_State state;
	struct Order_OP_Record *pFirst_record, *pLast_record;
public:
	Order();
	~Order();
	BOOL IsLong();
	BOOL IsShort();
	BOOL IsIOC();
	BOOL IsPending();
	BOOL IsOpened();
	int RefreshOrder(float date, float time);
	int OrderSend(char *symbol, int order_t, struct Order_Info *info);
	bool AddOrderRecord(struct Order_OP_Record *new_record);
	int OrderTrade(float trade_price, float trade_date, float trade_time);
	int OrderDelete(float trade_date, float trade_time);
	void OrderClose();
	void OrderModify();
	char *OrderSymbol();
	float OrderOpenTime();
	float OrderOpenDate();
	int OrderType();
	float OrderOpenPrice();
	float OrderLots();
	static int order_count;
};

class CaOrder : public Order {
  public:
    char caKeyNo[13];
    void Update_Order_Spool(struct DataItem *pData);
}
typedef CArray< class CaOrder, class CaOrder &> CCaOrderArray;
/*
#define SK_Quote_Connect		1
#define SK_Quote_Disconnect		0
#define SK_Quote
#define SK_Reply_Connect		1
#define SK_Reply_Disconnect		0
#define SK_Reply_NoneInit		SK_FAIL


class AT_Trade
{
public:
	AT_Trade() {};
	~AT_Trade() {};
	virtual int Order_Init()
	{
		return  0;
	};
	virtual int Reply_Init()
	{
		return  0;
	};
	virtual int Quote_Init()
	{
		return  0;
	};
	virtual int Quote_Connect()
	{
		return  0;
	};
	virtual int Reply_Connect()
	{
		return  0;
	};
	virtual int CheckConnection();
};

class Capital_AT_Trade:AT_Trade
{
	char caID[128];
	char caPass[32];
public:
	Capital_AT_Trade();
	~Capital_AT_Trade();
	virtual int Quote_Connect();
	virtual int Reply_Connect();
	virtual int CheckConnection();
	virtual int Reply_Init();
};*/


#endif