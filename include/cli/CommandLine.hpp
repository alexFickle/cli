#pragma once

#include "cli/GenericArgument.hpp"
#include "cli/details/Generator.hpp"

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>


namespace cli
{


class CommandLine
{
private:
	// contains all of the information needed about an argument during command
	// line argument parsing.
	struct ArgumentData : GenericArgument
	{
		ArgumentData(const GenericArgument &arg)
		    : GenericArgument(arg)
		{}

		std::size_t count = 0;
	};


public:
	CommandLine(
	    const char *description,
	    std::initializer_list<GenericArgument> arguments)
	    : _description(description)
	{
		std::copy(
		    arguments.begin(), arguments.end(), std::back_inserter(_args));

		std::vector<ArgumentData>::const_iterator startFlagsIt =
		    std::stable_partition(
		        _args.begin(), _args.end(), [](const ArgumentData &argData) {
			        if(argData.GetName() == nullptr)
			        {
				        return true;
			        }
			        return std::strncmp("--", argData.GetName(), 2) != 0;
		        });
		_numPositionals = startFlagsIt - _args.begin();
	}

	std::string GetUsage(const char *name) const
	{
		std::string usage = name;
		for(const ArgumentData &arg : _args)
		{
			usage += ' ';
			usage += arg.GetUsage();
		}
		return usage;
	}

	/// @brief Gets a help message for this command line.
	std::string GetHelp(const char *name) const
	{
		std::string help = _description;
		help += "\n\n";

		help += "Usage: \n  " + GetUsage(name);
		help += "\n\n";

		help += "Arguments: \n";
		for(const ArgumentData &arg : _args)
		{
			help += "  ";
			help += arg.GetHelp();
			help += '\n';
		}
		return help;
	}

	bool Run(const char *name, int argc, const char *const *argv)
	{
		if(argc < 0)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(name, argc, argv).  "
			    "argc must be non-negative");
		}
		if(argv == nullptr)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(name, argc, argv).  "
			    "argv must not be null.");
		}

		for(ArgumentData &argData : _args)
		{
			argData.count = 0;
		}

		const auto flagLookup = [&]() {
			std::unordered_map<
			    std::string_view,
			    std::vector<ArgumentData>::iterator>
			    lookup;
			for(std::vector<ArgumentData>::iterator it =
			        _args.begin() + _numPositionals;
			    it != _args.end();
			    ++it)
			{
				lookup.emplace(it->GetName(), it);
			}
			return lookup;
		}();
		auto positionalIt = _args.begin();
		const auto positionalEnd = _args.begin() + _numPositionals;

		details::Generator generator(argv, argv + argc);

		while(generator.Remaining() != 0)
		{
			const char *const arg = generator.Peek();
			if(arg == nullptr)
			{
				throw std::invalid_argument(
				    "Invalid argument to cli::CommandLine::Run(name, argc, "
				    "argv).  Null pointer as string in argv.");
			}

			if(arg[0] == '-')
			{
				// this argument is a flag
				auto flagEntry = flagLookup.find(arg);
				if(flagEntry == flagLookup.end())
				{
					throw std::invalid_argument(
					    "Invalid command line arguments.  Unknown flag: "
					    + std::string(arg));
				}
				if(flagEntry->second->count
				   == flagEntry->second->GetArity().inclusiveMax)
				{
					throw std::invalid_argument(
					    "Invalid command line arguments.  " + std::string(arg)
					    + " given more than the maximum of "
					    + std::to_string(
					          flagEntry->second->GetArity().inclusiveMax)
					    + " time(s).");
				}
				if(flagEntry->second->GetKind() == GenericArgument::Kind::HELP)
				{
					std::cout << GetHelp(name);
					return true;
				}
				// progress passed the flag
				generator.Next();
				// and handle any value
				flagEntry->second->Handle(generator);
				flagEntry->second->count++;
			}
			else
			{
				// this argument is a positional
				if(positionalIt == positionalEnd)
				{
					throw std::invalid_argument(
					    "Invalid command line arguments.  Unhandled "
					    "argument: "
					    + std::string(arg));
				}
				positionalIt->Handle(generator);
				positionalIt->count++;
				if(positionalIt->count == positionalIt->GetArity().inclusiveMax)
				{
					++positionalIt;
				}
			}
		}

		for(const ArgumentData &arg : _args)
		{
			if(arg.count < arg.GetArity().inclusiveMin)
			{
				throw std::invalid_argument(
				    "Invalid command line arguments.  "
				    + std::string(arg.GetName()) + " given "
				    + std::to_string(arg.count)
				    + " value(s), less than the minimum of "
				    + std::to_string(arg.GetArity().inclusiveMin)
				    + " value(s).");
			}
		}
		return false;
	}

	bool Run(int argc, const char *const *argv)
	{
		if(argc < 1)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(argc, argv).  argc "
			    "must be at least one.");
		}
		if(argv == nullptr)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(argc, argv).  argv "
			    "can not be NULL.");
		}
		return Run(*argv, argc - 1, argv + 1);
	}

private:
	std::vector<ArgumentData> _args;
	const char *_description;
	std::size_t _numPositionals;
};


} // namespace cli
