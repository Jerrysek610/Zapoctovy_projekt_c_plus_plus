#pragma once
///Trida provadejici synchronizaci operaci s akcemi 
class SeatSync
{
public:
	/**Konstruktor, ktery inicializuje druh operace
	* @param op Operace*/
	explicit inline SeatSync(std::string op) : operation(op), position(0), size(0), cost(0), cena(0)
	{
	}

	/**Konstruktor, ktery inicializuje druh operace, pozici
	* @param op Operace
	* @param position Pozice pro operaci
	* @param size Velikost*/
	explicit inline SeatSync(std::string op, size_t position, size_t size) : operation(op), position(position), size(size), cost(0), cena(0)
	{
	}

	/**Konstruktor, ktery inicializuje druh operace, pozici, velikost a cenu za sedadlo
	* @param op Operace
	* @param position Pozice pro operaci
	* @param size Velikost
	* @param cost Cena sedadel*/
	explicit inline SeatSync(std::string op, size_t position, size_t size, int cost) : operation(op), position(position), size(size), cost(cost), cena(0)
	{
	}

	/**Konstruktor, ktery inicializuje druh operace, pozici, velikost a cenu za sedadlo a celkovou cenu za akci
	* @param op Operace
	* @param position Pozice pro operaci
	* @param size Velikost
	* @param cost Cena sedadel
	* @param cena Cena za celou akci*/
	explicit inline SeatSync(std::string op, size_t position, size_t size, int cost, int cena) : operation(op), position(position), size(size), cost(cost), cena(cena)
	{
	}

	/**Vrati operaci
	* @return Operace*/
	inline std::string GetOp() const {
		return operation;
	}

	/**Vrati pozici, se kterou se provede operace
	* @return Pozice*/
	inline size_t GetPosition() const {
		return position;
	}

	/**Vraci velikost, kterou vyuziva operace
	* @return Velikost*/
	inline size_t GetSize() const {
		return size;
	}

	/**Vraci cenu vkladanych sedadel
	* @return Cena*/
	inline int GetCost() const {
		return cost;
	}

	/**Vraci celkovou cenu za akci
	* @return Cena za akci*/
	inline int GetCena() const
	{
		return cena;
	}

private:
	///Operace, kterou provedeme
	std::string operation;
	///Pozice pro operaci
	size_t position;
	///Velikost, kterou vyuziva operace
	size_t size;
	///Cena pridavanych sedadel
	int cost;
	///Cena za akci pro poradatele
	int cena;
};