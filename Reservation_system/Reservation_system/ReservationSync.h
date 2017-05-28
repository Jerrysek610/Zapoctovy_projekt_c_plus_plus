#pragma once
#include <vector>
#include "Structures.h"
///Trida slouzici k synchronizaci rezervaci
class ReservationSync
{
public:
	/**Konstruktor, co nam inicializuje polozky, ktere nejsou kontejner
	* @param op Operace
	* @param p Osoba
	* @param pswd Heslo*/
	inline ReservationSync(std::string op,name p, std::string pswd):operation(op),hashed_password(pswd)
	{
		person.firstName = p.firstName;
		person.surName = p.surName;
		person.email = p.email;
	}
	/**Pridani sedadla s cenou
	* @param add Sedadlo
	* @param c Cena*/
	inline void AddSeat(size_t add, int c)
	{
		seats.push_back(add);
		cost.push_back(c);
	}
	/**Pridani vice sedadel
	* @param adds Sedadla
	* @param costs Ceny*/
	inline void AddSeats(std::vector<size_t>& adds, std::vector<int>& costs)
	{
		seats.insert(seats.end(),adds.begin(), adds.end());
		cost.insert(cost.end(), costs.begin(), costs.end());
	}

	/**Vraci operaci
	* @return Operaci*/
	inline std::string GetOp() const
	{
		return operation;
	}
	/**Vraci sedadla
	* @return Sedadla*/
	inline std::vector<size_t>& GetSeats()
	{
		return seats;
	}
	/**Vraci ceny sedadel
	* @return Vraci ceny*/
	inline std::vector<int>& GetCost()
	{
		return cost;
	}
	/**Vrati osobu, kera provedla rezervaci
	* @return Osobu*/
	inline name GetPerson() const
	{
		return person;
	}
	/**Vrati hash hesla
	* @return Hash hesla*/
	inline std::string GetPassword() const
	{
		return hashed_password;
	}
private:
	///Operace, kterou provadime
	std::string operation;
	///Kontaktni udaje osoby
	name person;
	///cisla sedadel
	std::vector<size_t> seats;
	///Ceny sedadel
	std::vector<int> cost;
	///Hash hesla
	std::string hashed_password;
};
