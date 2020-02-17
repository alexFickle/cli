#pragma once

#include "cli/Arity.hpp"
#include "cli/details/Destination.hpp"
#include "cli/details/Generator.hpp"

#include <utility>
#include <variant>


namespace cli
{


class GenericArgument
{
public:
	enum class Kind
	{
		NORMAL
	};

	struct NormalState
	{
		details::Destination destination;
		Arity arity;

		NormalState(details::Destination destination_, Arity arity_)
		    : destination(destination_)
		    , arity(arity_)
		{}
	};

public:
	/// @brief Constructor for a normal argument.
	/// @details Do not call directly, use cli::Argument().
	GenericArgument(
	    const char *name,
	    details::Destination destination,
	    Arity arity,
	    const char *help)
	    : _name(name)
	    , _state(std::in_place_type<NormalState>, destination, arity)
	    , _help(help)
	{}

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
		}
	}

	/// @brief Gets a string to use in the help message for this argument.
	const char *GetHelp() const noexcept
	{
		return _help;
	}


private:
	const char *_name;
	std::variant<NormalState> _state;
	const char *_help;
};


} // namespace cli
