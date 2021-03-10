#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "parser/Parser/XmlParser.h"
#include "parser/Parser/StringStream.h"
#include "parser/Parser/SrcMLCaller.h"


void help();
void help(std::string);
void start(std::string args[]);
void quit(std::string args[]);


int main(int argc, char* argv[])
{
    /*std::string path = "C:\\Users\\ilan\\Downloads\\linux-master\\linux-master\\arch\\alpha";
    StringStream* ss = SrcMLCaller::StartSrcML(path);
    XmlParser xmlparser = XmlParser(path.length() + 1);
    std::vector<std::string> hashes = xmlparser.ParseXML(ss);

    std::cout << hashes.size() << '\n';*/

    std::map<std::string, std::function<void(std::string*)>> argmap = 
    {
        {"start", start},
        {"-s", start},
        {"quit", quit},
        {"-q", quit},
    };

    if (argc < 2)
    {
        help();
        return 1;
    }
    if (argmap.count(std::string(argv[1])) == 0)
    {
        help();
        return 1;
    }

    std::string* args = new std::string[argc]();
    std::map<std::string, std::string> flags;

    for (int i = 2; i < argc; i++)
    {
        std::string s = argv[i];
        if (s[0] == '-')
        {
            std::string flag = s.substr(1);
            if (++i >= argc)
            {
                help("Missing arguments for " + s + "\n");
                return 2;
            }
            std::string value = argv[i];

            if (value[0] == '-')
            {
                help(value + "is not a valid argument for " + s);
                return 2;
            }

            flags[flag] = value;
        }
    }

    argmap[std::string(argv[1])](args);

    return 0;
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