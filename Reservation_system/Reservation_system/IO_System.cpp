#include "IO_System.h"
#include <sstream>
#include "functions.h"

/**Provede update z fileName.data do prislusnych souboru
* @param seats Soubor pro zaznamenani operaci s budovou
* @param reservations Soubor pro zaznamenani operaci s rezervacemi
* @param reservation Synchronizace rezervaci, ktere jeste nejsou zapsany*/
void Update(std::fstream& seats, std::fstream& reservations, std::vector<SeatSync>& seat, std::vector<ReservationSync>& reservation)
{
	for (auto i = 0; i < seat.size(); ++i)
	{
		if (seat[i].GetOp() == "CR")
			seats << "CR" << std::endl << seat[i].GetSize() << ";"<<seat[i].GetCena()<< std::endl;
		else if (seat[i].GetOp() == "RS")
			seats << "RS" << std::endl << seat[i].GetSize() << ";" << seat[i].GetCena() << std::endl;
		else if (seat[i].GetOp() == "IR")
			seats << "IR" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << ";" << seat[i].GetCost() << std::endl;
		else if (seat[i].GetOp() == "IC")
			seats << "IC" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << ";" << seat[i].GetCost() << std::endl;
		else if (seat[i].GetOp() == "RR")
			seats << "RR" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << std::endl;
		else if (seat[i].GetOp() == "RC")
			seats << "RC" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << std::endl;
	}

	for (size_t i = 0; i < reservation.size(); ++i)
	{
		if (reservation[i].GetOp() == "A")
		{
			reservations << "A" << std::endl;
			reservations << reservation[i].GetPerson().firstName << ";" << reservation[i].GetPerson().surName << ";" << reservation[i].GetPerson().email << ";" << reservation[i].GetPassword() << std::endl;
			for (auto j = 0; j < reservation[i].GetSeats().size(); ++j)
			{
				reservations << reservation[i].GetSeats()[j] << ";" << reservation[i].GetCost()[j] << std::endl;
			}
		}
		else if (reservation[i].GetOp() == "+")
		{
			reservations << "+" << std::endl;
			reservations << reservation[i].GetPerson().firstName << ";" << reservation[i].GetPerson().surName << ";" << reservation[i].GetPerson().email << ";" << reservation[i].GetPassword() << std::endl;
			for (auto j = 0; j < reservation[i].GetSeats().size(); ++j)
			{
				reservations << reservation[i].GetSeats()[j] << ";" << reservation[i].GetCost()[j] << std::endl;
			}
		}
		else if (reservation[i].GetOp() == "-")
		{
			reservations << "-" << std::endl;
			reservations << reservation[i].GetPerson().firstName << ";" << reservation[i].GetPerson().surName << ";" << reservation[i].GetPerson().email << ";" << reservation[i].GetPassword() << std::endl;
		}
	}
	seat.clear();
	reservation.clear();
}

/**Provede ulozeni minimalne n rezervaci + rezervaci, coz bude ulozeni jedne atomicke jednotky, aby nedoslo ke ztrate cele prace
* @param seats Soubor pro zaznamenani operaci s budovou
* @param reservations Soubor pro zaznamenani operaci s rezervacemi
* @param tmp Soubor pro docasne zaznamenani oepraci
* @param filename Nazev akce
* @param seat Synchronizace budovy
* @param reservation Synchronizace rezervaci, ktere jeste nejsou zapsany*/

void Serialize_Records(std::fstream& seats, std::fstream& reservations, std::fstream& tmp, std::string filename, std::vector<SeatSync>& seat, std::vector<ReservationSync>& reservation)
{
	reservations = std::fstream("workingDIR/" + filename + "_reservations.dmp", std::ios_base::out | std::ios_base::app | std::ios_base::ate);
	tmp = std::fstream("workingDIR/" + filename + ".data", std::ios_base::out | std::ios_base::app);
	seats = std::fstream("workingDIR/" + filename + "_seats.dmp", std::ios_base::out | std::ios_base::app | std::ios_base::ate);

	for (auto i = 0; i < seat.size(); ++i)
	{
		if (seat[i].GetOp() == "CR")
			tmp << "CR" << std::endl << seat[i].GetSize() << ";" << std::to_string(seat[i].GetCena()) << std::endl;
		else if (seat[i].GetOp() == "RS")
			tmp << "RS" << std::endl << seat[i].GetSize() << ";" << std::to_string(seat[i].GetCena()) << std::endl;
		else if (seat[i].GetOp() == "IR")
			tmp << "IR" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << ";"<<seat[i].GetCost() << std::endl;
		else if(seat[i].GetOp() == "IC")
			tmp << "IC" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << ";" << seat[i].GetCost() << std::endl;
		else if(seat[i].GetOp() == "RR")
			tmp << "RR" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << std::endl;
		else if(seat[i].GetOp() == "RC")
			tmp << "RC" << std::endl << seat[i].GetPosition() << ";" << seat[i].GetSize() << std::endl;
	}
	for (size_t i = 0; i < reservation.size(); ++i)
	{
		if (reservation[i].GetOp() == "A")
		{
			tmp << "A" << std::endl;
			tmp << reservation[i].GetPerson().firstName<< ";" << reservation[i].GetPerson().surName << ";" << reservation[i].GetPerson().email << ";" << reservation[i].GetPassword() << std::endl;
			for (auto j = 0; j < reservation[i].GetSeats().size(); ++j)
			{
				tmp << reservation[i].GetSeats()[j] << ";" << reservation[i].GetCost()[j]<<std::endl;
			}
		}
		else if(reservation[i].GetOp() == "+")
		{
			tmp << "+" << std::endl;
			tmp << reservation[i].GetPerson().firstName << ";" << reservation[i].GetPerson().surName << ";" << reservation[i].GetPerson().email << ";" << reservation[i].GetPassword() << std::endl;
			for (auto j = 0; j < reservation[i].GetSeats().size(); ++j)
			{
				tmp << reservation[i].GetSeats()[j] << ";" << reservation[i].GetCost()[j] << std::endl;
			}
		}
		else if(reservation[i].GetOp() == "-")
		{
			tmp << "-" << std::endl;
			tmp << reservation[i].GetPerson().firstName << ";" << reservation[i].GetPerson().surName << ";" << reservation[i].GetPerson().email << ";" << reservation[i].GetPassword() << std::endl;
		}
	}
	tmp << seats.tellp() << std::endl;
	tmp << reservations.tellp() << std::endl;
	tmp << "#END OF DATA#";
	tmp.close();

	Update(seats, reservations, seat, reservation);

	reservations.close();
	seats.close();
	tmp = std::fstream("workingDIR/" + filename + ".data", std::ios_base::out|std::ios_base::trunc);
	tmp.close();
}

/**Provede zapsani, pokud je command true, jedna se o ulozeni uzivatelem, jinak se jedna o preventivni volani po kazde akci
* @param command Priznak pro zapis
* @return Povedl se zapis?*/
bool IO_System::Write(bool command)
{
	//Pokud neni dosazen pocet zaznamu k serializaci a nejedna se o prikaz, nic neprovadej
	if (seat.size()+reservation.size() < serialize && !command)
		return true;

	if (seat.size() == 0 && reservation.size() == 0)
		return false;
	
	Serialize_Records(seats,reservations, tmp, filename, seat, reservation);

	return true;
}
/**Precte operace se sedadly a provede je
* @param filename Nazev akce
* @param seats Soubor pro synchronizaci budovy
* @param action Budova*/
bool ReadSeats(std::string& filename, std::fstream& seats, Building& action)
{
	auto op = false;
	auto RS = false;
	auto CR = false;
	auto insertOP = false;
	auto removeOP = false;

	std::string operation;
	std::string line;

	if (seats.peek() == std::fstream::traits_type::eof())
		return true;

	while (seats.good())
	{
		std::getline(seats, line);
		auto split = Split(line, ';');

		//pokud jsme jiz precetli zalozeni akce a nasli jsme prazdny radek, vracime true
		if (split.size() == 0&&CR)
			return true;
		//Cteni operace a nastaveni priznaku pro nasledujici data
		if (split.size() == 1 && !op)
		{
			operation = line == "IC" || line == "IR" || line =="RR" || line == "RC" || line == "CR" || line == "RS" ? line : "ERROR";
			if (operation == "RS")
				RS = true;
			else if (operation == "CR")
				CR = true;
			else if (operation=="IC"|| operation == "IR")
				insertOP = true;
			
			else if (operation == "RC" || operation == "RR")
				removeOP = true;

			else if (operation == "ERROR")
			{
				action = Building(0, 0, filename);
				return false;
			}
			op = true;
		}
		else if (split.size() == 3 && insertOP && CR)
		{
			//Nastaven priznak vkladani a je splnen pocet argumentu a akce je vytvorena
			insertOP = false;
			op = false;

			if (!IsNumber(split[0]))
			{
				action = Building(0, 0, filename);
				return false;
			}

			if (!IsNumber(split[1]))
			{
				action = Building(0, 0, filename);
				return false;
			}

			if (!IsNumber(split[2]))
			{
				action = Building(0, 0, filename);
				return false;
			}

			if(operation=="IR")
			{
				action.Insert_Row(std::stoull(split[0]), std::stoull(split[1]), std::stoi(split[2]));
			}
			else
			{
				action.Insert_Column(std::stoull(split[0]), std::stoull(split[1]), std::stoi(split[2]));
			}
		}
		else if (split.size()==2 && op && removeOP && CR)
		{
			//Nastaven priznak mazani a je splnen pocet argumentu a akce je vytvorena
			op = false;
			removeOP = false;

			if (!IsNumber(split[0]))
			{
				action = Building(0, 0, filename);
				return false;
			}

			if (!IsNumber(split[1]))
			{
				action = Building(0, 0, filename);
				return false;
			}

			if (operation == "RR")
			{
				action.Remove_Row(std::stoull(split[0]), std::stoull(split[1]));
			}
			else
			{
				action.Remove_Column(std::stoull(split[0]), std::stoull(split[1]));
			}
		}
		else if (split.size() == 2 && op && RS && CR)
		{
			//nastaven priznak na zmenu maximalniho poctu sedadel
			RS = false;
			op = false;

			if (!IsNumber(split[0]))
				return false;

			action.Resize(std::stoull(split[0]),std::stoi(split[1]));
		}
		else if (split.size() == 2 && op && CR)
		{
			//nastaven priznak na vytvoreni akce
			op = false;
			if (!IsNumber(split[0]))
				return false;

			action.Resize(std::stoull(split[0]),std::stoi(split[1]));
		}
		else
		{
			//pokud nesplnime podminky, soubor je modifikovany
			action = Building(0, 0, filename);
			return false;
		}
	}
	return true;
}
/**Cteni rezervaci akce
* @param filename Nazev akce
* @param reservations Soubor pro sychronizaci rezervaci
* @param action Budova
* @return Zda se povedlo cteni*/
bool ReadReservations(std::string& filename, std::fstream& reservations, Building& action)
{

	auto op = false;
	auto data = false;
	auto key = false;
	std::string line;

	auto alter = false;
	auto plus = false;
	auto minus = false;


	std::vector<size_t> sedadla;
	std::vector<int> ceny;

	std::string passwd;
	name person;

	if (reservations.peek() == std::fstream::traits_type::eof())
		return true;

	while (reservations.good())
	{
		std::getline(reservations, line);
		auto split = Split(line, ';');
		if (split.size() == 0 && data && !key&&!op)
			continue;
		if (split.size() == 1 && !op)
		{
			if (plus)
			{
				//provede operaci pridani po nacteni vsech rezervaci na osobu
				action.Add_Reservation(sedadla, ceny, person, passwd);
				sedadla.clear();
				ceny.clear();
			}
			else if (minus)
			{
				//provede operaci odebrani rezervace na osobu
				action.Remove_Reservation(person, passwd);
			}
			else if (alter)
			{
				//provede editaci rezervace (nahrazeni rezervace novou) po nacteni vsech rezervaci na osobu
				action.Alter_Reservation(sedadla, ceny, person, passwd);
			}

			//zaznamenani operace
			auto operation = line == "A" || line == "+" || line == "-" ? line : "ERROR";
			if (operation == "ERROR")
			{
				action = Building(0, 0, filename);
				return false;
			}

			if (line == "+")
			{
				//nastavi priznak pro pridani rezervace
				plus = true;
				minus = false;
				alter = false;
			}
			else if (line == "-")
			{
				//nastavi priznak pro smazani rezervace
				plus = false;
				minus = true;
				alter = false;
			}
			else if (line == "A")
			{
				//nastavi priznak pro editaci rezervace
				plus = false;
				minus = false;
				alter = true;
			}

			op = true;
			data = false;
			key = false;

		}
		else if ((split.size() == 4 && !key && !data || split.size() == 4 && !key && data) && op)
		{
			//precteni klice (jmeno,prijmeni,email) a hesla v pripade, ze se precetla pouze operace, nebo jiz pred tim byly precteny nejake data (ci pouze operace)
			key = true;
			data = false;

			person.firstName = split[0];
			person.surName = split[1];
			person.email = split[2];

			passwd = split[3];

			if(minus)
			{
				//pro operaci odebirani neni vice potreba => nastavime, ze jsme jiz data cetli
				op = false;
				key = false;
				data = true;
			}
		}
		else if ((split.size() == 2 && key && !data || split.size() == 2 && !key && data) && !minus)
		{
			//precteme data po precteni klice, nebo pred tim dat a zaroven jsme cetli operaci a neni to minus
			data = true;
			key = false;
			op = false;
			sedadla.push_back(std::stoull(split[0]));
			ceny.push_back(std::stoi(split[1]));
		}
		else
		{
			//pokud nenastala zadna podminka, je soubor modifikovan
			action = Building(0, 0, filename);
			return false;
		}
	}
	//Provedeni posledni operace
	if (plus)
	{
		//provede operaci pridani po nacteni vsech rezervaci na osobu
		action.Add_Reservation(sedadla, ceny, person, passwd);
		sedadla.clear();
		ceny.clear();
	}
	else if (minus)
	{
		//provede operaci odebrani rezervace na osobu
		action.Remove_Reservation(person, passwd);
	}
	else if (alter)
	{
		//provede editaci rezervace (nahrazeni rezervace novou) po nacteni vsech rezervaci na osobu
		action.Alter_Reservation(sedadla, ceny, person, passwd);
	}
	return true;
}
/**Precte serializovanou budovu az po platne oznacena data
* @param action Akce
* @param filename Nazev akce
* @return Zda se povedlo cteni*/
bool IO_System::Read(Building& action, std::string filename)
{

	reservations = std::fstream("workingDIR/" + filename + "_reservations.dmp", std::ios_base::in);
	seats = std::fstream("workingDIR/" + filename + "_seats.dmp", std::ios_base::in);

	if (!ReadSeats(filename, seats, action))
		return false;

	if (!ReadReservations(filename, reservations, action))
		return false;

	return true;
}

/**Precteme fileName.data z tmp, ktery je otevreny a rezervace nastrkame do vektorů se synchronizacnimi tridami a tez dame do seat_f a reservation_f odkazy na platne data z fileName.data
* @param tmp Soubor pro docasne ukladani operaci
* @param seat Neserializovane operace akce
* @param reservation Neserializovane rezervace
* @param seat_f odkaz na velikost souboru s operacemi akce
* @param reservation_f odkaz na velikost souboru s rezervacemi
* @return Zda se cteni povedlo nebo ne*/
bool ReadTmp(std::fstream& tmp, std::vector<SeatSync>& seat, std::vector<ReservationSync>& reservation, fpos_t& seat_f, fpos_t& reservation_f)
{

	std::string line="";

	while (tmp.good())
	{
		if(line.empty())
			std::getline(tmp, line);
		auto split = Split(line, ';');

		if(split.size()==1)
		{
			auto operation = line;
			if(operation=="IC" || operation=="IR")
			{
				//Zpracovani operace
				if(tmp.good())
				{
					std::getline(tmp, line);
					split = Split(line, ';');
					if(split.size()==3)
					{
						if (IsNumber(split[0]) && IsNumber(split[1]) && IsNumber(split[2]))
							seat.push_back(SeatSync{ operation,std::stoull(split[0]),std::stoull(split[1]),std::stoi(split[2]) });
						line = "";
					}
					else
					{
						//Pokud spatny tvar, vse vycistime a vratime false
						seat.clear();
						reservation.clear();
						return false;
					}
				}
			}
			else if(operation == "RC" || operation == "RR")
			{
				//Zpracovani operace
				if (tmp.good())
				{
					std::getline(tmp, line);
					split = Split(line, ';');
					if (split.size() == 2)
					{
						if (IsNumber(split[0]) && IsNumber(split[1]))
							seat.push_back(SeatSync{ operation,std::stoull(split[0]),std::stoull(split[1])});
						line = "";
					}
					else
					{
						//Pokud spatny tvar, vse vycistime a vratime false
						seat.clear();
						reservation.clear();
						return false;
					}
				}
			}
			else if(operation == "RS" || operation == "CR")
			{
				//Zpracovani operace
				if (tmp.good())
				{
					std::getline(tmp, line);
					split = Split(line, ';');
					if (split.size() == 2)
					{
						if (IsNumber(split[0]))
							seat.push_back(SeatSync{ operation,0,std::stoull(split[0]),0,std::stoi(split[1]) });
						line = "";
					}
					else
					{
						//Pokud spatny tvar, vse vycistime a vratime false
						seat.clear();
						reservation.clear();
						return false;
					}
				}
			}
			else if(operation == "A" || operation == "+")
			{
				//Zpracovani operace
				if (tmp.good())
				{
					std::getline(tmp, line);
					split = Split(line, ';');
					if (split.size() == 4)
					{
						name person;
						person.firstName = split[0];
						person.surName = split[1];
						person.email = split[2];

						std::vector<size_t> tmpSeat;
						std::vector<int> tempCost;

						ReservationSync sync{ operation,person,split[3] };

						//cteme rezervovana sedadla
						while(tmp.good())
						{
							std::getline(tmp, line);
							split = Split(line, ';');
							if (split.size() != 2)
								break;
							if (IsNumber(split[0]) && IsNumber(split[1]))
							{
								tmpSeat.push_back(std::stoull(split[0]));
								tempCost.push_back(std::stoi(split[1]));
							}
							else
							{
								//Pokud spatny tvar, vse vycistime a vratime false
								seat.clear();
								reservation.clear();
								return false;
							}
						}
						sync.AddSeats(tmpSeat,tempCost);
						reservation.push_back(sync);
					}
					else
					{
						//Pokud spatny tvar, vse vycistime a vratime false
						seat.clear();
						reservation.clear();
						return false;
					}
				}
			}
			else if(operation == "-")
			{
				//Zpracovani operace
				if (tmp.good())
				{
					std::getline(tmp, line);
					split = Split(line, ';');
					if (split.size() == 4)
					{
						name person;
						person.firstName = split[0];
						person.surName = split[1];
						person.email = split[2];
						reservation.push_back(ReservationSync{ operation,person,split[3] });
						line = "";
					}
					else
					{
						//Pokud spatny tvar, vse vycistime a vratime false
						seat.clear();
						reservation.clear();
						return false;
					}
				}
			}
			else if(IsNumber(operation))
			{
				//Zpracovani adres na konci docasneho souboru
				seat_f = std::stoull(operation);
				if (tmp.good()) {
					std::getline(tmp, line);
					split = Split(line, ';');
					line = "";
					if (split.size() == 1 && IsNumber(split[0]))
						reservation_f = std::stoull(split[0]);					
					else
					{
						//Pokud spatny tvar, vse vycistime a vratime false
						seat.clear();
						reservation.clear();
						return false;
					}
				}
				else
				{
					//Pokud spatny tvar, vse vycistime a vratime false
					seat.clear();
					reservation.clear();
					return false;
				}
			}
		}
		else
		{
			//Pokud spatny tvar, vse vycistime a vratime false
			seat.clear();
			reservation.clear();
			return false;
		}
	}
	//Pokud na konci neni ukonceni dat, vracime false
	if (line == "#END OF DATA#")
		return true;

	seat.clear();
	reservation.clear();
	return false;
}
/**Provedeme pocatecni serializaci, aby se soubor fileName.data dostal do inicializacneho stavu
* @param seat Neserializovane operace akce
* @param reservation Neserializovane rezervace*/
void IO_System::SerializeIO(std::vector<SeatSync>& seat, std::vector<ReservationSync>& reservation)
{
	//tmp prazdny?
	//ne => provedeme test konce, pripadne update
	reservations = std::fstream("workingDIR/" + filename + "_reservations.dmp", std::ios_base::in | std::ios::binary);
	tmp = std::fstream("workingDIR/" + filename + ".data", std::ios_base::in);
	seats = std::fstream("workingDIR/" + filename + "_seats.dmp", std::ios_base::in| std::ios::binary);

	if (tmp.peek() != std::fstream::traits_type::eof())
	{
		fpos_t seatF=0;
		fpos_t reservationF = 0;
		if(ReadTmp(tmp,seat, reservation,seatF,reservationF))
		{

			seats.seekg(0, std::ios::end);
			reservations.seekg(0, std::ios::end);

			fpos_t seatR = seats.tellg();
			fpos_t reservationR = reservations.tellg();


			if(seatR!=seatF||reservationR!=reservationF)
			{
				//pokud byl soubor modifikovan nebo doslo k preruseni pri appendu do souboru:
				//precteme data ze souboru pouze po platna, znovu je do souboru zapiseme a pote provedeme append
				//Pokud nastane tato situace, nebudeme predpokladat, ze dojde k dalsimu vypadku v nejblizsi kratke dobe
				std::string strS;
				std::string strR;
				std::string tempS;
				std::string tempR;

				strS.resize(seatF);
				strR.resize(reservationF);
				seats.seekg(0, std::ios::beg);
				reservations.seekg(0, std::ios::beg);
				seats.read(&strS[0], strS.size());
				reservations.read(&strR[0], strR.size());

				//Sjednoceni oddelovacu radku
				for (size_t i = 0; i < strS.size(); ++i)
				{
					if (strS[i] == '\r')
						continue;
					tempS.push_back(strS[i]);
				}
				strS = std::move(tempS);
				for (size_t i = 0; i < strR.size(); ++i)
				{
					if (strR[i] == '\r')
						continue;
					tempR.push_back(strR[i]);
				}
				strR = std::move(tempR);

				seats.close();
				reservations.close();

				//prepis dat zpet
				seats.open("workingDIR/" + filename + "_seats.dmp", std::ios_base::out);
				reservations.open("workingDIR/" + filename + "_reservations.dmp", std::ios_base::out);

				seats.write(&strS[0], strS.size());
				reservations.write(&strR[0], strR.size());

				seats.close();
				reservations.close();

				//otevreni v modu append pro funkci Update
				seats.open("workingDIR/" + filename + "_seats.dmp", std::ios_base::out | std::ios_base::app);
				reservations.open("workingDIR/" + filename + "_reservations.dmp", std::ios_base::out | std::ios_base::app);
			}
			
			Update(seats, reservations, seat, reservation);
		}

		tmp.close();
		tmp = std::fstream("workingDIR/" + filename + ".data", std::ios_base::out | std::ios_base::trunc);
	}
	reservations.close();
	seats.close();
	tmp.close();
}
