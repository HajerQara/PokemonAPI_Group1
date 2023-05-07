#include <iostream>
#include <string>
#include <list>
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
		///* Convert the vector<char> to a string
		//string dataStr(data.begin(), data.end());

		// Parse the JSON string
		//json jsonData = json::parse(dataStr);

		// Call the ParseJson method with the parsed JSON data
		//it.ParseJson(jsonData);*/

		ParseJson();

	};

	void ParseJson()
	{
		// Parse the JSON data
		json jp = json::parse(jsonData);

		// Finds out how many pokemon's in the query 
		size_t pokeCount = jp["results"].size();

		poke = new Poke * [pokeCount]; //creates an array that can hold all pokemons based on how many are queried

		for (size_t i = 0; i < pokeCount; i++)
		{
			poke[i] = new Poke;

			poke[i]->name = jp["results"][i]["name"].get<string>;
			poke[i]->abilities = jp["results"][i]["name"]["abilities"][i]["ability"][i].get<string>;


		};

	}
};

// Define operator<< as a standalone function
template <class T, class S>
std::ostream& operator<<(std::ostream& cout, const Pokemon<T, S>& pc) {
	cout << pc << endl;
	return cout;
};

class PokemonTypeData
{
public:
	struct TypeData
	{
		string TypeName;
		list<PokemonAPI::POKETYPES> DoubleDamageTo;
		list<PokemonAPI::POKETYPES> HalfDamageTo;
		list<PokemonAPI::POKETYPES> NoDamageTo;
		list<PokemonAPI::POKETYPES> DoubleDamageFrom;
		list<PokemonAPI::POKETYPES> HalfDamageFrom;
		list<PokemonAPI::POKETYPES> NoDamageFrom;
	};

	PokemonTypeData();
	// GetStrengths() const;

private:
	map<PokemonAPI::POKETYPES, TypeData> TypeStrengthsAndWeaknesses;
	void ParseJson(json j);
};

PokemonTypeData::PokemonTypeData()
{
	const string ALL_TYPES[] =
	{
		"normal", "fire", "water", "grass", "electric", "ice", "fighting", "poison", "ground", "flying",
		"psychic", "bug", "rock", "ghost", "dark", "dragon", "steel", "fairy"
	};

	Pokemon<PokemonAPI, PokemonAPI::Pokemon> temp_server;
	temp_server.Connect("pokeapi.co");

	for (string t : ALL_TYPES)
	{
		// Temporarily using pseudo code
		/*
		// Get automatically calls ParseJson at end of data
		temp_server.Get("/api/v2/type/" + t);

		*/
	}
}

void PokemonTypeData::ParseJson(json j)
{
	TypeData temp_struct;

	temp_struct.TypeName = j["name"].get<string>();

	if (j.contains("damage_relations"))
	{
		auto obj = j["damage_relations"];
		
		// Takes the name of each type, converts to the enum PokeTypes, and emplaces in the proper list
		if (obj.contains("double_damage_from"))
		{
			for (auto s : obj["double_damage_from"])
			{
				temp_struct.DoubleDamageFrom.emplace_back(
					PokemonAPI::StringToType(s["name"].get<string>()));
			}
		}

		if (obj.contains("half_damage_from"))
		{
			for (auto s : obj["half_damage_from"])
			{
				temp_struct.HalfDamageFrom.emplace_back(
					PokemonAPI::StringToType(s["name"].get<string>()));
			}
		}

		if (obj.contains("no_damage_from"))
		{
			for (auto s : obj["no_damage_from"])
			{
				temp_struct.NoDamageFrom.emplace_back(
					PokemonAPI::StringToType(s["name"].get<string>()));
			}
		}

		if (obj.contains("double_damage_to"))
		{
			for (auto s : obj["double_damage_to"])
			{
				temp_struct.DoubleDamageTo.emplace_back(
					PokemonAPI::StringToType(s["name"].get<string>()));
			}
		}

		if (obj.contains("half_damage_to"))
		{
			for (auto s : obj["half_damage_to"])
			{
				temp_struct.HalfDamageTo.emplace_back(
					PokemonAPI::StringToType(s["name"].get<string>()));
			}
		}

		if (obj.contains("no_damage_to"))
		{
			for (auto s : obj["no_damage_to"])
			{
				temp_struct.NoDamageTo.emplace_back(
					PokemonAPI::StringToType(s["name"].get<string>()));
			}
		}
	}

}

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
		FAIRY,
		UNKNOWN
	};

	struct Pokemon
	{
		string name;
		vector<string> abilities;
		vector<string> weaknesses;

	};

	void ParseWeakness(const json& jp)
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
	static POKETYPES StringToType(string s);
	static string TypeWeakness(POKETYPES t);

private:
	Pokemon poke;
	vector<Pokemon> Pokemons;
};

string PokemonAPI::TypeToString(POKETYPES t)
{
	// Converts the enum POKETYPES to a string for ease of printing
	string type_to_print = "unknown";

	switch (t)
	{
	case NORMAL:
	{
		type_to_print = "normal";
		break;
	}
	case FIRE:
	{
		type_to_print = "fire";
		break;
	}
	case WATER:
	{
		type_to_print = "water";
		break;
	}
	case GRASS:
	{
		type_to_print = "grass";
		break;
	}
	case ELECTRIC:
	{
		type_to_print = "electric";
		break;
	}
	case ICE:
	{
		type_to_print = "ice";
		break;
	}
	case FIGHTING:
	{
		type_to_print = "fighting";
		break;
	}
	case POISON:
	{
		type_to_print = "poison";
		break;
	}
	case GROUND:
	{
		type_to_print = "ground";
		break;
	}
	case FLYING:
	{
		type_to_print = "flying";
		break;
	}
	case PSYCHIC:
	{
		type_to_print = "psychic";
		break;
	}
	case BUG:
	{
		type_to_print = "bug";
		break;
	}
	case ROCK:
	{
		type_to_print = "rock";
		break;
	}
	case GHOST:
	{
		type_to_print = "ghost";
		break;
	}
	case DARK:
	{
		type_to_print = "dark";
		break;
	}
	case DRAGON:
	{
		type_to_print = "dragon";
		break;
	}
	case STEEL:
	{
		type_to_print = "steel";
		break;
	}
	case FAIRY:
	{
		type_to_print = "fairy";
		break;
	}
	default:
		break;
	}

	return type_to_print;
}

//a function to see pokemons weakness by comparing its type 
string PokemonAPI::TypeWeakness(POKETYPES t)
{
	string weak;
	switch (t)
	{
	case NORMAL:
	{
		weak = "Rock, Ghost, Steel";
		weaknesses.push_back(weak);
		break;
	}
	case FIRE:
	{
		weak = "Ground, Rock, Water";
		weaknesses.push_back(weak);
		break;
	}
	case WATER:
	{
		weak = "Grass, Dragon, Electric";
		weaknesses.push_back(weak);
		break;
	}
	case GRASS:
	{
		weak = "Flying, Poison, Bug, Steel, Fire, Grass, Dragon, Ice";
		weaknesses.push_back(weak);
		break;
	}
	case ELECTRIC:
	{
		weak = "Ground, Grass, Electric, Dragon";
		weaknesses.push_back(weak);
		break;
	}
	case ICE:
	{
		weak = "Steel, Fire, Water,Rock, Fighting";
		weaknesses.push_back(weak);
		break;
	}
	case FIGHTING:
	{
		weak = "Flying, Poison, Psychic, Bug, Ghost, Fairy";
		weaknesses.push_back(weak);
		break;
	}
	case POISON:
	{
		weak = "Psychic, Ground, Rock, Ghost, Steel";
		weaknesses.push_back(weak);
		break;
	}
	case GROUND:
	{
		weak = "Flying, Bug, Grass, Water, Ice";
		weaknesses.push_back(weak);
		break;
	}
	case FLYING:
	{
		weak = "Rock, Steel, Electric, Ice";
		weaknesses.push_back(weak);
		break;
	}
	case PSYCHIC:
	{
		weak = "Steel, Bug, Dark, Ghost";
		weaknesses.push_back(weak);
		break;
	}
	case BUG:
	{
		weak = "Fighting, Flying, Poison, Ghost, Steel, Fire, Fairy, Rock";
		weaknesses.push_back(weak);
		break;
	}
	case ROCK:
	{
		weak = "Fighting, Ground, Steel, Water, Grass";
		weaknesses.push_back(weak);
		break;
	}
	case GHOST:
	{
		weak = "Normal, Dark";
		weaknesses.push_back(weak);
		break;
	}
	case DARK:
	{
		weak = "Fighting, Dark, Fairy, Bug";
		weaknesses.push_back(weak);
		break;
	}
	case DRAGON:
	{
		weak = "Steel, Fairy, Ice";
		weaknesses.push_back(weak);
		break;
	}
	case STEEL:
	{
		weak = "Fire, Water, Electric, Fighting, Ground";
		weaknesses.push_back(weak);
		break;
	}
	case FAIRY:
	{
		weak = "Poison, Steel, Fire";
		weaknesses.push_back(weak);
		break;
	}
	default:
	{
		weak = "Unknown";
		weaknesses.push_back(weak);
		break;
	}

	return type_to_print;

	};
}

PokemonAPI::POKETYPES PokemonAPI::StringToType(string s)
{
	// Converts the a string to the enum POKETYPES
	POKETYPES type_to_get = UNKNOWN;
	map<string, POKETYPES> StringTypeMap = {
		{"normal", NORMAL}, {"fire", FIRE}, {"water", WATER}, {"grass", GRASS },
		{"electric", ELECTRIC }, {"ice", ICE}, { "fighting", FIGHTING}, { "poison", POISON},
		{ "ground", GROUND}, { "flying", FLYING}, { "psychic", PSYCHIC}, { "bug", BUG},
		{ "rock", ROCK }, { "ghost", GHOST}, { "dark", DARK}, { "dragon", DRAGON},
		{ "steel", STEEL}, { "fairy", FAIRY}, { "unknown", UNKNOWN}
	};

	try
	{
		type_to_get = StringTypeMap[s];
	}
	catch (...) // If the string isn't in the map, or any other error comes up, just returns UNKNOWN 
	{
		type_to_get = UNKNOWN;
	}
	
	return type_to_get;
}

int main(int argc, char* argv[])
{
	Pokemon<PokemonAPI, PokemonAPI::Pokemon> p;
	p.Connect("pokeapi.co");
	p.Get("/api/v2/pokemon/");

	return 0;
}