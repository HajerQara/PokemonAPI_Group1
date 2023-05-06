#include <iostream>
#include <string>
#include<list>
#include <vector>
#include <cstdlib>
#include "HttpClient.h"

// Quell the nlohmann::json warnings.
#pragma warning( push )
#pragma warning(disable:28020)
#include "json.hpp"
#pragma warning( pop )

using namespace std;
using json = nlohmann::json;

///////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T, class S>
class Pokemon : public  HttpClient 
{
private:
    vector<char> data;
    T it;

public:
	Pokemon() {};
	~Pokemon() {};

	S& operator[](int i)
	{
		assert(0 <= i && i < data.size());
		return *data[i];
	};

	Pokemon<T, S> operator ++(int i)
	{
		return *this++;
	};

protected:
	virtual void StartOfData() {};

	virtual void Data(const char arrData[], const unsigned int iSize)
	{
		data.insert(data.end(), arrData, arrData + iSize);
	};

	virtual void EndOfData()
	{
		ParseJson();

	};
};

// Define operator<< as a standalone function
template <class T, class S>
std::ostream& operator<<(std::ostream& cout, const Pokemon<T, S>& pc) {
	cout << pc << endl;
	return cout;
};

class PokemonAPI 
{
public:
	enum POKETYPES
	{
		NORMAL, 
		FIRE, 
		WATER, 
		GRASS, 
		ELECTRIC, 
		ICE, 
		FIGHTING, 
		POISON,
		GROUND, 
		FLYING, 
		PSYCHIC, 
		BUG, 
		ROCK, 
		GHOST, 
		DARK, 
		DRAGON, 
		STEEL, 
		FAIRY
	};

	struct Pokemon 
	{
		string name;

	
	};
	void ParseJson(const json& jp)
	{
		for (auto it = jp.begin(); it != jp.end(); ++it)
		{

		};
	};

private:
	Pokemon poki;
	vector<Pokemon> Pokemons;


};


int main(int argc, char* argv[])
{
	Pokemon<PokemonAPI,PokemonAPI::Pokemon> p;
	p.Connect("pokeapi.co");
	p.Get("/api/v2/pokemon/");

	return 0;

}
