#ifndef SERVER_H
#define SERVER_H

#include "session.h"

class server
{
public:
	server(boost::asio::io_service& io_service, unsigned short port);

	std::string get_password() const;

	void start_accept();

	void do_accept(session* new_session, const boost::system::error_code& error);

private:
  boost::asio::io_service& io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ssl::context context_;
};


#endif