#include "stdafx.h"
#include "Functions.h"
#include "KLine_Stream.h"

TCandleStick m_candlestick;
TICK m_tick;

map<int, string> CKLineStream::mTimeFrameName = init_timeframe_map();
CKLineStream::CKLineStream(int time_frame) {
	stream_file = "";
	//ios::binary=
	nTimeFrame = time_frame;
	txt_out.open( "c:\\temp\\TX00_txt", ios::out | ios::ate | ios::trunc );
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

	txt_out.flush();
	txt_out.close();
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
	int i = 0, j = 0, comsume_tok = 1;

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
	for ( vector<string>::iterator itr = mData_Element.begin(); itr != mData_Element.end(); ) {
	   m_get_str = (*itr).c_str();
	   switch (i) {
		 case 0:  //parse date mm/dd/yyyy
             mTok_date = tokenize( m_get_str, " \/" );
			 m_candlestick.mDate = 100 * atoi(mTok_date.at(0).c_str()) + atoi(mTok_date.at(1).c_str()) + 10000 * atoi(mTok_date.at(2).c_str());
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
	
	//if ( dup_itr == pList_KLineData->end() ) {
		//pList_KLineData->push_back(m_candlestick);
		/*sync list<TCandleStick> with KLine data file stream*/
		//m_str_filename = "I:\\2014 VC project\\Trade_Test\\Debug\\data\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
		m_str_filename = "C:\\temp\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
		if ( !file_stream_info.count( m_str_filename ) ) {
			p_fs = new fstream( m_str_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate );
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
			txt_out << m_candlestick.mDate << ", ";
			txt_out << m_candlestick.mTime << ", ";
			txt_out << m_candlestick.mOpen << ", ";
			txt_out << m_candlestick.mHigh << ", ";
			txt_out << m_candlestick.mLow << ", ";
			txt_out << m_candlestick.mClose << ", ";
			txt_out << m_candlestick.mVolumn << "\n";
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
					for ( i = 0; i < n_insert_pos; i++ ) {
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
					}
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
						txt_out << m_candlestick.mDate << ", ";
						txt_out << m_candlestick.mTime << ", ";
						txt_out << m_candlestick.mOpen << ", ";
						txt_out << m_candlestick.mHigh << ", ";
						txt_out << m_candlestick.mLow << ", ";
						txt_out << m_candlestick.mClose << ", ";
						txt_out << m_candlestick.mVolumn << "\n";
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
						txt_out << m_file_candlestick.mDate << ", ";
						txt_out << m_file_candlestick.mTime << ", ";
						txt_out << m_file_candlestick.mOpen << ", ";
						txt_out << m_file_candlestick.mHigh << ", ";
						txt_out << m_file_candlestick.mLow << ", ";
						txt_out << m_file_candlestick.mClose << ", ";
						txt_out << m_file_candlestick.mVolumn << "\n";
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
							mMap_KLine_open_time [ m_str_symbol ] = m_file_candlestick.mTime;
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

	m_str_symbol = ticker_symbol;
	//m_str_filename = "I:\\2014 VC project\\Trade_Test\\Debug\\data\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	m_str_filename = "C:\\temp\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
	//m_str_filename = ".\\data\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];

	map<string, list<TCandleStick>*>::iterator itr = mMap_stock_kline.find( m_str_symbol );
	map<string, TKLineData_FileInfo *>::iterator itr1 =  file_stream_info.find( m_str_filename );
	/*test if a key exist*/
	if ( itr == mMap_stock_kline.end() || !mMap_stock_kline.count(m_str_symbol) ) {
		pList_KLineData = new list<TCandleStick>;
		mMap_stock_kline.insert(pair<string, list<TCandleStick>*>(m_str_symbol, pList_KLineData));

		p_fs = new fstream( m_str_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate );
		if ( p_fs->is_open() == true ) {
		}
		else {
			delete p_fs;
			p_fs = new fstream( m_str_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
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
		nRead_candlesticks = 1;
		while ( ( f_size - nRead_candlesticks*nSize ) >= 0 &&  nRead_candlesticks <= 1000 ) {
			p_fs->seekg ( ( f_size - nRead_candlesticks*nSize ), ios::beg );
			p_fs->read ( (char *) &m_file_candlestick, nSize );
			pList_KLineData->insert ( pList_KLineData->begin(), m_file_candlestick );
			nRead_candlesticks++;
			if ( m_file_candlestick.mTime < mMap_KLine_open_time [ m_str_symbol ] ) {
				mMap_KLine_open_time [ m_str_symbol ] = m_file_candlestick.mTime;
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
	
	list<TCandleStick> *pList_KLineData;
	int next_trading_date = -1, next_kline_close_time, m_open_time, m_open_hour, m_open_min, m_open_sec;
	int m_tick_hour, m_tick_min, m_tick_sec;
	SYSTEMTIME ti; 
	if ( mMap_stock_kline.count( symbol ) ) {
		pList_KLineData = mMap_stock_kline [ symbol ];
		m_candlestick = pList_KLineData->back();
		next_trading_date = m_candlestick.mDate + 1;
		m_candlestick.mDate = next_trading_date;
	}
	else {
		GetSystemTime(&ti);
		m_candlestick.mDate = next_trading_date = ti.wDay + ti.wMonth * 100 + ti.wYear * 10000;
	}
	
	char str_buf [ 100 ];
	m_str_filename = "C:\\temp\\" + string( itoa(m_candlestick.mDate, str_buf, 10 ) ) + "-" + symbol;
	if ( !mMap_stock_ticks.count( symbol ) ) {
		pList_TickData = new list<TICK>;
		mMap_stock_ticks[ symbol ] = pList_TickData;

		p_fs = new fstream( m_str_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate );
		if ( p_fs->is_open() == true ) {
		}
		else {
			delete p_fs;
			p_fs = new fstream( m_str_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
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

	m_tick.m_nPtr = nPtr;
	m_tick.m_nTime = nTime;
	m_tick.m_nBid = nBid;
	m_tick.m_nAsk = nAsk;
	m_tick.m_nClose = nClose;
	m_tick.m_nQty = nQty;

	TICK m_init_tick = { -1, 0, 0, 0, 0, 0 };
	itr1 = pList_TickData->begin();
	itr2 = pList_TickData->end();

	TICK m_list_tick;
	bool found_in_vec = false;
	int n_insert_pos = pTick_file_info->n_list_begin, n_insert_pos1, i, j, n_pos;
#if 1
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
#endif

	list<TCandleStick>::reverse_iterator itr3;
	TCandleStick *m_pTick_candlestick;
	bool found_tick_in_candlestick = false, found_in_file = false;
	float new_price;
#if 1
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

			if ( m_tick.m_nPtr == 0 ) {
				mMap_intraday_open_time.insert (pair<string, int>(symbol, m_tick.m_nTime));
			}

			if ( !mMap_tick_compose_kline.count( symbol ) ) {
				pList_KLineData = new list<TCandleStick>;
				mMap_tick_compose_kline.insert( pair<string, list<TCandleStick>*> ( symbol, pList_KLineData ) );
			}
			else {
				pList_KLineData = mMap_tick_compose_kline[ symbol ];
			}
/*collapse tick data into appropriate candlestick*/
			if ( nTimeFrame <= 2 ) {
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
						  next_kline_close_time = 60 * m_open_hour + ( m_open_min + 15 * ( ( ( 60 * m_tick_hour + m_tick_min ) - ( 60 * m_open_hour + m_open_min ) ) / 15 + 1 ) );
						  for (  itr3 = pList_KLineData->rbegin(); itr3 != pList_KLineData->rend(); itr3++ ) {
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
						  }
						  else {
							  new_price = ( float ) nClose / 100;
							  m_pTick_candlestick->mClose = new_price;
							  if ( m_pTick_candlestick->mHigh < new_price )
								  m_pTick_candlestick->mHigh = new_price;
							  else
								  if ( m_pTick_candlestick->mLow > new_price )
									  m_pTick_candlestick->mLow = new_price;
							  m_pTick_candlestick->mVolumn += nQty;
						  }
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
void CKLineStream::candlestick_collapse ( char * ticker_symbol, int n_sticks ) {
	list<TCandleStick> *pList_KLineData;
	TKLineData_FileInfo *pOrig_KLine_file_info, *pNew_KLine_file_info;
	string m_str_symbol, m_orig_filename, m_new_filename;
	char str_buf[ 100 ];
	int max_sticks;
	fstream *p_orig_fs, *p_new_fs;
	int nSize = sizeof ( TCandleStick );

	if ( !mMap_stock_kline.count ( ticker_symbol ) )
		return;
	else {
		m_str_symbol = ticker_symbol;
		m_orig_filename = "C:\\temp\\" + m_str_symbol + "_" + mTimeFrameName[ nTimeFrame ];
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
				m_new_filename = "C:\\temp\\" + m_str_symbol;

				if ( !mMap_stock_kline.count ( m_str_symbol ) ) {
					pList_KLineData = new list<TCandleStick>;
					mMap_stock_kline.insert ( pair<string, list<TCandleStick> *> ( m_str_symbol, pList_KLineData ) );

					pNew_KLine_file_info = new TKLineData_FileInfo;
					p_new_fs = new fstream( m_new_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate );
					if ( p_new_fs->is_open() == true ) {
					}
					else {
						delete p_new_fs;
						p_new_fs = new fstream( m_new_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
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
					p_new_fs->seekp ( pNew_KLine_file_info->n_fsize, ios::beg );
					p_new_fs->write ( (char *) &m_candlestick, nSize );	
					pNew_KLine_file_info->n_fsize += nSize;

					pList_KLineData->insert ( pList_KLineData->end(), m_candlestick );
					if ( pNew_KLine_file_info->n_list_begin == -1 )
						pNew_KLine_file_info->n_list_begin = pNew_KLine_file_info->n_list_end = 0;
					else {
						pNew_KLine_file_info->n_list_end++;

						if ( ( pNew_KLine_file_info->n_list_end - pNew_KLine_file_info->n_list_begin + 1 ) > 1000 ) {
							pNew_KLine_file_info->n_list_begin++;
							pList_KLineData->pop_front();
						}
					}
				}
			}
			break;
		case 2:
			max_sticks = 180 / 30;
			if ( n_sticks <= max_sticks ) {
			
			}
			break;
		default:
			return;
	}

	list<TCandleStick>::reverse_iterator itr;
	//pNew_KLine_file_info->
	int n_MA_Groups;
	n_MA_Groups = pList_KLineData->size() - 15 + 1;
	for ( itr = pList_KLineData->rbegin(), i = 0; i < n_MA_Groups; itr++, i++ ) {
	}
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