#ifndef SESSION_H
#define SESSION_H

#include "libs.h"
#include <iostream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

class session
{
public:
	session(boost::asio::io_service& io_service, boost::asio::ssl::context& context);

	void start();

	ssl_socket::lowest_layer_type& socket();

private:
	
	void do_handshake(const boost::system::error_code& error);
	
	void check_deadline();

	void do_read(const boost::system::error_code& error, size_t bytes_transferred);

	void do_write(const boost::system::error_code& error);

private:
	boost::asio::deadline_timer deadline_;
	
	ssl_socket socket_;

	enum { msg_length = 512 };//512

	char data_[msg_length];

	std::string buf_r;

	int timeout;

	std::string flag_stop = "\r\n\r\n";
	
};

#endif