#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
//#include <regex>
#include <stdio.h>
#include <string.h>
#include <algorithm>

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

		timeframe_name_map[0] = "1分鐘線";
		timeframe_name_map[1] = "5分鐘線";
		timeframe_name_map[2] = "30分鐘線";
		timeframe_name_map[3] = "日線228天";
		timeframe_name_map[4] = "完整日線";
		timeframe_name_map[5] = "週線";
		timeframe_name_map[6] = "月線";

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
	map<string, TKLineData_FileInfo *> file_stream_info;

	/*map stock symbol to it's link-list KLine data*/
	map<string, list<TCandleStick>*> mMap_stock_kline;

	/* split string */
	vector<string> tokenize( const char *input_str, char *delimiter );

	/* load KLine data from archive into `mMap_stock_kline`　when call capital API SKQuoteLib_GetKLine() */
	void load_KLine_from_archive ( char * ticker_symbol );
};