# Scope
This project is built up out of multiple components that all come together to make a distributed database that stores code fragments and can handle user queries. TThe components are a Spider, a Parser, a distributed Database, an API for that database and a Controller program. The spider is used to fetch repositories from sites like GitHub and the Software Heritage Graph and turns these into individual code fragments and metadata. The parser is used to turn code fragments and metadata into a hash with metadata that can be stored in the database. The distributed database is used to store all of these code fragments and can be queried to return code fragments based on the hashes or the metadata. Finally, the controller program facilitates the communication between these different components and the user and ties everything together.
To get a better view of the full scope of the project, make sure to also check out the repositories of  the three components.

# Setting up
Other than pulling the controller, make sure to also pull the submodules using `git submodule update --recursive --remote`. 
# Building

## Visual Studio
Open the solution file and build the project in Visual Studio. Note that you will not be able to pass on arguments in the local VS debugger

## CMake

### Instruction
- start a command prompt in the root folder and run the following commands
- make sure to have cmake (https://cmake.org/) installed.
- `mkdir build && cd build`
- `cmake ../Executable`
- `cmake --build .`
- `cd Debug`
- To make sure it all works, run `searchseco -v`
### Common errors on Linux
- `Could NOT find Boost`. To install Boost run `sudo apt-get install libboost-all-dev`.
- `Could NOT find CURL`. To install Curl run `sudo apt-get install libcurl4-openssl-dev`.

## Common errors on Windows
- `Could NOT find Boost`:
   - Download Boost 1.76.0 (https://www.boost.org/users/download/) and extract it. 
   - Open a command prompt as administrator and navigate to `C:/Program files`.
   - Run `mkdir Boost`.
   - Move the unzipped folder to this folder.
   - Navigate the command prompt to `C:/Program Files/Boost/boost_1_76_0`.
   - Run `bootstrap.bat`.
   - Run `b2.exe`

# Using
After building the program, it can be called using the following syntax: `searchseco command mandatory_argument [-optional_flag flag_argument]`. To see the full list of commands and their functionality see the official documentation. LINK HERE.

# Dependencies

## searchseco dependencies
The functionality of this program depends on three other searchseco projects and their dependencies:
- [Spider](https://git.science.uu.nl/searchseco/spider)
- [Parser](https://git.science.uu.nl/searchseco/parser)
- [Database API](https://git.science.uu.nl/searchseco/database-api)

## External dependencies
- Boost 1.76.0 (https://www.boost.org/users/download/). 
