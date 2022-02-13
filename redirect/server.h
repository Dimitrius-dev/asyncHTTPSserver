#ifndef SERVER_H
#define SERVER_H

#include "session.h"

class server
{
public:
	server(boost::asio::io_context& io_context, short port);

private:
	void do_accept();

	boost::asio::io_context &io_context_;//order matters
	tcp::acceptor acceptor_;//order matters
	
};

#endif