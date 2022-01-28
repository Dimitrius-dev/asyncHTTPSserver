#include "server.h"

server::server(boost::asio::io_service& io_service, unsigned short port)
	: io_service_(io_service),
		acceptor_(io_service,
  		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
		context_(boost::asio::ssl::context::sslv23)
{
	context_.set_options(
		boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use);
	context_.set_password_callback(boost::bind(&server::get_password, this));
	context_.use_certificate_chain_file("keys/pem/server.pem");
	std::cout<<"error1\n";
	context_.use_private_key_file("keys/pem/server.pem", boost::asio::ssl::context::pem);
	std::cout<<"error2\n";
	//context_.use_tmp_dh_file("keys/pem/dh2048.pem");//?
	std::cout<<"error3\n";

	start_accept();
}

std::string server::get_password() const
{
	return "";
}

void server::start_accept()
{
	session* new_session = new session(io_service_, context_);
	acceptor_.async_accept(new_session->socket(),
		boost::bind(&server::do_accept, this, new_session,
		  boost::asio::placeholders::error));
}

void server::do_accept(session* new_session, const boost::system::error_code& error)
{
	if (!error)
	{
		new_session->start();
	}
	else
	{
		delete new_session;
	}

	start_accept();
}
