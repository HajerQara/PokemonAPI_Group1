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
		/*ParseJson();*/
		// Convert the vector<char> to a string
		string dataStr(data.begin(), data.end());

		// Parse the JSON string
		json jsonData = json::parse(dataStr);

		// Call the ParseJson method with the parsed JSON data
		it.ParseJson(jsonData);

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
	struct Pokemon 
	{
		string name;
		vector<string> weaknesses;
	
	};
	void ParseJson(const json& jp)
	{
		for (auto it = jp.begin(); it != jp.end(); ++it)
		{
			Pokemon p;
			p.name = it.value().at("name");

			// Get the weaknesses for the pokemon
			auto weaknesses = it.value().at("weaknesses");
			for (auto& w : weaknesses)
			{
				p.weaknesses.push_back(w);
			}

			Pokemons.push_back(p);
		};
	};

	// Method to search for Pokemon by weakness
	vector<Pokemon> pokemonWeakness(const string& weakness) const
	{
		vector<Pokemon> result;
		for (const auto& p : Pokemons)
		{
			if (find(p.weaknesses.begin(), p.weaknesses.end(), weakness) != p.weaknesses.end())
			{
				result.push_back(p);
			}
		}
		return result;
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
