#include "session.h"

using boost::asio::ip::tcp;

session::session(tcp::socket socket, boost::asio::ssl::context& context, boost::asio::io_context &io_context)//, boost::asio::io_context& io_context)
    : socket_(std::move(socket), context), deadline_(io_context)
{
}


void session::check_deadline()
{
	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		//std::cout<<"===============exit=================\n";
		disconnect();	
	}

}

void session::disconnect()
{
	//std::cout<<"disconnect(start)\n";

	boost::system::error_code ec;
	socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

	if(!ec)
	{
		//std::cout << "closing the socket, thread = "<<pthread_self()<<'\n';

		if(socket_.lowest_layer().is_open())
		{					
			socket_.lowest_layer().close(ec);
			//std::cout<<"dec\n";
		}
		else
		{
			//std::cout<<"error2\n";
		}
	}
	else
	{
		//std::cout<<"error1\n";
	}
	

	//std::cout<<"disconnect(end)\n";
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
			//std::cout<<"do_handshake - successful(do_handshake)\n";
			do_read();
		}
		else
		{
			//std::cout<<"socket deleted(do_handshake)\n";
		}
	});
}

void session::do_read()
{
	deadline_.expires_from_now(boost::posix_time::seconds(timeout));
	deadline_.async_wait(boost::bind(&session::check_deadline, this));

	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, msg_length),
	[this, self](const boost::system::error_code& ec, std::size_t length)
	{
		deadline_.cancel();
		
		if( (ec == boost::asio::ssl::error::stream_truncated) || 
			(ec == boost::asio::error::eof) )
		{
			//std::cout<<"stream_truncated(ok)\n";
			disconnect();
		}
		else
		{
			if (!ec)
			{
				//std::cout<<"do_read\n";
				std::string buf = "";

				buf.assign(data_, msg_length);
				
				//memset(data_,' ', msg_length);

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
				//std::cout<<"socket deleted(do_read)\n";
				//std::cout<<"error: "<<ec<<" error what: "<<ec.message()<<'\n';
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
			//std::cout<<"do_write\n";

			buf_r.clear();

			do_read();
		}
		else
		{
			//std::cout<<"socket deleted(do_write)\n";
		}
	});
}

