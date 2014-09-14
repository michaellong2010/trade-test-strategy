#include "Account.h"

TOrder_info m_order;
CAccount::CAccount() {
	margin = free_margin = equity = 200000;
}

int CAccount::Place_Open_Order ( string symbol, int nPtr, int position_type /*, int nTime,int nBid, int nAsk, int nClose, int nQty*/ )
{
#if 1
	map< string, list<TOrder_info>* >::iterator itr;
	itr = mMap_open_order.find ( symbol );

	list<TOrder_info> *pList_open_order, *pList_close_order;

	if ( itr == mMap_open_order.end() ) {
		pList_open_order = new list<TOrder_info>;
		mMap_open_order.insert ( pair< string, list<TOrder_info>* > ( symbol, pList_open_order ) );

		pList_close_order = new list<TOrder_info>;
		mMap_close_order.insert ( pair< string, list<TOrder_info>* > ( symbol, pList_close_order ) );
	}
	else {
		pList_open_order = mMap_open_order [ symbol ];
		pList_close_order = mMap_open_order [ symbol ];
	}

	/*close opposite position*/
	list<TOrder_info>::iterator itr1;
	for ( itr1 = pList_open_order->begin(); itr1 != pList_open_order->end(); ) {
		if ( itr1->position_type != position_type ) {
			m_order = *itr1;
			m_order.exit_tick = nPtr + 1;
			pList_close_order->insert ( pList_close_order->begin(), m_order );
			pList_open_order->erase ( itr1++ );
		}
	}

	/*create new order with position_type*/
	m_order.entry_tick = nPtr + 1;
	if ( position_type == Long_position ) {
		m_order.order_type = OP_BUY;
		m_order.position_type = Long_position;
	}
	else {
		m_order.order_type = OP_SHORT;
		m_order.position_type = Short_position;
	}
	m_order.lots = 1;

	pList_open_order->insert ( pList_open_order->begin(), m_order );
#endif

	return 0;
}