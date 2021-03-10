#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "parser/Parser/XmlParser.h"
#include "parser/Parser/StringStream.h"
#include "parser/Parser/SrcMLCaller.h"
#include "Parser.h"


void help();
void help(std::string);
void start(std::string args[]);
void quit(std::string args[]);


int main(int argc, char* argv[])
{
	//std::string path = "\"c:\\users\\izak\\documents\\uu vakken\\3.3 sp\\code\\parser\"";
	//stringstream* ss = srcmlcaller::startsrcml(path);
	//xmlparser xmlparser = xmlparser(path.length() + 1);
	//std::vector<std::string> hashes = xmlparser.parsexml(ss);

	//std::cout << hashes.size() << '\n';
	Parser::parse("config.json", nullptr, argc);

}

void help()
{
	std::cout << "help section is yet to be implemented :(";

}
void help(std::string message)
{
	std::cout << message;
	help();
}

void start(std::string args[])
{
	std::cout << "Starting program...";
}

void quit(std::string args[])
{
	std::cout << "Stoping program...";
}