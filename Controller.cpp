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
	Parser::parse("config.txt", nullptr, argc);

}

