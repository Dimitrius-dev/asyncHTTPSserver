
#include "server.h"

server::server(boost::asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      context_(boost::asio::ssl::context::sslv23), io_context_(io_context)
{
	context_.set_options(
	boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use);
	context_.set_password_callback(std::bind(&server::get_password, this));

	context_.use_certificate_chain_file("keys/pem1/server.crt");
	//std::cout<<"use_certificate_chain_file - ok\n";
	context_.use_private_key_file("keys/pem1/server.key", boost::asio::ssl::context::pem);
	//std::cout<<"use_private_key_file - ok\n";
	context_.use_tmp_dh_file("keys/pem1/dh2048.pem");
	//std::cout<<"use_tmp_dh_file - ok\n";

	do_accept();
}

std::string server::get_password() const
{
	return "test";
}

void server::do_accept()
{
	acceptor_.async_accept(
	[this](const boost::system::error_code& error, tcp::socket socket)
	{
		if (!error)
		{
			std::make_shared<session>(std::move(socket), context_, io_context_)->start();
		}
		
		do_accept();
	});
}
