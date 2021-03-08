#include <iostream>
#include <string>
#include <map>
#include <functional>


void help();
void start(std::string args[]);
void quit(std::string args[]);


int main(int argc, char* argv[])
{
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
            std::string value = argv[++i];

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

void start(std::string args[])
{
    std::cout << "Starting program...";
}

void quit(std::string args[])
{
    std::cout << "Stoping program...";
}