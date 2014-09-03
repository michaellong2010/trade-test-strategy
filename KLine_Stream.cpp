#include "stdafx.h"
#include "Functions.h"
#include "KLine_Stream.h"

TCandleStick m_candlestick;

map<int, string> CKLineStream::mTimeFrameName = init_timeframe_map();
CKLineStream::CKLineStream(int time_frame) {
	stream_file = "";
	//ios::binary=
	nTimeFrame = time_frame;
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
		p_fs->close();
		nSize = sizeof((*itr1).second);
		delete ((*itr1).second->p_fs);
		if ( nSize == 4 )
			delete (*itr1).second;
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


	/* tokenize caData¡Ather delimiter is ", "
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
	list<TCandleStick>::iterator dup_itr = std::find(pList_KLineData->begin(), pList_KLineData->end(), m_candlestick);
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
	int end, n_insert_pos;
	n_bytes = sizeof(TCandleStick);
	if ( dup_itr == pList_KLineData->end() ) {
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
		}
		else {
			itr1 = pList_KLineData->begin();
			itr2 = pList_KLineData->end();

			for ( n_insert_pos = pKLine_file_info->n_list_begin; itr1 != itr2; itr1++, n_insert_pos++ ) {
				m_file_candlestick = (*itr1);
				if ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime == m_file_candlestick.mTime ) {
				}
				else
					if ( m_candlestick.mDate > m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime > m_file_candlestick.mTime ) ) {
					}
					else
						if ( m_candlestick.mDate < m_file_candlestick.mDate || ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime < m_file_candlestick.mTime ) ) {
							break;
						}
			}
			/*search candlestick in file*/
			bool found_in_file = false;
			for ( i = 0; i < n_insert_pos; i++ ) {
				p_fs->seekg ( i*n_bytes, ios::beg );
				p_fs->read ( (char *) &m_file_candlestick, n_bytes );
				if ( m_candlestick.mDate == m_file_candlestick.mDate && m_candlestick.mTime == m_file_candlestick.mTime ) {
					found_in_file = true;
				}
			}
			if ( found_in_file == false ) {
				pList_KLineData->insert ( itr1, m_candlestick );
				itr1--;
				itr2 = pList_KLineData->end();
				for ( ; itr1 != itr2; itr1++, n_insert_pos++ ) {
					m_file_candlestick = (*itr1);
					//pKLine_file_info->n_fsize += n_bytes;
					p_fs->seekp ( n_insert_pos*n_bytes, ios::beg );
					p_fs->write ( (char *) &m_file_candlestick, n_bytes );
				}
				p_fs->flush();
				pKLine_file_info->n_fsize = pList_KLineData->size() * n_bytes;
				pKLine_file_info->n_list_end++;
				if ( ( pKLine_file_info->n_list_end - pKLine_file_info->n_list_begin + 1 ) > 1000 ) {
					pKLine_file_info->n_list_begin++;
					pList_KLineData->pop_front();
				}
			}
		}
	}
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
		/*sync all TCandleStick in archive(p_fs) into pList_KLineData*/
		nRead_candlesticks = 1;
		while ( ( f_size - nRead_candlesticks*nSize ) >= 0 &&  nRead_candlesticks < 1000 ) {
			p_fs->seekg ( ( f_size - nRead_candlesticks*nSize ), ios::beg );
			p_fs->read ( (char *) &m_file_candlestick, nSize );
			pList_KLineData->insert ( pList_KLineData->begin(), m_file_candlestick );
			nRead_candlesticks++;

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