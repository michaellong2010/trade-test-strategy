#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
//#include <regex>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "Functions.h"

using namespace std;
using namespace std::tr1;

struct ByLength : public std::binary_function<string, string, bool>
{
    bool operator()(const string& lhs, const string& rhs) const
    {
        return lhs.length() < rhs.length();
    }
};

struct TKLineData_FileInfo {
	fstream *p_fs; // file stream
	long n_fsize; //
	int n_list_begin, n_list_end; //element<TCandleStick> range index within file stream，the range also sync in memory list<TCandleStick>
	TKLineData_FileInfo() {
		p_fs = NULL;
		n_fsize = n_list_begin = n_list_end = -1;
	}
};

struct TCandleStick {
	int mDate;
	int mTime;
	float mOpen, mHigh, mLow, mClose;
	int mVolumn;
	int operator == (int a) {
		return 0;
	}
	const bool operator == (const TCandleStick &a)  {
		if ( a.mDate == mDate && a.mTime == mTime )
			return true;
		else
			return false;
	}
};

/*handle multi-symbols & single time frame for each symbol*/
class CKLineStream {
private:
	static map<int, string> mTimeFrameName;
	static map<int, string> init_timeframe_map() {
		map<int, string> timeframe_name_map;

		timeframe_name_map[0] = "1min";
		timeframe_name_map[1] = "5min";
		timeframe_name_map[2] = "30min";
		timeframe_name_map[3] = "day";
		timeframe_name_map[4] = "full_day";
		timeframe_name_map[5] = "week";
		timeframe_name_map[6] = "month";

		return timeframe_name_map;
	}
	int nTimeFrame;
public:
	CKLineStream(int time_frame);	// 標準建構函式
	~CKLineStream();

	/*current focus stream file name synopsis： <stock_NO>_<year>_<time period of time frame>*/
	CString stream_file;
	fstream fios;

	int Push_KLine_Data(CString, CString);
	int Push_KLine_Data( char * caStockNo, char * caData );
	int Push_Tick_Data( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill );
	map<string, TKLineData_FileInfo *> file_stream_info;

	/*map stock symbol to it's link-list KLine data*/
	map<string, list<TCandleStick>*> mMap_stock_kline;

	/* split string */
	vector<string> tokenize( const char *input_str, char *delimiter );

	/* load KLine data from archive into `mMap_stock_kline`　when call capital API SKQuoteLib_GetKLine() */
	void load_KLine_from_archive ( char * ticker_symbol );

	ofstream txt_out;
	/*map stock symbol to reference it's vector<tick>*/
	map<string, vector<TICK>*> mMap_stock_ticks;
	map<string, int> mMap_intraday_open_time;
	map<string, list<TCandleStick>*> mMap_tick_compose_kline;
};