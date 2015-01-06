#include "stdafx.h"
#include "Functions.h"
#include "KLine_Stream.h"

TCandleStick m_candlestick;
TICK m_tick;
extern DWORD g_ThreadID_KLine [ 3 ];

map<int, string> CKLineStream::mTimeFrameName = init_timeframe_map();
int days_difference ( int d1, int m1, int y1, int d2, int m2, int y2 );
int next_trade_date ( int current_date );
int previous_nDay_date ( int current_date,  int days );
CKLineStream::CKLineStream( int time_frame, int n_sticks, bool need_store_tick ) {
	stream_file = "";
	//ios::binary=
	nTimeFrame = time_frame;
	//txt_out.open( "c:\\temp\\TX00_txt", ios::out | ios::ate | ios::trunc );
	n_collapse_sticks = n_sticks;
	is_tick_in_kline = false;
	store_tick_file = need_store_tick;
	MA1_list_ready = MA2_list_ready = false;

	TCHAR path [ 200 ];
	GetCurrentDirectory ( 200, path );	
	//path_bufW = path;
	//path_bufW = path_bufW + _T("\\data\\");
	path_bufA = path;
	path_bufA = path_bufA + "\\data\\";
	path_bufW = path_bufA;
	//wcstombs( path_buf, path, sizeof(path_buf) );
	//strcpy ( path_buf, path );
	//strcat ( path_buf, "\\data\\");
	_mkdir ( path_bufA.GetString() );
	int n = days_difference ( 1, 1, 2012, 31, 12, 2012 );
	n = days_difference ( 30, 12, 2012, 31, 12, 2012 );
}

CKLineStream::~CKLineStream() {
	int nSize;
	fstream *p_fs;

	for (map<string, list<TCandleStick>*>::iterator itr = mMap_stock_kline.begin();  itr != mMap_stock_kline.end(); itr++) {
		nSize = sizeof((*itr).first);
		nSize = sizeof((*itr).second);
		if ( nSize == 4 )
			delete (*itr).second;
	}

	/*cloase all file stream*/
	for ( map<string, TKLineData_FileInfo *>::iterator itr1 = file_stream_info.begin(); itr1 != file_stream_info.end(); itr1++ ) {
		p_fs = ((*itr1).second)->p_fs;
		p_fs->flush();
		p_fs->close();
		nSize = sizeof((*itr1).second);
		delete ((*itr1).second->p_fs);
		if ( nSize == 4 )
			delete (*itr1).second;
	}

	for ( map<string, list<TICK>*>::iterator itr = mMap_stock_ticks.begin(); itr != mMap_stock_ticks.end(); itr++ ) {
		delete (*itr).second;

	}

	for ( map<string, list<TCandleStick>*>::iterator itr = mMap_tick_compose_kline.begin(); itr != mMap_tick_compose_kline.end(); itr++ ) {
		delete (*itr).second;
	}

	for ( map<string, TKLineData_FileInfo *>::iterator itr1 = mMap_tickfile_stream_info.begin(); itr1 != mMap_tickfile_stream_info.end(); itr1++ ) {
		p_fs = ((*itr1).second)->p_fs;
		p_fs->flush();
		p_fs->close();
		nSize = sizeof((*itr1).second);
		delete ((*itr1).second->p_fs);
		if ( nSize == 4 )
			delete (*itr1).second;
	}

	for ( map<string, list<double>*>::iterator itr1 = mMap_MA1.begin(); itr1 != mMap_MA1.end(); itr1++ )
		delete (*itr1).second;
	for ( map<string, list<double>*>::iterator itr1 = mMap_MA2.begin(); itr1 != mMap_MA2.end(); itr1++ )
		delete (*itr1).second;

	if ( txt_out.is_open() ) {
		txt_out.flush();
		txt_out.close();
	}
}

int CKLineStream::Push_KLine_Data(CString Stock_No, CString Data) {
	/*refer capital API OnNotifyKLineData( char * caStockNo, char * caData )*/
	CString fname, strToken, str;
	int nTokenPos = 0, nTokenNum = 0;
    //regex expr;

	
	fname = Stock_No;
	//fios.get
	if ( this->fios.is_open() == true )
		str = Data;
	else
		str = Data;
	do
	{
		// do something with strToken
		// ....
		strToken = str.Tokenize(_T(", "), nTokenPos);
		nTokenNum++;

		if ( strToken != "" ) {
			switch (nTokenNum) {
			
			}
		}

	}while (!strToken.IsEmpty());

	return 0;
}

int CKLineStream::Push_KLine_Data( char * caStockNo, char * caData ) {
	string m_str_symbol;
	list<TCandleStick> *pList_KLineData;

    m_str_symbol = caStockNo;


	/* tokenize caData，ther delimiter is ", "
	06/06/2014, 08:50, 913800, 914800, 913700, 914600, 3964 */
	char * pch, buf_str[100];
	const char *m_get_str;
	vector<string> mData_Element, mTok_date, mTok_time;
	string m_str_element;
	int i = 0, j = 0, comsume_tok = 1, current_date;
	SYSTEMTIME ti;

	if ( strlen(caData) > 0 ) {
	mData_Element = tokenize( caData, " ," );
	/*strcpy( buf_str, caData );
	pch = strtok ( buf_str," ," );
	while( pch != NULL ) {
		m_str_element = pch;
		mData_Element.push_back(m_str_element);
		pch = strtok (NULL, " ,.-");
	}*/

	i = 0;
	//GetSystemTime ( &ti ) ;
	::GetLocalTime ( &ti );
	current_date = 10000 * ti.wYear + 100 * ti.wMonth + ti.wDay;
	if ( ti.wDayOfWeek == 0 ) {
		current_date = previous_nDay_date ( current_date, 2 );
	}
	else
		if ( ti.wDayOfWeek == 6 ) {
			current_date = previous_nDay_date ( current_date, 1 );
		}
		else
			if ( ti.wDayOfWeek == 1 && server_escape_seconds < ( ( 8 * 60 + 30 ) * 60 ) ) {
				current_date = previous_nDay_date ( current_date, 3 );
			}
	int day, month, year;
	for ( vector<string>::iterator itr = mData_Element.begin(); itr != mData_Element.end(); ) {
	   m_get_str = (*itr).c_str();
	   switch (i) {
		 case 0:  //parse date mm/dd/yyyy
             mTok_date = tokenize( m_get_str, " \/" );
			 day = atoi(mTok_date.at(1).c_str());
			 month = atoi(mTok_date.at(0).c_str());
			 year = atoi(mTok_date.at(2).c_str());
			 m_candlestick.mDate = 100 * month + day + 10000 * year;
			 if ( current_date == m_candlestick.mDate )
				 return 0;
			 else
				 if ( current_date > m_candlestick.mDate && days_difference ( day, month, year, ti.wDay, ti.wMonth, ti.wYear ) == 1 && server_escape_seconds < ( ( 8 * 60 + 30 ) * 60 ) )
					 return 0;
			 break;
		 case 1:  //parse time hh:mm
			 if ( !strchr( m_get_str, ':' ) ) {
				 m_candlestick.mTime = 0;
				 comsume_tok = 0;
			 }
			 else {
				 mTok_time = tokenize( m_get_str, " :" );
				 m_candlestick.mTime = 60 * atoi(mTok_time.at(0).c_str()) + atoi(mTok_time.at(1).c_str());
				 comsume_tok = 1;
			 }
			 break;
		 case 2:
			 m_candlestick.mOpen = atof( m_get_str ) / 100;
			 comsume_tok = 1;
			 break;
		 case 3:
			 m_candlestick.mHigh = atof( m_get_str ) / 100;
			 break;
		 case 4:
			 m_candlestick.mLow = atof( m_get_str ) / 100;
			 break;
		 case 5:
			 m_candlestick.mClose = atof( m_get_str ) / 100;
			 break;
		 case 6:
			 m_candlestick.mVolumn = atoi( m_get_str );
			 break;
	   }
	   i++;

	   if ( comsume_tok )
		   itr++;
	}
	}

	map<string, list<TCandleStick>*>::iterator itr = mMap_stock_kline.find(m_str_symbol);
	/*test if a key exist*/
	if ( itr == mMap_stock_kline.end() || !mMap_stock_kline.count(m_str_symbol) ) {
		pList_KLineData = new list<TCandleStick>;
		mMap_stock_kline.insert(pair<string, list<TCandleStick>*>(m_str_symbol, pList_KLineData));
	}
	else {
		pList_KLineData = (*itr).second;
	}

	/*exclude and guarantee unique element add a candlestick to list*/
	int myints[] = { 10, 20, 30, 40 };
	int *p;
	p = std::find( myints, myints + 4, 30 );
	list<TCandleStick>::iterator dup_itr; // = std::find(pList_KLineData->begin(), pList_KLineData->end(), m_candlestick);
	list<TCandleStick>::iterator itr1, itr2;
	//*p = m_candlestick == 5;
	//*p = sizeof(TCandleStick);
	/*std::vector<int> myvector (myints,myints+4);
	std::vector<int>::iterator dup_itr;
	dup_itr = find (myvector.begin(), myvector.end(), 30);
	p = &myvector.at(0);*/

	string m_str_filename;
	fstream *p_fs;
	TKLineData_FileInfo *pKLine_file_info;
	TCandleStick m_file_candlestick;
	long f_size, n_bytes;
	int end, n_insert_pos, n_insert_pos1;
	n_bytes = sizeof(TCandleStick);
	DWORD cur_tid;
	cur_tid = GetCurrentThreadId();
	CStringW KLine_filenameW;

	
	//if ( dup_itr == pList_KLineData->end() ) {
		//pList_KLineData->push_back(m_candlestick);
		/*sync list<TCandleStick> with KLine data file stream*/
		//m_str_filename = "I:\\2014 VC project\\Trade_Test\\Debug\\data\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
		//m_str_filename = "C:\\temp\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	m_str_filename = path_bufA.GetString() + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	KLine_filenameW = m_str_filename.c_str();
		if ( !file_stream_info.count( m_str_filename ) ) {
			p_fs = new fstream( KLine_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate );
			pKLine_file_info = new TKLineData_FileInfo();
			pKLine_file_info->p_fs = p_fs;
			pKLine_file_info->n_fsize = f_size = p_fs->tellg();
			//pKLine_file_info->n_list_end = pKLine_file_info->n_fsize / sizeof( TCandleStick ) - 1;
			n_bytes = sizeof(TCandleStick);
			end = 1;

			while ( ( f_size - end*n_bytes ) >=  0 && end < 1000 ) {
				p_fs->seekg ( ( f_size - end*n_bytes ), ios::beg );
				p_fs->read ( (char *) &m_file_candlestick, n_bytes );
				/*if ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime == m_file_candlestick.mTime ) {
				}
				else
					if ( m_candlestick.mDate > m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime > m_file_candlestick.mTime ) ) {
					}
					else
						if ( m_candlestick.mDate < m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime < m_file_candlestick.mTime ) ) {
						}*/
			}
			file_stream_info.insert( std::pair<string, TKLineData_FileInfo *> ( m_str_filename, pKLine_file_info ));
		}
		else {
			map<string, TKLineData_FileInfo *>::iterator itr =  file_stream_info.find( m_str_filename );
			pKLine_file_info = (*itr).second;
			p_fs = pKLine_file_info->p_fs;
		}
		/*if ( pList_KLineData->size() == 2 ) {
			itr1 = pList_KLineData->begin();
			itr2 = pList_KLineData->end();
			itr2--;
			//nTimeFrame = (*itr2).mTime - (*itr1).mTime;
		}
		else
			if ( pList_KLineData->size() > 2 ) {
			
			}*/

		if ( pList_KLineData->size() == 0 ) {
			pList_KLineData->push_back(m_candlestick);
			pKLine_file_info->n_fsize += n_bytes;
			pKLine_file_info->n_list_begin = pKLine_file_info->n_list_end = 0;
			p_fs->seekp ( 0, ios::beg );
			p_fs->write ( (char *) &m_candlestick, n_bytes );
#if 0
			if ( GetCurrentThreadId() == g_ThreadID_KLine[ 0 ] ) {
			txt_out << m_candlestick.mDate << ", ";
			txt_out << m_candlestick.mTime << ", ";
			txt_out << m_candlestick.mOpen << ", ";
			txt_out << m_candlestick.mHigh << ", ";
			txt_out << m_candlestick.mLow << ", ";
			txt_out << m_candlestick.mClose << ", ";
			txt_out << m_candlestick.mVolumn << "\n";
			}
#endif
		}
		else {
			itr1 = pList_KLineData->begin();
			itr2 = pList_KLineData->end();
			/*itr2--;			
			m_file_candlestick = (*itr2);
			if ( m_candlestick.mDate > m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime > m_file_candlestick.mTime ) ) {
				n_insert_pos = pKLine_file_info->n_list_begin + pList_KLineData->size();
				pList_KLineData->insert ( pList_KLineData->end(), m_candlestick );
				p_fs->seekp ( n_insert_pos * n_bytes, ios::beg );
				p_fs->write ( (char *) &m_file_candlestick, n_bytes );
			}
			else {
				m_file_candlestick = (*itr1);
			}*/

/*search candlestick in list or file*/
			bool found_in_file = false, found_in_list = false;
			for ( n_insert_pos = pKLine_file_info->n_list_begin; itr1 != itr2; itr1++, n_insert_pos++ ) {
				m_file_candlestick = (*itr1);
				if ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime == m_file_candlestick.mTime ) {
					found_in_list = true;
					break;
				}
				else
					if ( m_candlestick.mDate > m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime > m_file_candlestick.mTime ) ) {
					}
					else
						if ( m_candlestick.mDate < m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime < m_file_candlestick.mTime ) ) {
							break;
						}
			}

			if ( found_in_list == false) {
				if ( itr1 == pList_KLineData->begin() ) {
					i = 0;
					found_in_file = search_KLine_in_file( pKLine_file_info, 0, n_insert_pos - 1, i );
					/*for ( i = 0; i < n_insert_pos; i++ ) {
						p_fs->seekg ( i*n_bytes, ios::beg );
						p_fs->read ( (char *) &m_file_candlestick, n_bytes );
						if ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime == m_file_candlestick.mTime ) {
							found_in_file = true;
							break;
						}
						else
							if ( m_candlestick.mDate > m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime > m_file_candlestick.mTime ) ) {
							}
							else
								if ( m_candlestick.mDate < m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime < m_file_candlestick.mTime ) ) {
									break;
								}
					}*/
					if ( found_in_file == false ) {
						n_insert_pos1 = i;
						for ( j = pKLine_file_info->n_list_end; j >= i; j-- ) {
							p_fs->seekg ( j*n_bytes, ios::beg );
							p_fs->read ( (char *) &m_file_candlestick, n_bytes );
							p_fs->seekp ( (j+1)*n_bytes, ios::beg );
							p_fs->write ( (char *) &m_file_candlestick, n_bytes );
						}
						p_fs->seekp ( (n_insert_pos1)*n_bytes, ios::beg );
						p_fs->write ( (char *) &m_candlestick, n_bytes );
#if 0
						if ( GetCurrentThreadId() == g_ThreadID_KLine[ 0 ] ) {
						txt_out << m_candlestick.mDate << ", ";
						txt_out << m_candlestick.mTime << ", ";
						txt_out << m_candlestick.mOpen << ", ";
						txt_out << m_candlestick.mHigh << ", ";
						txt_out << m_candlestick.mLow << ", ";
						txt_out << m_candlestick.mClose << ", ";
						txt_out << m_candlestick.mVolumn << "\n";
						}
#endif
						pKLine_file_info->n_list_end++;
						pKLine_file_info->n_list_begin++;
						pKLine_file_info->n_fsize = ( pKLine_file_info->n_list_end + 1 ) * n_bytes;
						if ( ( pKLine_file_info->n_list_end - pKLine_file_info->n_list_begin + 1 ) < 1000 ) {
							p_fs->seekg ( n_insert_pos*n_bytes, ios::beg );
							p_fs->read ( (char *) &m_file_candlestick, n_bytes );
							pList_KLineData->insert ( pList_KLineData->begin(), m_file_candlestick );
							pKLine_file_info->n_list_begin--;
						}
					}
				}
				else {
					pList_KLineData->insert ( itr1, m_candlestick );
					itr1--;
					itr2 = pList_KLineData->end();
					for ( ; itr1 != itr2; itr1++, n_insert_pos++ ) {
						m_file_candlestick = (*itr1);
						//pKLine_file_info->n_fsize += n_bytes;
						p_fs->seekp ( n_insert_pos*n_bytes, ios::beg );
						p_fs->write ( (char *) &m_file_candlestick, n_bytes );
#if 0
						if ( GetCurrentThreadId() == g_ThreadID_KLine[ 0 ] ) {
						txt_out << m_file_candlestick.mDate << ", ";
						txt_out << m_file_candlestick.mTime << ", ";
						txt_out << m_file_candlestick.mOpen << ", ";
						txt_out << m_file_candlestick.mHigh << ", ";
						txt_out << m_file_candlestick.mLow << ", ";
						txt_out << m_file_candlestick.mClose << ", ";
						txt_out << m_file_candlestick.mVolumn << "\n";
						}
#endif
					}
					pKLine_file_info->n_list_end++;
					pKLine_file_info->n_fsize = ( pKLine_file_info->n_list_end + 1 ) * n_bytes;
					if ( ( pKLine_file_info->n_list_end - pKLine_file_info->n_list_begin + 1 ) > 1000 ) {
						pKLine_file_info->n_list_begin++;
						pList_KLineData->pop_front();
					}
					else {
						if ( m_candlestick.mTime < mMap_KLine_open_time [ m_str_symbol ] )
							mMap_KLine_open_time [ m_str_symbol ] = m_candlestick.mTime;
					}
				}
			}
		}
	//}
	p_fs->flush();
	*p = pList_KLineData->size();
	/*TStock *pStock;

	pStock = new TStock();*/
	return 0;
}

bool CKLineStream::search_KLine_in_file( TKLineData_FileInfo *pKLine_file_info, int lower_bound, int upper_bound, int &ins_pos ) {
	TCandleStick m_middle_candletick;
	fstream *p_fs;
	int middle;

	p_fs = pKLine_file_info->p_fs;
	if ( lower_bound > upper_bound ) {
		return false;
	}
	else {
		middle = ( lower_bound + upper_bound ) / 2;
		p_fs->seekg ( middle*sizeof(TCandleStick), ios::beg );
		p_fs->read ( (char *) &m_middle_candletick, sizeof(TCandleStick) );

		if ( lower_bound == upper_bound ) {
			if ( m_candlestick.mDate == m_middle_candletick.mDate && m_candlestick.mTime == m_middle_candletick.mTime ) {
				ins_pos = lower_bound;
				return true;
			}
			else
				if ( m_candlestick.mDate > m_middle_candletick.mDate || ( m_candlestick.mDate == m_middle_candletick.mDate && m_candlestick.mTime > m_middle_candletick.mTime ) ) {
					//return lower_bound + 1;
					ins_pos = lower_bound + 1;
					return false;

				}
				else
					if ( m_candlestick.mDate < m_middle_candletick.mDate || ( m_candlestick.mDate == m_middle_candletick.mDate && m_candlestick.mTime < m_middle_candletick.mTime ) ) {
						//return lower_bound;
						ins_pos = lower_bound;
						return false;
					}
		}
		else {
			if ( m_candlestick.mDate < m_middle_candletick.mDate || ( m_candlestick.mDate == m_middle_candletick.mDate && m_candlestick.mTime <= m_middle_candletick.mTime ) )
				return search_KLine_in_file ( pKLine_file_info, lower_bound, middle, ins_pos );
			else
				return search_KLine_in_file ( pKLine_file_info, middle + 1, upper_bound, ins_pos );
		}
	}
}

vector<string> CKLineStream::tokenize( const char *input_str, char *delimiter ) {
	char * pch, buf_str[100];
	const char *m_get_str;
	vector<string> mToken;
	string m_str_element;

	strcpy( buf_str, input_str );
	pch = strtok ( buf_str, delimiter );
	while( pch != NULL ) {
		m_str_element = pch;
		mToken.push_back(m_str_element);
		pch = strtok ( NULL, delimiter );
	}

	return mToken;
}

void CKLineStream::load_KLine_from_archive ( char * ticker_symbol ) {
	/*candlesticks archive->mMap_stock_kline*/

	string m_str_symbol, m_str_filename;
	list<TCandleStick> *pList_KLineData;
	fstream *p_fs;
	TKLineData_FileInfo *pKLine_file_info;
	int nSize = sizeof(TCandleStick), nRead_candlesticks;
	long f_size;
	TCandleStick m_file_candlestick;
	DWORD cur_tid;
	int current_date, i;
	SYSTEMTIME ti;
	int year, month, day;
	string txt_out_filename;
	CStringW txt_filenameW, KLine_filenameW;
	int strip_repeat_mDate = 0, strip_repeat_mTime = 0;
	
	//GetSystemTime ( &ti ) ;
	::GetLocalTime ( &ti );
	current_date = ti.wYear * 10000 + ti.wMonth * 100 + ti.wDay;
	if ( ti.wDayOfWeek == 0 ) {
		current_date = previous_nDay_date ( current_date, 2 );
	}
	else
		if ( ti.wDayOfWeek == 6 ) {
			current_date = previous_nDay_date ( current_date, 1 );
		}
		else
			if ( ti.wDayOfWeek == 1 && server_escape_seconds < ( ( 8 * 60 + 30 ) * 60 ) ) {
				current_date = previous_nDay_date ( current_date, 3 );
			}
	cur_tid = GetCurrentThreadId();
	txt_out_filename = path_bufA.GetString();
	txt_out_filename =  txt_out_filename + "TX00_txt";
	txt_filenameW = txt_out_filename.c_str();
	if ( GetCurrentThreadId() == g_ThreadID_KLine[ 0 ] )
		//txt_out.open( "c:\\temp\\TX00_txt", ios::out | ios::ate | ios::trunc );
		txt_out.open( txt_filenameW.GetString(), ios::out | ios::ate | ios::trunc );
	m_str_symbol = ticker_symbol;
	//m_str_filename = "I:\\2014 VC project\\Trade_Test\\Debug\\data\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	//m_str_filename = "C:\\temp\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	m_str_filename = path_bufA.GetString() + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	KLine_filenameW = m_str_filename.c_str();
	//m_str_filename = ".\\data\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];

	map<string, list<TCandleStick>*>::iterator itr = mMap_stock_kline.find( m_str_symbol );
	map<string, TKLineData_FileInfo *>::iterator itr1 =  file_stream_info.find( m_str_filename );
	/*test if a key exist*/
	if ( itr == mMap_stock_kline.end() || !mMap_stock_kline.count(m_str_symbol) ) {
		pList_KLineData = new list<TCandleStick>;
		mMap_stock_kline.insert(pair<string, list<TCandleStick>*>(m_str_symbol, pList_KLineData));

		p_fs = new fstream( KLine_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate );
		if ( p_fs->is_open() == true ) {
		}
		else {
			delete p_fs;
			p_fs = new fstream( KLine_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
		}
		pKLine_file_info = new TKLineData_FileInfo();
		pKLine_file_info->n_fsize = f_size = p_fs->tellg();
		pKLine_file_info->n_list_end = ( pKLine_file_info->n_fsize / nSize ) - 1;
		if ( pKLine_file_info->n_list_end >= 0 ) {
			if ( pKLine_file_info->n_list_end < 1000 )
				pKLine_file_info->n_list_begin = 0;
			else
				pKLine_file_info->n_list_begin = pKLine_file_info->n_list_end - 999;
		}
		else
			pKLine_file_info->n_list_begin = -1;
		pKLine_file_info->p_fs = p_fs;
		file_stream_info.insert( std::pair<string, TKLineData_FileInfo *> ( m_str_filename, pKLine_file_info ));
		mMap_KLine_open_time [ m_str_symbol ] = 24 * 60;
		/*sync all TCandleStick in archive(p_fs) into pList_KLineData*/
		i = nRead_candlesticks = 1;
		while ( ( f_size - i*nSize ) >= 0 &&  nRead_candlesticks <= 1000 ) {
			p_fs->seekg ( ( f_size - i*nSize ), ios::beg );
			p_fs->read ( (char *) &m_file_candlestick, nSize );
			i++;
			//if ( current_date == m_file_candlestick.mDate )
			year = m_file_candlestick.mDate / 10000;
			month = ( m_file_candlestick.mDate - 10000 * year ) / 100;
			day = m_file_candlestick.mDate - 10000 * year - 100 * month;
			if ( current_date == m_file_candlestick.mDate || ( current_date > m_file_candlestick.mDate && days_difference ( day, month, year, ti.wDay, ti.wMonth, ti.wYear ) == 1 && server_escape_seconds < ( ( 8 * 60 + 30 ) * 60 ) ) )
				continue;
			else {
				if ( strip_repeat_mDate != m_file_candlestick.mDate || strip_repeat_mTime != m_file_candlestick.mTime ) {
				pList_KLineData->insert ( pList_KLineData->begin(), m_file_candlestick );
				nRead_candlesticks++;
				if ( m_file_candlestick.mTime < mMap_KLine_open_time [ m_str_symbol ] ) {
					mMap_KLine_open_time [ m_str_symbol ] = m_file_candlestick.mTime;
				}
				strip_repeat_mDate = m_file_candlestick.mDate;
				strip_repeat_mTime = m_file_candlestick.mTime;
				}
			}

			//if ( pKLine_file_info->n_list_end == -1 )
				//pKLine_file_info->n_list_end
		}
	}
	else {
		pList_KLineData = (*itr).second;

		pKLine_file_info = (*itr1).second;
		p_fs = pKLine_file_info->p_fs;	
	}

	/*if ( !file_stream_info.count( m_str_filename ) ) {

	}
	else {
	}*/
}

int CKLineStream::Push_Tick_Data( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill ) {
/*tick output format
TICK 編號:0 時間:90003 買價:-999999 賣價:-999999 成交價:3255 量:66 
*/
	list<TICK> *pList_TickData;
	list<TICK>::iterator  itr1, itr2;
	int n_VecCap, n_VecSize;
	TTickData_FileInfo *pTick_file_info;
	string m_str_filename;
	fstream *p_fs;
	long f_size;
	int nSize = sizeof(TICK), nRead_ticks;
	TICK m_file_tick;
	
	list<TCandleStick> *pList_KLineData, *pList_KLineData1;
	int next_trading_date = -1, next_kline_close_time, m_open_time, m_open_hour, m_open_min, m_open_sec;
	int m_tick_hour, m_tick_min, m_tick_sec;
	SYSTEMTIME ti;
	if ( mMap_stock_kline.count( symbol ) ) {
		pList_KLineData = mMap_stock_kline [ symbol ];
		m_candlestick = pList_KLineData->back();
		if ( is_tick_in_kline == true ) {
		}
		else {
			//next_trading_date = m_candlestick.mDate + 1;
			next_trading_date = next_trade_date ( m_candlestick.mDate );
			m_candlestick.mDate = next_trading_date;
		}
	}
	else {
		//GetSystemTime(&ti);
		GetLocalTime (&ti);
		//if ( current_date == m_file_candlestick.mDate || ( current_date > m_candlestick.mDate && days_difference ( day, month, year, ti.wDay, ti.wMonth, ti.year ) == 1 && server_escape_seconds < ( ( 8 * 60 + 30 ) * 60 ) ) )
		m_candlestick.mDate = next_trading_date = ti.wDay + ti.wMonth * 100 + ti.wYear * 10000;
	}
	
	char str_buf [ 100 ];
	CStringW tick_filenameW;
	if ( store_tick_file ==true ) {
	//m_str_filename = "C:\\temp\\" + string( itoa(m_candlestick.mDate, str_buf, 10 ) ) + "-" + symbol;
	m_str_filename = path_bufA.GetString() + string( itoa(m_candlestick.mDate, str_buf, 10 ) ) + "-" + symbol;
	tick_filenameW = m_str_filename.c_str();
	if ( !mMap_stock_ticks.count( symbol ) ) {
		pList_TickData = new list<TICK>;
		mMap_stock_ticks[ symbol ] = pList_TickData;

		p_fs = new fstream( tick_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate );
		if ( p_fs->is_open() == true ) {
		}
		else {
			delete p_fs;
			p_fs = new fstream( tick_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
		}
		pTick_file_info = new TTickData_FileInfo;
		pTick_file_info->n_fsize = f_size = p_fs->tellg();
		pTick_file_info->n_list_end = ( pTick_file_info->n_fsize / nSize ) - 1;
		if ( pTick_file_info->n_list_end >= 0 ) {
			if ( pTick_file_info->n_list_end < 1000 )
				pTick_file_info->n_list_begin = 0;
			else
				pTick_file_info->n_list_begin = pTick_file_info->n_list_end - 999;
		}
		else
			pTick_file_info->n_list_begin = -1;
		pTick_file_info->p_fs = p_fs;

		mMap_tickfile_stream_info.insert ( std::pair<string, TTickData_FileInfo *> ( m_str_filename, pTick_file_info ) );
		/*sync all Tick in archive(p_fs) into pList_KLineData*/
		nRead_ticks = 1;
		while ( ( f_size - nRead_ticks * nSize ) >= 0 &&  nRead_ticks <= 1000 ) {
			p_fs->seekg ( ( f_size - nRead_ticks * nSize ), ios::beg );
			p_fs->read ( (char *) &m_file_tick, nSize );
			pList_TickData->insert ( pList_TickData->begin(), m_file_tick );
			nRead_ticks++;

			//if ( pKLine_file_info->n_list_end == -1 )
			//pKLine_file_info->n_list_end
		}
	}
	else {
		pList_TickData = mMap_stock_ticks[ symbol ];

		pTick_file_info = mMap_tickfile_stream_info[ m_str_filename ];
		p_fs = pTick_file_info->p_fs;
	}
	TICK m_init_tick = { -1, 0, 0, 0, 0, 0 };
	itr1 = pList_TickData->begin();
	itr2 = pList_TickData->end();
	}

	m_tick.m_nPtr = nPtr;
	m_tick.m_nTime = nTime;
	m_tick.m_nBid = nBid;
	m_tick.m_nAsk = nAsk;
	m_tick.m_nClose = nClose;
	m_tick.m_nQty = nQty;

	TICK m_list_tick;
	bool found_in_vec = false;
	int n_insert_pos, n_insert_pos1, i, j, n_pos;
#if 1
	if ( store_tick_file ==true ) {
		n_insert_pos = pTick_file_info->n_list_begin;
	if ( std::find( pList_TickData->begin(), pList_TickData->end(), nPtr ) ==  pList_TickData->end() ) {
	for ( n_pos = 0, n_insert_pos = pTick_file_info->n_list_begin ; itr1 != itr2; itr1++, n_pos++, n_insert_pos++ ) {
		m_list_tick = (*itr1);
		if ( m_tick.m_nPtr == m_list_tick.m_nPtr ) {
			found_in_vec = true;
			break;
		}
		else
			if ( m_tick.m_nPtr > m_list_tick.m_nPtr ) {
			}
			else
				if ( m_tick.m_nPtr < m_list_tick.m_nPtr ) {
					break;
				}
	}
	}
	else
		found_in_vec = true;
	}
#endif

	list<TCandleStick>::reverse_iterator itr3;
	TCandleStick *m_pTick_candlestick;
	bool found_tick_in_candlestick = false, found_in_file = false;
	float new_price;
#if 1
	if ( store_tick_file ==true ) {
	if ( found_in_vec == false) {
		if ( itr1 == pList_TickData->begin() ) {
			/*for ( i = 0; i < n_insert_pos; i++ ) {
				p_fs->seekg ( i*nSize, ios::beg );
				p_fs->read ( (char *) &m_file_tick, nSize );
				if ( m_tick.m_nPtr == m_file_tick.m_nPtr ) {
					found_in_file = true;
					break;
				}
				else
					if ( m_tick.m_nPtr > m_file_tick.m_nPtr ) {
					}
					else
						if ( m_tick.m_nPtr < m_file_tick.m_nPtr ) {
							break;
						}
			}*/
			i = 0;
			found_in_file = search_tick_in_file ( pTick_file_info, 0, n_insert_pos - 1, i );

			if ( found_in_file == false ) {
				n_insert_pos1 = i;
				for ( j = pTick_file_info->n_list_end; j >= i; j-- ) {
					p_fs->seekg ( j*nSize, ios::beg );
					p_fs->read ( (char *) &m_file_tick, nSize );
					p_fs->seekp ( (j+1)*nSize, ios::beg );
					p_fs->write ( (char *) &m_file_tick, nSize );
				}
				p_fs->seekp ( (n_insert_pos1)*nSize, ios::beg );
				p_fs->write ( (char *) &m_tick, nSize );
				pTick_file_info->n_list_end++;
				pTick_file_info->n_list_begin++;
				pTick_file_info->n_fsize = ( pTick_file_info->n_list_end + 1 ) * nSize;
				if ( n_insert_pos >= 0 && ( pTick_file_info->n_list_end - pTick_file_info->n_list_begin + 1 ) < 1000 ) {
					p_fs->seekg ( n_insert_pos*nSize, ios::beg );
					p_fs->read ( (char *) &m_file_tick, nSize );
					pList_TickData->insert ( pList_TickData->begin(), m_file_tick );
					pTick_file_info->n_list_begin--;
				}
				else
					if ( ( pTick_file_info->n_list_end - pTick_file_info->n_list_begin + 1 ) == 1 )
						pList_TickData->insert( itr1, m_tick );
			}
		}
		else {
			pList_TickData->insert( itr1, m_tick );
			/*sync ticks in vector with ticks in file*/
			itr1--;
			itr2 = pList_TickData->end();
			
			for ( /*i = n_pos*/; itr1 != itr2 ; n_insert_pos++, /*i++*/ itr1++ ) {
				//m_file_tick = pList_TickData->at(i);
				m_file_tick = ( *itr1 );
				p_fs->seekp ( n_insert_pos * nSize, ios::beg );
				p_fs->write ( (char *) &m_file_tick, nSize );
			}
			pTick_file_info->n_list_end++;
			pTick_file_info->n_fsize = ( pTick_file_info->n_list_end + 1 ) * nSize;
			if ( ( pTick_file_info->n_list_end - pTick_file_info->n_list_begin + 1 ) > 1000 ) {
				pTick_file_info->n_list_begin++;
				//pList_TickData->erase( pVector_TickData->begin() );
				pList_TickData->pop_front();
			}
		}
		p_fs->flush();
	}
	}
#endif

#if 0
	if ( nPtr > pTick_file_info->n_list_end ) {
		pList_TickData->insert( pList_TickData->end(), m_tick );
		pTick_file_info->n_list_end++;
		pTick_file_info->n_fsize = ( pTick_file_info->n_list_end + 1 ) * nSize;
		if ( ( pTick_file_info->n_list_end - pTick_file_info->n_list_begin + 1 ) > 1000 ) {
			pTick_file_info->n_list_begin++;
			pList_TickData->erase( pList_TickData->begin() );
		}
		p_fs->seekp ( pTick_file_info->n_list_end * nSize, ios::beg );
		p_fs->write ( (char *) &m_tick, nSize );
	}
#endif

	string m_new_filename, m_str_symbol;
	list<double> *pList_MA1, *pList_MA2;
	double sum, mean;
	static int is_last_tick_backfill = 1;
	int n_MA_Groups, n_rev_ins_pos;
	//static bool MA15_list_ready = false, MA22_list_ready = false;
	list <TCandleStick>::reverse_iterator itr5, itr6, itr7;
	list <double>::reverse_iterator itr9;
			if ( m_tick.m_nPtr == 0 ) {
				mMap_intraday_open_time.insert (pair<string, int>(symbol, m_tick.m_nTime));
			}

			switch ( nTimeFrame ) {
					case 1:
						sprintf( str_buf, "%s_%dmin", symbol.c_str(), 5 * n_collapse_sticks );
						m_str_symbol = str_buf;
						//m_new_filename = "C:\\temp\\" + m_str_symbol;
						m_new_filename = path_bufA.GetString() + m_str_symbol;
						break;
					case 4:
						m_str_symbol = symbol + "_" + mTimeFrameName[ nTimeFrame ];
						break;
			}
			if ( n_collapse_sticks == 1 ) {
				if ( !mMap_tick_compose_kline.count( symbol ) ) {
					pList_KLineData = new list<TCandleStick>;
					mMap_tick_compose_kline.insert( pair<string, list<TCandleStick>*> ( symbol, pList_KLineData ) );
				}
				else {
					pList_KLineData = mMap_tick_compose_kline[ symbol ];
				}
				
				if ( !mMap_stock_kline.count( symbol ) ) {
					pList_KLineData1 = NULL;
				}
				else {
					pList_KLineData1 = mMap_stock_kline[ symbol ];
				}
			}
			else {
				if ( !mMap_tick_compose_kline.count( m_str_symbol ) ) {
					pList_KLineData = new list<TCandleStick>;
					mMap_tick_compose_kline.insert( pair<string, list<TCandleStick>*> ( m_str_symbol, pList_KLineData ) );
				}
				else {
					pList_KLineData = mMap_tick_compose_kline[ m_str_symbol ];
				}
				if ( !mMap_stock_kline.count( m_str_symbol ) ) {
					pList_KLineData1 = NULL;
				}
				else {
					pList_KLineData1 = mMap_stock_kline[ m_str_symbol ];
				}
			}
			if ( !mMap_MA1.count ( m_str_symbol ) ) {
				pList_MA1 = new list<double>;
				mMap_MA1.insert( pair<string, list<double>*> ( m_str_symbol, pList_MA1 ) );
			}
			else
				pList_MA1 = mMap_MA1 [ m_str_symbol ];
			if ( !mMap_MA2.count ( m_str_symbol ) ) {
				pList_MA2 = new list<double>;
				mMap_MA2.insert( pair<string, list<double>*> ( m_str_symbol, pList_MA2 ) );
			}
			else
				pList_MA2 = mMap_MA2 [ m_str_symbol ];
/*collapse tick data into appropriate candlestick*/
			if ( nTimeFrame <= 4 ) {
/*intraday timeframe，take the first tick time as start within the day*/
				if ( mMap_intraday_open_time.count( symbol ) ) {
					m_open_time = mMap_intraday_open_time[ symbol ];
					m_open_hour = m_open_time / 10000;
					m_open_min = ( m_open_time - m_open_hour * 10000 ) / 100;
					m_tick_hour = m_tick.m_nTime / 10000;
					m_tick_min = ( m_tick.m_nTime - m_tick_hour * 10000 ) / 100;
				
					switch ( nTimeFrame ) {
				      case 0: //1min
						  break;
					  case 1: //5min
					  case 4: //day
						  if ( nTimeFrame == 1)
							  kline_close_time = next_kline_close_time = 60 * m_open_hour + ( m_open_min + 5 * n_collapse_sticks * ( ( ( 60 * m_tick_hour + m_tick_min ) - ( 60 * m_open_hour + m_open_min ) ) / ( 5 * n_collapse_sticks ) + 1 ) );
						  else
							  if ( nTimeFrame == 4)
								  kline_close_time = next_kline_close_time = 0;
						  for (  n_rev_ins_pos = 0, itr3 = pList_KLineData->rbegin(); itr3 != pList_KLineData->rend(); itr3++, n_rev_ins_pos++ ) {
							  m_pTick_candlestick = (TCandleStick *) &(*itr3);
							  if ( m_pTick_candlestick->mTime == next_kline_close_time ) {
								  found_tick_in_candlestick = true;
								  break;
							  }
							  else
								  if ( next_kline_close_time > m_pTick_candlestick->mTime ) {
									  break;
								  }
								  else
									  if ( next_kline_close_time < m_pTick_candlestick->mTime ) {
									  }
						  }
						  if ( found_tick_in_candlestick == false ) {
							  m_candlestick.mTime = next_kline_close_time;
							  new_price = ( float ) nClose / 100;
						  //new_price = floor( ( new_price * 100 ) + 0.5 ) / 100;
						  //new_price = 32.55;
							  m_candlestick.mOpen = new_price;
							  m_candlestick.mHigh = new_price;
							  m_candlestick.mLow = new_price;
							  m_candlestick.mClose = new_price;
							  m_candlestick.mVolumn = nQty;
							  pList_KLineData->insert( itr3.base(), m_candlestick );
#if 1
							  if ( /*is_last_tick_backfill == 1 && is_BackFill == 0*/ MA1_list_ready == false ) {
								  if ( pList_KLineData1 == NULL ) {
									  /*if ( is_tick_in_kline == false && pList_KLineData->size() >= 15 ) {
										  n_MA_Groups = pList_KLineData->size() - 15 + 1;
										  for ( itr5 = itr6 = pList_KLineData->rbegin(), sum = 0, j = 0; j < 15; j++, itr6++ ) {
											  sum += itr6->mClose;
										  }
										  mean = sum / 15;
										  pList_MA15->insert ( pList_MA15->begin(), mean );
										  for ( i = 1; i < n_MA_Groups && i < 1000; itr5++, itr6++, i++ ) {
											  sum = sum + itr6->mClose - itr5->mClose;
											  mean = sum / 15;
											  pList_MA15->insert ( pList_MA15->begin(), mean );
										  }
									  }*/
								  }
								  else
									  if ( is_tick_in_kline == false ) {
										  //MA10
										  if ( ( pList_KLineData->size() + pList_KLineData1->size() ) >= mMA1_period ) {
											  n_MA_Groups = pList_KLineData->size() + pList_KLineData1->size() - mMA1_period + 1;
											  for ( itr5 = pList_KLineData->rbegin(), i = 0; i < n_MA_Groups && i < 1000 && i < pList_KLineData->size(); itr5++, i++ ) {
												  for ( itr6 = itr5, sum = 0, j = 0; j < mMA1_period && itr6 != pList_KLineData->rend(); j++, itr6++ ) {
													  sum += itr6->mClose;
												  }
												  for ( itr6 = pList_KLineData1->rbegin(); j < mMA1_period; j++, itr6++ ) {
													  sum += itr6->mClose;
												  }
												  mean = sum / mMA1_period;
												  pList_MA1->insert ( pList_MA1->begin(), mean );
											  }
											  if ( pList_KLineData1->size() >= mMA1_period ) {
												  n_MA_Groups = pList_KLineData1->size() - mMA1_period + 1;
												  for ( itr5 = itr6 = pList_KLineData1->rbegin(), sum = 0, j = 0; j < mMA1_period; j++, itr6++ ) {
													  sum += itr6->mClose;
												  }
												  mean = sum / mMA1_period;
												  pList_MA1->insert ( pList_MA1->begin(), mean );
												  for ( i = 1; i < n_MA_Groups && i < 1000; itr5++, itr6++, i++ ) {
													  sum = sum + itr6->mClose - itr5->mClose;
													  mean = sum / mMA1_period;
													  pList_MA1->insert ( pList_MA1->begin(), mean );
												  }
											  }
										  }
									  }
									  else {
										  //MA10
										  if ( pList_KLineData1->size() >= mMA1_period ) {
											  n_MA_Groups = pList_KLineData1->size() - mMA1_period + 1;
											  for ( itr5 = itr6 = pList_KLineData1->rbegin(), sum = 0, j = 0; j < mMA1_period; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  mean = sum / mMA1_period;
											  pList_MA1->insert ( pList_MA1->begin(), mean );
											  for ( i = 1; i < n_MA_Groups && i < 1000; i++, itr6++, itr5++ ) {
												  sum = sum + itr6->mClose - itr5->mClose;
												  mean = sum / mMA1_period;
												  pList_MA1->insert ( pList_MA1->begin(), mean );
											  }
										  }
									  }
									  
									  MA1_list_ready = true;
							  }
							  else
								  if ( MA1_list_ready == true /*&& is_BackFill == 0*/ ) {
									  if ( pList_KLineData1 == NULL ) {
										  if ( is_tick_in_kline == false && pList_KLineData->size() >= mMA1_period ) {
											  pList_MA1->insert ( pList_MA1->end(), mean );
											  for ( itr9 = pList_MA1->rbegin(), itr6 = itr5 = pList_KLineData->rbegin(), sum = 0, j = 0; j < mMA1_period; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  mean = sum / mMA1_period;												 
											  (*itr9) = mean;											  
											  for ( i = 1; i < (n_rev_ins_pos + 1); i++, itr5++, itr9++ ) {
												  sum = sum + itr6->mClose - itr5->mClose;
												  mean = sum / mMA1_period;												 
												  (*itr9) = mean;
											  }
											  if ( pList_MA1->size() > 1000 )
												  pList_MA1->pop_front();
										  }
									  }
									  else
										  if ( is_tick_in_kline == false ) {
											  if ( ( pList_KLineData->size() + pList_KLineData1->size() ) >= mMA1_period ) {
												  pList_MA1->insert ( pList_MA1->end(), mean );
												  for ( itr9 = pList_MA1->rbegin(), itr5 = pList_KLineData->rbegin(), i = 0; i < (n_rev_ins_pos + 1); itr5++, i++, itr9++ ) {
													  for ( itr6 = itr5, sum = 0, j = 0; j < mMA1_period && itr6 != pList_KLineData->rend(); j++, itr6++ ) {
														  sum += itr6->mClose;
													  }
													  for ( itr6 = pList_KLineData1->rbegin(); j < mMA1_period; j++, itr6++ ) {
														  sum += itr6->mClose;
													  }
													  mean = sum / mMA1_period;
													  (*itr9) = mean;
												  }
												  if ( pList_MA1->size() > 1000 )
													  pList_MA1->pop_front();
											  }
										  }
								  }
#endif

#if 1
							  if ( MA2_list_ready == false ) {
								  if ( pList_KLineData1 == NULL ) {
								  }
								  else
									  if ( is_tick_in_kline == false ) {
										  //MA22
										  if ( ( pList_KLineData->size() + pList_KLineData1->size() ) >= mMA2_period ) {
											  n_MA_Groups = pList_KLineData->size() + pList_KLineData1->size() - mMA2_period + 1;
											  for ( itr5 = pList_KLineData->rbegin(), i = 0; i < n_MA_Groups && i < 1000 && i < pList_KLineData->size(); itr5++, i++ ) {
												  for ( itr6 = itr5, sum = 0, j = 0; j < mMA2_period && itr6 != pList_KLineData->rend(); j++, itr6++ ) {
													  sum += itr6->mClose;
												  }
												  for ( itr6 = pList_KLineData1->rbegin(); j < mMA2_period; j++, itr6++ ) {
													  sum += itr6->mClose;
												  }
												  mean = sum / mMA2_period;
												  pList_MA2->insert ( pList_MA2->begin(), mean );
											  }
											  if ( pList_KLineData1->size() >= mMA2_period ) {
												  n_MA_Groups = pList_KLineData1->size() - mMA2_period + 1;
												  for ( itr5 = itr6 = pList_KLineData1->rbegin(), sum = 0, j = 0; j < mMA2_period; j++, itr6++ ) {
													  sum += itr6->mClose;
												  }
												  mean = sum / mMA2_period;
												  pList_MA2->insert ( pList_MA2->begin(), mean );
												  for ( i = 1; i < n_MA_Groups && i < 1000; itr5++, itr6++, i++ ) {
													  sum = sum + itr6->mClose - itr5->mClose;
													  mean = sum / mMA2_period;
													  pList_MA2->insert ( pList_MA2->begin(), mean );
												  }
											  }
										  }
									  }
									  else {
										  //MA22
										  if ( pList_KLineData1->size() >= mMA2_period ) {
											  n_MA_Groups = pList_KLineData1->size() - mMA2_period + 1;
											  for ( itr5 = itr6 = pList_KLineData1->rbegin(), sum = 0, j = 0; j < mMA2_period; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  mean = sum / mMA2_period;
											  pList_MA2->insert ( pList_MA2->begin(), mean );
											  for ( i = 1; i < n_MA_Groups && i < 1000; i++, itr6++, itr5++ ) {
												  sum = sum + itr6->mClose - itr5->mClose;
												  mean = sum / mMA2_period;
												  pList_MA2->insert ( pList_MA2->begin(), mean );
											  }
										  }
									  }
									  
									  MA2_list_ready = true;
							  }
							  else
								  if ( MA2_list_ready == true /*&& is_BackFill == 0*/ ) {
									  if ( pList_KLineData1 == NULL ) {
										  if ( is_tick_in_kline == false && pList_KLineData->size() >= mMA2_period ) {
											  pList_MA2->insert ( pList_MA2->end(), mean );
											  for ( itr9 = pList_MA2->rbegin(), itr6 = itr5 = pList_KLineData->rbegin(), sum = 0, j = 0; j < mMA2_period; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  mean = sum / mMA2_period;												 
											  (*itr9) = mean;											  
											  for ( i = 1; i < (n_rev_ins_pos + 1); i++, itr5++, itr9++ ) {
												  sum = sum + itr6->mClose - itr5->mClose;
												  mean = sum / mMA2_period;												 
												  (*itr9) = mean;
											  }
											  if ( pList_MA1->size() > 1000 )
												  pList_MA1->pop_front();
										  }
									  }
									  else
										  if ( is_tick_in_kline == false ) {
											  if ( ( pList_KLineData->size() + pList_KLineData1->size() ) >= mMA2_period ) {
												  pList_MA2->insert ( pList_MA2->end(), mean );
												  for ( itr9 = pList_MA2->rbegin(), itr5 = pList_KLineData->rbegin(), i = 0; i < (n_rev_ins_pos + 1); itr5++, i++, itr9++ ) {
													  for ( itr6 = itr5, sum = 0, j = 0; j < mMA2_period && itr6 != pList_KLineData->rend(); j++, itr6++ ) {
														  sum += itr6->mClose;
													  }
													  for ( itr6 = pList_KLineData1->rbegin(); j < mMA2_period; j++, itr6++ ) {
														  sum += itr6->mClose;
													  }
													  mean = sum / mMA2_period;
													  (*itr9) = mean;
												  }
												  if ( pList_MA1->size() > 1000 )
													  pList_MA1->pop_front();
											  }
										  }
								  }
#endif

						  }
						  else {
							  new_price = ( float ) nClose / 100;

#if 1
							  if ( MA1_list_ready == true && is_tick_in_kline == false ) {
								  if ( pList_KLineData1 == NULL ) {
									  for ( itr9 = pList_MA1->rbegin(), itr5 = pList_KLineData->rbegin(), i = 0; i < ( n_rev_ins_pos + 1 - mMA1_period ); i++, itr5++, itr9++ ) {
									  }
									  if ( ( pList_KLineData->size() - i ) >= mMA1_period ) {
										  //pList_MA15->insert ( pList_MA15->end(), mean );
										  //for ( itr6 = itr5, j = 0; j < ( n_rev_ins_pos - i ); j++, itr6++ );
										  for ( ; i < ( n_rev_ins_pos + 1 ); i++, itr5++, itr9++ ) {
											  /*for ( itr6 = itr5, sum = 0, j = 0; j < 15; j++, itr6++ ) {
											  sum += itr6->mClose;
											  }
											  mean = sum / 15;*/

											  mean = (*itr9);
											  sum = mMA1_period * mean - m_pTick_candlestick->mClose + new_price;
											  (*itr9) = sum / mMA1_period;
										  }
									  }
								  }
								  else {
									  for ( itr9 = pList_MA1->rbegin(), itr5 = pList_KLineData->rbegin(), i = 0; i < ( n_rev_ins_pos + 1 - mMA1_period ); i++, itr5++, itr9++ ) {
									  }
									  if ( ( pList_KLineData->size() + pList_KLineData1->size() - i ) >= mMA1_period ) {

										  //pList_MA15->insert ( pList_MA15->end(), mean );
										  for ( ; i < ( n_rev_ins_pos + 1 ); itr5++, i++, itr9++ ) {
											  /*for ( itr6 = itr5, sum = 0, j = 0; j < 15; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  for ( itr6 = pList_KLineData1->rbegin(); j < 15; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  mean = sum / 15;*/
											  /*for ( itr6 = itr5, j = 0; j < ( n_rev_ins_pos - i ) && itr6 != pList_KLineData->rend(); j++, itr6++ );
											  if ( itr6 == pList_KLineData->rend() )
												  for ( itr6 = pList_KLineData->rbegin(); j < ( n_rev_ins_pos - i ); j++, itr6++ );*/
											  mean = (*itr9);
											  sum = mMA1_period * mean - m_pTick_candlestick->mClose + new_price;
											  (*itr9) = sum / mMA1_period;
										  }
									  }
								  }
							  }
#endif

							  //MA22
#if 1
							  if ( MA2_list_ready == true && is_tick_in_kline == false ) {
								  if ( pList_KLineData1 == NULL ) {
									  for ( itr9 = pList_MA2->rbegin(), itr5 = pList_KLineData->rbegin(), i = 0; i < ( n_rev_ins_pos + 1 - mMA2_period ); i++, itr5++, itr9++ ) {
									  }
									  if ( ( pList_KLineData->size() - i ) >= mMA2_period ) {
										  //pList_MA22->insert ( pList_MA22->end(), mean );
										  //for ( itr6 = itr5, j = 0; j < ( n_rev_ins_pos - i ); j++, itr6++ );
										  for ( ; i < ( n_rev_ins_pos + 1 ); i++, itr5++, itr9++ ) {
											  /*for ( itr6 = itr5, sum = 0, j = 0; j < 22; j++, itr6++ ) {
											  sum += itr6->mClose;
											  }
											  mean = sum / 22;*/

											  mean = (*itr9);
											  sum = mMA2_period * mean - m_pTick_candlestick->mClose + new_price;
											  (*itr9) = sum / mMA2_period;
										  }
									  }
								  }
								  else {
									  for ( itr9 = pList_MA2->rbegin(), itr5 = pList_KLineData->rbegin(), i = 0; i < ( n_rev_ins_pos + 1 - mMA2_period ); i++, itr5++, itr9++ ) {
									  }
									  if ( ( pList_KLineData->size() + pList_KLineData1->size() - i ) >= mMA2_period ) {

										  //pList_MA22->insert ( pList_MA22->end(), mean );
										  for ( ; i < ( n_rev_ins_pos + 1 ); itr5++, i++, itr9++ ) {
											  /*for ( itr6 = itr5, sum = 0, j = 0; j < 22; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  for ( itr6 = pList_KLineData1->rbegin(); j < 22; j++, itr6++ ) {
												  sum += itr6->mClose;
											  }
											  mean = sum / 22;*/
											  /*for ( itr6 = itr5, j = 0; j < ( n_rev_ins_pos - i ) && itr6 != pList_KLineData->rend(); j++, itr6++ );
											  if ( itr6 == pList_KLineData->rend() )
												  for ( itr6 = pList_KLineData->rbegin(); j < ( n_rev_ins_pos - i ); j++, itr6++ );*/
											  mean = (*itr9);
											  sum = mMA2_period * mean - m_pTick_candlestick->mClose + new_price;
											  (*itr9) = sum / mMA2_period;
										  }
									  }
								  }
							  }
#endif
							  m_pTick_candlestick->mClose = new_price;
							  if ( m_pTick_candlestick->mHigh < new_price )
								  m_pTick_candlestick->mHigh = new_price;
							  else
								  if ( m_pTick_candlestick->mLow > new_price )
									  m_pTick_candlestick->mLow = new_price;
							  m_pTick_candlestick->mVolumn += nQty;
						  }

						  							  /*if ( ( pList_KLineData1 != NULL && ( pList_KLineData->size() + pList_KLineData1->size() ) >= 15 )  ||
								  ( pList_KLineData1 == NULL && pList_KLineData->size() >= 15 )) {
								  for ( 
							  }*/
							  /*if ( pList_KLineData->size() >= 15 ) {
							  
							  }
							  else
								  if ( ( pList_KLineData1 != NULL && ( pList_KLineData->size() + pList_KLineData1->size() ) >= 15 ) ) {
									  for ( sum = 0, i =0, itr5 = pList_KLineData->rbegin(); i < 15 && itr5 != pList_KLineData->rend(); i++, itr5++ ) {
										  sum += itr5->mClose;
									  }
									  for ( itr5 = pList_KLineData1->rbegin(); itr5 != pList_KLineData1->rend() && i < 15; i++, itr5++ ) {
										  sum += itr5->mClose;
									  }
								  }*/
						  break;
					  case 2: //30min
						  break;
					}
				}
			}
			else {
/*interday timeframe，week/month/year candlestick chart*/	
			}
	/*n_VecCap = pVector_TickData->capacity( );
	if ( n_VecCap <= nPtr )
		pVector_TickData->reserve( nPtr + 100);
		//pVector_TickData->resize( nPtr + 1, m_init_tick );

	n_VecCap = pVector_TickData->capacity( );
	n_VecSize = pVector_TickData->size();*/
	//(*pVector_TickData)[ 0 ] = m_tick;

	//pVector_TickData->push_back( m_tick );

	//pVector_TickData->at( nPtr ) = m_tick;
	//n_VecSize = pVector_TickData->capacity();

	is_last_tick_backfill = is_BackFill;
	return 0;
}

//search_tick_in_file( TTickData_FileInfo *pTick_file_info, 0, n_insert_pos )
bool CKLineStream::search_tick_in_file( TTickData_FileInfo *pTick_file_info, int lower_bound, int upper_bound, int &ins_pos ) {
	TICK m_first_tick, m_last_tick, m_middle_tick;
	fstream *p_fs;
	int middle;

	if ( lower_bound > upper_bound ) {
		return false;
	}
	else {
		p_fs = pTick_file_info->p_fs;
		p_fs->seekg ( lower_bound*sizeof(TICK), ios::beg );
		p_fs->read ( (char *) &m_first_tick, sizeof(TICK) );

		p_fs->seekg ( upper_bound*sizeof(TICK), ios::beg );
		p_fs->read ( (char *) &m_last_tick, sizeof(TICK) );

		middle = ( lower_bound + upper_bound ) / 2;
		p_fs->seekg ( middle*sizeof(TICK), ios::beg );
		p_fs->read ( (char *) &m_middle_tick, sizeof(TICK) );

		if ( lower_bound == upper_bound ) {
			if ( m_tick.m_nPtr == m_first_tick.m_nPtr ) {
				/*found_in_file = true;
				break;*/
				//return -1;
				ins_pos = lower_bound;
				return true;
			}
			else
				if ( m_tick.m_nPtr > m_first_tick.m_nPtr ) {
					//return lower_bound + 1;
					ins_pos = lower_bound + 1;
					return false;

				}
				else
					if ( m_tick.m_nPtr < m_first_tick.m_nPtr ) {
						//return lower_bound;
						ins_pos = lower_bound;
						return false;
					}
		}
		else {
			if ( m_tick.m_nPtr <= m_middle_tick.m_nPtr && m_tick.m_nPtr >= m_first_tick.m_nPtr )
				return search_tick_in_file ( pTick_file_info, lower_bound, middle, ins_pos );
			else
				return search_tick_in_file ( pTick_file_info, middle + 1, upper_bound, ins_pos );
		}
	}
	//if ( m_tick.m_nPtr <= m_middle_tick && m_tick.m_nPtr >= m_first_tick.m_nPtr )
/*
	if ( m_tick.m_nPtr <= m_first_tick.m_nPtr )
		return lower_bound;
	else
		if ( m_tick.m_nPtr >= m_last_tick.m_nPtr )
			return upper_bound + 1;
		else
			if ( m_tick.m_nPtr == m_middle_tick.m_nPtr )
				return middle;
			else
				if ( m_tick.m_nPtr <= m_middle_tick && m_tick.m_nPtr >= m_first_tick.m_nPtr )


		if ( m_tick.m_nPtr == m_file_tick.m_nPtr ) {
			found_in_file = true;
			break;
		}
		else
			if ( m_tick.m_nPtr > m_file_tick.m_nPtr ) {
			}
			else
				if ( m_tick.m_nPtr < m_file_tick.m_nPtr ) {
					break;
				}
*/
}

/*group several sticks into single stick*/
void CKLineStream::candlestick_collapse ( char * ticker_symbol ) {
	list<TCandleStick> *pList_KLineData;
	TKLineData_FileInfo *pOrig_KLine_file_info, *pNew_KLine_file_info;
	string m_str_symbol, m_orig_filename, m_new_filename;
	char str_buf[ 100 ];
	int max_sticks;
	fstream *p_orig_fs, *p_new_fs;
	int nSize = sizeof ( TCandleStick );
	int n_sticks = this->n_collapse_sticks;

	if ( !mMap_stock_kline.count ( ticker_symbol ) )
		return;
	else {
		m_str_symbol = ticker_symbol;
		//m_orig_filename = "C:\\temp\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
		m_orig_filename = path_bufA.GetString() + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
		pList_KLineData = mMap_stock_kline [ ticker_symbol ];
		pOrig_KLine_file_info = file_stream_info [ m_orig_filename ];
		p_orig_fs = pOrig_KLine_file_info->p_fs;
		p_orig_fs->flush();
	}

	if ( n_sticks == 1 )
		return;

	int n_total_sticks, i, j, nRead_sticks;
	TCandleStick m_raw_candlestick;
	int m_open_time, m_open_hour, m_open_min, kline_start_index;
	CStringW collapsed_filenameW;
	int current_date = get_trading_date ( ticker_symbol );
	/*calculate MA15*/
	/*list<TCandleStick>::reverse_iterator itr, itr1;
	list<TCandleStick>::iterator itr2, itr3;
	double mean, sum;
	list<double> *pList_MA15;*/
	switch ( nTimeFrame ) {
		case 0:
			max_sticks = 180 / 1;
			if ( n_sticks <= max_sticks ) {
			}
			break;
		case 1:
			max_sticks = 180 / 5;
			if ( n_sticks <= max_sticks ) {
				sprintf( str_buf, "%s_%dmin", ticker_symbol, 5 * n_sticks );
				m_str_symbol = str_buf;
				//m_new_filename = "C:\\temp\\" + m_str_symbol;
				m_new_filename = path_bufA.GetString() + m_str_symbol;
				collapsed_filenameW = m_new_filename.c_str();

				if ( !mMap_stock_kline.count ( m_str_symbol ) ) {
					pList_KLineData = new list<TCandleStick>;
					mMap_stock_kline.insert ( pair<string, list<TCandleStick> *> ( m_str_symbol, pList_KLineData ) );

					pNew_KLine_file_info = new TKLineData_FileInfo;
					p_new_fs = new fstream( collapsed_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate );
					if ( p_new_fs->is_open() == true ) {
					}
					else {
						delete p_new_fs;
						p_new_fs = new fstream( collapsed_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
					}
					
					pNew_KLine_file_info->p_fs = p_new_fs;
					file_stream_info.insert ( pair<string, TKLineData_FileInfo *> ( m_new_filename, pNew_KLine_file_info ) );
					pNew_KLine_file_info->n_fsize = 0; i = 0;
				}
				else {
					pList_KLineData = mMap_stock_kline [ m_str_symbol ];
					pNew_KLine_file_info = file_stream_info [ m_new_filename ];
					p_new_fs = pNew_KLine_file_info->p_fs;

					for ( i = 0; i <= pOrig_KLine_file_info->n_list_end; i++ ) {
						p_orig_fs->seekg ( pOrig_KLine_file_info->n_fsize - (i+1)*nSize, ios::beg );
						p_orig_fs->read ( (char *) &m_candlestick, nSize );
						if ( pList_KLineData->back().mTime == m_candlestick.mTime && pList_KLineData->back().mDate == m_candlestick.mDate )
							break;
					}
					i = pOrig_KLine_file_info->n_list_end - i + 1;
				}

				if ( mMap_intraday_open_time.count( ticker_symbol ) ) {
					m_open_time = mMap_intraday_open_time[ ticker_symbol ];
					m_open_hour = m_open_time / 10000;
					m_open_min = ( m_open_time - m_open_hour * 10000 ) / 100;
					m_open_time = m_open_hour * 60 + m_open_min;
				}
				else {
					m_open_time = mMap_KLine_open_time [ ticker_symbol ] - 5;
				}
				n_total_sticks = pOrig_KLine_file_info->n_fsize / nSize;
				m_candlestick.mDate = 0;
				for ( ; i < n_total_sticks; /*i += n_sticks*/ ) {
					/*sync the time frame*/
					for ( ; i < n_total_sticks; i++) {
						p_orig_fs->seekg ( i*nSize, ios::beg );
						p_orig_fs->read ( (char *) &m_raw_candlestick, nSize );
						if ( m_raw_candlestick.mDate == current_date )
							continue;
						if ( ( m_raw_candlestick.mTime - m_open_time ) % ( n_sticks * 5 ) == 5) {
							if ( ( pNew_KLine_file_info->n_fsize - nSize ) >= 0 ) {
								p_new_fs->seekp ( pNew_KLine_file_info->n_fsize - nSize, ios::beg );
								p_new_fs->write ( (char *) &m_candlestick, nSize );
							}
							break;
						}
						else {
							if ( m_candlestick.mDate == 0 ) {
								if ( ( pNew_KLine_file_info->n_fsize - nSize ) >= 0 ) {
									p_new_fs->seekg ( pNew_KLine_file_info->n_fsize - nSize, ios::beg );
									p_new_fs->read ( (char *) &m_candlestick, nSize );
								}
							}
							m_candlestick.mClose = m_raw_candlestick.mClose;
							m_candlestick.mDate = m_raw_candlestick.mDate;
							m_candlestick.mTime = m_raw_candlestick.mTime;
							m_candlestick.mVolumn += m_raw_candlestick.mVolumn;
							if ( m_raw_candlestick.mHigh > m_candlestick.mHigh )
								m_candlestick.mHigh = m_raw_candlestick.mHigh;
							if ( m_raw_candlestick.mLow < m_candlestick.mLow )
								m_candlestick.mClose = m_raw_candlestick.mLow;
						}
					}
					for ( j = i, kline_start_index = i; j < ( kline_start_index + n_sticks ) && j < n_total_sticks; j++, i++ ) {
						p_orig_fs->seekg ( j*nSize, ios::beg );
						p_orig_fs->read ( (char *) &m_raw_candlestick, nSize );
						if ( m_raw_candlestick.mDate == current_date )
							continue;
						m_candlestick.mClose = m_raw_candlestick.mClose;
						m_candlestick.mDate = m_raw_candlestick.mDate;
						m_candlestick.mTime = m_raw_candlestick.mTime;
						if ( j == kline_start_index ) {
							m_candlestick = m_raw_candlestick;
						}
						else {
							m_candlestick.mVolumn += m_raw_candlestick.mVolumn;
							if ( m_raw_candlestick.mHigh > m_candlestick.mHigh )
								m_candlestick.mHigh = m_raw_candlestick.mHigh;
							if ( m_raw_candlestick.mLow < m_candlestick.mLow )
								m_candlestick.mClose = m_raw_candlestick.mLow;
						}
					}
					if ( m_raw_candlestick.mDate == current_date )
						continue;
					p_new_fs->seekp ( pNew_KLine_file_info->n_fsize, ios::beg );
					p_new_fs->write ( (char *) &m_candlestick, nSize );	
					pNew_KLine_file_info->n_fsize += nSize;

					pList_KLineData->insert ( pList_KLineData->end(), m_candlestick );
					if ( pNew_KLine_file_info->n_list_begin == -1 )
						pNew_KLine_file_info->n_list_begin = pNew_KLine_file_info->n_list_end = 0;
					else {
						pNew_KLine_file_info->n_list_end++;

						/*if ( pList_KLineData->size() >= 15 ) {
							if ( !mMap_MA15.count ( m_str_symbol ) ) {
								pList_MA15 = new list<double>;
								mMap_MA15.insert( pair<string, list<double>*> ( m_str_symbol, pList_MA15 ) );
							}
							else {
								pList_MA15 = mMap_MA15 [ m_str_symbol ];
							}
							if ( pList_KLineData->size() == 15 ) {
								itr = pList_KLineData->rbegin();
								for ( sum = 0, j = 0, itr1 = itr; j < 15; j++, itr1++ ) {
									sum += itr1->mClose;
								}
							}
							else {
								//itr = pList_KLineData->rbegin();
								//itr3 = itr1.base();
								itr1 = itr = pList_KLineData->rbegin();
								j = 0;
								while ( j++ < 15 )
									itr1++;
								//itr1 = itr + 15;
								//if (  itr == pList_KLineData->rbegin() ) {
								sum = sum + itr->mClose - itr1->mClose;
								//sum = sum + itr->mClose - itr3->mClose;
								//}
							}
							//sum = mean;
							mean = sum / 15;
							pList_MA15->insert ( pList_MA15->end(), mean );
							if ( pList_MA15->size() == 1001 )
								pList_MA15->pop_front();
						}*/
						if ( ( pNew_KLine_file_info->n_list_end - pNew_KLine_file_info->n_list_begin + 1 ) > 1000 ) {
							pNew_KLine_file_info->n_list_begin++;
							pList_KLineData->pop_front();
						}
					}
				}
			}
			p_new_fs->flush();
			break;
		case 2:
			max_sticks = 180 / 30;
			if ( n_sticks <= max_sticks ) {
			
			}
			break;
		default:
			return;
	}



	/*int n_MA_Groups;
	if ( pList_KLineData->size() >= 15 ) {

		n_MA_Groups = pList_KLineData->size() - 15 + 1;
		for ( itr = pList_KLineData->rbegin(), i = 0; i < n_MA_Groups && i < 1000; itr++, i++ ) {
			for ( itr1 = itr, mean = 0, j = 0; j < 15; j++, itr1++ ) {
				mean += itr1->mClose;
			}
			
		}
	}*/
}

VOID CALLBACK OnTimerProc(
  HWND hwnd,         // handle to window
  UINT uMsg,         // WM_TIMER message
  UINT_PTR idEvent,  // timer identifier
  DWORD dwTime       // current system time
  ) {
	  ::KillTimer( AfxGetApp ()->GetMainWnd ()->m_hWnd, 198 );
  //open_quote();
}

void CKLineStream::set_KLine_ready( char * ticker_symbol ) {
	mMap_kline_ready [ ticker_symbol ] = true;
}

void CKLineStream::KLine_server_time( int total_secconds ) {
	/*time duration in millisecond from now to next 15:00:00 PM*/
	int escape_time;
    kline_fetch_server_time = total_secconds;
    GetSystemTime( &kline_fetch_system_time );
	if ( total_secconds < 15 * 60 * 60)
		escape_time = ( 15 * 60 * 60 - total_secconds ) * 1000;
	else
		escape_time = ( 24 * 60 * 60 - total_secconds + 15 * 60 * 60 ) * 1000;
	escape_time = 10000;
	::SetTimer( AfxGetApp ()->GetMainWnd ()->m_hWnd, 198, escape_time, (TIMERPROC)OnTimerProc );
}

bool CKLineStream::get_KLine_ready ( char * ticker_symbol ) {
	return mMap_kline_ready [ ticker_symbol ];
}

void CKLineStream::sync_server_time ( int total_secconds ) {
	server_escape_seconds = total_secconds;
	if ( server_escape_seconds >= ( ( 8 * 60 + 30 ) * 60 ) && server_escape_seconds <= ( 15 * 60 * 60 ) )
		this->is_tick_in_kline = false;
	else
		this->is_tick_in_kline = false; //true
}

void CKLineStream::setting_MA ( int MA1_period, int MA2_period ) {
	mMA1_period = (double) MA1_period;
	mMA2_period = (double) MA2_period;
}

int CKLineStream::get_trading_date ( string ticker_symbol ) {
	list<TCandleStick> *pList_KLineData;
	SYSTEMTIME ti;
	int next_trading_date = -1;
	if ( mMap_kline_ready.count( ticker_symbol ) ) {
		if ( mMap_kline_ready [ ticker_symbol ] == true ) {
			if ( mMap_stock_kline.count( ticker_symbol ) ) {
				pList_KLineData = mMap_stock_kline [ ticker_symbol ];
				m_candlestick = pList_KLineData->back();
				if ( is_tick_in_kline == true ) {
				}
				else {
					//next_trading_date = m_candlestick.mDate + 1;
					next_trading_date = next_trade_date ( m_candlestick.mDate );
					m_candlestick.mDate = next_trading_date;
				}
			}
			else {
				//GetSystemTime(&ti);
				GetLocalTime (&ti);
				//if ( current_date == m_file_candlestick.mDate || ( current_date > m_candlestick.mDate && days_difference ( day, month, year, ti.wDay, ti.wMonth, ti.year ) == 1 && server_escape_seconds < ( ( 8 * 60 + 30 ) * 60 ) ) )
				m_candlestick.mDate = next_trading_date = ti.wDay + ti.wMonth * 100 + ti.wYear * 10000;
			}
		}
		else {
			AfxMessageBox(_T("Can't retrieve trading date!"));
			return -1;
		}
	}
	else {
		AfxMessageBox(_T("Can't retrieve trading date!"));
		return -1;
	}
		
	return m_candlestick.mDate;
}

int days_difference ( int d1, int m1, int y1, int d2, int m2, int y2 ) {
	int i,temp,sum=0,month[]={31,28,31,30,31,30,31,31,30,31,30,31};

	//printf("Enter Date 1 : (DD-MM-YYYY) :");
	//scanf("%d %d %d",&d1,&m1,&y1);

	//printf("Enter Date 2 : (DD-MM-YYYY) :");
	//scanf("%d %d %d",&d2,&m2,&y2);

	temp=d1;

	for(i=m1;i<m2+(y2-y1)*12;i++)
	{
		if(i>12)
		{
			i=1;
			y1++;
		}

		if(i==2)
		{
			if(y1%4==0 && (y1%100!=0 || y1%400==0))
				month[i-1]=29;
			else
				month[i-1]=28;
		}

		sum=sum+(month[i-1]-temp);
		temp=0;

		//printf("Month = %d\t Days = %d\t Sum = %d\n",i,month[i-1],sum);

		//if(i%12==0)
			//getch();

	}

	sum=sum+d2-temp;
	//printf("\n Total Number of Days are : %5d",sum);
	return sum;
}

int next_trade_date ( int current_date ) {
	int day, month, year;
	int next_day, next_month, next_year, next_date;
	year = current_date / 10000;
	month = ( current_date - 10000 * year ) / 100;
	day = current_date - 10000 * year - 100 * month;

	int month_days[]={31,28,31,30,31,30,31,31,30,31,30,31};

	if ( month == 2 ) {
		if(year%4==0 && (year%100!=0 || year%400==0))
			month_days[1]=29;
		else
			month_days[1]=28;
	}

	next_year = year;
	next_month = month;
	next_day = day;
	if ( month_days [month-1] == day ) {
		next_month = month + 1;
		next_day = 1;
		if ( next_month > 12 ) {
			next_year = year + 1;
		}
		else {
		}
	}
	else
		if ( month_days [month-1] > day ) {
			next_day = day + 1;
		}

	next_date = next_year * 10000 + next_month * 100 + next_day;
	SYSTEMTIME t = { next_year, next_month, -1 /*ignored*/, next_day };
	FILETIME ft;
	HRESULT hrto   = SystemTimeToFileTime( &t, &ft );
	HRESULT hrback = FileTimeToSystemTime( &ft, &t );
	WORD dayofweek = t.wDayOfWeek;

	if ( dayofweek == 0 )
		next_date = next_trade_date ( next_date );
	else
		if ( dayofweek == 6 ) {
			next_date = next_trade_date ( next_date );
			//next_date = next_trade_date ( next_date );
		}
	return next_date;
}

int previous_nDay_date ( int current_date,  int days ) {
	int day, month, year;
	int previous_day, previous_month, previous_year, previous_date;
	year = current_date / 10000;
	month = ( current_date - 10000 * year ) / 100;
	day = current_date - 10000 * year - 100 * month;
	int month_days[]={31,28,31,30,31,30,31,31,30,31,30,31};

	if ( month == 2 || month == 3 || month == 1 ) {
		if(year%4==0 && (year%100!=0 || year%400==0))
			month_days[1]=29;
		else
			month_days[1]=28;
	}

	previous_year = year;
	previous_month = month;
	previous_day = day;

	while ( days > 0 ) {
		if ( previous_day == 1) {
			if ( previous_month == 1) {
				previous_day = 31;
				previous_month = 12;
				previous_year -= 1;
				if(previous_year%4==0 && (previous_year%100!=0 || previous_year%400==0))
					month_days[1]=29;
				else
					month_days[1]=28;
			}
			else {
				previous_month -= 1;
				previous_day = month_days [ previous_month ];
			}
		}
		else {
			previous_day -= 1;
		}
		days--;
	}
	previous_date = previous_year * 10000 + previous_month * 100 + previous_day;

	return previous_date;
}