
#include "session.h"

session::session(tcp::socket socket, boost::asio::io_context &io_context_)
: socket_(std::move(socket)), deadline_(io_context_)
{
}

void session::start()
{
	do_read();
}

void session::check_deadline()
{
	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		//std::cout<<"===============exit=================\n";
		//std::cout<<"socket deleted\n";
	
		socket_.close();
	}

}

void session::do_read()
{
	deadline_.expires_from_now(boost::posix_time::seconds(timeout));
	deadline_.async_wait(boost::bind(&session::check_deadline, this));

	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
	[this, self](boost::system::error_code ec, std::size_t length)
	{
		deadline_.cancel();

		if (!ec)
		{
			std::string url = "https://studentprojects.ru/";		
	
			std::string buf_data = std::string("<head>") + flag_stop +
							"<meta http-equiv=\"refresh\" content=\"0;URL=" + url + "\" />" + flag_stop +
							"</head>" + flag_stop;
		
			std::string redirect_msg = std::string("HTTP/1.1 200 OK") + flag_stop +
								"Version: HTTP/1.1" + flag_stop +
								"Content-Type: text/html; charset=utf-8" + flag_stop +
								"Content-Length: " + std::to_string(buf_data.size()) + flag_stop +
								flag_stop +
								buf_data + 
								flag_stop;

			do_write(redirect_msg.c_str(), redirect_msg.length());
		}
	});
}

void session::do_write(const char *data_send, std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(data_send, length),
	[this, self](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			do_read();
		}
	});
}