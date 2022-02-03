#include "parser.h"

Parser::Parser()
{
}

void Parser::setRequest(std::string request)
{
	this->request = request;
}

std::string Parser::start()
{
	/*
	if(request.find("GET") != std::string::npos)
	{
		std::cout<<"GET\n";
		return get();
	}

	if(request.find("POST") != std::string::npos)
	{
		std::cout<<"POST\n";
		return post();
	}
	*/

	return give(); 

	
}

bool Parser::is_image(std::string line)
{
	for(int i = 0; i < type_images_size;i++)
	{
		if(line.find(type_images[i]) != std::string::npos)
		{
			current_type = type_images[i];
			return true;
		}
	}
	return false;
}

bool Parser::is_page(std::string line)
{
	for(int i = 0; i < type_pages_size;i++)
	{
		if( (line.find(type_pages[i]) != std::string::npos)&&(!line.empty()) )
		{
			current_type = type_pages[i];
			return true;
		}
	}
	return false;
}

std::string Parser::give()
{
	
	if(request.find("POST") != std::string::npos)
	{
		int st = request.find(flag_stop);
		int end = request.find(' ');
		content = request.substr(st , end - st);
		//std::cout<<"========CONTENT: "<<content<<'\n';
	}

	

	int iter = request.find('/');
	std::string filepath = request.substr(iter + 1, request.find(' ', iter) - iter - 1);
	//std::cout<<"filepath: "<<filepath<<"="<<'\n';

	std::string buf = "";
	std::string buf_full = "";
	std::string buf_data = "";
	
	if(filepath.empty())//home page
	{
		//std::cout<<"home page\n";

		buf = "text/html; charset=utf-8";
		filepath = "html/main.html";
		open_file(filepath, buf_data);	
	}
	else
	{
		if(is_page(filepath))//other page
		{
			//std::cout<<"other page\n";
			//std::cout<<"current_type: <<"<<current_type<<"\n";			

			buf = std::string("text/") + current_type.substr(1,current_type.length()) + "; charset=utf-8";

			open_file(std::string("html/") + filepath, buf_data);
		}	
	}

		

	
	if(is_image(filepath))
	{
		buf = std::string("image/") + current_type.substr(1,current_type.length());

		open_file(std::string("images/") + filepath, buf_data);
	}

	if(buf_data == "not_found")
	{
		buf_full = std::string("HTTP/1.1 404 NOT FOUND\r\n") +
					"Version: HTTP/1.1\r\n" +
					flag_stop +
					flag_stop;	
	}
	else
	{
		buf_full = std::string("HTTP/1.1 200 OK\r\n") +
					"Version: HTTP/1.1\r\n" +
					"Content-Type: " + buf + "\r\n" +
					"Content-Length: " + std::to_string(buf_data.size()) +
					flag_stop +
					buf_data + 
					flag_stop;
	}

	
		

	return buf_full;
	
}


void Parser::open_file(std::string filepath, std::string &buf){

	std::ifstream fin;
	fin.open(filepath, std::ios::binary );
	if(!fin.is_open()){
		std::cout<<"error1\n";
		buf = "not_found";
	}
	else{
		char ch;
		while(fin.get(ch)){
				buf.push_back(ch);
		}
		//std::cout<<"file "<<v<<'\n';
	}

	fin.close();
}