#ifndef SESSION_H
#define SESSION_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
	session(tcp::socket socket, boost::asio::io_context &io_context_);

	void start();

private:
	void do_read();

	void do_write(const char *data_send, std::size_t length);

	void check_deadline();

	void disconnect();

	std::string flag_stop = "\r\n";

	boost::asio::deadline_timer deadline_;//order matters
	tcp::socket socket_;//order matters

	enum { max_length = 1024 };
	char data_[max_length];

	int timeout = 12;	
};

#endif