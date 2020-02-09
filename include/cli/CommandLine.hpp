#pragma once

#include "cli/Argument.hpp"

#include <algorithm>
#include <cstring>
#include <initializer_list>
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
	struct ArgumentData : Argument
	{
		ArgumentData(const Argument &arg)
		    : Argument(arg)
		{}

		std::size_t count = 0;
	};


public:
	CommandLine(
	    const char *description,
	    std::initializer_list<Argument> arguments)
	    : _description(description)
	{
		std::copy(
		    arguments.begin(), arguments.end(), std::back_inserter(_args));

		std::vector<ArgumentData>::const_iterator startFlagsIt =
		    std::stable_partition(
		        _args.begin(), _args.end(), [](const ArgumentData &argData) {
			        if(argData.name == nullptr)
			        {
				        return true;
			        }
			        return std::strncmp("--", argData.name, 2) != 0;
		        });
		_numPositionals = startFlagsIt - _args.begin();
	}

	void Run(const char *name, const char *const *argv, int argc)
	{
		if(argc < 0)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(name, argv, argc).  "
			    "argc must be non-negative");
		}
		if(argv == nullptr)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(name, argv, argc).  "
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
				lookup.emplace(it->name, it);
			}
			return lookup;
		}();
		auto positionalIt = _args.begin();
		const auto positionalEnd = _args.begin() + _numPositionals;
		auto activeFlagIt = _args.end();

		for(auto cliIt = argv; cliIt != argv + argc; ++cliIt)
		{

			if(*cliIt == nullptr)
			{
				throw std::invalid_argument(
				    "Invalid argument to cli::CommandLine::Run(name, argv, "
				    "argc).  Null pointer as string in argv.");
			}


			if(**cliIt == '-')
			{
				// this argument is a flag
				auto flagEntry = flagLookup.find(*cliIt);
				if(flagEntry == flagLookup.end())
				{
					throw std::invalid_argument(
					    "Invalid command line arguments.  Unknown flag: "
					    + std::string(*cliIt));
				}

				if(activeFlagIt != _args.end())
				{
					// but we expected a value
					throw std::invalid_argument(
					    "Invalid command line arguments.  Expected a value "
					    "after "
					    + std::string(activeFlagIt->name)
					    + ", not another flag: " + flagEntry->second->name);
				}
				activeFlagIt = flagEntry->second;
			}
			else
			{
				// this argument is a value
				if(activeFlagIt != _args.end())
				{
					// argument is for a flag
					if(activeFlagIt->count == activeFlagIt->arity.inclusiveMax)
					{
						throw std::invalid_argument(
						    "Invalid command line arguments.  Flag "
						    + std::string(activeFlagIt->name)
						    + "used more than the maximum of "
						    + std::to_string(activeFlagIt->arity.inclusiveMax)
						    + " time(s).");
					}
					activeFlagIt->dest.Store(*cliIt);
					activeFlagIt->count++;
					activeFlagIt = _args.end();
				}
				else
				{
					// argument is for a positional
					if(positionalIt == positionalEnd)
					{
						throw std::invalid_argument(
						    "Invalid command line arguments.  Unhandled "
						    "argument: "
						    + std::string(*cliIt));
					}
					positionalIt->dest.Store(*cliIt);
					positionalIt->count++;
					if(positionalIt->count == positionalIt->arity.inclusiveMax)
					{
						++positionalIt;
					}
				}
			}
		}

		for(const ArgumentData &arg : _args)
		{
			if(arg.count < arg.arity.inclusiveMin)
			{
				throw std::invalid_argument(
				    "Invalid command line arguments.  " + std::string(arg.name)
				    + " given " + std::to_string(arg.count)
				    + " value(s), less than the minimum of "
				    + std::to_string(arg.arity.inclusiveMin) + " value(s).");
			}
		}
	}

	void Run(const char *const *argv, int argc)
	{
		if(argc < 1)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(argv, argc).  argc "
			    "must be at least one.");
		}
		if(argv == nullptr)
		{
			throw std::invalid_argument(
			    "Invalid argument to cli::CommandLine::Run(argv, argc).  argv "
			    "can not be NULL.");
		}
		Run(*argv, argv + 1, argc - 1);
	}

private:
	std::vector<ArgumentData> _args;
	const char *_description;
	std::size_t _numPositionals;
};


} // namespace cli
