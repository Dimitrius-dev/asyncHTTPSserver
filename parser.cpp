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

std::string Parser::get()
{
	int iter = request.find('/');
	std::string filepath = request.substr(iter + 1, request.find(' ', iter) - iter - 1);
	std::cout<<"filepath: "<<filepath<<'\n';

	std::string buf = "";
	std::string buf_full = "";
	std::string buf_html = "";
	
	if(filepath == "")
	{
		buf = "text/html; charset=utf-8";

		buf_html = std::string("<html>\n") + 
						"<body>\n" +
						"<h1>Привет?</h1>\n" +
						"<h2>Ну ты и лох</h2>\n" +
						"<p><img src=\"photo.jpg\"></p>\n"
						"</body>\n"
						"</html>";

		buf_full = std::string("HTTP/1.1 200 OK\r\n") +
						"Version: HTTP/1.1\r\n" +
						"Content-Type: " + buf + "\r\n" +
						"Content-Length: " + std::to_string(buf_html.length()) +
						flag_stop +
						buf_html + 
						flag_stop;
	}

	if(filepath == "photo.jpg")
	{
		buf = "image/jpeg";
		
		//std::vector<char> v;
		std::string v;

		open_file(filepath, v);

		buf_full = std::string("HTTP/1.1 200 OK\r\n") +
						"Version: HTTP/1.1\r\n" +
						"Content-Type: " + buf + "\r\n" +
						"Content-Length: " + std::to_string(v.size()) +
						flag_stop +
						v + 
						flag_stop;
		
	}

	

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
		std::cout<<"file "<<v<<'\n';
	}

	fin.close();
}