#include "session.h"

session::session(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context)
    : socket_(io_service, context), deadline_(io_service)
{
	
}

void session::check_deadline()
{

	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{


		//std::cout<<"===============exit=================\n";
		//std::cout<<"socket deleted\n";

		//socket_.lowest_layer().close();

		//boost::system::error_code ec;
		//socket_.lowest_layer().cancel(ec);
		//socket_.lowest_layer().shutdown();		
		socket_.lowest_layer().close();
		//do_async_ssl_shutdown();


	}

}



ssl_socket::lowest_layer_type& session::socket()
{
	return socket_.lowest_layer();
}

void session::start()
{
	std::cout<<"===== Connected =====\n";

	socket_.async_handshake(boost::asio::ssl::stream_base::server,
	boost::bind(&session::do_handshake, this,
	  boost::asio::placeholders::error));
}

void session::do_handshake(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout<<"do_handshake - successful(do_handshake)\n";

		deadline_.expires_from_now(boost::posix_time::seconds(timeout));
		deadline_.async_wait(boost::bind(&session::check_deadline, this));

		socket_.async_read_some(boost::asio::buffer(data_, msg_length),
		  boost::bind(&session::do_read, this,
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout<<"socket deleted(do_handshake)\n";
		delete this;
	}
}

void session::do_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	//std::cout<<"bytes_transferred = "<<bytes_transferred<<" --------------------------------\n";

	deadline_.cancel();
	
	//std::cout<<"error: "<<error<<" bytes_transferred: "<<bytes_transferred<<'\n';
	//std::cout<<"error what: "<<error.message()<<'\n';	

	if( (!error)||(error == boost::asio::ssl::error::stream_truncated) )//bytes_transferred == 0
	{

		std::string buf = "";
		buf.assign(data_, msg_length);
		memset(data_,' ', msg_length);

		

		if (buf.find(flag_stop) != std::string::npos) 
		{
			buf_r.append(buf);
			
			//std::cout<<"\ninfo from browser:\n"<<buf_r<<'\n';

			parser.setRequest(buf_r);

			std::string buf_s = "";

			buf_s.append(parser.start());

			//std::cout<<"buf_s: "<<buf_s<<'\n';

			boost::asio::async_write(socket_,
			  boost::asio::buffer(buf_s.c_str(), buf_s.length()),
			  boost::bind(&session::do_write, this,
			    boost::asio::placeholders::error));
			
			
		}
		else
		{

			//std::cout<<"=====added=====\n";
			buf_r.append(buf);
			buf = "";

			deadline_.expires_from_now(boost::posix_time::seconds(timeout));
			deadline_.async_wait(boost::bind(&session::check_deadline, this));

			socket_.async_read_some(boost::asio::buffer(data_, msg_length),
			  boost::bind(&session::do_read, this,
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred));
		}

	}
	else
	{
		std::cout<<"socket deleted(do_read)\n";
		delete this;
	}
}

void session::do_write(const boost::system::error_code& error)
{
	if(!error)
	{
		buf_r.clear();
		//deadline_.expires_from_now(boost::posix_time::seconds(timeout));
		//deadline_.async_wait(boost::bind(&session::check_deadline, this));
		//std::cout<<"\nsended to browser:\n";

		deadline_.expires_from_now(boost::posix_time::seconds(timeout));
		deadline_.async_wait(boost::bind(&session::check_deadline, this));

		socket_.async_read_some(boost::asio::buffer(data_, msg_length),
		  boost::bind(&session::do_read, this,
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout<<"socket deleted(do_write)\n";
		delete this;
	}
}
