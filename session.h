#ifndef SESSION_H
#define SESSION_H

#include "libs.h"
#include "parser.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

//typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket, boost::asio::ssl::context& context, boost::asio::io_context &io_context_);// boost::asio::io_context& io_context);
	
	void start();

	//ssl_socket::lowest_layer_type& socket();

private:

	void check_deadline();

	void disconnect();

	void do_handshake();

	void do_read();

	void do_write(const char *data_send, std::size_t length);//const char *data_send
	
	boost::asio::deadline_timer deadline_;//order matters	
	boost::asio::ssl::stream<tcp::socket> socket_;//order matters	
	//ssl_socket socket_;
	
	enum { msg_length = 2048 };// buffer of https request (read)
	char data_[msg_length];
	std::string buf_r;
	std::string buf_s;

	int timeout = 25; // timeout keep alive

	std::string flag_stop = "\r\n";

	Parser parser;

};

#endif