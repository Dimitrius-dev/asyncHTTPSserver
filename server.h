#ifndef SERVER_H
#define SERVER_H

#include "session.h"

#define CHAINFILE "keys/letsencrypt/fullchain.pem"
#define PRIVKEY "keys/letsencrypt/privkey.pem"

//"keys/letsencrypt/fullchain.pem"
//"keys/letsencrypt/privkey.pem"

//"keys/pem1/server.crt"
//"keys/pem1/server.key"

class server
{
public:
  server(boost::asio::io_context& io_context, unsigned short port);

private:

	std::string get_password() const;

 	void do_accept();

	tcp::acceptor acceptor_;
	boost::asio::ssl::context context_;

	boost::asio::io_context &io_context_;
};

#endif