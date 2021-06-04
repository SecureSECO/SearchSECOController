/*
This program has been developed by students from the bachelor Computer Science at
Utrecht University within the Software Project course.
© Copyright Utrecht University (Department of Information and Computing Sciences)
*/


// Constants.
#define LOCALHOST "127.0.0.1"
#define PORT "6969"


namespace systemTestsUtils 
{
	/// <summary>
	/// Starts up the database API mock so it can be used in tests.
	/// </summary>
	void startAPIMock();

	/// <summary>
	/// Transforms a given command line to a usable format.
	/// </summary>
	/// <param name="words"> The command that should be executed, splitted on spaces. </param>
	/// <param name="path"> The path from which the executable is called. This is optional as it is not used in all commands. </param>
	/// <returns> The information provided in the arguments in the correct format for the entrypoint. </returns>
	char** getArgv(std::vector<std::string> &words, std::string path = "path");
}