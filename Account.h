#include <string>
#include <deque>
#include <map>
#include <list>
using namespace std;
#define Long_position 0
#define Short_position 1

enum Order_Type { OP_BUY = 0, OP_SHORT, OP_BUYLIMIT, OP_SHORTLIMIT, OP_BUYSTOP, OP_SHORTSTOP };

struct TOrder_info {
	int order_type, position_type;
	int lots;
	int entry_tick, exit_tick;
	double open_price, close_price;

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

	/*place a order with market price¡Astage into pending status at tick==nPtr and deal at next tick*/
	int Place_Open_Order ( string symbol, int nPtr, int position_type/*, int nTime,int nBid, int nAsk, int nClose, int nQty*/ );
	map< string, list<TOrder_info>* > mMap_open_order, mMap_close_order;
};