#pragma once
#include <exception>

///Vyjimka, rikajici, ze byla zadana nespravna cesta
class bad_path : public std::exception
{
public:
	const char* what() const throw() override
	{
		return "Bad path to files.";
	}
};
///Vyjimka, rikajici, ze neexistuje slozka
class not_dir : public std::exception
{
public:
	const char* what() const throw() override
	{
		return "The workingDIR directory doesn't exist next to exe file.";
	}
};
///Vyjimka, rikajici, ze souradnice neexistuji
class bad_coordinate: public std::exception
{
public:
	const char* what() const throw() override
	{
		return "Coordinates don't exist.";
	}
};