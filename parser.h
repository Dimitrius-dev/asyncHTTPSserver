#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Parser
{
public:
	Parser();

	std::string start();

	void setRequest(std::string request);

	std::string give();

	void open_file(std::string filepath, std::string &v);

	bool is_image(std::string line);

	bool is_page(std::string line);

private:
	std::string request = "";
	std::string content = "";

	std::string flag_stop = "\r\n";//"\r\n\r\n";

	
	std::string current_type;

	int type_images_size = 4;
	std::string type_images[4] = {
		".jpg",
		".png",
		".x-icon",
		".ico"
	};

	int type_pages_size = 3;
	std::string type_pages[3] = {
		".html",
		".php",
		".css"
	};

};

#endif