#pragma once
#include <iterator>
#include <vector>
#include <sstream>
#include <algorithm>

/**Provede nakouskovani podle delimiteru a da kusy do sablonovaneho result
* @param s Parsovany vstup
* @param delim Oddelovac podle ktereho kouskujeme vstup
* @param result Sablovany vystup, kam davame kusy ze vstupu
* @return Nakouskovany vstup*/
template<typename Out>
inline void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}
/**Provede nakouskovani podle delimiteru a da kusy do vektoru
* @param s Parsovany vstup
* @param delim Oddelovac podle ktereho kouskujeme vstup
* @return Nakouskovany vstup*/
inline std::vector<std::string> Split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

/**Zjisti, zda retezec se sklada pouze z cisel (neberou se zaporne)
* @param split Testovaci vstup
* @return ANo - Ne*/
inline bool IsNumber(std::string split)
{
	for (size_t i = 0; i < split.size(); ++i)
		if (!isdigit(split[i]))
			return false;
	return true;
}
/**Vraci, zda jsou oba chary mezery
* @param lhs Levy char
* @param rhs pravy char
* @return Ano - Ne*/
inline bool BothAreSpaces(char lhs, char rhs) {
	return lhs == rhs && lhs == ' ';
}

/**Provede soucet ve vektoru
* @param is Vstupni data k secteni
* @return Soucet*/
inline size_t Sum(std::vector<size_t>& is)
{
	size_t sum = 0;
	std::for_each(is.begin(), is.end(), [&](size_t n) {
		sum += n;
	});
	return sum;
}
/**Vrati pri true ano jinak ne
* @param seat Vstupni bit
* @return Ano - Ne*/
inline std::string GetBool(const bool seat)
{
	return seat ? "Ano" : "Ne";
}

/**Spocita celkovy zisk
* @param seats Obsazenost sedadel
* @param is Ceny za sedadla
* @param cena Cena akce
* @return Celkovy zisk*/
inline std::string Hodnot(const std::vector<bool>& seats, const std::vector<int>& is, int cena)
{
	auto sum = -1*cena;
	for (size_t i = 0; i < seats.size(); ++i)
		if (seats[i])
			sum+=is[i];
	return std::to_string(sum);
}

/**Spocita pocet volnych mist
* @param seats Sedadla
* @return Pocet volnych mist*/
inline std::string SumFalse(const std::vector<bool>& seats)
{
	size_t sum = 0;
	for (size_t i = 0; i < seats.size(); ++i)
		if (!seats[i])
			++sum;
	return std::to_string(sum);
}