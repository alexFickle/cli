#include "cli.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <vector>


int main(int argc, const char *const *argv)
{
	std::vector<int> numbers;
	std::optional<std::string> flag;
	bool isBoolFlagGiven;

	using cli::help;

	cli::CommandLine commandLine(
	    "Basic example command line.",
	    {cli::Help("--help"),
	     cli::Usage("--usage"),
	     cli::Version("--version", "1.0.0"),
	     cli::Argument("numbers", numbers, help = "positional args"),
	     cli::Argument("--flag", flag, help = "optional flag"),
	     cli::StoreTrue(
	         "--bool", isBoolFlagGiven, help = "argumentless flag")});

	try
	{
		if(commandLine.Run(argc, argv))
		{
			// informational flag was given
			return 0;
		}
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what();
		return 1;
	}

	// use command line arguments
	std::cout << numbers.size() << std::endl;
	std::cout << flag.value_or("<flag not given>") << std::endl;
	std::cout << isBoolFlagGiven << std::endl;
	return 0;
}
