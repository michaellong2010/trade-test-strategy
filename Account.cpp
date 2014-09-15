#include "Account.h"

TOrder_info m_order;
CAccount::CAccount() {
	string m_portfolio_filename, m_txt_portfolio_filename;
	long f_size;
	int nRead_orders;
	mMap_origin_margin.insert ( pair <string, double> ( "TX00", 83000 ) );
	mMap_keep_margin.insert ( pair <string, double> ( "TX00", 64000 ) );
	mMap_perpip_value.insert ( pair <string, double> ( "TX00", 200 ) );
	margin = free_margin = equity = 300000;

	list<TOrder_info> *pList_open_order, *pList_close_order;
	m_portfolio_filename = "C:\\temp\\account_portfolio_summary.bin";
	m_txt_portfolio_filename = "C:\\temp\\account_portfolio_summary.txt";
	p_portfolio_fs = new fstream( m_portfolio_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate );
	txt_portfolio_fs.open( m_txt_portfolio_filename.c_str(), ios::out | ios::ate | ios::trunc );
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
		}
	}
	else {
		delete p_portfolio_fs;
		p_portfolio_fs = new fstream( m_portfolio_filename.c_str(), ios::in | ios::out | ios::binary | ios::ate | ios::trunc );
		if ( p_portfolio_fs->is_open() ) {
			p_portfolio_fs->seekp ( 0, ios::beg );
			p_portfolio_fs->write ( (char *) &margin, sizeof( double ) );
			p_portfolio_fs->seekp ( sizeof( double ), ios::beg );
			p_portfolio_fs->write ( (char *) &free_margin, sizeof( double ) );
			p_portfolio_fs->seekp ( 2 * sizeof( double ), ios::beg );
			p_portfolio_fs->write ( (char *) &equity, sizeof( double ) );
		}
	}
}

CAccount::~CAccount() {
	for ( map< string, list<TOrder_info>* >::iterator itr1 = mMap_open_order.begin(); itr1 != mMap_open_order.end(); itr1++ )
		delete (*itr1).second;

	for ( map< string, list<TOrder_info>* >::iterator itr1 = mMap_close_order.begin(); itr1 != mMap_close_order.end(); itr1++ )
		delete (*itr1).second;

	if ( p_portfolio_fs != NULL && p_portfolio_fs->is_open() ) {
		p_portfolio_fs->flush();
		p_portfolio_fs->close();
		delete p_portfolio_fs;
	}

	if ( txt_portfolio_fs.is_open() ) {
		txt_portfolio_fs.flush();
		txt_portfolio_fs.close();
	}
}

int CAccount::Place_Open_Order ( string symbol, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty, int is_BackFill, int position_type )
{
#if 1
	map< string, list<TOrder_info>* >::iterator itr;
	itr = mMap_open_order.find ( symbol );

	list<TOrder_info> *pList_open_order, *pList_close_order;

	if ( is_BackFill == 1)
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

	/*close opposite position in open order*/
	double new_free_margin, ticker_margin, pip_value, profit_loss, keep_margin, trading_fee = 100;
	pip_value = mMap_perpip_value [ symbol ];
	ticker_margin = mMap_origin_margin [ symbol ];
	keep_margin = mMap_keep_margin [ symbol ];
	list<TOrder_info>::iterator itr1;
	for ( itr1 = pList_open_order->begin(); itr1 != pList_open_order->end(); ) {
		m_order = *itr1;
		if ( m_order.entry_tick == nPtr ) {
			if ( m_order.position_type == Long_position ) {
				m_order.open_price = nAsk / 100;
			}
			else
				if ( m_order.position_type == Short_position ) {
					m_order.open_price = nBid / 100;
				}
				m_order.profit_loss = m_order.max_profit = m_order.max_loss = 0;
		}
		else {
			profit_loss = m_order.lots * ( nClose / 100 - m_order.open_price ) * pip_value;
			equity = equity - m_order.profit_loss + profit_loss;
			m_order.profit_loss = profit_loss;

			/*instantaneous keep margin*/
			if ( profit_loss < 0 && ( ticker_margin + profit_loss ) < keep_margin ) {
			}
				
			if ( profit_loss > m_order.max_profit )
				m_order.max_profit = profit_loss;
			if ( profit_loss < m_order.max_loss )
				m_order.max_loss = profit_loss;
		}

		if ( m_order.position_type != position_type && m_order.exit_tick == -1) {
			m_order.exit_tick = nPtr + 1;
			pList_close_order->insert ( pList_close_order->end(), m_order );
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
		if (  m_order.exit_tick == nPtr ) {
			if ( m_order.position_type == Long_position ) {
				m_order.close_price = nBid / 100;
			}
			else
				if ( m_order.position_type == Short_position ) {
					m_order.close_price = nAsk / 100;
				}

				profit_loss = m_order.lots * ( m_order.close_price - m_order.open_price ) * pip_value - trading_fee;
				m_order.profit_loss = profit_loss;
				free_margin = free_margin  + profit_loss;
				if ( profit_loss > m_order.max_profit )
					m_order.max_profit = profit_loss;
				if ( profit_loss < m_order.max_loss )
					m_order.max_loss = profit_loss;
				margin = margin + profit_loss;
				*itr1 = m_order;
		}
	}

	/*create new order with position_type*/
	if ( pList_open_order->size() == 0 ) {
		new_free_margin = free_margin - ticker_margin;

		if ( new_free_margin > 0 && 100 * ( new_free_margin / margin ) > 30.0 ) {
			strcpy ( m_order.ticker_symbol, symbol.c_str() );
			m_order.entry_tick = nPtr + 1;
			m_order.exit_tick = -1;
			if ( position_type == Long_position ) {
				m_order.order_type = OP_BUY;
				m_order.position_type = Long_position;
			}
			else {
				m_order.order_type = OP_SHORT;
				m_order.position_type = Short_position;
			}
			m_order.lots = 1;

			free_margin = new_free_margin;
		}
		pList_open_order->insert ( pList_open_order->end(), m_order );
	}
#endif

	return 0;
}

void CAccount::refresh_portfolio() {
	list<TOrder_info> *pList_open_order, *pList_close_order;
	map < string, list<TOrder_info>* >::iterator itr, itr1;
	list<TOrder_info>::iterator itr2;
	int n_orders = 0;

#if 1
	txt_portfolio_fs << "margin: " << margin << "\n";
	txt_portfolio_fs << "free_margin: " << free_margin << "\n";
	txt_portfolio_fs << "equity: " << equity << "\n";
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

			for ( itr2 = pList_open_order->begin(); itr2 != pList_open_order->end(); itr2++, n_orders++ ) {
				m_order = *itr2;
				p_portfolio_fs->seekp ( 3 * sizeof ( double ) + n_orders * sizeof ( TOrder_info ), ios::beg );
				p_portfolio_fs->write ( (char *) &m_order, sizeof( TOrder_info ) );
#if 1
				txt_portfolio_fs << "\n============" << m_order.ticker_symbol <<  "open order===========\n";
				txt_portfolio_fs << m_order.ticker_symbol << ", " << m_order.open_price << ", " << m_order.close_price << \
					", " << m_order.entry_tick << ", " << m_order.exit_tick << ", " << m_order.lots << ", " << "position_type: " << m_order.position_type << \
					", " << m_order.profit_loss << ", " << m_order.max_loss << ", " << m_order.max_profit << "\n";
#endif
			}

#if 1
				txt_portfolio_fs << "\n============" << m_order.ticker_symbol <<  "close order===========\n";
#endif
			for ( itr2 = pList_close_order->begin(); itr2 != pList_close_order->end(); itr2++, n_orders++ ) {
				m_order = *itr2;
				p_portfolio_fs->seekp ( 3 * sizeof ( double ) + n_orders * sizeof ( TOrder_info ), ios::beg );
				p_portfolio_fs->write ( (char *) &m_order, sizeof( TOrder_info ) );
#if 1

				txt_portfolio_fs << m_order.ticker_symbol << ", " << m_order.open_price << ", " << m_order.close_price << \
					", " << m_order.entry_tick << ", " << m_order.exit_tick << ", " << m_order.lots << ", " << "position_type: " << m_order.position_type << \
					", " << m_order.profit_loss << ", " << m_order.max_loss << ", " << m_order.max_profit << "\n";
#endif
			}
#if 1
			txt_portfolio_fs << "\n symbol seperate=============================================================\n\n\n";
#endif
		}
	}
}