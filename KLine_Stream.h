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
#include <math.h>
#include <direct.h>

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
typedef TKLineData_FileInfo TTickData_FileInfo;

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

	bool search_tick_in_file( TTickData_FileInfo *, int, int, int & );
	bool search_KLine_in_file( TKLineData_FileInfo *pKLine_file_info, int lower_bound, int upper_bound, int &ins_pos );
	int n_collapse_sticks;
	int server_escape_seconds;
	bool store_tick_file;
	bool MA1_list_ready, MA2_list_ready;
	char path_buf [ 200 ];
public:
	CKLineStream(int time_frame, int n_sticks, bool need_store_tick);	// 標準建構函式
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

	/*map indicate if KLine data is ready*/
	map<string, bool> mMap_kline_ready;

	/* split string */
	vector<string> tokenize( const char *input_str, char *delimiter );

	/* load KLine data from archive into `mMap_stock_kline`　when call capital API SKQuoteLib_GetKLine() */
	void load_KLine_from_archive ( char * ticker_symbol );
	/*set/get KLine ready status of symbols*/
	void set_KLine_ready ( char * ticker_symbol );
	bool get_KLine_ready ( char * ticker_symbol );

	ofstream txt_out;
	/*map stock symbol to reference it's vector<tick>*/
	map<string, list<TICK>*> mMap_stock_ticks;
	map<string, int> mMap_intraday_open_time, mMap_KLine_open_time;
	map<string, list<TCandleStick>*> mMap_tick_compose_kline;
	map<string, TKLineData_FileInfo *> mMap_tickfile_stream_info;

	SYSTEMTIME kline_fetch_system_time;
	/*record fetch kline data server time*/
	void KLine_server_time( int total_secconds );
	int kline_fetch_server_time;

	/*collapse multiple candlestick form another time frame KLine*/
	void candlestick_collapse ( char * ticker_symbol );

	/*calculate mean average of KLine data MA15&MA22*/
	map<string, list<double>*> mMap_MA1, mMap_MA2;

	bool is_tick_in_kline;
	void sync_server_time ( int total_secconds );

	double mMA1_period, mMA2_period;
	void setting_MA ( int MA1_period, int MA2_period );
};