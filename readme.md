# Scope
With this program, a user can upload their code to the searchseco database or check if their code contains duplicates in other code. To get a better view of the full scope of the project, make sure to also check out the repositories of  the three components.

# Setting up
Other than pulling the controller, make sure to also pull the submodules using `git submodule update --recursive --remote`. 
# Building

## Visual Studio
Open the solution file and build the project in Visual Studio. Note that you will not be able to pass on arguments in t

## CMake
- start an administrator command prompt in the root folder and run the following commands
- `mkdir build && cd build`
- `cmake ../Executable`
- `cmake --build .`
- `cd Debug`
- To make sure it all works, run `searchseco -h`

# Dependencies
The functionality of this program depends on three other searchseco projects and their dependencies:
- [Spider](https://git.science.uu.nl/searchseco/spider)
- [Parser](https://git.science.uu.nl/searchseco/parser)
- [Database API](https://git.science.uu.nl/searchseco/database-api)
