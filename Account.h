#include "Order.h"
#include "OrderTesterDlg.h"
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
#define Close_stoploss_position 5

enum Order_Type { OP_BUY = 0, OP_SHORT, OP_BUYLIMIT, OP_SHORTLIMIT, OP_BUYSTOP, OP_SHORTSTOP };

struct TOrder_info {
	int order_type, position_type;
	int lots, open_time, open_date;
	int entry_tick, exit_tick;
	double open_price, close_price;
	double max_profit, max_loss, profit_loss;
	char ticker_symbol [ 15 ];
	int exit_reason;
	double MA10_15min, MA22_15min, MA10_day, MA22_day;
	double stoploss;

	TOrder_info() {
		entry_tick = exit_tick = -1;
		open_price = close_price = 0;
		MA10_15min = MA22_15min = MA10_day = MA22_day = 0;
	}
};

struct TStrategy_info {
	BOOL isConfigure;
	int nType, time_frame, n_sticks;
	int mMA1_period, mMA2_period, mMA3_period;
	string symbol;
	BOOL enable_MA_margin;
	double mMA1_margin, mMA2_margin, mMA3_margin;
	double m_cur_MA1_margin, m_cur_MA2_margin, m_cur_MA3_margin;
	BOOL m_en_stoploss, m_en_trailing_stop, m_en_trade_MA_ambigous;
	int m_stoploss;
	BOOL m_en_gap, m_bid_ask_weight_ratio, m_en_open_interest;
	BOOL m_simulation_only;
};

class CAccount {
private:
	/*margin, free-margin, equity*/
	double margin, free_margin, equity;
	double final_close;
	boolean m_en_trailing_stop, m_en_stoploss;
	double m_trailing_stop_points;
	int m_nPtr;
	int kline_close_time, m_stoploss_count, tradable_time;
	int nTimeFrame;
public:
	CAccount( string account_name, int time_frame );
	~CAccount();

	/*place a order with market price¡Astage into pending status at tick==nPtr and deal at next tick*/
	int Place_Open_Order ( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill, int position_type, double MA10_15min, double MA22_15min, double MA10_day, double MA22_day, int nDate );
	map< string, list<TOrder_info>* > mMap_open_order, mMap_close_order;
	map< string, double > mMap_origin_margin, mMap_keep_margin, mMap_perpip_value;

	fstream *p_portfolio_fs;
	ofstream txt_portfolio_fs;
	void refresh_portfolio( bool exit_trading );
	void update_kline_close_time ( int new_close_time );
	void set_stoploss ( int, int, int );

	/*20150106 added by michael*/
	static int m_Account_count;
	int m_Account_index;
	COrder *m_pOrder_operator;
	void bind_order_operator ( COrder * pOrder_operator );

	TStrategy_info m_Strategy;
	//double m_orig_MA1_margin, m_orig_MA2_margin, m_orig_MA3_margin;
	int m_current_tick_time;
	void reset( TStrategy_info &strategy );
};