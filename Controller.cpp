#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "parser/Parser/XmlParser.h"
#include "parser/Parser/StringStream.h"
#include "parser/Parser/SrcMLCaller.h"
#include "Parser.h"
#include "Commands.h"


void help();
void help(std::string);



int main(int argc, char* argv[])
{
	/*std::string path = "\"c:\\users\\izak\\documents\\uu vakken\\3.3 sp\\code\\parser\"";
	StringStream* ss = SrcMLCaller::StartSrcML(path);
	XmlParser xmlparser = XmlParser(path.length() + 1);
	std::vector<std::string> hashes = xmlparser.ParseXML(ss);

	std::cout << hashes.size() << '\n';*/


	std::string* args = new std::string[argc]();

	for (int i = 0; i < argc; i++)
	{
		args[i] = argv[i];
	}

	std::map<std::string, std::string> flagArgs = Parser::parse("config.txt", args, argc);
	if (flagArgs.count("command") == 0)
	{
		std::cout << "No command found\n";
		return 2;
	}
	if (Commands::isCommand(flagArgs["command"]))
	{
		Commands::execute(flagArgs["command"], flagArgs);
	}
	else
	{
		std::cout << "command not implemented\n";
	}
}