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

	static string TypeToString(POKETYPES t);

private:
	Pokemon poki;
	vector<Pokemon> Pokemons;
};

string PokemonAPI::TypeToString(POKETYPES t)
{
	// Converts the enum POKETYPES to a string for ease of printing
	string type_to_print = "Unknown";

	switch (t)
	{
	case NORMAL:
	{
		type_to_print = "Normal";
		break;
	}
	case FIRE:
	{
		type_to_print = "Fire";
		break;
	}
	case WATER:
	{
		type_to_print = "Water";
		break;
	}
	case GRASS:
	{
		type_to_print = "Grass";
		break;
	}
	case ELECTRIC:
	{
		type_to_print = "Electric";
		break;
	}
	case ICE:
	{
		type_to_print = "Ice";
		break;
	}
	case FIGHTING:
	{
		type_to_print = "Fighting";
		break;
	}
	case POISON:
	{
		type_to_print = "Poison";
		break;
	}
	case GROUND:
	{
		type_to_print = "Ground";
		break;
	}
	case FLYING:
	{
		type_to_print = "Flying";
		break;
	}
	case PSYCHIC:
	{
		type_to_print = "Psychic";
		break;
	}
	case BUG:
	{
		type_to_print = "Bug";
		break;
	}
	case ROCK:
	{
		type_to_print = "Rock";
		break;
	}
	case GHOST:
	{
		type_to_print = "Ghost";
		break;
	}
	case DARK:
	{
		type_to_print = "Dark";
		break;
	}
	case DRAGON:
	{
		type_to_print = "Dragon";
		break;
	}
	case STEEL:
	{
		type_to_print = "Steel";
		break;
	}
	case FAIRY:
	{
		type_to_print = "Fairy";
		break;
	}
	default:
		break;
	}

	return type_to_print;
}

int main(int argc, char* argv[])
{
	Pokemon<PokemonAPI,PokemonAPI::Pokemon> p;
	p.Connect("pokeapi.co");
	p.Get("/api/v2/pokemon/");

	return 0;

}
