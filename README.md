# SearchSECO Controller
This is the Controller module used by the SearchSECO project. It is responsible for mediating communication between the other modules, and talking to the user. 

# Installation
After cloning the repository, make sure to initialize and update the submodules by calling 
```
> git submodule init 
> git submodule update
```

## Dependencies
- Windows 10+, Ubuntu 18.04+. Might work on older Windows versions or other UNIX systems but has not been tested.
- [Boost 1.76.0](https://www.boost.org/users/download/)
   - UNIX
      - Install `libboost-all-dev`
   - Windows
      - Save the unzipped directory at the path `C:\Program Files\Boost\boost_1_76_0`.
      - In an adminstrative command prompt, run 
      ```
      > bootstrap.bat
      > b2.exe
      ```
- [SearchSECOCrawler](https://git.science.uu.nl/searchseco/crawler)
- [SearchSECOSpider](https://git.science.uu.nl/searchseco/spider-revisited)
- [SearchSECOParser](https://git.science.uu.nl/searchseco/parser)

## Building
### CMake
Make sure [CMake](https://cmake.org/download) is installed.
- In the root of the project, run
```
> mkdir build && cd build
> cmake ../SearchSECOController
> cmake --build .
```
- Verify the install was successful by running
```
> ./searchseco -v   (UNIX)
> searchseco -v     (Windows)
```

### Visual Studio
Open the solution and simply build the project. When the terminal is started up, you need to only pass the command + flags, without the preceding `searchseco`.

# Usage
After building you can set a (GitHub) personal access token in `./build/cfg/config.txt`. For info on creating this token click [here](https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token). The token needs no permissions (see step 7 of guide) so it will not be able to interact with your personal GitHub account in any way. Without providing a token, the program will not be able to use the SearchSECOCrawler (making it impossible to crawl repositories or upload projects).

After building the program, it can be called using the following syntax:  
```
searchseco command mandatory_argument [--optional_flag flag_argument]
``` 
To see the full list of commands and their functionality see the official [documentation](LINK HERE) or use the `-h` flag.

## Running using Docker
The start command of the conrtoller can also be run using Docker. For this you need to have [Docker](https://docs.docker.com/get-docker/) installed. After this you can use the following command in the main folder to build the image:
`docker build -t controller-image .`
This will build the image for the container. After this you can use the following command to run the container:
`docker run --name=controller-container --network=host -d --restart=always controller-image`
This will start the container in detached mode and it will restart automatically. This is perfect for when you just want to start the worker node. If you want to see the console and be able to issue the stop command you should use the following command:
`docker run --name=controller-container --network=host -i controller-image`

# License

This project is licensed under the GNU Affero General Public License v3.0. See [LICENSE](LICENSE) for more info.

This program has been developed by students from the bachelor Computer Science at Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)

## Licenses for code from other projects

The SearchSECOController does not use licensed external code itself, though the SearchSECO modules do. Please refer to their respective README files:

* [SearchSECOCrawler]()
* [SearchSECOParser]()
* [SearchSECODatabaseAPI]()
