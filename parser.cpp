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

	return ""; 

	
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

std::string Parser::get()
{
	int iter = request.find('/');
	std::string filepath = request.substr(iter + 1, request.find(' ', iter) - iter - 1);
	std::cout<<"filepath: "<<filepath<<'\n';

	std::string buf = "";
	std::string buf_full = "";
	std::string buf_data = "";
	
	if(filepath == "")
	{
		buf = "text/html; charset=utf-8";

		filepath = "html/main.html";

		open_file(filepath, buf_data);
	}

	if(is_image(filepath))
	{
		buf = std::string("image/") + current_type;

		open_file(std::string("images/") + filepath, buf_data);
	}



	buf_full = std::string("HTTP/1.1 200 OK\r\n") +
					"Version: HTTP/1.1\r\n" +
					"Content-Type: " + buf + "\r\n" +
					"Content-Length: " + std::to_string(buf_data.size()) +
					flag_stop +
					buf_data + 
					flag_stop;
		

	return buf_full;
	
}

std::string Parser::post()
{
	return "";
}

void Parser::open_file(std::string filepath, std::string &v){

	std::ifstream fin;
	fin.open(filepath, std::ios::binary );
	if(!fin.is_open()){
		std::cout<<"error1\n";	
	}
	else{
		char ch;
		while(fin.get(ch)){
				v.push_back(ch);
		}
		//std::cout<<"file "<<v<<'\n';
	}

	fin.close();
}