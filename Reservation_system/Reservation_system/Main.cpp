#include "Building.h"
#include <iostream>
#include <sstream>
#include "IO_System.h"
#include "Structures.h"
#include <algorithm>
#include "functions.h"

/**Naseptavac prikazu
* @param whispering Databaze napovedy
* @param pattern Hledany retezec*/
void Whisper(std::map<std::string, std::vector<std::string>>& whispering, std::string pattern) {

	auto tmp = whispering.find(pattern);
	if (whispering.end() == tmp)
		std::cout << "Nejsou dalsi moznosti" << std::endl;
	else
		for (size_t i = 0; i<tmp->second.size(); ++i)
			std::cout << tmp->second[i]<<std::endl;
}

/**Naseptavac se podiva na vsechny argumenty a pouzije naseptani
* @param whispering Databaze napovedy
* @param split Nakouskovana radka
* @param pattern  Hledany retezec v napovede
* @param dolni Dolni index projitiych dat
* @return Zdali se povedla operace*/
bool WhisperArgs(std::map<std::string,std::vector<std::string>>& whispering,const std::vector<std::string>& split, std::string pattern, int dolni)
{
	size_t j = 0;
	auto end = false;

	for (size_t i = dolni; i < split.size(); ++i)
	{
		if (j > 0)
		{
			std::stringstream ss;
			ss << pattern << " arg" << j;
			pattern = ss.str();
		}
		if(split[i]=="?")
		{
			Whisper(whispering, pattern);
			end = true;
		}
		++j;
	}
	return end;
}

/**Nacteni akce pod jmenem Name a predani akce do action
* @param action Ulozene nactene akce s klicem jmeno akce
* @param actionsIO Ulozene soubory pripravene k zaznamenavani akci
* @param name jmeno akce*/
void Load(std::map<std::string, std::unique_ptr<Building>>& action, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO,  std::string name)
{
	auto tempStream = std::fstream{ "workingDIR/" + name + "_seats.dmp" ,std::ios_base::in };
	if (!tempStream.is_open())
	{
		std::cout << "Neni co nacist" << std::endl;
		tempStream.close();
	}
	else
	{
		tempStream.close();
		auto tmp = actionsIO.find(name);
		Building build{ 0, 0, name };

		if (tmp == actionsIO.end())
			tmp = actionsIO.emplace(std::make_pair(name, std::make_unique<IO_System>(name))).first;

		std::vector<SeatSync> seat;
		std::vector<ReservationSync> reservation;

		//provede inisializaci, aby bylo mozno provest cteni
		tmp->second->SerializeIO(seat, reservation);

		if (!tmp->second->Read(build, name))
			std::cout << "Soubor byl modifikovan, nelze precist data" << std::endl;
		else
			action.emplace(std::make_pair(name,std::make_unique<Building>(std::move(build))));
	}
}

/**Provede se kontrola argumentu a zavola se nacitani akce
* @param whispering Databaze napovedy
* @param split Nakouskovana command line
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce*/
void Loading(std::map<std::string, std::vector<std::string>>&whispering, std::vector<std::string>& split, std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO)
{
	if (WhisperArgs(whispering, split, "load", 1))
		return;
	if (split.size() < 2)
		std::cout << "Chybi argumenty" << std::endl;
	else
		Load(actions, actionsIO, split[1]);
}

/**Ulozeni akce pod jmenem Name
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce pro kterou se provede operace*/
void Save(const std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, const std::string name)
{
	auto tmp = actionsIO.find(name);

	if (tmp == actionsIO.end())
		std::cout << "Snazite se ulozit nevytvorenou akci" << std::endl;
	else
		tmp->second->Write(true);
}

/**Provede se kontrola argumentu a zavola se ulozeni akce
* @param whispering Databaze napovedy
* @param split Nakouskovana command line
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti*/
void Saving(std::map<std::string, std::vector<std::string>>&whispering, const std::vector<std::string>& split, const std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO)
{
	if (WhisperArgs(whispering, split, "save", 1))
		return;
	if (split.size() < 2)
		std::cout << "Chybi argumenty" << std::endl;
	else
		Save(actions, actionsIO, split[1]);
}

/**Vytvoreni udalosti v pameti
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param arg Argument velikosti akce
* @param argCost Argument ceny akce
* @throws bad_path Spatna cesta
* @throws not_dir Chybejici slozka workingDIR
 */
void Create(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, std::string arg, std::string argCost)
{	
	std::fstream test("workingDIR/" + name + ".data", std::ios_base::in);
	if (test.is_open())
		std::cout << "Akce jiz byla nekdy vytvorena, zkuste ji nacist"<<std::endl;
	else if (IsNumber(arg))
	{
		size_t size = std::stoull(arg);
		if (IsNumber(argCost))
		{
			auto cena = std::stoi(argCost);
			auto temp = actions.find(name);
			if (temp == actions.end())
			{
				actions.emplace(std::make_pair(name, std::make_unique<Building>(size, cena, name)));
				auto tmp = actionsIO.emplace(std::make_pair(name, std::make_unique<IO_System>(name)));
				SeatSync seat{ "CR",0,size,0,cena};
				tmp.first->second->GetNonSyncSeats().push_back(seat);
			}
			else
				std::cout << "Akce jiz existuje v pameti" << std::endl;
		}
		else
			std::cout << "Treti argument neni cislo" << std::endl;
	}
	else
		std::cout << "Druhy argument neni cislo" << std::endl;

}
/**Provede se kontrola argumentu a vytvorime udalost
* @param whispering Databaze napovedy
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param split Nakouskovana command line
* @throws bad_path Spatna cesta 
* @throws not_dir Chybejici slozka workingDIR*/
void Creating(std::map<std::string, std::vector<std::string>>&whispering, std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, const std::vector<std::string>& split)
{
	if (WhisperArgs(whispering, split, "create", 1))
		return;
	if (split.size() < 4)
		std::cout << "Chybi argumenty" << std::endl;
	else
		Create(actions, actionsIO, split[1], split[2],split[3]);
}
/**Vlozi radek do akce
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param arg1 Pozice, za kterou budeme vkladat
* @param arg2 Pocet vkladanych pozic
* @param arg3 Cena vkladanych pozic*/
void InsertRow(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, std::string arg1, std::string arg2, std::string arg3)
{
	//Kontrola argumentu
	auto digit1 = IsNumber(arg1);
	auto digit2 = IsNumber(arg2);
	auto digit3 = IsNumber(arg3);

	if (digit1)
	{
		size_t position = std::stoull(arg1);
		if(digit2)
		{
			size_t size = std::stoull(arg2);
			if(digit3)
			{
				auto cost = std::stoi(arg3);
				auto temp = actions.find(name);
				if(temp!=actions.end())
				{
					SeatSync seat{ "IR",position,size,cost };
					if(!temp->second->Insert_Row(position, size, cost))
						std::cout << "Operace se nezdarila kvuli neexistujici pozici nebo prilis velkemu poctu sedadel. Prekontrolujte prosim svuj prikaz" << std::endl;
					else
					{
						auto tmp = actionsIO.find(name);
						tmp->second->GetNonSyncSeats().push_back(seat);
						tmp->second->Write(false);
					}
				}
				else
					std::cout << "Paty argument neni nazev akce, ktera je v pameti" << std::endl;
			}
			else
				std::cout << "Osmy argument neni cislo" << std::endl;
		}
		else
			std::cout << "Sedmy argument neni cislo" << std::endl;
	}
	else
		std::cout << "sesty argument neni cislo" << std::endl;
}

/**Vlozi sloupec do akce
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param arg1 Pozice, za kterou budeme vkladat
* @param arg2 Pocet vkladanych pozic
* @param arg3 Cena vkladanych pozic*/
void InsertColumn(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, std::string arg1, std::string arg2, std::string arg3)
{
	//Kontrola argumentu
	auto digit1 = IsNumber(arg1);
	auto digit2 = IsNumber(arg2);
	auto digit3 = IsNumber(arg3);

	if (digit1)
	{
		size_t position = std::stoull(arg1);
		if (digit2)
		{
			size_t size = std::stoull(arg2);
			if (digit3)
			{
				auto cost = std::stoi(arg3);
				auto temp = actions.find(name);
				if (temp != actions.end())
				{
					SeatSync seat{ "IC",position, size, cost };
					if (!temp->second->Insert_Column(position, size, cost))
						std::cout << "Operace se nezdarila bud kvuli neexistujici pozici nebo prilis velkemu poctu sedadel. Prekontrolujte prosim svuj prikaz" << std::endl;
					else
					{
						auto tmp = actionsIO.find(name);
						tmp->second->GetNonSyncSeats().push_back(seat);
						tmp->second->Write(false);
					}
				}
				else
					std::cout << "Paty argument neni nazev akce, ktera je v pameti" << std::endl;
			}
			else
				std::cout << "Osmy argument neni cislo" << std::endl;
		}
		else
			std::cout << "Sedmy argument neni cislo" << std::endl;
	}
	else
		std::cout << "sesty argument neni cislo" << std::endl;
}

/**Smaze radek v akci
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param arg1 Pozice, na ktere mazeme
* @param arg2 Pocet smazanych sedadel*/
void RemoveRow(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, std::string arg1, std::string arg2)
{
	//Kontrola argumentu
	auto digit1 = IsNumber(arg1);
	auto digit2 = IsNumber(arg2);

	if (digit1)
	{
		size_t position = std::stoull(arg1);
		if (digit2)
		{
			size_t size = std::stoull(arg2);
			auto temp = actions.find(name);
			if (temp != actions.end())
			{
				SeatSync seat{ "RR",position, size };
				if (!temp->second->Remove_Row(position, size))
					std::cout << "Operace se nezdarila bud kvuli velikosti mazani rovno 0, nebo kvuli prekroceni velikosti mazani, nebo kvuli neexistujici pozici. Prekontrolujte prosim svuj prikaz" << std::endl;
				else
				{
					auto tmp = actionsIO.find(name);
					tmp->second->GetNonSyncSeats().push_back(seat);
					tmp->second->Write(false);
				}
			}
			else
				std::cout << "Paty argument neni nazev akce, ktera je v pameti" << std::endl;
		}
		else
			std::cout << "Sedmy argument neni cislo" << std::endl;
	}
	else
		std::cout << "sesty argument neni cislo" << std::endl;
}

/**Smaze sloupec v akci
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param arg1 Pozice, na ktere mazeme
* @param arg2 Pocet smazanych sedadel*/
void RemoveColumn(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, std::string arg1, std::string arg2)
{
	auto digit1 = IsNumber(arg1);
	auto digit2 = IsNumber(arg2);

	if (digit1)
	{
		size_t position = std::stoull(arg1);
		if (digit2)
		{
			size_t size = std::stoull(arg2);
			auto temp = actions.find(name);
			if (temp != actions.end())
			{
				SeatSync seat{ "RC",position, size };
				if (!temp->second->Remove_Column(position, size))
					std::cout << "Operace se nezdarila bud kvuli velikosti mazani rovno 0, nebo kvuli prekroceni velikosti mazani, nebo kvuli neexistujici pozici. Prekontrolujte prosim svuj prikaz" << std::endl;
				else
				{
					auto tmp = actionsIO.find(name);
					tmp->second->GetNonSyncSeats().push_back(seat);
					tmp->second->Write(false);
				}
			}
			else
				std::cout << "Paty argument neni nazev akce, ktera je v pameti" << std::endl;
		}
		else
			std::cout << "Sedmy argument neni cislo" << std::endl;
	}
	else
		std::cout << "sesty argument neni cislo" << std::endl;
}

/**Zmeni velikost maximalniho poctu sedadel na akci
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param size Maximalni pocet sedadel na akci
* @param argCost Cena za akci*/
void Resize(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, std::string size,std::string argCost)
{
	auto temp = actions.find(name);
	if (temp != actions.end())
	{
		if(!IsNumber(size))
			std::cout << "Ctvrty argument neni cislo" << std::endl;
		else
		{
			size_t sizeN = std::stoull(size);
			if(IsNumber(argCost))
			{
				size_t cena = std::stoull(argCost);
				if (!temp->second->Resize(sizeN,cena))
					std::cout << "Velikost je mensi, nez aktualni maximalni velikost mist na akci" << std::endl;
				else
				{
					SeatSync seat{ "RS",0, sizeN };
					auto tmp = actionsIO.find(name);
					tmp->second->GetNonSyncSeats().push_back(seat);
					tmp->second->Write(false);
				}
			}
			else
			{
				std::cout << "Paty argument neni cislo" << std::endl;
			}
		}	
	}
	else
		std::cout << "treti argument neni nazev akce, ktera je v pameti" << std::endl;

}
/**Provede vypis informaci o akci
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param Name Jmeno akce*/
void Info(std::map<std::string, std::unique_ptr<Building>>& actions, std::string name)
{
	auto temp = actions.find(name);
	if (temp != actions.end())
	{
		std::cout<<temp->second->to_string()<<std::endl;
	}
	else
		std::cout << "ctvrty argument neni nazev akce, ktera je v pameti" << std::endl;

}
/**Provede operaci po vybrani building
* @param whispering Databaze napovedy
* @param split Nakouskovana command line
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti*/
void ChooseBuilding(std::map<std::string,std::vector<std::string>>&whispering, const std::vector<std::string>& split, std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO)
{
	if (split.size() < 3)
		std::cout << "Chybi argumenty"<<std::endl;
	else if (split[2] == "insert")
	{
		if (split.size() < 4)
			std::cout << "Chybi argumenty" << std::endl;
		else if (split[3] == "row")
		{
			if (WhisperArgs(whispering, split, "action building insert row", 4))
				return;
			if (split.size() < 8)
				std::cout << "Chybi argumenty" << std::endl;
			else
				InsertRow(actions, actionsIO, split[4], split[5], split[6], split[7]);
		}
		else if (split[3] == "column")
		{
			if (WhisperArgs(whispering, split, "action building insert column", 4))
				return;
			if (split.size() < 8)
				std::cout << "Chybi argumenty" << std::endl;
			else
				InsertColumn(actions, actionsIO, split[4], split[5], split[6], split[7]);
		}
		else if (split[3] == "?") {
			Whisper(whispering, "action building insert");
		}
		else
			std::cout << "Uvedeny retezec neodpovida zadnemu prikazu, pro vice informaci napiste: ?" << std::endl;
	}
	else if (split[2] == "remove")
	{
		if (split.size() < 4)
			std::cout << "Chybi argumenty" << std::endl;
		else if (split[3] == "row")
		{
			if (WhisperArgs(whispering, split, "action building remove row", 4))
				return;
			if (split.size() < 7)
				std::cout << "Chybi argumenty" << std::endl;
			else
				RemoveRow(actions, actionsIO, split[4], split[5], split[6]);
		}
		else if (split[3] == "column")
		{
			if (WhisperArgs(whispering, split, "action building remove column", 4))
				return;
			if (split.size() < 7)
				std::cout << "Chybi argumenty" << std::endl;
			else
				RemoveColumn(actions, actionsIO, split[4], split[5], split[6]);
		}
		else if(split[3]=="?"){
			Whisper(whispering, "action building remove");
		}
		else
		    std::cout<< "Uvedeny retezec neodpovida zadnemu prikazu, pro vice informaci napiste: ?" << std::endl;
	}
	else if (split[2] == "resize")
	{
		if (WhisperArgs(whispering, split, "action building resize", 3))
			return;
		if (split.size() < 6)
			std::cout << "Chybi argumenty" << std::endl;
		else
			Resize(actions, actionsIO, split[3], split[4], split[5]);
	}
	else if (split[2] == "info")
	{
		if (WhisperArgs(whispering, split, "action building info", 3))
			return;
		if (split.size() < 4)
			std::cout << "Chybi argumenty" << std::endl;
		else
			Info(actions, split[3]);
	}
	else if(split[2]=="?"){
			Whisper(whispering, "action building");
		}
		else
		    std::cout<< "Uvedeny retezec neodpovida zadnemu prikazu, pro vice informaci napiste: ?" << std::endl;
}

/**Provede nalezeni rezervace
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param name Jmeno akce
* @param person Osoba, jejiz rezervace hledame*/
void FindReservation(std::map<std::string, std::unique_ptr<Building>>& actions, std::string name, ::name person)
{
	auto tmp = actions.find(name);
	if (tmp != actions.end())
	{
		auto temp = tmp->second->Find_Reservation(person);
		if (temp == nullptr)
			std::cout << "Rezervace na dane akci neexistuje" << std::endl;
		else
			std::cout << temp->to_string();
	}
	else
		std::cout << "Dana akce neni nahrana v pameti" << std::endl;
}
/**Provede smazani rezervace - pokud tato osoba ma rezervaci
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param name Jmeno akce
* @param person Osoba, jejiz rezervace hledame
* @param password Heslo pro upravu/smazani rezervace*/
void RemoveReservation(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::string name, ::name person, std::string password)
{
	auto tmp = actions.find(name);
	if (tmp != actions.end())
	{
		auto temp = tmp->second->Find_Reservation(person);
		if (temp == nullptr)
			std::cout << "Rezervace na dane akci neexistuje" << std::endl;
		else
		{
			auto reservation = ReservationSync("-", person, password);
			reservation.AddSeats(temp->Get_Seats(), temp->Get_Cost());
			if (tmp->second->Remove_Reservation(person, password))
			{
				actionsIO.find(name)->second->GetNonSyncReservations().push_back(reservation);
				actionsIO.find(name)->second->Write(false);
			}

			else
				std::cout << "spatne heslo, smazani rezervace nebylo uskutecneno" << std::endl;
		}
	}
	else
		std::cout << "Dana akce neni nahrana v pameti" << std::endl;
}

/**Provede editaci rezervace
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param seats Sedadla pro upravu rezervace
* @param cost Zaplaceno za sedadla
* @param name Jmeno akce
* @param person Osoba, jejiz rezervace hledame
* @param password Heslo pro upravu/smazani rezervace*/
void AlterReservation(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::vector<size_t>& seats, std::vector<int>& cost, std::string name, ::name person, std::string password)
{
	auto tmp = actions.find(name);
	if (tmp != actions.end())
	{
		auto temp = tmp->second->Find_Reservation(person);
		if (temp == nullptr)
			std::cout << "Rezervace na dane akci neexistuje" << std::endl;
		if (tmp->second->Alter_Reservation(seats, cost, person, password))
		{
			auto reservation = ReservationSync("A", person, password);
			reservation.AddSeats(seats, cost);
			actionsIO.find(name)->second->GetNonSyncReservations().push_back(reservation);
			actionsIO.find(name)->second->Write(false);
		}
		else
			std::cout << "Hesla se neshoduji" << std::endl;
	}
	else
		std::cout << "Dana akce neni nahrana v pameti" << std::endl;
}

/**Provede pridani rezervace
* @param actions Ulozene akce v pameti s klicem jmeno akce
* @param actionsIO Ulozene soubory pro jednotlive akce v pameti
* @param seats Sedadla pro upravu rezervace
* @param cost Zaplaceno za sedadla
* @param name Jmeno akce
* @param person Osoba, jejiz rezervace hledame
* @param password Heslo pro upravu/smazani rezervace*/
void AddReservation(std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO, std::vector<size_t>& seats,std::vector<int>& cost, std::string name, ::name person, std::string password)
{
	auto tmp = actions.find(name);
	if (tmp != actions.end())
	{
		auto temp = tmp->second->Find_Reservation(person);
		if (temp != nullptr)
			std::cout << "Rezervace na dane akci existuje" << std::endl;
		if(tmp->second->Add_Reservation(seats, cost, person, password))
		{
			auto reservation = ReservationSync("+", person, password);
			reservation.AddSeats(seats, cost);
			actionsIO.find(name)->second->GetNonSyncReservations().push_back(reservation);
			actionsIO.find(name)->second->Write(false);
		}
		else
			std::cout << "Mista uz jsou obsazeny" << std::endl;
	}
	else
		std::cout << "Dana akce neni nahrana v pameti" << std::endl;
}

/**Vrati nam nactena sedadla
* @param action Akce na ktere cteme
* @param input Vstupni data k parsovani
* @param seats LinkedList, kam davame parsovana sedadla
* @param cost LinkedList kam dáváme parsovanou cenu
* @return Zda se operace povedla*/
bool GetSeats(Building* action, std::string& input, std::vector<size_t>& seats, std::vector<int>& cost) {
	//([rada;sloupec;cena],...)
	//jsme ve cteni argumentu sedadla
	auto in=false;
	//pocet uzavorkovani
	auto bracket =0;
	//cteni prvniho argumentu
	auto first=false;
	//cteni druheho argumentu
	auto second=false;
	std::string tmp = "";
	size_t x = 0;
	size_t y = 0;
	
	if (input[0] != '(' || input[input.length() - 1] != ')')
	{
		std::cout << "spatny tvar vstupu pro nacteni sedadel." << std::endl;
		return false;
	}
	for(size_t i=1; i<input.size()-1;++i){
		if (input[i] == '[' && !in) {
			++bracket;
			in = true;
		}
		//zpracovani prvniho ciselneho a druheho ciselneho argumentu (zpracovani souradnic)
		else if (input[i] == ';'&&in && (first&&!second || !first && !second)) {
			if (!first && !second)
				x = std::stoull(tmp);
			else
				y= std::stoull(tmp);
			auto temp = first;
			first = !first && !second ? true : false;
			second = temp && !second ? true : false;
			tmp = "";
		}
		//zpracovani ceny
		else if(input[i]==']'&&in&&!first&&second){
			coordinates a;
			a.rows = x;
			a.columns = y;
			if (x == 0 || y == 0)
			{
				std::cout << "spatny tvar vstupu pro nacteni sedadel." << std::endl;
				seats.clear();
				cost.clear();
				return false;
			}
			seats.push_back(action->Get_Array_Position(a));
			cost.push_back(std::stoi(tmp));
			--bracket;
			in=false;
			second=false;
			tmp="";
		}
		else if(in&&isdigit(input[i])){
			tmp+=input[i];
		}
		else{
			std::cout << "spatny tvar vstupu pro nacteni sedadel." << std::endl;
			seats.clear();
			cost.clear();
			return false;
		}
	}
	if(bracket!=0)
	{
		std::cout << "spatny tvar vstupu pro nacteni sedadel." << std::endl;
		seats.clear();
		cost.clear();
		return false;
	}
	return true;
}

/**Vraci osobu, pokud je udana ve spravnem tvaru, jinak vraci osobu s chybnou informaci
* @param input Zpracovavany kus textu
* @param output Naprasovana osoba
* @return Zda se operace povedla*/
bool GetPerson(std::string& input,name& output)
{
	if (input[0] != '(' || input[input.length() - 1] != ')')
	{
		std::cout << "spatny tvar vstupu pro nacteni osoby." << std::endl;
		return false;
	}
	auto tmp=Split(input.substr(0, input.length() - 1).substr(1), ';');

	if(tmp.size()!=3)
	{
		std::cout << "spatny tvar vstupu pro nacteni osoby." << std::endl;
		return false;
	}
	output.firstName = tmp[0];
	output.surName = tmp[1];
	output.email = tmp[2];

	return true;
}

/**Provede vyber po moznosti reservations
* @param whispering Databaze napovedy
* @param split Nakouskovana command line
* @param actions Akce na ktere cteme
* @param actionsIO Soubory akci nactenych v pameti pripravene pro zapis*/
void ChooseReservations(std::map<std::string, std::vector<std::string>>&whispering, std::vector<std::string>& split, std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO)
{
	if (split.size() < 2)
		std::cout << "Chybi argumenty" << std::endl;
	else if (split[2] == "alter")
	{
		if (WhisperArgs(whispering, split, "action reservations alter", 3))
			return;
		if (split.size() < 7)
			std::cout << "Chybi argumenty" << std::endl;
		else
		{
			std::vector<size_t> seats;
			std::vector<int> cost;
			name output;
			auto building = actions.find(split[3]);
			if (building != actions.end() && GetPerson(split[5], output) && GetSeats(building->second.get(), split[4], seats, cost))
				AlterReservation(actions, actionsIO, seats, cost, split[3], output, split[6]);
		}
	}
	else if (split[2] == "add")
	{
		if (WhisperArgs(whispering, split, "action reservations add", 3))
			return;
		if (split.size() < 7)
			std::cout << "Chybi argumenty" << std::endl;
		else
		{
			std::vector<size_t> seats;
			std::vector<int> cost;
			name output;
			auto building = actions.find(split[3]);
			if(building != actions.end()&&GetPerson(split[5],output)&& GetSeats(building->second.get(),split[4], seats, cost))
				AddReservation(actions, actionsIO, seats, cost, split[3], output, split[6]);
		}
	}
	else if (split[2] == "remove")
	{
		if (WhisperArgs(whispering, split, "action reservations remove", 3))
			return;
		if (split.size() < 6)
			std::cout << "Chybi argumenty" << std::endl;
		else
		{
			name output;
			if(GetPerson(split[4], output))
				RemoveReservation(actions, actionsIO, split[3], output, split[5]);
		}

	}
	else if (split[2] == "find")
	{
		if (WhisperArgs(whispering, split, "action reservations find", 3))
			return;
		if (split.size() < 5)
			std::cout << "Chybi argumenty" << std::endl;
		else
		{
			name output;
			if(GetPerson(split[4], output))
				FindReservation(actions, split[3], output);
		}
	}
	else if(split[2]=="?"){
			Whisper(whispering, "action reservations");
		}
		else
		    std::cout<< "Uvedeny retezec neodpovida zadnemu prikazu, pro vice informaci napiste: ?" << std::endl;
}

/**Provede vyber po moznosti action
* @param whispering Databaze napovedy
* @param split Nakouskovana command line
* @param actions Akce na ktere cteme
* @param actionsIO Soubory akci nactenych v pameti pripravene pro zapis*/
void Action(std::map<std::string, std::vector<std::string>>&whispering, std::vector<std::string>& split, std::map<std::string, std::unique_ptr<Building>>& actions, std::map<std::string, std::unique_ptr<IO_System>>& actionsIO)
{
	if (split.size() < 2)
		std::cout << "Chybi argumenty" << std::endl;
	else if (split[1] == "building")
	{
		ChooseBuilding(whispering,split, actions, actionsIO);
	}
	else if (split[1] == "reservations")
	{
		ChooseReservations(whispering,split, actions, actionsIO);
	}
	else if(split[1]=="?"){
			Whisper(whispering, "action");
		}
		else
		    std::cout<< "Uvedeny retezec neodpovida zadnemu prikazu, pro vice informaci napiste: ?" << std::endl;
}

/**Nacteni konfigurace naseptavace
* @param whispering Databaze napovedy
* @param confMap Soubor pro docasne ukladani*/
void ReadMap(std::map<std::string, std::vector<std::string>>& whispering, std::ifstream& confMap)
{
	std::string line;
	auto val = false;
	std::string key;
	std::vector<std::string> values;

	while (confMap.good())
	{
		std::getline(confMap, line);
		if (line == "ENDVALUES")
		{
			val = false;
			whispering.emplace(std::make_pair(key, values));
			values.clear();
			key = "";
			continue;
		}
		if (!val)
		{
			key = line;
			val = true;
		}
		else
			values.push_back(line);
	}
	
}

///Hlavni program
int main() {

	std::map<std::string, std::unique_ptr<Building>> actions;
	std::map<std::string, std::unique_ptr<IO_System>> actionsIO;
	std::map<std::string,std::vector<std::string>> whispering;
	
	try
	{
		std::ifstream confMap("workingDIR/config.map", std::ios_base::in);
		if (!confMap.is_open())
			throw bad_path();
		ReadMap(whispering, confMap);
	}catch(bad_path ex)
	{
		std::string eline;
		std::cout << "Byl smazan konfiguracni soubor.";
		std::cin >> eline;
		return 1;
	}

	std::cout << "Vitejte v programu na spravu rezervaci. Pro vice informaci o prikazech napiste znak ?."<<std::endl;
	
	while(true)
	{
		std::string line;
		std::getline(std::cin, line);

		//Nahrazeni duplicit
		auto new_end = std::unique(line.begin(), line.end(), BothAreSpaces);
        line.erase(new_end, line.end()); 
		
		auto split=Split(line, ' ');
		try
		{
			if (split.size() < 1)
				std::cout << "Chybi argumenty" << std::endl;
			else if (split[0] == "load")
			{
				Loading(whispering, split, actions, actionsIO);
			}
			else if (split[0] == "save")
			{
				Saving(whispering, split, actions, actionsIO);
			}
			else if (split[0] == "create")
			{
				Creating(whispering, actions, actionsIO, split);
			}
			else if (split[0] == "action")
			{
				Action(whispering, split, actions, actionsIO);
			}
			else if (split[0] == "exit")
				break;
			else if (split[0] == "?") {
				Whisper(whispering, "Null");
			}
			else
				std::cout << "Uvedeny retezec neodpovida zadnemu prikazu, pro vice informaci napiste: ?" << std::endl;
		}
		catch (bad_path ex)
		{
			std::string eline;
			std::cout<<ex.what();
			std::cin >> eline;
			return 1;
		}
		catch (bad_coordinate ex)
		{

			std::string eline;
			std::cout << ex.what();
			std::cin >> eline;
			return 1;
		}
		catch (not_dir ex)
		{
			std::string eline;
			std::cout << ex.what();
			std::cin >> eline;
			return 1;
		}
		catch(std::exception ex)
		{
			std::string eline;
			std::cout << ex.what();
			std::cin >> eline;
			return 1;
		}
	}
}
