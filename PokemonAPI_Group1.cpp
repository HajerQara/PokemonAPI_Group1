#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cstdlib>
#include "HttpClient.h"
#include  <conio.h> 

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
	int current_page = 1;

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

		// Error came up here; "Syntax error: unexpected token 'identifier', expected 'type specifier'"
		poke = new Poke * [pokeCount]; //creates an array that can hold all pokemons based on how many are queried

		for (size_t i = 0; i < pokeCount; i++)
		{
			poke[i] = new Poke;

			poke[i]->name = jp["results"][i]["name"].get<string>;
			poke[i]->abilities = jp["results"][i]["name"]["abilities"][i]["ability"][i].get<string>;
			poke[i]->types = jp["results"][i]["name"]["types"][i]["type"][i]["name"].get<string>;


		};

	}
};

// Define operator<< as a standalone function
template <class T, class S>
std::ostream& operator<<(std::ostream& cout, const Pokemon<T, S>& pc) {
	cout << "" << "Name: " << pc.name << std::endl; << endl;
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
		FAIRY,
		UNKNOWN
	};

	struct Pokemon
	{
		string name;
		vector<string> abilities;
		vector<string> weaknesses;
		vector<string> types;
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
	static POKETYPES TypeWeakness(POKETYPES t, Pokemon p);

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

//Searches based on Type of Pokemon entered by the user
template <class T, class S>
list<string> SearchPokeType(PokemonAPI::POKETYPES type, const Pokemon<T,S> &p) {
	list<string> result;
	for (const auto& p : type) {
		if (std::find(p.types.begin(), p.types.end(), type) != p.types.end()) {
			result.push_back(p.name);
		}
	}
	return result;
}

//Compares the types to later sort alphabetically 
template <class T, class S>
bool sortPokeType(const Pokemon<T, S>& p, const Pokemon<T, S> & c) {
	return p.type < c.type;
}


//A function to see pokemons weakness by comparing its type 
PokemonAPI::POKETYPES PokemonAPI::TypeWeakness(POKETYPES t, Pokemon p)
{
	string weak;
	switch (t)
	{
	case NORMAL:
	{
		weak = "Rock, Ghost, Steel";
		p.weaknesses.push_back(weak);
		break;
	}
	case FIRE:
	{
		weak = "Ground, Rock, Water";
		p.weaknesses.push_back(weak);
		break;
	}
	case WATER:
	{
		weak = "Grass, Dragon, Electric";
		p.weaknesses.push_back(weak);
		break;
	}
	case GRASS:
	{
		weak = "Flying, Poison, Bug, Steel, Fire, Grass, Dragon, Ice";
		p.weaknesses.push_back(weak);
		break;
	}
	case ELECTRIC:
	{
		weak = "Ground, Grass, Electric, Dragon";
		p.weaknesses.push_back(weak);
		break;
	}
	case ICE:
	{
		weak = "Steel, Fire, Water,Rock, Fighting";
		p.weaknesses.push_back(weak);
		break;
	}
	case FIGHTING:
	{
		weak = "Flying, Poison, Psychic, Bug, Ghost, Fairy";
		p.weaknesses.push_back(weak);
		break;
	}
	case POISON:
	{
		weak = "Psychic, Ground, Rock, Ghost, Steel";
		p.weaknesses.push_back(weak);
		break;
	}
	case GROUND:
	{
		weak = "Flying, Bug, Grass, Water, Ice";
		p.weaknesses.push_back(weak);
		break;
	}
	case FLYING:
	{
		weak = "Rock, Steel, Electric, Ice";
		p.weaknesses.push_back(weak);
		break;
	}
	case PSYCHIC:
	{
		weak = "Steel, Bug, Dark, Ghost";
		p.weaknesses.push_back(weak);
		break;
	}
	case BUG:
	{
		weak = "Fighting, Flying, Poison, Ghost, Steel, Fire, Fairy, Rock";
		p.weaknesses.push_back(weak);
		break;
	}
	case ROCK:
	{
		weak = "Fighting, Ground, Steel, Water, Grass";
		p.weaknesses.push_back(weak);
		break;
	}
	case GHOST:
	{
		weak = "Normal, Dark";
		p.weaknesses.push_back(weak);
		break;
	}
	case DARK:
	{
		weak = "Fighting, Dark, Fairy, Bug";
		p.weaknesses.push_back(weak);
		break;
	}
	case DRAGON:
	{
		weak = "Steel, Fairy, Ice";
		p.weaknesses.push_back(weak);
		break;
	}
	case STEEL:
	{
		weak = "Fire, Water, Electric, Fighting, Ground";
		p.weaknesses.push_back(weak);
		break;
	}
	case FAIRY:
	{
		weak = "Poison, Steel, Fire";
		p.weaknesses.push_back(weak);
		break;
	}
	default:
	{
		weak = "Unknown";
		p.weaknesses.push_back(weak);
		break;
	}
	};

	return t;

	
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

// Class designed to store each type's weaknesses and strengths for use in
// determining a pokemon's weaknesses/strengths. Since Pokemon can have two
// types, their weaknesses/strengths can vary significantly, and need to be
// calculated.
class PokemonTypeData : public HttpClient
{
public:
	struct TypeData
	{
		string TypeName;
		list<PokemonAPI::POKETYPES> DoubleDamageTo = { };
		list<PokemonAPI::POKETYPES> HalfDamageTo = { };
		list<PokemonAPI::POKETYPES> NoDamageTo = { };
		list<PokemonAPI::POKETYPES> DoubleDamageFrom = { };
		list<PokemonAPI::POKETYPES> HalfDamageFrom = { };
		list<PokemonAPI::POKETYPES> NoDamageFrom = { };
	};

	PokemonTypeData();
	~PokemonTypeData();
	// GetStrengths() const;

	// void sort(pokemon.begin(), pokemon.end(), sortPokeType); //sorts by type alphabetically

	void StartOfData() { data.clear(); };

	void Data(const char arrData[], const unsigned int iSize)
	{
		data.insert(data.end(), arrData, arrData + iSize);
	};

	void EndOfData()
	{
		string json_data = "";
		for (char c : data)
		{
			json_data += c;
		}

		json j = json::parse(json_data);
		ParseJson(j);
	};

	friend ostream& operator<<(ostream& output, const PokemonTypeData& client);

	list<string> GetQuadStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetDoubleStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetHalfStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetQuarterStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetNoStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetQuadWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetDoubleWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetHalfWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetQuarterWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);
	list<string> GetNoWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo);

private:
	map<PokemonAPI::POKETYPES, TypeData> *TypeStrengthsAndWeaknesses;
	void ParseJson(json j);
	vector<char> data;
};

PokemonTypeData::~PokemonTypeData()
{
	delete TypeStrengthsAndWeaknesses;
}

PokemonTypeData::PokemonTypeData()
{
	const string ALL_TYPES[] =
	{
		"normal", "fire", "water", "grass", "electric", "ice", "fighting", "poison", "ground", "flying",
		"psychic", "bug", "rock", "ghost", "dark", "dragon", "steel", "fairy"
	};

	TypeStrengthsAndWeaknesses = new map<PokemonAPI::POKETYPES, TypeData>();

	Connect("pokeapi.co");

	for (string t : ALL_TYPES)
	{
		// Get automatically calls ParseJson at end of data
		Get("/api/v2/type/" + t);
	}
}

void PokemonTypeData::ParseJson(json j)
{
	TypeData temp_struct;

	temp_struct.TypeName = j["name"].get<string>();

	if (j.contains("damage_relations"))
	{
		auto obj = j["damage_relations"].get<json>();

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

	TypeStrengthsAndWeaknesses->emplace(
		PokemonAPI::StringToType(temp_struct.TypeName), temp_struct);
}

ostream& operator<<(ostream& output, const PokemonTypeData& client)
{
	for (auto type_data : *client.TypeStrengthsAndWeaknesses)
	{
		output << type_data.second.TypeName << ", 2x to: ";
		for (auto strength : type_data.second.DoubleDamageTo)
			output << PokemonAPI::TypeToString(strength) << ", ";
		output << endl;
	}
		
	return output;
}

list<string> PokemonTypeData::GetQuadStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{
	list<string> TypeValues = { };
	list<PokemonAPI::POKETYPES> FirstTypeDoubles = { }; // First type's 2x strengths
	bool FoundFirst = false;

	for (auto pair : *TypeStrengthsAndWeaknesses)
	{
		if (!FoundFirst && (pair.first == typeOne || pair.first == typeTwo))
		{
			for (PokemonAPI::POKETYPES t : pair.second.DoubleDamageTo)
				FirstTypeDoubles.emplace_back(t);
			FoundFirst = true;
			continue;
		}
		else if (FoundFirst && (pair.first == typeOne || pair.first == typeTwo))
		{
			for (PokemonAPI::POKETYPES t : pair.second.DoubleDamageTo)
			{
				for (PokemonAPI::POKETYPES f : FirstTypeDoubles)
					if (t == f) // If there's two 2x strengths, add to the 4x list
					{
						TypeValues.emplace_back(PokemonAPI::TypeToString(t));
						break;
					}
			}
			break;
		}
	}

	return TypeValues;
}

list<string> PokemonTypeData::GetDoubleStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{
	list<string> TypeValues = { };
	list<PokemonAPI::POKETYPES> FirstTypeDoubles = { }; // First type's 2x strengths
	bool FoundFirst = false;

	for (auto pair : *TypeStrengthsAndWeaknesses)
	{
		if (!FoundFirst && (pair.first == typeOne || pair.first == typeTwo))
		{
			for (PokemonAPI::POKETYPES t : pair.second.DoubleDamageTo)
				FirstTypeDoubles.emplace_back(t);
			FoundFirst = true;
			continue;
		}
		else if (FoundFirst && (pair.first == typeOne || pair.first == typeTwo))
		{
			for (PokemonAPI::POKETYPES t : pair.second.DoubleDamageTo)
			{
				bool match_found = false;
				for (PokemonAPI::POKETYPES f : FirstTypeDoubles)
				{
					if (t == f) // If there's two 2x strengths, don't add to the 2x list
					{
						match_found = true;
						break;
					}
				}
				
				if (!match_found)
					FirstTypeDoubles.emplace_back(t);
			}
			break;
		}
	}

	for (PokemonAPI::POKETYPES t : FirstTypeDoubles)
		TypeValues.emplace_back(PokemonAPI::TypeToString(t));

	return TypeValues;
}

list<string> PokemonTypeData::GetHalfStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetQuarterStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetNoStrength(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetQuadWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetDoubleWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetHalfWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetQuarterWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

list<string> PokemonTypeData::GetNoWeak(PokemonAPI::POKETYPES typeOne, PokemonAPI::POKETYPES typeTwo)
{

}

string EnterSearchTerm()
{
	// Function for entering a search term. Pulled from the Unsplash code we worked on
	string new_term = "";
	char c = ' ';
	const int KEY_ENTER = 13;
	const int KEY_BACKSPACE = 8;

	cout << "\nType new search term and then press enter: ";

	while (c != KEY_ENTER)
	{
		c = _getch();

		if (IsCharAlphaNumeric(c)) // Only allowing alphanumeric characters for search
		{
			new_term += c;
			cout << c;
		}

		else if (c == KEY_BACKSPACE && new_term.size() > 0) // Only allows backspacing if the string isn't empty
		{
			new_term.pop_back();
			cout << '\b' << " " << '\b'; // Backpaces, overwrites with a space, then backspaces again to put cursor at right position
		}
	}
	cout << "\nNew Search Term: " << new_term << endl;
	return new_term;
}

int main(int argc, char* argv[])
{
	// Key codes for use in UI
	const int ARROW_UP = 72;
	const int ARROW_DOWN = 80;
	const int KEY_ESCAPE = 27;	// Exit button is ESCAPE
	const int KEY_E = 101;		// Enter search term with E key
	// const int KEY_O = 111;	// Unused
	const int KEY_ENTER = 13;
	const int KEY_T = 116;		// Search by Type

	string search_term = "";
	int c = 0; // The char that is being accessed whenever a key is pressed

	int TotalResultsFromSearch = 0;

	// App header on start
	cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl
		<< "||||||||||||||| Welcome to the Pokemon App! |||||||||||||||||||||" << endl
		<< "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl << endl;

	// Initializes type data source, pulls strengths/weaknesses for each type
	PokemonTypeData t;

	Pokemon<PokemonAPI, PokemonAPI::Pokemon> p;
	p.Connect("pokeapi.co");
	p.Get("/api/v2/pokemon/");

	while (c != KEY_ESCAPE)
	{
		cout << "Controls: \n\tExit: Escape Key\n\tSearch by Name: E Key or Enter Key\n\tSearch by Type: T"
			<< "\n\tNext Page: Down Key\n\tPrevious Page: Up Key" << endl;

		c = _getch();

		// For testing what each key's integer value is
		// cout << c << endl;

		// Searching by Name
		if (c == KEY_E || c == KEY_ENTER)
		{
			cout << "Searching by name..." << endl;
			search_term = EnterSearchTerm();
			p.current_page = 1;
			p.Get("/api/v2/pokemon/" + search_term);
			cout << p << endl;
		}

		// Searching by Type
		else if (c == KEY_T)
		{
			cout << "Searching by type..." << endl;
			search_term = EnterSearchTerm();
			p.current_page = 1;
			p.Get("/api/v2/type/" + search_term);
			cout << p << endl;
		}

		// If going to the previous page:
		else if (c == ARROW_UP)
		{
			if (p.current_page == 1)
				cout << "No pages further back!" << endl;
			else
			{
				p.current_page += 1;

				cout << p << endl;
			}
		}

		// If going to the next page:
		else if (c == ARROW_DOWN)
		{
			/*if (p.current_page == p.GetTotalPages(TotalResultsFromSearch))
				cout << "No more pages left!" << endl;
			else
			{
				p.current_page -= 1;

				cout << p << endl;
			}*/
		}

	}

	return 0;
}
