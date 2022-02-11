#include "session.h"

using boost::asio::ip::tcp;

session::session(tcp::socket socket, boost::asio::ssl::context& context, boost::asio::io_context &io_context)//, boost::asio::io_context& io_context)
    : socket_(std::move(socket), context), deadline_(io_context)
{
}

/*
ssl_socket::lowest_layer_type& session::socket()
{
	return socket_.lowest_layer();
}
*/


void session::check_deadline()
{
	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		//std::cout<<"===============exit=================\n";
		//std::cout<<"socket deleted(timeout)"<<&socket_<<'\n';

		//boost::system::error_code ec;
		//socket_.lowest_layer().cancel(ec);
		//socket_.lowest_layer().shutdown();
		
		//---------------------------------		
		//socket_.lowest_layer().close();// - old variant
		//---------------------------------

		disconnect();
				
	
	}

}

void session::disconnect()
{
	socket_.lowest_layer().close();// - old variant
	//socket_.shutdown();
}


void session::start()
{
	//std::cout<<"===== Connected =====\n";

	do_handshake();
}

void session::do_handshake()
{

	auto self(shared_from_this());
	socket_.async_handshake(boost::asio::ssl::stream_base::server, 
	[this, self](const boost::system::error_code& error)
	{
		if (!error)
		{
			//std::cout<<"do_handshake - successful(do_handshake)"<<&socket_<<'\n';
			do_read();
		}
		else
		{
			//std::cout<<"socket deleted(do_handshake)"<<&socket_<<'\n';
		}
	});
}

void session::do_read()
{
	deadline_.expires_from_now(boost::posix_time::seconds(timeout));
	deadline_.async_wait(boost::bind(&session::check_deadline, this));

	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_),
	[this, self](const boost::system::error_code& ec, std::size_t length)
	{
		deadline_.cancel();
		
		if(ec == boost::asio::ssl::error::stream_truncated)
		{
			//std::cout<<"stream_truncated(ok)"<<&socket_<<'\n';
			disconnect();
		}
		else
		{
			if (!ec)
			{
				//std::cout<<"do_read"<<&socket_<<'\n';
				std::string buf = "";

				buf.assign(data_, msg_length);
				memset(data_,' ', msg_length);

				buf_r.append(buf);
				
				//std::cout<<"info from browser:\n";
				//std::cout<<"\ninfo from browser:\n"<<buf_r<<'\n';

				parser.setRequest(buf_r);
				
				buf_s = "";
				buf_s.append(parser.start());

				//std::cout<<"buf_s: \n";
				//std::cout<<"buf_s: "<<buf_s<<'\n';

				do_write(buf_s.c_str(), buf_s.length());
			}
			else
			{
				//std::cout<<"socket deleted(do_read)"<<&socket_<<'\n';
			}
		}
		


		
	});
}

void session::do_write(const char *data_send, std::size_t length)//const char *data_send
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(data_send, length),
	[this, self](const boost::system::error_code& ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			//std::cout<<"do_write"<<&socket_<<'\n';

			buf_r.clear();

			do_read();
		}
	});
}

