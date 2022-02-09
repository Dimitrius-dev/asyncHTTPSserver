#include "server.h"

server::server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), io_context_(io_context)
{
	do_accept();
}

void server::do_accept()
{
	acceptor_.async_accept(
	[this](const boost::system::error_code& error, tcp::socket socket)
	{
		if (!error)
		{
			std::make_shared<session>(std::move(socket), io_context_)->start();
		}
		
		do_accept();
	});
}

