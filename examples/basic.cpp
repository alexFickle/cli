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
	    {cli::Argument("numbers", numbers, "positional args"),
	     cli::Argument("--flag", flag, "optional flag")});

	try
	{
		commandLine.Run(argc, argv);
	}
	catch(const std::exception &e)
	{
		std::cerr << e.what();
		return 1;
	}

	return 0;
}
