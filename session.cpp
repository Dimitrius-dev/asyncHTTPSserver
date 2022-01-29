#include "session.h"

session::session(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context)
    : socket_(io_service, context), deadline_(io_service)
{
	timeout = 120;
}

void session::check_deadline()
{

	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		std::cout<<"socket deleted\n";

		//socket_.close();//??

		socket_.lowest_layer().close();
	}

}



ssl_socket::lowest_layer_type& session::socket()
{
	return socket_.lowest_layer();
}

void session::start()
{
	socket_.async_handshake(boost::asio::ssl::stream_base::server,
	boost::bind(&session::do_handshake, this,
	  boost::asio::placeholders::error));
}

void session::do_handshake(const boost::system::error_code& error)
{
	if (!error)
	{
		//deadline_.expires_from_now(boost::posix_time::seconds(timeout));
		//deadline_.async_wait(boost::bind(&session::check_deadline, this));

		//deadline_.cancel();

		socket_.async_read_some(boost::asio::buffer(data_, msg_length),
		  boost::bind(&session::do_read, this,
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		delete this;
	}
}

void session::do_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (!error)
	{

		std::string buf = "";
		buf.assign(data_, msg_length);
		memset(data_,' ', msg_length);

		

		if (buf.find(flag_stop) != std::string::npos) 
		{
			buf_r.append(buf);

			std::cout<<"\n========size of buf_r======== : "<<buf_r.length()<<'\n';
			
			std::cout<<"\ninfo from browser:\n"<<buf_r<<'\n';


			std::string buf_html =  std::string("<html>\n") + 
						"<body>\n" +
						"<h1>Привет?</h1>\n" +
						"<h2>Ну ты и лох</h2>\n" +
						"<p>ахахахахха</p>\n"
						"</body>\n"
						"</html>";

			std::string buf_s = std::string("HTTP/1.1 200 OK\r\n") +
						"Version: HTTP/1.1\r\n" +
						"Content-Type: text/html; charset=utf-8\r\n" +
						"Content-Length: " + std::to_string(buf_html.length()) +
						flag_stop +
						buf_html + 
						flag_stop;

			boost::asio::async_write(socket_,
			  boost::asio::buffer(buf_s.c_str(), buf_s.length()),
			  boost::bind(&session::do_write, this,
			    boost::asio::placeholders::error));
			
			
		}
		else
		{

			buf_r.append(buf);
			buf = "";

			socket_.async_read_some(boost::asio::buffer(data_, msg_length),
		  boost::bind(&session::do_read, this,
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred));
		}

	}
	else
	{
		delete this;
	}
}

void session::do_write(const boost::system::error_code& error)
{
	if (!error)
	{
		buf_r.clear();
		//deadline_.expires_from_now(boost::posix_time::seconds(timeout));
		//deadline_.async_wait(boost::bind(&session::check_deadline, this));	

		socket_.async_read_some(boost::asio::buffer(data_, msg_length),
		  boost::bind(&session::do_read, this,
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		delete this;
	}
}
