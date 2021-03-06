
#include "pch.h"
#include <iostream>
#include <string>
#include <iterator> 
#include <unordered_map>
#include <vector>
#include <stack>
#include <time.h>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// A class that runs the time, checks if the hero has worked or attacked the dungeon today, how many adventures 
//has left etc.
class Game {
private:
	int time; 
	bool worked;
	int adventuresLeft;
	bool attackDungeon;
	bool newDay;

public:
	Game() {
		time = 0;
		worked = false;
		adventuresLeft = 5;
		attackDungeon = false;
		newDay = false;
	}

	bool hasWorked() { return worked; }
	int getAdventuresLeft() { return adventuresLeft; }
	int hasAttackDungeon() { return attackDungeon; }

	void adventure() { adventuresLeft--;  }
	void Work() { worked = true; }
	void AttackDungeon() { attackDungeon = true; } 

	void Day() { //Resets
			 worked = false;
			attackDungeon = false;
			 adventuresLeft = 5;
		}
	
	int getTime() { return time; }
	
	void spendTime(int hours) {
		if (time + hours >= 24) {
			Day();
			int remaining = time + hours - 24;
			time = remaining;
		}
		else {
			time += hours;
		}
	}
};

class Entity {
private:
	string Name;
	string battleClass; 
	int Level;
	int Experience;
	int Gold; 
	int Life;
	//Stats
	int Strength;
	int Agility;
	int Intelligence;
	int Constitution;
	int Defence;
protected:
	Game newGame;
public:
	//constructors
	Entity() {};
	Entity(string name) { //Creates new character from level 1 and all atributes are the initial ones and/or default 
		Name = name;
		Level = 1;
		Gold = 100;
		Experience = 0;
		Strength = 10;
		Agility = 10;
		Intelligence = 10;
		Constitution = 10;
		Defence = 10;
	};

	//Constructor for already developped character and/or Monster
	Entity(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence) {
		Name = name;
		Level = level;
		Strength = strength;
		Agility = dexterity;
		Intelligence = intelligence;
		Constitution = constitution;
		Defence = defence;
	};

	void increaseGold(int value) {
		Gold += value;
	}

	void increaseEXP(int value) {
		Experience += value;
	}

	void levelUp() {
		if (Experience > (30 * Level)) {
			Level++;
			Experience = 0;
		}
	}

	int getTime() {
		return newGame.getTime();
	}

	virtual string getClass() {
		return "";
	}
	virtual int getLife() {
		return Constitution;
	}

	string getName() { return Name; }
	int getLevel() { return Level; }
	int getGold() { return Gold; }

	int getStrength() { return Strength; }
	int getAgility() { return Agility; }
	int getIntelligence() { return Intelligence; }
	int getConstitution() { return Constitution; }
	int getDefence() { return Defence; };
	

	virtual int getAttackValue(int dice1, int dice2) { return 0; };
	virtual int getDamage() { return 0; };

	void work(int hours) {
		if (!newGame.hasWorked()) {
			if (hours > 0 && hours <= 10) {
				Gold += hours * (Level * 10);
				cout << "You've worked " << hours << " hours and you have earned " << (hours * (Level * 10)) << " Gold" << endl;
				newGame.Work();
				newGame.spendTime(hours);
			}
			else {
				cout << "The workshift is only between 1 and 10 hours!";
			}
		}
		else {
			cout << "You have already worked today. You can go back to work Tomorrow.";
		}
	}

	void increaseStats(char stat) {
		int StrTimes = 2; // variables to hold how many times an attribute has been increased
		int AgiTimes = 2;
		int IntTimes = 2;
		int ConsTimes = 2;
		int DefTimes = 2;

			switch (stat) {
			case 's':
				if (Gold > 5 * StrTimes) {
					Strength++;
					Gold -= 5 * StrTimes;
					StrTimes++;
					cout << "Strength was increased! New Strength: " << getStrength() << endl;
				}
				else {
					notEnoughMoney();
				}
				break;
			case 'a': 
				if (Gold > 5 * AgiTimes) {
					Agility++;
					Gold -= 5 * AgiTimes;
					AgiTimes++;
					cout << "Agility was increased! New Agility: " << getAgility() << endl;
				}
				else {
					notEnoughMoney();
				}
				break;
			case 'i':
				if (Gold > 5 * IntTimes) {
				Intelligence++;
				Gold -= 5 * IntTimes;
				IntTimes++;
				cout << "Intelligence was increased! New Intelligence: " << getIntelligence() << endl;
				}else {
					notEnoughMoney();
				}
				break;
			case 'c': 
				if (Gold > 5 * ConsTimes) {
					Constitution++;
					Gold -= 5 * ConsTimes;
					ConsTimes++;
					cout << "Constitution was increased! New Constitution: " << getConstitution() << endl;
				}
				else {
					notEnoughMoney();
				}
				break;
			case 'd': 
				if (Gold > 5 * DefTimes) {
				Defence++;
				Gold -= 5 * DefTimes;
				DefTimes++;
				cout << "Defence was increased! New Defence: " << getDefence() << endl;
				}else {
					notEnoughMoney();
				}
				break;
			default: cout << "Invalid stat." << endl;
			}
		}

	void notEnoughMoney() { //Message that appears when the user wants to increase an attribute but they don't have enough money
		system("CLS"); // clear screen
		cout << "You don't have enough gold. Press 'e' and go to work or to an adventure." << endl;
	}
		
};

class Monster : public Entity {
public:
	Monster() {};
	Monster(string name) : Entity(name) {};
	Monster(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence)
		: Entity(name, level, strength, dexterity, intelligence, constitution, defence) {};

	virtual int getAttackValue(int dice1, int dice2) { return 0; };

	virtual int getLife() { return 0; };

	virtual string getClass() { return ""; };
};

class CombatMonster : public Monster {
public:
	CombatMonster(string name) : Monster(name) {};
	CombatMonster(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence)
		: Monster(name, level, strength, dexterity, intelligence, constitution, defence) {};
	virtual string getClass() { return "Combat"; }

	int getDamage() {
		return ((rand() % 4 + 1) * (Entity::getStrength() / 10));
	};
	 int getAttackValue(int dice1, int dice2) {
		 return Entity::getStrength() + dice1 + dice2;
	 }

	 int getLife() {
		 return (Entity::getConstitution() * 5 * (Entity::getLevel() + 1));
	 }
};

class AgileMonster : public Monster {
public:
	AgileMonster(string name) : Monster(name) {};
	AgileMonster(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence)
		: Monster(name, level, strength, dexterity, intelligence, constitution, defence) {};
	string getClass() { return "Agile"; }

	int getDamage() {
		return ((rand() % 6 + 2) * (Entity::getAgility() / 10));
	};
	int getAttackValue(int dice1, int dice2) {
		return Entity::getAgility() + dice1 + dice2;
	}

	int getLife() {
		return (Entity::getConstitution() * 3 * (Entity::getLevel() + 1));
	}
};

class MagicalMonster : public Monster {
public:
	MagicalMonster(string name) : Monster(name) {};
	MagicalMonster(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence)
		: Monster(name, level, strength, dexterity, intelligence, constitution, defence) {};

	string getClass() { return "Magical"; }
	
	int getDamage() {
		return ((rand() % 8 + 3) * (Entity::getIntelligence() / 10));
	};

	int getLife() {
		return (Entity::getConstitution() * 2 * (Entity::getLevel() + 1));
	}
};

class Hero : public Entity {
public:
	Hero() {};
	Hero(string name) : Entity(name) {};
	Hero(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence) 
		: Entity(name,level,strength,dexterity,intelligence,constitution, defence) {};


	virtual string getClass() {
		return ""; //Warrior or Archer or Wizard, 
	}
	virtual int getLife() {
		return (Entity::getConstitution() * 5 * (Entity::getLevel() + 1)); //Each class uses different formula
	}	
	virtual int getDamage() { return 0; }; // A bit different formula for each childclass
	virtual int getAttackValue(int dice1, int dice2) { return 0; }; //A bit different formula for each childclass
	


	bool attackDungeon() {
		if (!newGame.hasAttackDungeon()) {
			newGame.spendTime(3);
			newGame.AttackDungeon();
			return true;
		}
		return false;
	};
	void attackHero() {
		newGame.spendTime(2);
	};

	int getAdventuresLeft() {
		return newGame.getAdventuresLeft();
	}
	bool adventureTime() {
		if (newGame.getAdventuresLeft() > 0) {
			newGame.adventure();
			newGame.spendTime(3);
			return true;
		}
		else {
			cout << "You have had 5 adventures today and you don't have thirst for adventures anymore." << endl;
			cout << "You will have thirst for adventures tomorrow." << endl;
			return false;
		}
	};
};

// Warriors have the most life points but the least damage value
class Warrior : public Hero {
public:
	Warrior(string name) : Hero(name) {};
	Warrior(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence) 
		: Hero(name, level, strength, dexterity, intelligence, constitution, defence) {};
	
	
	string getClass() { return "Warrior"; }

	int getDamage() {  
		return ((rand() % 4 + 1) * (Entity::getStrength() / 10)); 
	};
	int getLife() {
		return (Entity::getConstitution() * 6 * (Entity::getLevel() + 1)); 
	}

		int getAttackValue(int dice1, int dice2) {
		return Entity::getStrength() + dice1 + dice2;
	}

};
// Wizards have the least life points but the highest damage value
class Wizard : public Hero {
public:
	Wizard(string name) : Hero(name) {};
	Wizard(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence) :
		Hero(name, level, strength, dexterity, intelligence, constitution, defence) {};

	string getClass() { return "Wizard"; }

	int getLife() {
		return (Entity::getConstitution() * 2 * (Entity::getLevel() + 1));
	}
	int getDamage() {
		return ((rand() % 8 + 3) * (Entity::getIntelligence() / 10));
	};
	int getAttackValue(int dice1, int dice2) {
		return Entity::getIntelligence() + dice1 + dice2;
	};
	};

//Archers are in between
	class Archer : public Hero {
	public:
		Archer(string name) : Hero(name) {};
		Archer(string name, int level, int strength, int dexterity, int intelligence, int constitution, int defence) :
			Hero(name, level, strength, dexterity, intelligence, constitution, defence) {};

		string getClass() { return "Archer"; }
		int getLife() {
			return (Entity::getConstitution() * 4 * (Entity::getLevel() + 1));
		};
		int getDamage() {
			return ((rand() % 6 + 2) * (Entity::getAgility() / 10));
		};
		int getAttackValue(int dice1, int dice2) {
			return Entity::getAgility() + dice1 + dice2;
		}
	};

	// Battle Simulation, returns true if the user's hero wins; Definitely needs more advanced algorithm for improving the quality
	//of the battle, now it uses mostly randomly generated numbers and luck rather than strength and skill
	bool Fighting(Hero* hero, Entity* opponent) {
		cout << "Opponent's Name: " << opponent->getName() << endl;
		cout << "Opponent's Level: " << opponent->getLevel() << endl;
		cout << "Opponent's Class: " << opponent->getClass() << endl;
		cout << "Opponent's Strength: " << opponent->getStrength() << endl;
		cout << "Opponent's Agility: " << opponent->getAgility() << endl;
		cout << "Opponent's Intelligence: " << opponent->getIntelligence() << endl;
		cout << "Opponent's Constitution: " << opponent->getConstitution() << endl;
		cout << "Opponent's Defense: " << opponent->getDefence() << endl;
		int heroLife = hero->getLife();
		int monsterLife = opponent->getLife();
		int turn = 0;
		bool runaway = false;
		char command = NULL;
		srand((unsigned)time(0));
		do {
			cout << endl << "Turn " << (turn+1) << endl;
			int skillsHero = hero->getStrength() + hero->getAgility() + hero->getIntelligence();
			int skillsOpponent = opponent->getStrength() + opponent->getAgility() + opponent->getIntelligence();
			int total = skillsHero + skillsOpponent;
			int attacker = (rand() % total);
			//cout << attacker << endl;
			if (attacker < skillsHero) {
				cout << "You attack" << endl;
				int dice1 = rand() % 6 + 1;
				int dice2 = rand() % 6 + 1;
				int attackValue = hero->getAttackValue(dice1, dice2);
				cout << "Rolled values: " << dice1 << " and " << dice2 << endl;
				cout << "Your attack value: " << attackValue << endl;
				if (attackValue > opponent->getDefence()) {
					cout << "Your attack was successful." << endl;
					monsterLife -= hero->getDamage() * (rand() % 10);
					cout << opponent->getName() << "'s remaining Life Points: " << monsterLife << endl;
				}
				else {
					cout << "Your opponent blocked your attack!" << endl;
					monsterLife -= (hero->getDamage() * (rand() % 10)) / 2;
					cout << opponent->getName() << "'s remaining Life Points: " << monsterLife << endl;
				}
			}
			else {
				cout << opponent->getName() + " attacks." << endl;
				int dice1 = rand() % 6 + 1;
				int dice2 = rand() % 6 + 1;
				int attackValue = opponent->getAttackValue(dice1, dice2);
				cout << "Rolled values:" << dice1 << " and " << dice2 << endl;
				cout << opponent->getName() + "'s attack value: " << attackValue <<endl;
				if (attackValue > hero->getDefence()) {
					cout << opponent->getName() + "'s attack was successful." <<endl;
					heroLife -= (opponent->getDamage() * (rand() % 10));
					cout << "Your remaining Life Points: " << heroLife << endl;
				}
				else {
					cout << "You blocked " << opponent->getName() + "'s!" << endl;
					heroLife -= (opponent->getDamage() * (rand() % 10)) / 2;
					cout << "Your remaining Life Points: " << heroLife << endl;
				}
			}
			cout << "Your life points: " << heroLife << endl;
			cout << opponent->getName() << " life points: " << monsterLife << endl;
			cout << endl;

			if (monsterLife <= 0) {
				cout << "You defeated " << opponent->getName() << "!" << endl;
				return true;
			}
			else if (heroLife <= 0) {
				cout << "Too weak, too slow! You were beaten!" << endl;
				return false;
			}
			turn++;
			if (turn < 2) {
				cout << "Press any key to continue. (Or type 'r' to run away)" << endl;
				cin >> command;
				switch (command) {
				case 'r':
					cout << "You ran away. You coward!" << endl;
					runaway = true;
					return false;
				default:
					continue;
				}
			} 
		} while (heroLife > 0 && monsterLife > 0 && !runaway);
		cout << endl;
	}
	
	//Splits a sentence into vector of strings
	vector<std::string> split(const string& s, char delimiter)
	{
		vector<string> tokens;
		string token;
		istringstream tokenStream(s);
		while (getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	
	//Adds the monsters from the text file into a stack
	stack<Monster*> createStackOfMonsters() {
		stack<Monster*> dungeon;
		string line = "";
		ifstream dungeons;
		dungeons.open("dungeons.txt");
		if (dungeons.fail()) {
			cout << "Failed to open file" << endl;
		}
		int count = 1;

		// Takes the input from the text file, assigns it, and creates an object for each Monster
		while (getline(dungeons, line, '\n'))  
		{
			vector<string> stats = split(line, '-');
			string name = stats[0];
			int level = stoi(stats[1]);
			int strength = stoi(stats[2]);
			int agility = stoi(stats[3]);
			int intelligence = stoi(stats[4]);
			int constitution = stoi(stats[5]);
			int defence = stoi(stats[6]);
			if (count % 3 == 1) {
				dungeon.push(new CombatMonster(name, level, strength, agility, intelligence, constitution, defence));
			}
			else if (count % 3 == 2) {
				dungeon.push(new AgileMonster(name, level, strength, agility, intelligence, constitution, defence));
			}
			else {
				dungeon.push(new MagicalMonster(name, level, strength, agility, intelligence, constitution, defence));
			}

			count++;
			
		}
		return dungeon;
	}

	// Adds heroes from a text file into an unordered map
	unordered_map<string, Hero*> HallOfFame() {
		unordered_map<string, Hero*> heroes; //This are predefined heroes with which the user can only have
		string line = "";                    //practise battles with them
		ifstream hero;
		hero.open("heroes.txt");
		if (hero.fail()) {
			cout << "Failed to open file" << endl;
		}
		int count = 1;

		while (getline(hero, line, '\n'))  
		{
			vector<string> stats = split(line, '-');
			string name = stats[0];
			int level = stoi(stats[1]);
			int strength = stoi(stats[2]);
			int agility = stoi(stats[3]);
			int intelligence = stoi(stats[4]);
			int constitution = stoi(stats[5]);
			int defence = stoi(stats[6]);
			if (count % 3 == 1) {
				heroes.insert(pair<string, Hero*>(name, new Warrior(name, level, strength, agility, intelligence, constitution, defence)));
			}
			else if (count % 3 == 2) {
				heroes.insert(pair<string, Hero*>(name, new Archer(name, level, strength, agility, intelligence, constitution, defence)));

			}
			else {
				heroes.insert(pair<string, Hero*>(name, new Wizard(name, level, strength, agility, intelligence, constitution, defence)));
			}

			count++;

		}
		return heroes;
	}

	//Asks the user to write a name and to choose their fighting class
	Hero* intro() {
		cout << "With the disappearance of King Targon, the country Aris fell into darkness." << endl <<
			"For a quarter of a century the cities in the kingdom where conquered by beasts and monsters that keep terrorizing \nthe innocent people." << endl <<
		    "More than ever Aris needs brave heroes that will kill the monsters, freed the people and return the light into Aris." << endl;
		string name;
		string battleClass;
		Hero* you = NULL;
		cout << endl;
		cout << "Welcome to the World of Aris! How are you called? " << endl;
		cin >> name;
		do {
			cout << "Now choose your battle class: (Warrior, Archer, Wizard)" << endl;
			cin >> battleClass;

			if (battleClass == "Warrior" || battleClass == "warrior") {
				cout << "Good choice " << name << "! Warriors' main attribute is Strength!" << endl;
				you = new Warrior(name);
			}
			else if (battleClass == "Wizard" || battleClass == "wizard") {
				cout << "Good choice " << name << "! Wizards' main attribute is Intelligence!" << endl;
				you = new Wizard(name);
			}
			else if (battleClass == "Archer" || battleClass == "archer") {
				cout << "Good choice " << name << "! Archers' main attribute is Agility!" << endl;
				you = new Archer(name);
			}
			else {
				cout << "Invalid battle class!" << endl;
			}
		} while (battleClass != "Warrior" && battleClass != "Archer" && battleClass != "Wizard"
			&& battleClass != "warrior" && battleClass != "archer" && battleClass != "wizard");
			return you;
	}

	// When all monsters in the dungeon are defeated, the user has beaten the game and his hero is added to the heroes text file
	bool Win(Hero* hero, stack<Monster*> &dungeon, unordered_map<string, Hero*> heroes) {
		if (dungeon.empty()) {
						cout << "Congratulations!, you defeated all Monsters and freed the land of Aris!" << endl;
						cout << "The Arisian people are building a statue in your honour and they will remember you forever" << endl;
						cout << "Your name will be added to the Hall of Fame of the best Heroes of Aris!" << endl;
						ofstream hallOfFame;
						hallOfFame.open("heroes.txt");
						if (hallOfFame.is_open()) {
							hallOfFame << hero->getName() << "-";
							hallOfFame << hero->getLevel() << "-";
							hallOfFame << hero->getStrength() << "-";
							hallOfFame << hero->getAgility() << "-";
							hallOfFame << hero->getIntelligence() << "-";
							hallOfFame << hero->getConstitution() << "-";
							hallOfFame << hero->getIntelligence() << "-";
							}
						return true;

					}
		return false;
	}

	// Shows the information and gets input from the user
	void game(Hero* hero, stack<Monster*> &dungeon, unordered_map<string, Hero*> heroes) {
		hero->levelUp();
		char userChoice = NULL;
		cout << endl;
		cout << "Your stats:" << endl;
		cout << "Name: " << hero->getName() << endl;
		cout << "Level: " << hero->getLevel() << endl;
		cout << "Gold: " << hero->getGold() << endl;
		cout << "Strength: " << hero->getStrength() << endl;
		cout << "Agility: " << hero->getAgility() << endl;
		cout << "Intelligence: " << hero->getIntelligence() << endl;
		cout << "Constitution: " << hero->getConstitution() << endl;
		cout << "Defence: " << hero->getDefence() << endl;
		cout << endl;
		cout << endl;
		cout << "Time: " << hero->getTime() << endl;
		cout << "Actions (type the number)" << endl;
		cout << "1. Increase stats" << endl;
		cout << "2. Work" << endl;
		cout << "3. Go to adventure" << endl;
		cout << "4. Fight in the dungeon" << endl;
		cout << "5. Arena fight" << endl;

		cin >> userChoice;
		system("CLS"); //clear console
		switch (userChoice) {
		case '1':
			cout << "You are in the training camp now. Follow the instructions:" << endl;
			while (userChoice != 'e') {
				cout << "type 's' for strength, 'i' for intelligence, 'a' for agility, 'c' for constitution and 'd' for defence or 'e' to exit" << endl;
				do {
					cin >> userChoice;
					if (userChoice == 's' || userChoice == 'a' || userChoice == 'c' || userChoice == 'd' || userChoice == 'i') {
						hero->increaseStats(userChoice);
					}
					else {
						if (userChoice == 'e') {
							system("CLS"); //clear console
							break;
						}
						cout << "You entered invalid stat" << endl;
					}
				} while (userChoice != 's' && userChoice != 'a' && userChoice != 'c' && userChoice != 'd' && userChoice != 'i');
				cout << endl;
			}
			break;
		case '2':
			int hours;
			cout << "How many hours do you want to work? (1-10)" << endl;
			cin >> hours;
			hero->work(hours); //There should be some time to pass to receive the money. In this version the user receives them right away
			cout << endl;
			break;

		case '3':
			if (hero->adventureTime()) { // The hero has adventures left > 0
				cout << "Hey, psst young hero! I have some missions for you..." << endl;
				cout << "Choose difficulty: 1 for easy, 2 for medium, 3 for hard: " << endl;
				cin >> userChoice;
				if (userChoice == '1') {
					int reward = (hero->getLevel() * 10) + rand() % 20;
					int experience = (hero->getLevel() * 2) + rand() % 10;
					cout << "You have successfully completed your adventure!" << endl;
					cout << "Reward: " << reward << " Gold!" << endl;
					cout << "Experience gained: " << experience << endl;
					hero->increaseGold(reward);
					hero->increaseEXP(experience);
				}
				else if (userChoice == '2') {
					int percent = rand() % 10; //Again it is about luck, not skill, Probably in the future I will add fights against monsters
					if (percent > 5) {
						int reward = (hero->getLevel() * 15) + rand() % 50;
						int experience = (hero->getLevel() * 4) + rand() % 10 + 5;
						cout << "You have successfully completed your adventure!" << endl;
						cout << "Reward: " << reward << " Gold!" << endl;
						cout << "Experience gained: " << experience << endl;
						hero->increaseGold(reward);
						hero->increaseEXP(experience);

					}
					else {
						cout << "Unfortunately, you couldn't finish your adventure." << endl;
					}
				}
				else if (userChoice == '3') {
					int percent = rand() % 10;
					if (percent > 8) {
						int reward = (hero->getLevel() * 30) + rand() % 100;
						int experience = (hero->getLevel() * 6) + rand() % 10 + 10;
						cout << "You have successfully completed your adventure!" << endl;
						cout << "Reward: " << reward << " Gold!" << endl;
						cout << "Experience gained: " << experience << endl;
						hero->increaseGold(reward);
						hero->increaseEXP(experience);
					}
					else {
						cout << "Unfortunately, you couldn't finish your adventure." << endl;
					}
				}

				else {
					cout << "Invalid Input for an adventure!" << endl;
				}
			}
			cout << "Adventures left: " << hero->getAdventuresLeft() << endl;
			break;

		case '4':
			if (hero->attackDungeon()) { // true if the dungeon hasn't been attacked today
				cout << "You see a shadow approaching you. As it comes closer you realize it is a " << dungeon.top()->getName() << endl;
				cout << "Shakingly, you shout and run towards the monster, ready for a deadly battle!" << endl;
				if (Fighting(hero, dungeon.top())) {  //Attacking the Dungeon or top Monster of the Stack
					int reward = 500 * dungeon.top()->getLevel(); // If true, the user receives gold and the Monster
					int experience = 15 * dungeon.top()->getLevel(); //is popped out of the stack
					cout << "You win " << reward << " Gold!" << endl;
					cout << "Experience gained: " << experience << endl;
					hero->increaseGold(reward);
					hero->increaseEXP(experience);
					dungeon.pop();
				}
			}
			else {
				cout << "You have attacked the dungeon today. Return tomorrow.";
			}
			break;
		case '5':
			cout << "Anytime is a good time for a healthy fight!" << endl;
			hero->attackHero();
			string userInput = "";
			unordered_map<string, Hero*>::iterator it; //Displays the Available heroes, the user can battle just for practise
			cout << "NAME\tLEVEL\tBattle Class" << endl;
			for (it = heroes.begin(); it != heroes.end(); it++)
			{
				cout << it->first;
				cout << '\t' << it->second->getLevel() << '\t' << it->second->getClass() 
					<< endl;
			}
			cout << "Type the name of the hero you want to challenge: ";
			cin >> userInput;
			it = heroes.find(userInput); // Try to find the hero, with whom the user wants to challenge
			if (!(it == heroes.end()))
			{
				//hero found;
				system("CLS");
				cout << it->first << ":Hey bro, I heard you wanna fight! Let me see your skills" << endl;
				Fighting(hero, heroes[userInput]);
			}
			else {
				cout << "Such hero does not exist!" << endl;
			}
			break;
		}
	}

	
	int main()
	{
		stack<Monster*> dungeon = createStackOfMonsters();
		unordered_map<string, Hero*> heroes = HallOfFame();
		Hero* you = intro();
		do  {
			game(you, dungeon, heroes);
		} while ((!Win(you, dungeon, heroes)));
		


	}



