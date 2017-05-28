#pragma once
#include "Building.h"
#include "Exceptions.h"
#include <windows.h>
#include <fstream>
#include "SeatSync.h"
#include "ReservationSync.h"


///Trida, zabyvajici se serializaci jedne akce
class IO_System
{
public:
	/**Konstruktor, ktery si uloze jmeno souboru, vytvore/otevre podle toho soubor s rozvrzenem akce a soubor s rezervacemi akce
	* @param filename Nazev souboru
	* @param n Pocet operaci pro automaticke ulozeni
	* @throws bad_path Spatna cesta 
	* @throws not_dir Chybejici slozka workingDIR
	 */
	explicit inline IO_System(std::string filename, size_t n=5): filename(filename), serialize(n)
	{
		auto ftyp = GetFileAttributesA("workingDIR");
		//spatna cesta
		if (ftyp == INVALID_FILE_ATTRIBUTES)
		{
			throw bad_path();
		}
		//neexistuje slozka
		if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY))
		{
			throw not_dir();
		}

		//vytvorene soubore - / je univerzalne cesta
		reservations = std::fstream("workingDIR/" + filename + "_reservations.dmp", std::ios_base::out|std::ios_base::app);
		tmp = std::fstream("workingDIR/"+filename+".data", std::ios_base::out|std::ios_base::app);
		seats = std::fstream("workingDIR/" + filename + "_seats.dmp", std::ios_base::out | std::ios_base::app);

		reservations.close();
		tmp.close();
		seats.close();
	}

	/**Provede zapsani, pokud je command true, jedna se o ulozeni uzivatelem, jinak se jedna o preventivni volani po kazde akci
	* @param command Priznak pro zapis
	* @return Povedl se zapis?*/
	bool Write(bool command);

	/**Precte serializovanou budovu az po platne oznacena data
	* @param action Akce
	* @param filename Nazev akce
	* @return Zda se povedlo cteni*/
	bool Read(Building& action, std::string filename);

	/**Provedeme pocatecni serializaci, aby se soubor fileName.data dostal do inicializacneho stavu
	* @param seat Neserializovane operace akce
	* @param reservation Neserializovane rezervace*/
	void SerializeIO(std::vector<SeatSync>& seat, std::vector<ReservationSync>& reservation);

	/**Vrate pocet aktualne zapsanych rezervaci v pameti, ktere jeste neprosli synchronizaci
	* @return Odkaz na Nesynchronizovane rezervace*/
	inline std::vector<ReservationSync>& GetNonSyncReservations()
	{
		return reservation;
	}
	/**Vrati pocet aktualne zapsanych zmen v budove, ulozenych v pameti, ktere jeste neprosli synchronizaci
	* @return Odkaz na nesychronizovane operace akce*/
	inline std::vector<SeatSync>& GetNonSyncSeats()
	{
		return seat;
	}
private:
	///Nazev akce, od ktere se budou vytvaret soubory
	std::string filename;
	///Rezervace cekajici na synchronizaci
	std::vector<ReservationSync> reservation;
	///Sedadla cekajece na synchronizaci
	std::vector<SeatSync> seat;
	///Pocet sedadel, po kterem se provede serializace
	size_t serialize;
	///Zapis struktury akce \n\n
	///Struktura akce pri operaci IC|IR:\n
	/// Operace IC|IR \n
	/// pozice;velikost;cena \n
	/// Struktura akce pri operaci RC|RR:\n
	/// Operace RC|RR \n
	/// pozice;velikost \n
	/// Struktura akce pri operaci RS a CR: \n
	/// Operace RS|CR \n
	/// cislo;cena
	std::fstream seats;
	///Zapis rezervace na akci \n\n
	///Struktura rezervace pro A|+:\n
	///Operace A|+
	///Jmeno; Prejmene; email; hash hesla
	///cislo sedadla (od 1); cena\n
	///cislo sedadla (od 1); cena\n
	///...;\n
	///Struktura rezervace pro -:\n
	///Operace - \n
	///Jmeno; Prejmene; email; hash hesla \n
	std::fstream reservations;
	///Zapis provadejece operace - zapese se zde jedna operace, ktera se pak updatne do data a pote se prepese do preslusneho souboru.\n \n
	///Struktura tmp je nasledujece:
	///1. Operace A|+|-|IC|IR|CR|RS (rekajece nam, co mame provest)
	///2. data k zapisu
	///3. Odkaz do seats
	///4. Odkaz do reservations
	///5. #END OF DATA# znacka\n \n
	///Update dat se provede nasledovne:
	///1. Nacteme vsechny zmeny (nebo je jiz mame nactene), ktere je nutno aplikovat, do mapy podle klece
	///2. Zahajeme prepis dat do filename.data.
	///3. Provedeme prepis dat do konecnych soubore (append)
	///4. Provedeme vyprazdnene filename.data souboru\n \n
	///Mozne stavy soubore:
	///1. V tmp nene nic/neexistuje => inicializacne stav - vse je v poradku
	///2. V tmp je zacatek souboru bez znacky ukoncene dat:\n
	///a) Nedoslo k zapisu operace - data v reservations/seats jsou platna. Operace jsou ztraceny\n
	///3. V tmp je znacka konce => nebyl proveden presun, nebo nebyl dokoncen
	std::fstream tmp;
};