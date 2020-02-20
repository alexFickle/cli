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

	cli::CommandLine commandLine(
	    "Basic example command line.",
	    {cli::Help("--help"),
	     cli::Usage("--usage"),
	     cli::Version("--version", "1.0.0"),
	     cli::Argument("numbers", numbers, "positional args"),
	     cli::Argument("--flag", flag, "optional flag")});

	try
	{
		if(commandLine.Run(argc, argv))
		{
			// --help was given
			return 0;
		}
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what();
		return 1;
	}

	// use numbers and flag
	return 0;
}
