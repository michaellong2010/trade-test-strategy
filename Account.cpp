#include "stdafx.h"
#include "Account.h"

int CAccount::m_Account_count = 0;
TOrder_info m_order;
/*account_name is a unique identifier for each account*/
CAccount::CAccount( string account_name, int time_frame ) {
	string m_portfolio_filename, m_txt_portfolio_filename;
	long f_size;
	int nRead_orders;
	mMap_origin_margin.insert ( pair <string, double> ( "TX00", 83000 ) );
	mMap_keep_margin.insert ( pair <string, double> ( "TX00", 64000 ) );
	mMap_perpip_value.insert ( pair <string, double> ( "TX00", 200 ) );
	mMap_origin_margin.insert ( pair <string, double> ( "MTX00", 20750 ) );
	mMap_keep_margin.insert ( pair <string, double> ( "MTX00", 16000 ) );
	mMap_perpip_value.insert ( pair <string, double> ( "MTX00", 50 ) );
	margin = free_margin = equity = 300000;

	TCHAR path [ 200 ];
    CStringW path_bufW, bin_filenameW, txt_filenameW;
	CStringA path_bufA;
	GetCurrentDirectory(200, path);
	//AfxMessageBox( path );
	//wcstombs( path_buf, path, sizeof(path_buf) );
	//strcpy ( path_buf, path );
	path_bufA = path;
	list<TOrder_info> *pList_open_order, *pList_close_order;
	//m_portfolio_filename = "C:\\temp\\" + account_name + ".bin";
	//m_txt_portfolio_filename = "C:\\temp\\" + account_name + ".txt";
	//strcat ( path_buf, "\\report\\");
	path_bufA = path_bufA + _T("\\report\\");
	path_bufW = path_bufA;
	_mkdir ( (char *)path_bufA.GetString() );
	m_portfolio_filename = ((char *)path_bufA.GetString()) + account_name + ".bin";
	m_txt_portfolio_filename = ((char *)path_bufA.GetString()) + account_name + ".txt";
	//AfxMessageBox( m_portfolio_filename );
	bin_filenameW = m_portfolio_filename.c_str();
	p_portfolio_fs = new fstream( bin_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate );
	txt_filenameW = m_txt_portfolio_filename.c_str();
	txt_portfolio_fs.open( txt_filenameW.GetString(), ios::out | ios::ate | ios::trunc );
	if ( p_portfolio_fs->is_open() == true ) {
		f_size = p_portfolio_fs->tellg();
		if ( f_size >= 3 * sizeof( double ) ) {
			p_portfolio_fs->seekg ( 0, ios::beg );
			p_portfolio_fs->read ( (char *) &margin, sizeof( double ) );
			p_portfolio_fs->seekg ( sizeof( double ), ios::beg );
			p_portfolio_fs->read ( (char *) &free_margin, sizeof( double ) );
			p_portfolio_fs->seekg ( 2 * sizeof( double ), ios::beg );
			p_portfolio_fs->read ( (char *) &equity, sizeof( double ) );

			nRead_orders = 1;
			while ( 3 * sizeof( double ) + nRead_orders * sizeof( TOrder_info ) <= f_size ) {
				p_portfolio_fs->seekg ( 3 * sizeof( double ) + ( nRead_orders - 1) * sizeof( TOrder_info ), ios::beg );
				p_portfolio_fs->read ( (char *) &m_order, sizeof( TOrder_info ) );

				if ( !mMap_open_order.count ( m_order.ticker_symbol ) ) {
					pList_open_order = new list<TOrder_info>;
					mMap_open_order.insert ( pair< string, list<TOrder_info>* > ( m_order.ticker_symbol, pList_open_order ) );

					pList_close_order = new list<TOrder_info>;
					mMap_close_order.insert ( pair< string, list<TOrder_info>* > ( m_order.ticker_symbol, pList_close_order ) );
				}
				else {
					pList_open_order = mMap_open_order [ m_order.ticker_symbol ];
					pList_close_order = mMap_close_order [ m_order.ticker_symbol ];
				}
				if ( m_order.entry_tick == -1 ) {
				}
				else
					if ( m_order.entry_tick >= 0 ) {
						if ( m_order.exit_tick == -1 ) {
							pList_open_order->insert ( pList_open_order->end(), m_order );
						}
						else 
							if ( m_order.exit_tick >= 0 ) {
								pList_close_order->insert ( pList_close_order->end(), m_order );
							}
					}
				nRead_orders++;
			}
		}
		else {
			p_portfolio_fs->seekp ( 0, ios::beg );
			p_portfolio_fs->write ( (char *) &margin, sizeof( double ) );
			p_portfolio_fs->seekp ( sizeof( double ), ios::beg );
			p_portfolio_fs->write ( (char *) &free_margin, sizeof( double ) );
			p_portfolio_fs->seekp ( 2 * sizeof( double ), ios::beg );
			p_portfolio_fs->write ( (char *) &equity, sizeof( double ) );
			p_portfolio_fs->flush();
		}
	}
	else {
		delete p_portfolio_fs;
		p_portfolio_fs = new fstream( bin_filenameW.GetString(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
		if ( p_portfolio_fs->is_open() ) {
			p_portfolio_fs->seekp ( 0, ios::beg );
			p_portfolio_fs->write ( (char *) &margin, sizeof( double ) );
			p_portfolio_fs->seekp ( sizeof( double ), ios::beg );
			p_portfolio_fs->write ( (char *) &free_margin, sizeof( double ) );
			p_portfolio_fs->seekp ( 2 * sizeof( double ), ios::beg );
			p_portfolio_fs->write ( (char *) &equity, sizeof( double ) );
			p_portfolio_fs->flush();
		}
	}
	m_en_trailing_stop = false;
	m_trailing_stop_points = 100;
	kline_close_time = m_stoploss_count = tradable_time = 0;
	nTimeFrame = time_frame;

	m_Account_index = m_Account_count++;
	m_pOrder_operator = NULL;
}

void CAccount::reset( TStrategy_info &strategy )
{

}

CAccount::~CAccount() {
	refresh_portfolio( true );
	for ( map< string, list<TOrder_info>* >::iterator itr1 = mMap_open_order.begin(); itr1 != mMap_open_order.end(); itr1++ )
		delete (*itr1).second;

	for ( map< string, list<TOrder_info>* >::iterator itr1 = mMap_close_order.begin(); itr1 != mMap_close_order.end(); itr1++ )
		delete (*itr1).second;

	if ( p_portfolio_fs != NULL && p_portfolio_fs->is_open() ) {
		p_portfolio_fs->flush();
		p_portfolio_fs->close();
	}
	delete p_portfolio_fs;

	if ( txt_portfolio_fs.is_open() ) {
		txt_portfolio_fs.flush();
		txt_portfolio_fs.close();
	}
}

int CAccount::Place_Open_Order ( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill, int position_type, double MA10_15min, double MA22_15min, double MA10_day, double MA22_day )
{
#if 1
	map< string, list<TOrder_info>* >::iterator itr;
	itr = mMap_open_order.find ( symbol );

	list<TOrder_info> *pList_open_order, *pList_close_order;

	if ( is_BackFill == 1 /*|| position_type == -1*/ )
		return 0;

	if ( itr == mMap_open_order.end() ) {
	//if ( !mMap_open_order.count ( symbol ) ) {
		pList_open_order = new list<TOrder_info>;
		mMap_open_order.insert ( pair< string, list<TOrder_info>* > ( symbol, pList_open_order ) );

		pList_close_order = new list<TOrder_info>;
		mMap_close_order.insert ( pair< string, list<TOrder_info>* > ( symbol, pList_close_order ) );
	}
	else {
		pList_open_order = mMap_open_order [ symbol ];
		pList_close_order = mMap_close_order [ symbol ];
	}

	final_close = (double) nClose / 100;
	/*close opposite position in open order*/
	double new_free_margin, ticker_margin, pip_value, profit_loss, keep_margin, trading_fee = 100;
	boolean m_touch_stoploss = false;
	pip_value = mMap_perpip_value [ symbol ];
	ticker_margin = mMap_origin_margin [ symbol ];
	keep_margin = mMap_keep_margin [ symbol ];
	list<TOrder_info>::iterator itr1;
	/*int m_tick_hour, m_tick_min, m_current_tick_time;
	m_tick_hour = nTime / 10000;
	m_tick_min = ( nTime - m_tick_hour * 10000 ) / 100;
	m_current_tick_time = 60 * m_tick_hour + m_tick_min;*/
	CCapitalOrder *pCapitalOrder = NULL;
	COPYDATASTRUCT cds;
	for ( itr1 = pList_open_order->begin(); itr1 != pList_open_order->end(); ) {
		m_order = *itr1;
		if ( m_order.entry_tick == nPtr && m_order.open_price == 0 ) {
			m_order.open_price = final_close;
			/*if ( m_order.position_type == Long_position ) {
				//m_order.open_price = nAsk / 100;
			}
			else
				if ( m_order.position_type == Short_position ) {
					//m_order.open_price = nBid / 100;
				}*/

			/*if total stoploss times larger than 2 in the current KLine then override the stoploss points*/
			if ( m_stoploss_count >= 2 ) {
				if ( m_order.position_type == Long_position )
					m_order.stoploss = final_close - 5;
				else
					if ( m_order.position_type == Short_position )
						m_order.stoploss = final_close + 5;
			}
			else
			if ( m_en_stoploss == true ) {			
				if ( m_order.position_type == Long_position )
					m_order.stoploss = final_close - m_trailing_stop_points;
				else
					if ( m_order.position_type == Short_position )
						m_order.stoploss = final_close + m_trailing_stop_points;
			}
			else {
				if ( m_order.position_type == Long_position )
					m_order.stoploss = 0;
				else
					if ( m_order.position_type == Short_position )
						m_order.stoploss = 100000;
			}
			/*if ( m_en_trailing_stop == true ) {
				if ( m_order.position_type == Long_position )
					m_order.stoploss = final_close - m_trailing_stop_points;
				else
					if ( m_order.position_type == Short_position )
						m_order.stoploss = final_close + m_trailing_stop_points;
			}
			else {
				if ( m_stoploss_count >= 2 ) {
					if ( m_order.position_type == Long_position )
						m_order.stoploss = final_close - 5;
					else
						if ( m_order.position_type == Short_position )
							m_order.stoploss = final_close + 5;
				}
				else {
					if ( m_order.position_type == Long_position )
						m_order.stoploss = 0;
					else
						if ( m_order.position_type == Short_position )
							m_order.stoploss = 100000;
				}
			}*/
			m_order.open_time = nTime;
			m_order.profit_loss = m_order.max_profit = m_order.max_loss = 0;
		}
		else {
			if ( m_order.position_type == Long_position )
				profit_loss = m_order.lots * ( nClose / 100 - m_order.open_price ) * pip_value;
			else
				if ( m_order.position_type == Short_position )
					profit_loss = m_order.lots * ( m_order.open_price - nClose / 100 ) * pip_value;
			equity = equity - m_order.profit_loss + profit_loss;
			m_order.profit_loss = profit_loss;

			/*instantaneous keep margin*/
			if ( profit_loss < 0 && ( ticker_margin + profit_loss ) < keep_margin ) {
			}
				
			if ( profit_loss > m_order.max_profit ) {
				m_order.max_profit = profit_loss;
				if ( m_en_trailing_stop == true ) {
					if ( m_order.position_type == Long_position )
						m_order.stoploss = final_close - m_trailing_stop_points;
					else
						if ( m_order.position_type == Short_position )
							m_order.stoploss = final_close + m_trailing_stop_points;
				}
			}
			else {
				if ( profit_loss < m_order.max_loss ) {
					m_order.max_loss = profit_loss;
				}
				if ( ( m_order.position_type == Long_position && final_close <= m_order.stoploss ) || 
					( m_order.position_type == Short_position && final_close >= m_order.stoploss ) )
					m_touch_stoploss = true;
			}
		}

		//if ( m_order.position_type != position_type && m_order.exit_tick == -1) {
		if ( ( m_order.exit_tick == -1 && m_order.position_type == Long_position && ( position_type == Short_position || position_type == Close_long_position || position_type == Close_all_position ) ) ||
			( m_order.exit_tick == -1 && m_order.position_type == Short_position && ( position_type == Long_position || position_type == Close_short_position || position_type == Close_all_position ) ) || m_touch_stoploss == true ) {
			m_order.exit_tick = nPtr + 1;
			if ( m_touch_stoploss == true ) {
				m_order.exit_reason = Close_stoploss_position;
			}
			else
				m_order.exit_reason = position_type;
			pList_close_order->insert ( pList_close_order->end(), m_order );
			/*20150106 added by michael*/
			if ( m_pOrder_operator && m_pOrder_operator->IsKindOf ( RUNTIME_CLASS ( CCapitalOrder ) ) ) {
				m_order.close_price = final_close;
				pCapitalOrder = ( CCapitalOrder * ) m_pOrder_operator;
				cds.cbData = sizeof ( TOrder_info );
				cds.lpData = & m_order;
				cds.dwData = ORDER_COPYDATA;
				::SendMessage ( pCapitalOrder->g_hWnd, WM_COPYDATA, ( WPARAM ) AfxGetMainWnd()->m_hWnd, ( LPARAM ) &cds );
				m_order.close_price = 0;
			}
			pList_open_order->erase ( itr1++ );

			free_margin += ticker_margin;
		}
		else {
			*itr1 = m_order;
			itr1++;
		}
	}

	/*fill with the close price*/
	for ( itr1 = pList_close_order->begin(); itr1 != pList_close_order->end(); itr1++ ) {
		m_order = *itr1;
		if (  m_order.exit_tick == nPtr && m_order.close_price == 0 ) {
			m_order.close_price = final_close;
			/*if ( m_order.position_type == Long_position ) {
				m_order.close_price = nBid / 100;
			}
			else
				if ( m_order.position_type == Short_position ) {
					m_order.close_price = nAsk / 100;
				}*/

				if ( m_order.position_type == Long_position )
					profit_loss = m_order.lots * ( m_order.close_price - m_order.open_price ) * pip_value - trading_fee;
				else
					if ( m_order.position_type == Short_position )
						profit_loss = m_order.lots * ( m_order.open_price - m_order.close_price ) * pip_value - trading_fee;
				equity = equity - m_order.profit_loss + profit_loss;
				m_order.profit_loss = profit_loss;
				free_margin = free_margin  + profit_loss;
				if ( profit_loss > m_order.max_profit ) {
					m_order.max_profit = profit_loss;
				}
				else
					if ( profit_loss < m_order.max_loss )
						m_order.max_loss = profit_loss;
				margin = margin + profit_loss;
				*itr1 = m_order;

				if ( profit_loss < 0 /* && m_stoploss_count < 6 */ ) {
					if ( m_stoploss_count == 0 ) {
						m_orig_MA1_margin = this->m_Strategy.mMA1_margin;
						m_orig_MA2_margin = this->m_Strategy.mMA2_margin;
						m_orig_MA3_margin = this->m_Strategy.mMA3_margin;
					}
					m_stoploss_count++;
					if ( ( m_stoploss_count == 2 && this->m_Strategy.nType < 2 ) || ( m_stoploss_count == 4 && this->m_Strategy.nType >= 2 ) ) {
						this->m_Strategy.mMA1_margin += 0.002;
						this->m_Strategy.mMA2_margin += 0.002;
						this->m_Strategy.mMA3_margin += 0.002;
					}
					else
						if ( ( m_stoploss_count == 4 && this->m_Strategy.nType < 2 ) || ( m_stoploss_count == 8 && this->m_Strategy.nType >= 2 ) ) {
							this->m_Strategy.mMA1_margin += 0.002;
							this->m_Strategy.mMA2_margin += 0.002;
							this->m_Strategy.mMA3_margin += 0.002;
						}
						else
							if ( m_stoploss_count == 6 && this->m_Strategy.nType < 2 ) {
								/*if ( kline_close_time == 0) {
								tradable_time = m_current_tick_time + 60;
								m_stoploss_count = 0;
								}
								else*/
								tradable_time = kline_close_time;
							}
							else
								if ( m_stoploss_count == 12 && this->m_Strategy.nType >= 2 ) {
									if ( kline_close_time == 0) {
										tradable_time = m_current_tick_time + 60;
										//m_stoploss_count = 0;
									}				
								}
				}
		}
	}

	/*create new order with position_type*/
	boolean tradable = true;
	double m_factor = 0, m_MA1;
	if ( pList_open_order->size() == 0 && ( position_type == Long_position || position_type == Short_position ) && m_current_tick_time >= tradable_time ) {
		new_free_margin = free_margin - ticker_margin;

		if ( new_free_margin > 0 && 100 * ( new_free_margin / margin ) > 30.0 ) {
			if ( nTimeFrame == 1 ) {
				m_MA1 = MA10_15min;
				if ( m_stoploss_count >= 2 && m_stoploss_count < 4 )
					m_factor = 0.005;
				else
					if ( m_stoploss_count >= 4 && m_stoploss_count < 6 )
						m_factor = 0.007;
			}
			else
				if ( nTimeFrame == 4 ) {
					m_MA1 = MA10_day;
					if ( m_stoploss_count >= 2 && m_stoploss_count < 4 )
						m_factor = 0.005;
					else
						if ( m_stoploss_count >= 4 && m_stoploss_count < 6 )
							m_factor = 0.007;
				}

			if ( m_stoploss_count < 2 )
			  tradable = true;
			else {
				if ( position_type == Long_position ) {
					/*if ( nClose > ( m_MA1 * ( 1 + m_factor ) ) )
						tradable = true;
					else
						tradable = false;*/
				}
				else
					if ( position_type == Short_position ) {
						/*if ( nClose < ( m_MA1 * ( 1 - m_factor ) ) )
							tradable = true;
						else
							tradable = false;*/
					}
			}
			if ( tradable == true ) {
			strcpy ( m_order.ticker_symbol, symbol.c_str() );
			m_order.entry_tick = nPtr + 1;
			m_order.open_price = m_order.close_price = 0;
			m_order.exit_tick = -1;
			m_order.MA10_15min = MA10_15min;
			m_order.MA22_15min = MA22_15min;
			m_order.MA10_day = MA10_day;
			m_order.MA22_day = MA22_day;
			if ( position_type == Long_position ) {
				m_order.order_type = OP_BUY;
				m_order.position_type = Long_position;
				//m_order.stoploss = 0;
			}
			else {
				m_order.order_type = OP_SHORT;
				m_order.position_type = Short_position;
				//m_order.stoploss =100000;
			}
			m_order.lots = 1;

			free_margin = new_free_margin;
			pList_open_order->insert ( pList_open_order->end(), m_order );
			
			/*20150106 added by michael*/
			if ( m_pOrder_operator && m_pOrder_operator->IsKindOf ( RUNTIME_CLASS ( CCapitalOrder ) ) ) {
				m_order.open_price = final_close;
				pCapitalOrder = ( CCapitalOrder * ) m_pOrder_operator;
				cds.cbData = sizeof ( TOrder_info );
				cds.lpData = & m_order;
				cds.dwData = ORDER_COPYDATA;
				::SendMessage ( pCapitalOrder->g_hWnd, WM_COPYDATA, ( WPARAM ) AfxGetMainWnd()->m_hWnd, ( LPARAM ) &cds );
				m_order.open_price = 0;
			}
			}
		}
	}
#endif

	return 0;
}

void CAccount::refresh_portfolio(bool exit_trading) {
	list<TOrder_info> *pList_open_order, *pList_close_order;
	map < string, list<TOrder_info>* >::iterator itr, itr1;
	list<TOrder_info>::iterator itr2;
	int n_orders = 0;

	txt_portfolio_fs.seekp( 0 );
	//txt_portfolio_fs.seekg( 0, os:beg );
#if 1
	txt_portfolio_fs << "margin: " << margin << "\n";
	txt_portfolio_fs << "free_margin: " << free_margin << "\n";
	txt_portfolio_fs << "equity: " << equity << "\n";

	txt_portfolio_fs << "ticker_symbol" << ", " << "open_price" << ", " << "close_price" << \
					", " << "entry_tick" << ", " << "exit_tick" << ", " << "open_time" << ", " << "lots" << ", " << "position_type: " << \
					", " << "profit_loss" << ", " << "max_loss" << ", " << "max_profit" << ", " << "MA10_min" << ", " << "MA22_min" << ", " << "MA10_day" << ", " << "MA22_day" << \
					", " << "stoploss: " << "\n";
#endif
	if ( p_portfolio_fs != NULL && p_portfolio_fs->is_open() ) {
		p_portfolio_fs->seekp ( 0, ios::beg );
		p_portfolio_fs->write ( (char *) &margin, sizeof( double ) );
		p_portfolio_fs->seekp ( sizeof( double ), ios::beg );
		p_portfolio_fs->write ( (char *) &free_margin, sizeof( double ) );
		p_portfolio_fs->seekp ( 2 * sizeof( double ), ios::beg );
		p_portfolio_fs->write ( (char *) &equity, sizeof( double ) );

		for ( itr = mMap_open_order.begin(), itr1 = mMap_close_order.begin(); itr != mMap_open_order.end(); itr++, itr1++ ) {
			pList_open_order = (*itr).second;
			pList_close_order = (*itr1).second;

			if ( exit_trading == true ) {
				for ( itr2 = pList_open_order->begin(); itr2 != pList_open_order->end(); ) {
					m_order = *itr2;
					/*give up the pending order at last tick*/
					if ( m_order.open_price == 0 && m_order.close_price == 0 && m_order.entry_tick > 0 && m_order.exit_tick == -1 )
						pList_open_order->erase ( itr2++ );
					else
						itr2++;
				}

				if ( pList_close_order->size() > 0 ) {
					m_order = pList_close_order->back();
					if ( m_order.close_price == 0 ) {
						m_order.exit_tick = -1;
						pList_open_order->insert ( pList_open_order->end(), m_order );
						pList_close_order->pop_back();
					}
				}
			}

			for ( itr2 = pList_open_order->begin(); itr2 != pList_open_order->end(); itr2++, n_orders++ ) {
				m_order = *itr2;
				p_portfolio_fs->seekp ( 3 * sizeof ( double ) + n_orders * sizeof ( TOrder_info ), ios::beg );
				p_portfolio_fs->write ( (char *) &m_order, sizeof( TOrder_info ) );
#if 1
				txt_portfolio_fs << "\n============" << m_order.ticker_symbol <<  " open order===========\n";
				txt_portfolio_fs << m_order.ticker_symbol << ", " << m_order.open_price << ", " << m_order.close_price << \
					", " << m_order.entry_tick << ", " << m_order.exit_tick << ", " <<  m_order.open_time << ", " << m_order.lots << ", " << "position_type: " << m_order.position_type << \
					", " << m_order.profit_loss << ", " << m_order.max_loss << ", " << m_order.max_profit << ", " << m_order.MA10_15min << ", " << m_order.MA22_15min << ", " << m_order.MA10_day << ", " << m_order.MA22_day << \
					", " << m_order.stoploss << "\n";
#endif
			}

#if 1
				txt_portfolio_fs << "\n============" << m_order.ticker_symbol <<  " close order===========\n";
#endif
			for ( itr2 = pList_close_order->begin(); itr2 != pList_close_order->end(); itr2++, n_orders++ ) {
				m_order = *itr2;
				p_portfolio_fs->seekp ( 3 * sizeof ( double ) + n_orders * sizeof ( TOrder_info ), ios::beg );
				p_portfolio_fs->write ( (char *) &m_order, sizeof( TOrder_info ) );
#if 1

				txt_portfolio_fs << m_order.ticker_symbol << ", " << m_order.open_price << ", " << m_order.close_price << \
					", " << m_order.entry_tick << ", " << m_order.exit_tick << ", " <<  m_order.open_time << ", " << m_order.lots << ", " << "position_type: " << m_order.position_type << \
					", " << m_order.profit_loss << ", " << m_order.max_loss << ", " << m_order.max_profit  << ", exit_reason: " << m_order.exit_reason << ", "<< m_order.MA10_15min << ", " << m_order.MA22_15min << ", " << m_order.MA10_day << ", " << m_order.MA22_day << \
					", " << m_order.stoploss << "\n";
#endif
			}
#if 1
			txt_portfolio_fs << "\n symbol seperate=============================================================\n\n\n";
#endif
		}
	}

	p_portfolio_fs->flush();
	txt_portfolio_fs.flush();
}

void CAccount::update_kline_close_time ( int new_close_time ) {
	if ( new_close_time > kline_close_time ) {
		if ( kline_close_time > 0 ) {
			this->m_Strategy.mMA1_margin = m_orig_MA1_margin;
			this->m_Strategy.mMA2_margin = m_orig_MA2_margin;
			this->m_Strategy.mMA3_margin = m_orig_MA3_margin;
		}
		kline_close_time = new_close_time;
		m_stoploss_count = 0;
	}
	else
		if ( m_stoploss_count >= 12 && this->m_Strategy.nType >= 2 && m_current_tick_time >= tradable_time ) {
			m_stoploss_count = 0;
			this->m_Strategy.mMA1_margin = m_orig_MA1_margin;
			this->m_Strategy.mMA2_margin = m_orig_MA2_margin;
			this->m_Strategy.mMA3_margin = m_orig_MA3_margin;
		}
}

void CAccount::set_stoploss ( int en_stoploss, int en_trailing_stop, int stoploss ) {
	if ( en_stoploss )
		m_en_stoploss = true;
	else
		m_en_stoploss = false;

	if ( en_trailing_stop )
		m_en_trailing_stop = true;
	else
		m_en_trailing_stop = false;

	m_trailing_stop_points = stoploss;
}

void CAccount::bind_order_operator ( COrder * pOrder_operator )
{
	m_pOrder_operator = pOrder_operator;
}