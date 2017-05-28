#pragma once
#include <vector>
#include "Reservation.h"
#include "Structures.h"
#include <memory>
#include <map>
#include "Exceptions.h"
#include "functions.h"


///Trida reprezentujici akci
class Building {

public:
	/**Konstruktor, ktery nam zalozi akci o maximalni velikosti size a priradi ji jmeno Name
	* @param size Maximalni pocet mist na akci
	* @param cena Celkova cena za akci
	* @param name Jmeno akce*/
	inline Building(size_t size, int cena, std::string name): size(size), cena(cena), name(name)
	{
	}

	/**Vrati maximalni velikost
	* @return Velikost*/
	inline size_t GetSize() const
	{
		return size;
	}

	/**Provede zmenu poctu mist na akci.
	* @param cena Celkova cena za akci
	* @param size Maximalni pocet mist na akci
	* @return Zdali se povedla operace*/
	inline bool Resize(size_t size, int cena){
		if(size<seats.size())
		return false;
		this->size=size;
		this->cena = cena;
		return true;
	}

	/**Vlozi radek sedadel za position-ty radek velikosti size (pokud position==0, vkladame na zacatek), pokud nastane chyba, vracime false
	* @param position Pozice, za kterou vkladame
	* @param size Pocet vkladanych pozic
	* @param costS Cena vkladanych sedadel.
	* @return Zdali se povedla operace*/
	inline bool Insert_Row(size_t position, size_t size, int costS)
	{
		if(size==0|| full.size()<position || this->size<size+Sum(full))
			return false;
			
		std::vector<bool> tmpS;
		std::vector<int> tmpC;
		//aktualni radek
		size_t row=1;
		//aktualni sloupec
		size_t counter=1;
		//pocet mist na aktualni radu
		auto sizeR=full.size()==0?0:full[0];
		//byl proveden zapis?
		auto written = false;
		//dolni mez pusunu rezervaci
		size_t offset = 0;
		
		for (size_t i = 0; i <cost.size() ; ++i){
			if(row == position+1 && !written){
				offset = i;
				for (size_t j = 0;  j<size ; ++j){
					tmpS.push_back(false);
			        tmpC.push_back(costS);
				}
				written = true;
			}
			
			if(counter==sizeR){
				++row;
				sizeR = row < full.size() ? full[row-1]:0;
				counter=0;
			}
			
			tmpS.push_back(seats[i]);
			tmpC.push_back(cost[i]);
			++counter;
		}
		//Pokud pridavame za posledni radu, provedeme po prepisu vsech dat pridani rady
		if(position == full.size())
			for (size_t j = 0; j<size; ++j) {
				tmpS.push_back(false);
				tmpC.push_back(costS);
			}
		//uprava rezervaci, aby byly aktualni
		if(reservations.size()!=0 && position != full.size())
		{
			for (auto element = reservations.begin();element!=reservations.end();++element)
			{
				for (size_t i = 0; i < element->second->Get_Seats().size(); ++i)
					if (element->second->Get_Seats()[i]-1 >= offset)
						element->second->Get_Seats()[i] += size;
			}
		}
		full.insert(full.begin()+position,size);
		
		seats=tmpS;
		cost=tmpC;
		
		return true;
	}
	/**Vlozi sloupec sedadel za position-ty sloupec (pokud position==0, vkladame na zacatek), pokud je radek kratsi nez position, vratime false.
	* @param position Pozice, za kterou vkladame
	* @param size Pocet vkladanych pozic
	* @param costS Cena vkladanych sedadel.
	* @return Zdali se povedla operace*/
	inline bool Insert_Column(size_t position, size_t size, int costS)
	{
		if (size == 0|| size>full.size() || this->size<size + Sum(full))
			return false;

		std::vector<bool> tmpS;
		std::vector<int> tmpC;
		//pocet aktualne vlozenych hodnot do sloupce
		size_t number=0;
		//aktualni radek
		size_t row = 1;
		//aktualni sloupec
		size_t counter = 1;
		//pocet mist na aktualni radu
		auto sizeR = full[0];
		
		for (size_t i = 0; i < full.size(); ++i)
			if (full[i] < position)
				return false;

		for (size_t i = 0; i <cost.size() ; ++i)
		{
			if(counter==position+1 && number<size){
			    tmpS.push_back(false);
			    tmpC.push_back(costS);
				coordinates a;
				a.rows = row;
				a.columns = counter;
				auto compare = Get_Array_Position(a);
				for (auto element = reservations.begin(); element != reservations.end(); ++element)
					for (size_t j = 0; j < element->second->Get_Seats().size(); ++j)
						if (element->second->Get_Seats()[j] >= compare+number)
							element->second->Get_Seats()[j] += 1;
			    ++number;
			}
			if(counter==sizeR){
				++row;
				sizeR = row < full.size() ? full[row - 1] : 0;
				counter=0;
				//Pokud pridavame na konec radku, pridame sedadla z konce rady a pote provedeme pridani sedadla
				if(position==sizeR&&sizeR!=0)
				{
					tmpS.push_back(seats[i]);
					tmpC.push_back(cost[i]);
					++counter;
					tmpS.push_back(false);
					tmpC.push_back(costS);
					++number;
					continue;
				}
			}
			tmpS.push_back(seats[i]);
			tmpC.push_back(cost[i]);
			++counter;
		}

		for (size_t i = 0; i <size; ++i)
		    full[i]+=1;
		
		seats=tmpS;
		cost=tmpC;
		
		return true;
	}
	/**Odebere position-ty radek sedadel a upravi podle toho rezervace, pokud radek neexistuje, vratime false.
	* @param position Pozice, kde mazeme
	* @param size Pocet mazanych pozic
	* @return Zdali se povedla operace*/
	inline bool Remove_Row(size_t position, size_t size)
	{

		if (full.size()<=position||position==0)
			return false;

		if (full[position-1] < size||size==0)
			return false;

		std::vector<bool> tmpS;
		std::vector<int> tmpC;
		//bylo provedeno mazani?
		auto del = false;
		//aktualni radek
		size_t row = 1;
		//aktualni sloupec
		size_t counter = 1;
		//pocet mist na aktualni radu
		auto sizeR = full[0];
		//urci nam od jakeho indexu mame posunout o size dolu rezervace
		size_t offset = 0;

		for (size_t i = 0; i <cost.size(); ++i) {
			if (row == position && !del) {
				del = true;
				//pokud nektere z mist, ktere mazeme, je obsazene, mazani prerusime
				for(auto j=i;j<i+size;++j)
					if (seats[j])
						return false;
				//skocime na posledni prvek, ktery mazeme
				offset = i;
				i += size-1;
				counter = size;
				//pokud jsme smazali cely radek, provedeme zvyseni radku a zvednuti sloupce na 1, jinak pouze zvedneme sloupec o dalsi prvek
				if (counter == sizeR) {
					++row;
					sizeR = row < full.size() ? full[row - 1] : 0;
					counter = 0;
				}
				continue;
			}

			if (counter == sizeR) {
				++row;
				sizeR = row < full.size() ? full[row - 1] : 0;
				counter = 0;
			}

			tmpS.push_back(seats[i]);
			tmpC.push_back(cost[i]);
			++counter;
		}

		//uprava rezervaci
		for (auto element = reservations.begin(); element != reservations.end(); ++element)
		{
			for (size_t i = 0; i < element->second->Get_Seats().size(); ++i)
				if (element->second->Get_Seats()[i] - 1 >= offset)
					element->second->Get_Seats()[i] -= size;
		}

		//domazani struktury
		if (size == full[position - 1])
			full.erase(full.begin() + position - 1);
		else
			full[position - 1] -= size;

		seats = tmpS;
		cost = tmpC;

		return true;
	}
	/**Odebere cely position-ty sloupec sedadel a upravi podle toho rezervace, pokud radek neexistuje, vratime false.
	* @param position Pozice, kde mazeme
	* @param size Pocet mazanych pozic
	* @return Zdali se povedla operace */
	inline bool Remove_Column(size_t position, size_t size)
	{
		//Neni co odstranovat
		if(full.size()==0||this->size<size)
			return false;

		std::vector<bool> tmpS;
		std::vector<int> tmpC;
		//kolik prvku bylo promazano
		size_t del = 0;
		//aktualni radek
		size_t row = 1;
		//aktualni sloupec
		size_t counter = 1;
		//pocet mist na aktualni radu
		auto sizeR = full[0];
		//indexy, kde vsude budeme mazat
		std::vector<size_t> temp;

		if (size == 0)
			return false;

		for (size_t i = 0; i < full.size(); ++i)
			if (full[i] < position)
				return false;

		for (size_t i = 0; i <cost.size(); ++i)
		{
			//Provedeni mazani
			if (counter == position && del<size)
			{
				//Pokud je sedadlo obsazene, mazani se neuskutecni
				if (seats[i])
					return false;
				temp.push_back(i);
				++del;
				//pokud mazeme posledni prvek v radku, provedeme i zvyseni radku
				if (counter == sizeR) {
					sizeR = row < full.size() ? full[row - 1] : 0;
					++row;
					counter = 0;
				}
				++counter;
				continue;
			}
			
			if (counter == sizeR) {
				sizeR = row < full.size() ? full[row - 1] : 0;
				++row;
				counter = 0;
			}
			tmpS.push_back(seats[i]);
			tmpC.push_back(cost[i]);
			++counter;
		}

		for (auto element = reservations.begin(); element != reservations.end(); ++element)
			for (size_t k = 0; k<temp.size(); ++k)
				for (size_t j = 0; j < element->second->Get_Seats().size(); ++j)
					if (element->second->Get_Seats()[j]-1 >= temp[k] - k)
						element->second->Get_Seats()[j] -= 1;

		//domazani struktury
		for (size_t i = 0; i <size; ++i)
		{
			full[i] -= 1;
			if (full[i] == 0)
				full.erase(full.begin() + i);
		}

		seats = tmpS;
		cost = tmpC;

		return true;
	}
	/**Provede editaci rezervace - udaje se nahradi novymi, pokud se shoduji hesla a prepise take obsazenost mist
	* @param seats Cisla sedadel, ktere vkladame
	* @param cost Cena sedadel, ktere vkladame
	* @param person Osoba slouzici jako klic k vyhledavani rezervaci
	* @param password Heslo potrebne ke zmene rezervace
	* @return Zdali se povedla operace*/
	inline bool Alter_Reservation(std::vector<size_t>& seats, std::vector<int>& cost, name person, std::string password)
	{
		auto tmp=reservations.find(person);
		if (reservations.end() == tmp)
			return false;

		if (tmp->second->CheckPassword(password))
		{
			tmp->second->Get_Person().firstName = person.firstName;
			tmp->second->Get_Person().surName = person.surName;
			tmp->second->Get_Person().email = person.email;

			for (size_t i = 0; i < tmp->second->Get_Seats().size(); ++i)
				this->seats[tmp->second->Get_Seats()[i]-1] = false;

			for (size_t i = 0; i < seats.size(); ++i)
				this->seats[seats[i]-1] = true;

			tmp->second->Get_Seats() = seats;
			tmp->second->Get_Cost() = cost;
			return true;
		}
		return false;
	}
	/**Vlozi rezervaci na konkretni mista pro konkretni osobu. Pokud je nektere z mist zabrane, vratime false.
	* @param seats Cisla sedadel, ktere vkladame
	* @param cost Cena sedadel, ktere vkladame
	* @param person Osoba slouzici jako klic k vyhledavani rezervaci
	* @param password Heslo potrebne k uprave rezervace
	* @return Zdali se povedla operace*/
	inline bool Add_Reservation(std::vector<size_t>& seats, std::vector<int>& cost, name person, std::string password)
	{
		auto tmp = reservations.find(person);
		if (tmp != reservations.end())
			return false;
		//test obsazenosti mist
		for (size_t i = 0; i < seats.size(); ++i)
		{
			if (this->seats[seats[i]-1])
				return false;
		}
		//obsazeni mist
		for (size_t i = 0; i < seats.size(); ++i)
		{
			this->seats[seats[i] - 1] = true;
		}
		reservations.emplace(std::make_pair(person, std::make_unique<Reservation>(Reservation{ seats, cost, person, password })));
		return true;
	}

	/**Zrusi rezervaci osoby person, pokud osoba nema rezervaci, vratime false.
	* @param person Osoba slouzici jako klic k vyhledani
	* @param password Heslo potrebne ke smazani rezervace
	* @return Zdali se povedla operace*/
	inline bool Remove_Reservation(name person,std::string password)
	{
		auto tmp = reservations.find(person);
		if(tmp==reservations.end())
			return false;
		if (tmp->second->CheckPassword(password))
		{
			//nastaveni neobsazenosti
			for (size_t i = 0; i < tmp->second->Get_Seats().size(); ++i)
				seats[tmp->second->Get_Seats()[i]-1] = false;
			reservations.erase(tmp);
			return true;
		}
		return false;
	}
	/**Vrati nam odkaz na rezervaci osoby person a nebo null
	* @param person Osoba, jejiz rezervaci chceme najit
	* @return Pozadovanou rezervaci nebo null*/
	inline Reservation* Find_Reservation(name person)
	{
		auto tmp = reservations.find(person);
		if (tmp == reservations.end())
			return nullptr;
		return tmp->second.get();
	}

	/**Vrati nam cenu vybraneho sedadla.
	* @param seat Cislo sedadla
	* @return Cena sedadla*/
	inline int Get_Cost(size_t seat)
	{
		return cost[seat];
	}
	/**Vrati obsazenost sedadel
	* @return Obsazenost sedadel ve forme LinkedList, true==obsazeno*/
	inline std::vector<bool>& Get_Seats()
	{
		return seats;
	}
	/**Vrati cenu sedadel
	* @return Cena sesdadel*/
	inline std::vector<int>& Get_Costs()
	{
		return cost;
	}

	/**Vrati nam pozici v poli ze souradnic position.
	* @param position Souradnice, u kterych chceme dostat cislo sedadla
	* @return Cislo sedadla*/
	inline size_t Get_Array_Position(coordinates position) {
		size_t sum = 0;
		if (position.rows>full.size())
			throw bad_coordinate();
		for (size_t i = 0; i<position.rows - 1; ++i)
			sum += full[i];
		if (position.columns>full[position.rows - 1])
			throw bad_coordinate();
		sum += position.columns;
		return sum;
	}

	/**Provede vypis parametru akce
	* @return Vypis v podobe String*/
	inline std::string to_string()
	{
		auto output = "Maximalni pocet mist na akci: "+std::to_string(size)+"\n";
		for (size_t i = 0; i < seats.size(); ++i)
			output += "sedadlo: " + std::to_string(i+1) + "    obsazeno?: " + GetBool(seats[i])+ "    cena:"+ std::to_string(cost[i]) + "\n";
		output += "Celkovy pocet volnych mist:" + SumFalse(seats) + "\n" + "Celkovy zisk: " + Hodnot(seats, cost, cena);

		return output;
	}

private:
	///rika nam, kolik mist muze byt v kazde rade
	std::vector<size_t> full;
	///Vsechny sedadla na akci
	std::vector<bool> seats;
	///Cena jednotlivych sedadel na akci
	std::vector<int> cost;
	///Maximalni velikost budovy
	size_t size;
	///Cena akce pro poradatele
	int cena;
	///Rezervace setridene podle osob
	std::map<name,std::unique_ptr<Reservation>> reservations;
	///Jmeno akce
	std::string name;
};
