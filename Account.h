#include <string>
#include <deque>
#include <map>
#include <list>
#include <fstream>
#include <tchar.h>
#include "stdafx.h"
#include <direct.h>
using namespace std;
#define Long_position 0
#define Short_position 1
#define Close_long_position 2
#define Close_short_position 3
#define Close_all_position 4

enum Order_Type { OP_BUY = 0, OP_SHORT, OP_BUYLIMIT, OP_SHORTLIMIT, OP_BUYSTOP, OP_SHORTSTOP };

struct TOrder_info {
	int order_type, position_type;
	int lots, open_time;
	int entry_tick, exit_tick;
	double open_price, close_price;
	double max_profit, max_loss, profit_loss;
	char ticker_symbol [ 15 ];
	int exit_reason;
	double MA10_15min, MA22_15min, MA10_day, MA22_day;

	TOrder_info() {
		entry_tick = exit_tick = -1;
		open_price = close_price = 0;
		MA10_15min = MA22_15min = MA10_day = MA22_day = 0;
	}
};

class CAccount {
private:
	/*margin, free-margin, equity*/
	double margin, free_margin, equity;
public:
	CAccount( string account_name );
	~CAccount();

	/*place a order with market priceĦAstage into pending status at tick==nPtr and deal at next tick*/
	int Place_Open_Order ( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill, int position_type, double MA10_15min, double MA22_15min, double MA10_day, double MA22_day );
	map< string, list<TOrder_info>* > mMap_open_order, mMap_close_order;
	map< string, double > mMap_origin_margin, mMap_keep_margin, mMap_perpip_value;

	fstream *p_portfolio_fs;
	ofstream txt_portfolio_fs;
	void refresh_portfolio();
};