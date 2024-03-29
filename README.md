# SearchSECO Controller
![Testing](https://github.com/SecureSECO/SearchSECOController/actions/workflows/testing.yml/badge.svg)

This is the Controller module used by the SearchSECO project. It is responsible for mediating communication between the other modules, and talking to the user. A reference documentation can be found [here](https://secureseco.github.io/SearchSECOController/).

# Running locally
If you want to run the controller connected to the official database, The easiest way is to use the [Docker hub image](https://hub.docker.com/r/searchseco/controller). To do this you need to have [Docker](https://docs.docker.com/get-docker/) installed. After this you can use the following command if you just want to run the start command:
```
docker run --name controller-container -e "github_token=<github_token>" -e "worker_name=<worker_name>" searchseco/controller
```
If you want to run a custom command you can use the following command:
```
docker run --name controller-container --entrypoint="./controller/build/searchseco" -e "github_token=<github_token>" -e "worker_name=<worker_name>" searchseco/controller <command>
```
In these commands you should replace `<github_token>` with your [github token](https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token) and `<worker_name>` with a name for you node to contact to the database with.

# Developing
For developing there are a couple of ways to run the local code as described here. A more elaborate explanation of the architecture can be found in the full [documentation](./Documentation/Documentation.pdf). 

## Cloning
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
- [SearchSECOCrawler](https://github.com/SecureSECO/SearchSECOCrawler)
- [SearchSECOSpider](https://github.com/SecureSECO/SearchSECOSpider)
- [SearchSECOParser](https://github.com/SecureSECO/SearchSECOParser)

## Configuration
There are a couple of configurations that can/need to be done. Firstly the `SearchSECOController/.env` file should contain the ip adresses and ports of the database apis to connect to. Secondly the `SearchSECOController/cfg/config.txt` file contains some more configurations. The worker name field should contain a name for your worker, which will be sent to the database api. Also the github token should have a correct github token. For info on creating this token click [here](https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token). The token needs no permissions (see step 7 of guide) so it will not be able to interact with your personal GitHub account in any way. Without providing a token, the program will not be able to use the SearchSECOCrawler (making it impossible to crawl repositories or upload projects). There are also some optional configurations in this file, like the vorbosity of the output and the amount of CPU cores to use.

## Building
### Docker
You can use the code to build a Docker image. Make sure [Docker](https://docs.docker.com/get-docker/) is installed. In the root of the project run
```
docker build -t controller-image .
```
This will build the image for the container. After this you can use the following command to run the container:
```
docker run --name=controller-container --network=host -d --restart=always controller-image
```
This will start the container in detached mode and it will restart automatically. This is perfect for when you just want to start the worker node. If you want to see the console and be able to issue the stop command you should use the following command:
```
docker run --name=controller-container --network=host -i controller-image
```

By default the build command will build an image that will run the start command. In order to use other commands with Docker you can remove the `, "start"` from the last line of the dockerfile and then use the same build command to build the image and then the interactive run command to run the program. It will listen for a command input, so any command can be put in.

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

## Usage
After building the program, it can be called using the following syntax:  
```
searchseco command mandatory_argument [--optional_flag flag_argument]
``` 
To see the full list of commands and their functionality see the official [user manual](./Documentation/User_Manual.pdf) or use the `-h` flag.

# License

This project is licensed under the GNU Affero General Public License v3.0. See [LICENSE](LICENSE) for more info.

This program has been developed by students from the bachelor Computer Science at Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)

## Licenses for code from other projects

The SearchSECOController does not use licensed external code itself, though the SearchSECO modules do. Please refer to their respective README files:

* [SearchSECOCrawler](https://github.com/SecureSECO/SearchSECOCrawler#licenses-for-code-from-other-projects)
* [SearchSECOParser](https://github.com/SecureSECO/SearchSECOParser#licenses-for-code-from-other-projects)
* [SearchSECODatabaseAPI](https://github.com/SecureSECO/SearchSECODatabaseAPI#licenses-for-code-from-other-projects)
