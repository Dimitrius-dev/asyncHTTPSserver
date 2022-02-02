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

	std::string get();

	std::string post();

	void open_file(std::string filepath, std::string &v);

private:
	std::string request = "";

	std::string flag_stop = "\r\n\r\n";
	
};

#endif