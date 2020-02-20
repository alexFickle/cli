#pragma once

#include "cli/Arity.hpp"
#include "cli/details/Destination.hpp"
#include "cli/details/Generator.hpp"
#include "cli/details/Usage.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <variant>


namespace cli
{


class GenericArgument
{
public:
	enum class Kind
	{
		NORMAL,
		HELP,
		USAGE,
		VERSION
	};

private:
	struct NormalState
	{
		details::Destination destination;
		Arity arity;

		NormalState(details::Destination destination_, Arity arity_)
		    : destination(destination_)
		    , arity(arity_)
		{}
	};

	struct HelpState
	{};

	struct UsageState
	{};

	struct VersionState
	{
		const char *version;

		VersionState(const char *version_)
		    : version(version_)
		{}
	};

public:
	/// @brief Constructor for a normal argument.
	/// @details Do not call directly, use cli::Argument().
	GenericArgument(
	    Kind kind,
	    const char *name,
	    details::Destination destination,
	    Arity arity,
	    const char *help)
	    : _name(name)
	    , _state(std::in_place_type<NormalState>, destination, arity)
	    , _help(help)
	{
		assert(kind == Kind::NORMAL);
	}

	/// @brief Constructor for help and usage.
	GenericArgument(Kind kind, const char *name, const char *help)
	    : _name(name)
	    , _state(std::in_place_type<HelpState>)
	    , _help(help)
	{
		switch(kind)
		{
			case Kind::HELP:
				break; // already constructed

			case Kind::USAGE:
				_state.emplace<static_cast<std::size_t>(Kind::USAGE)>();
				break;

			default:
				assert(false);
		}
	}

	/// @brief Constructor for version.
	GenericArgument(
	    Kind kind,
	    const char *name,
	    const char *version,
	    const char *help)
	    : _name(name)
	    , _state(std::in_place_type<VersionState>, version)
	    , _help(help)
	{
		assert(kind == Kind::VERSION);
	}

	/// @brief Gets the name of this argument.
	const char *GetName() const noexcept
	{
		return _name;
	}

	/// @brief Gets the kind of this argument.
	Kind GetKind() const noexcept
	{
		return static_cast<Kind>(_state.index());
	}

	/// @brief Gets the arity of this argument.
	Arity GetArity() const noexcept
	{
		switch(GetKind())
		{
			case Kind::NORMAL:
			{
				const NormalState &state =
				    std::get<static_cast<std::size_t>(Kind::NORMAL)>(_state);
				return state.arity;
			}

			default:
				return Arity::Optional();
		}
	}

	/// @brief Handles the occurrence of a command line argument.
	/// @param[in, out] generator Command line argument generator that contained
	/// this argument.  Should be at the value (if any) that this argument will
	/// be consuming.
	void Handle(details::Generator &generator)
	{
		switch(GetKind())
		{
			case Kind::NORMAL:
			{
				if(generator.Remaining() == 0)
				{
					throw std::invalid_argument(
					    "Invalid command line arguments: Excepted value after "
					    + std::string(GetName()));
				}
				NormalState &state =
				    std::get<static_cast<std::size_t>(Kind::NORMAL)>(_state);
				state.destination.Store(generator.Next());
				break;
			}

			default:
				break;
		}
	}

	const char *GetVersion() const
	{
		if(GetKind() == Kind::VERSION)
		{
			const VersionState &state =
			    std::get<static_cast<std::size_t>(Kind::VERSION)>(_state);
			return state.version;
		}
		return "";
	}

	/// @brief Gets a string to use in the usage message for this argument.
	std::string GetUsage() const
	{
		if(GetKind() == Kind::NORMAL && GetName()[0] == '-')
		{
			return details::MakeUsageString(
			    std::string(GetName()) + ' ' + (GetName() + 2), GetArity());
		}
		return details::MakeUsageString(GetName(), GetArity());
	}

	/// @brief Gets a string to use in the help message for this argument.
	std::string GetHelp() const
	{
		if(GetKind() == Kind::NORMAL && GetName()[0] == '-')
		{
			return std::string(GetName()) + ' ' + (GetName() + 2) + ": "
			    + _help;
		}
		return std::string(GetName()) + ": " + _help;
	}

private:
	const char *_name;
	std::variant<NormalState, HelpState, UsageState, VersionState> _state;
	const char *_help;
};


} // namespace cli
