#pragma once
#include <string>

///Struktura, ktera nam definuje souradnice v budove. Rows = x, Columns = y.
struct coordinates
{
	///x-ova souradnice
	size_t rows;
	///y-ova souradnice
	size_t columns;
};


///Struktura, definujici cloveka.
struct name
{
	///Krestni jmeno
	std::string firstName;
	///Prijmeni
	std::string surName;
	///Email, na ktery je provedena rezervace
	std::string email;
	/**Provadi porovnavani a zajistuje trideni v mape
	* @param other Druhy clen k porovnavani*/
	bool operator<(const name& other) const
	{
		if (firstName.compare(other.firstName) == 0)
		{
			if (surName.compare(other.surName) == 0)
				return email.compare(other.email)<0;
			return surName.compare(other.surName)<0;
		}
		return firstName.compare(other.firstName) < 0;
	}
};