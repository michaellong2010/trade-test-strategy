#include <string>
#include <deque>
#include <map>
#include <list>
#include <fstream>
using namespace std;
#define Long_position 0
#define Short_position 1

enum Order_Type { OP_BUY = 0, OP_SHORT, OP_BUYLIMIT, OP_SHORTLIMIT, OP_BUYSTOP, OP_SHORTSTOP };

struct TOrder_info {
	int order_type, position_type;
	int lots;
	int entry_tick, exit_tick;
	double open_price, close_price;
	double max_profit, max_loss, profit_loss;
	char ticker_symbol [ 15 ];

	TOrder_info() {
		entry_tick = exit_tick = -1;
		open_price = close_price = 0;
	}
};

class CAccount {
private:
	/*margin, free-margin, equity*/
	double margin, free_margin, equity;
public:
	CAccount();
	~CAccount();

	/*place a order with market price¡Astage into pending status at tick==nPtr and deal at next tick*/
	int Place_Open_Order ( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill, int position_type );
	map< string, list<TOrder_info>* > mMap_open_order, mMap_close_order;
	map< string, double > mMap_origin_margin, mMap_keep_margin, mMap_perpip_value;

	fstream *p_portfolio_fs;
	ofstream txt_portfolio_fs;
	void refresh_portfolio();
};