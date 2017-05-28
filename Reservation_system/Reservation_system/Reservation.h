#pragma once
#include "Structures.h"
#include <vector>

///Trida reprezentujici rezervaci
class Reservation {

public:
	/**Konstruktor, ktery dosadi sedadla, kontaktni udaje osoby, co si sedadla zabookovala a heslo.
	* @param seats Sedadla
	* @param cost Cena sedadel
	* @param person Osoba
	* @param password Heslo potrebna k editaci/mazani rezervace*/
	inline Reservation(std::vector<size_t>& seats,std::vector<int>& cost, name person, std::string password)
	{
		std::hash<std::string> str_hash;

		this->person.firstName = person.firstName;
		this->person.surName = person.surName;
		this->person.email = person.email;
		this->seats = seats;
		this->cost = cost;
		hashed_password = str_hash(password);
	}

	/**Vrati vektor cen listku.
	* @return Ceny sedadel*/
	inline std::vector<int>& Get_Cost()
	{
		return cost;
	}

	/**Vrati hash hesla
	* @return Hash hesla*/
	inline size_t Get_Hash() const
	{
		return hashed_password;
	}

	/**Vrati udaje o osobe, ktera provedla rezervaci
	* @return Osobu*/
	inline name& Get_Person()
	{
		return person;
	}

	/**Vrati sedadla, ktera jsou rezervovana
	* @return Vraci sedadla*/
	inline std::vector<size_t>& Get_Seats()
	{
		return seats;
	}

	/**Provede Vytvoreni hashe z password a zkontroluje ho s hashem v Rezervaci
	* @param passwd Heslo
	* @return Souhlasi, nebo ne*/
	inline bool CheckPassword(std::string passwd) const
	{
		std::hash<std::string> str_hash;
		if (str_hash(passwd) == hashed_password)
			return true;
		return false;
	}

	/**Prevod rezervace na text bez hesla
	* @return Textovou podobu rezervace*/
	inline std::string to_string()
	{
		std::string output = "";
		for (size_t i = 0; i < seats.size(); ++i)
		{
			output +="sedadlo: "+ std::to_string(seats[i])+"    cena: "+std::to_string(cost[i])+"\n";
		}
		output += "Jmeno: "+person.firstName+"   Prijmeni: " + person.surName +"   Email: "+ person.email+"\n";
		return output;
	}

private:

	///Kontaktni udaje osoby
	name person;
	///cisla sedadel
	std::vector<size_t> seats;
	///Ceny sedadel
	std::vector<int> cost;
	///Hash hesla
	size_t hashed_password;
};

