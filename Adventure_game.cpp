
// final.cpp
// 7/31/2020
// Nishant Balaji

#include <iostream>		// Any other libraries, too
#include <stdio.h>
#include <time.h>
#include <iomanip>
using namespace std;

// Struct to track location of player in each room
struct Position{
	int x;
	int y;
} location, previous;

// Weapons and their stats
struct Weapons{
	string name;
	string description;
	int damage;
} weapon1, weapon2, weapon3, weapon4, weapon5, weapon6;

// Healing items and their stats
struct Healing{
	string name;
	string description;
	int health;
} healing1, healing2, healing3, healing4, healing5;

// Rooms are all unified under here
class Room {
	string room_name;
	string room_description;
	string room_id;
	bool occupied;
	bool exit_n;
	bool exit_s;
	bool exit_e;
	bool exit_w;
	int arr[5][5] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	char empty[5][5] = {};
	public: Room(string name, string description, string id, bool north, bool south, bool east, bool west){
		room_name = name;
		room_description = description;
		room_id = id;
		exit_n = north;
		exit_s = south;
		exit_e = east;
		exit_w = west;
	}

	void getName(){
		cout << room_name;
	}

	void changeLocation(struct Position location, struct Position previous){

		// Going from a trap to another trap.
		if(arr[location.y][location.x] == 3 && arr[previous.y][previous.x] == 4){
			arr[location.y][location.x] = 4;
			arr[previous.y][previous.x] = 3;
			empty[location.y][location.x] = '0';
			empty[previous.y][previous.x] = 'U';
		}

		// Going from an empty space to a trap.
		else if(arr[location.y][location.x] == 3){
			arr[location.y][location.x] = 4;
			arr[previous.y][previous.x] = 0;
			empty[location.y][location.x] = '0';
			empty[previous.y][previous.x] = ' ';
		}

		// Going from a trap to an empty space.
		else if(arr[previous.y][previous.x] == 4){
			arr[location.y][location.x] = 1;
			arr[previous.y][previous.x] = 3;
			empty[location.y][location.x] = 'O';
			empty[previous.y][previous.x] = 'U';
		}

		// Normal movement from an empty space to another empty space.
		else{
			arr[location.y][location.x] = 1;
			arr[previous.y][previous.x] = 0;
			empty[location.y][location.x] = 'O';
			empty[previous.y][previous.x] = ' ';

		}
		
	}

	// Set the current arr to the given input, mainly used to track the death of monsters and update them in the room.
	void changeArray(int arrin[][5]){
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				arr[i][j] = arrin[i][j];
			}
		}
	}

	void spawnMonster(int num){
		
		// Skeletons
		if(num == 0){
			arr[1][1] = 2;
			arr[1][3] = 2;
			arr[2][2] = 2;
			arr[3][1] = 2;
			arr[3][3] = 2;
		}

		// Minions
		else if(num == 1){
			arr[1][2] = 2;
			arr[3][1] = 2;
			arr[3][3] = 2;
 		}

 		// Troll Imps
 		else if(num == 2){
 			arr[2][1] = 2;
 			arr[2][3] = 2;
  		}


  		// Troll Warriors
  		else if(num == 3){
  			arr[2][2] = 2;
  		}

  		// Ogre
  		else if(num == 4){
  			arr[2][2] = 2;
  			arr[2][3] = 2;
  			arr[3][2] = 2;
  			arr[3][3] = 2;
  		}

  		// Troll King
  		else if(num == 5){
  			arr[1][2] = 2;
  			arr[1][3] = 2;
			arr[2][2] = 2;
			arr[2][3] = 2;
			arr[3][2] = 2;
			arr[3][3] = 2;
  		}

  		// Demon
		else if(num == 6){
			arr[1][1] = 2;
			arr[1][2] = 2;
  			arr[1][3] = 2;
			arr[2][1] = 2;
			arr[2][2] = 2;
			arr[2][3] = 2;
			arr[3][1] = 2;
			arr[3][2] = 2;
			arr[3][3] = 2;
		}

		// Show the monsters in the room.
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				if(arr[i][j] == 2){
					empty[i][j] = 'X';
				}
			}
		}
	}

	// Specification A1 - Traps
	void spawnTrap(){
		// Spawn a random number of traps between 0 and 4
		int temp = rand() % 5;

		// Put the random number of traps at random locations, as long as 
		// they are not at the entrys or at the same spot as a monster.
		for(int i = 0; i < temp; i++){
			int x = rand() % 5;
			int y = rand() % 5;
			
			if((x == 0 && y == 2) || (x == 4 && y == 2) || (x == 2 && y == 0) || (x == 2 && y == 4)) {
				i--;
			}

			else if(arr[x][y] == 0){
				arr[x][y] = 3;
			}

			else{
				i--;
			}
		}

		// Initially hide the traps unless they have been shown already.
		for(int i = 0; i < 5; i++){
			for(int j = 0; j < 5; j++){
				if(arr[i][j] == 3 && (empty[i][j] != 'U' || empty[i][j] != '0')){
					empty[i][j] = ' ';
				}
			}
		}
	}

	void display(bool visited){
		system("clear");

		// Specification C4 – Abbreviated Room Description
		// Shows description if the room hasn't been visited and hides if the room has already been visited.
		if(!visited){
			cout << room_name << endl;
			cout << room_description << "\n\n" << endl;
		}
		else{
			cout << room_name << "\n\n\n" << endl;
		}

		// Change output based on placement of exits within the room.
		if(exit_n){
			cout << "—————  —————" << endl;
		}
		else{
			cout << "————————————" << endl;
		}

		for(int i = 0; i < 5; i ++){
			if(i == 2 && exit_w){
				cout << " ";
			}
			else{
				cout << "|";
			}
			// Shows the Player, Monsters and revealed traps in the room.
			for(int j = 0; j < 5; j++){
				if(arr[i][j] != 0){
					cout << empty[i][j] << " ";
				}
				else{
					cout << "  ";
				}
			}
			if(i == 2 && exit_e){
				cout << " " << endl;
			}
			else{
				cout << "|" << endl;
			}
		}
		if(exit_s){
			cout << "—————  —————" << endl;
		}
		else{
			cout << "————————————" << endl;
		}
	}

	bool northexit(){
		return exit_n;
	}

	bool southexit(){
		return exit_s;
	}

	bool eastexit(){
		return exit_e;
	}

	bool westexit(){
		return exit_w;
	}

	
	int getVal(int i, int j){
		return arr[i][j];
	}

};

// Specification A3 - Monster Class 
// The monsters are in a class of their own, with their own methods and variables. 
class Monster {
	string name;
	int hp;
	int damage;
	int health;
	public: Monster(string monster_name, int monster_hp, int monster_damage){
		name = monster_name;
		hp = monster_hp;
		damage = monster_damage;
		health = monster_hp;
	}

	int getDamage(){
		// Randomized Damage Multipliers for 'critical hits.'
		int dmg = rand() % 2 + 1;
		int temp = damage * dmg;
		return temp;
	}

	int getHealth(){
		return hp;
	}

	int getTotalhp(){
		return health;
	}


	void hurt(int amt){
		hp -= amt;
		if(hp < 0){
			hp = 0;
		}
	}

	string getName(){
		return name;
	}

	void resetHealth(){
		hp = health;
	}
};

class Player{
	string name;
	int hp;
	int damage;
	int health;
	public: Player(string player_name){
		name = player_name;
		hp = 100;
		damage = 10;
		health = 100;
	}

	string getName(){
		return name;
	}

	// Randomized Damage Multipliers for 'critical hits.'
	int getDamage(){
		int dmg = rand() % 4 + 1;
		int temp = damage * dmg;
		return temp;
	}

	int getTrueDamage(){
		return damage;
	}
	
	int getHealth(){
		return hp;
	}

	int getTotalhp(){
		return health;
	}

	void hurt(int amt){
		hp -= amt;
		if(hp < 0){
			hp = 0;
		}
	}

	// Adds to base damage of 10 instead of current damage to show only one 'equipped' weapon at a time.
	void increaseDamage(int amt){
		damage = 10;
		damage += amt;
	}

	void heal(){
		hp = health;
	}

	// Adds the amount to the base health of 100 instead of the current health to show only one 'equipped' healing item at a time.
	void increaseHealth(int amt){
		health = 100;
		health += amt;
		hp = health;
	}
};

// Global Variables

// Function Prototypes
void ProgramGreeting();	
void ProgramMenu();
int validInput(int, int);

int main() {
	system("clear");
	ProgramGreeting();	
	string name;
	// Specification C2 - Player Name
	cout << "Enter your name: ";
	cin >> name;

	Player player(name);

	srand(time(NULL));
	
	system("clear");
	
	// Specification C1 - Five Rooms
	// Specification B2 - More Rooms
	// I have a total of 8 rooms and they are all positioned differently in relation to one another.
	Room room1("Start Room", "This is the entrance to this dungeon, nobody has been inside in a hundred years.", "101", false, false, true, false);
	Room room2("Nexus Room", "This room seems to split into two diffent paths.", "102", true, false, true, true);
	Room room3("Lock Room", "At the end of this room lies a door with a huge lock. Where is the key?", "103", false, false, true, true);
	Room room4("Hall Room", "This room continues straigh ahead to the room above", "104", true, true, false, false);
	Room room5("Guarded Room", "This room has two giant statues of soldiers, looking like they're guarding something.", "104", false, true, true, false);
	Room room6("Key Room", "In this room is a giant key in a trophy chest, but it doesn't look like it fits the lock. Maybe there is another one?", "105", false, false, false, true);
	Room room7("Forbidden Room", "It doeesn't look like anyone is supposed to be here...", "106", false, true, false, true);
	Room room8("Exit Room", "There's the exit! Finally a way out of this dungeon!", "107", true, true, false, false);

	// Specification B1 - More Monsters
	// I added 7 different monsters, and the first five are randomly choosen from for the different rooms. 
	// The remaining two are shown only in the last two rooms, as they are 'bosses.'
	Monster monster1("Skeletons", 50, 5);
	Monster monster2("Minions", 50, 8);
	Monster monster3("Troll Imps", 75, 10);
	Monster monster4("Troll Warrior",100, 13);
	Monster monster5("Ogre", 125, 15);
	Monster monster6("Troll King", 150, 17);
	Monster monster7("Demon", 200, 20);
	
	bool cont = true;
	while(cont){
		
		// Specification B3 - Array of Objects
		// Here, I have two arrays: one is an array of Room objects, and the other is of Monster objects. 
		Room rooms[8] = {room1, room2, room3, room4, room5, room6, room7, room8};
		bool visited[8] = {false, false, false, false, false, false, false, false};
		int monsternum[8] = {};
		bool spawned[8] = {false, false, false, false, false, false, false, false};
		bool slain[8] = {true, false, false, false, false, false, false, false};
		bool trapspawned[8] = {false, false, false, false, false, false, false, false};
		bool key = false;
	
		Monster monsters[7] = {monster1, monster2, monster3, monster4, monster5, monster6, monster7};

		// Weapon Stats
		weapon1.name = "Rusty Sword";
		weapon1.description = "An old, rusty sword. 10 damage";
		weapon1.damage = 0;

		weapon2.name = "Dagger";
		weapon2.description = "15 damage";
		weapon2.damage = 5;

		weapon3.name = "Longsword";
		weapon3.description = "A lost longsword left by a knight. 20 damage";
		weapon3.damage = 10;

		weapon4.name = "Mace";
		weapon4.description = "A heavy mace. 25 damage";
		weapon4.damage = 15;

		weapon5.name = "Spear";
		weapon5.description = "A long spear left by an explorer. 30 damage";
		weapon5.damage = 20;

		weapon6.name = "King's Sword";
		weapon6.description = "A beautiful sword fit for a king. 35 damage.";
		weapon6.damage = 25;

		// Specification A4 - Array of Structs
		Weapons weaponrandom[6] = {weapon1, weapon2, weapon3, weapon4, weapon5, weapon6};
		int randweapon;
		// Records the weapons dropped by the monsters.
		string inventoryweaponsname[7] = {weapon1.name};
		string inventoryweaponsdesc[7] = {weapon1.description};
		// Tracks the first empty weapon index location
		int weaponloc = 1;
		// Tracks current weapon's damage
		int weapondmg = weapon1.damage;
		// Tracks the weapon that has the most damage, and thus is 'equipped.'
		int currentwep = 0;

		// Healing Stats
		healing1.name = "Charm of Good Health";
		healing1.description = "An old charm said to bring good health. +10 base health";
		healing1.health = 10;

		healing2.name = "Healing Amulet";
		healing2.description = "An amulet sent from the capitol mages. +20 base health";
		healing2.health = 20;

		healing3.name = "Elixir of Healing";
		healing3.description = "Elixir drawn from a magic spring. +30 base health.";
		healing3.health = 30;

		healing4.name = "Scroll of the Ancient Healers";
		healing4.description = "An ancient scroll with a healing spell. +40 base health.";
		healing4.health = 40;

		healing5.name = "Angel Tear";
		healing5.description = "A tear of an Angel. +50 base health";
		healing5.health = 50;

		Healing healingrandom[5] = {healing1, healing2, healing3, healing4, healing5};
		int randhealing;
		// Records the healing items dropped by the monsters.
		string inventoryhealingname[7] = {};
		string inventoryhealingdesc[7] = {};
		// Tracks the first empty healing item index location 
		int healingloc = 0;
		// Tracks current healing item's health
		int healinghth = 0;
		// Tracks the healing item that has the most health, and thus is 'equipped.'
		int currenthealth = 0;
		
		

		
		int randmon;
			
		bool invalid = true;
		int in = 0;
		char numin;
		
		// Specification C3 - Input Validation
		// Program Menu 
		while(invalid){
			system("clear");
			ProgramMenu();
			cin >> numin;
			in = numin - 48;
			
			// Play the game
			if(in == 1){
				invalid = false;
				continue;
			}

			// Help Menu
			else if(in == 2){
				cout << "                       Controls and Directions                    " << endl;
				cout << "==================================================================" << endl;
				cout << "W - move up" << endl;
				cout << "S - move down" << endl;
				cout << "A - move left" << endl;
				cout << "D - move right" << endl;
				cout << "L - room information" << endl;
				cout << "I - inventory" << endl;
				cout << "H - help\n" << endl;
				cout << "All commands must be followed by clicking the 'Enter' key.\n" << endl;
				cout << "You are represented by 'O', traps by 'U' and monsters are 'X'." << endl;
				cout << "Monsters can take up more than one space. Traps are invisible " << endl;
				cout << "first, but will be revealed when walked over, and they will deal " << endl;
				cout << "10 damage everytime you walk over them. Traps will turn into an " << endl;
				cout << "'0' when you are standing on them. When you enter a room, you  " << endl;
				cout << "will have to choose to either attack the monsters inside or run. " << endl;
				cout << "To attack after you choose to run, you must leave the room and " << endl;
				cout << "come back. Choose wisely! Kill the monsters and get to the end " << endl;
				cout << "of the dungeon to win! You also have a chance to do extra damage " << endl;
				cout << "with every attack. After killing a monster, you will gain a " << endl;
				cout << "healing item that will increase your health, and heal all lost " << endl;
				cout << "health. Only one weapon and healing item may be equipped at a " << endl;
				cout << "time, and this will be automatically done. You must kill all " << endl;
				cout << "the monsters to clear the dungeon! Make sure you are prepared!\n" << endl;
				cout << "==================================================================" << endl;
				cout << "Press 1 to return to the menu" << endl;
				while(invalid){
					cin >> numin;
					in = numin - 48;	
					if(in == 1){
						invalid = false;
						break;
					}
					else{
						cout << "Invalid input." << endl;
					}
				}
				invalid = true;
			}

			// Quit Game
			else if(in == 3){
				cont = false;
				return 0;
			}

			// Input validation
			else{
				cout << "Invalid input." << endl;
				in = 0;
			}
		}
	 	
	 	invalid = true;
	 	
	 	location.x = 0;
	 	location.y = 2;
		previous.x = 0;
		int roomNum = 0;
		char input;
		room1.changeLocation(location, previous);
		room1.display(visited[roomNum]);
	 	

		// Input and main gameplay
		while(invalid){
			

			cin >> input;
			input = toupper(input);
			
			int temp[5][5] = {};

			for(int i = 0; i < 5; i++){
				for(int j = 0; j < 5; j++){
					temp[i][j] = rooms[roomNum].getVal(i, j);
				}
			}

			bool dead = true;
			for(int i = 0; i < 8; i++){
				if(!slain[i]){
					dead = false;
					break;
				}
			}	

			// Move up
			if(input == 'W' && location.y != 0){
				if(temp[location.y-1][location.x] != 2){

					previous.y = location.y;
					previous.x = location.x;
					location.y -= 1;
					rooms[roomNum].changeLocation(location, previous);
					rooms[roomNum].display(visited[roomNum]);
					// Checks if you  are on a trap or walking into a trap
					if(temp[location.y][location.x] == 3 || temp[location.y][location.x] == 4){
						// Deals damage if you walk into a trap
						player.hurt(10);
						cout << "You have walked into a trap! You have taken 10 damage." << endl;
						cout << "Health" << endl;
						cout << "======" << endl;
						cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
					}
				}
			}

			// Move down
			else if(input == 'S' && location.y != 4){
				if(temp[location.y+1][location.x] != 2){
					previous.y = location.y;
					previous.x = location.x;
					location.y += 1;
					rooms[roomNum].changeLocation(location, previous);
					rooms[roomNum].display(visited[roomNum]);
					if(temp[location.y][location.x] == 3 || temp[location.y][location.x] == 4){
						player.hurt(10);
						cout << "You have walked into a trap! You have taken 10 damage." << endl;
						cout << "Health" << endl;
						cout << "======" << endl;
						cout << player.getName() << " :" << player.getHealth() << "/" << player.getTotalhp() << endl;	
					}
				}
			}

			// Move left
			else if(input == 'A' && location.x != 0){
				if(temp[location.y][location.x-1] != 2){
					previous.x = location.x;
					previous.y = location.y;
					location.x -= 1;
					rooms[roomNum].changeLocation(location, previous);
					rooms[roomNum].display(visited[roomNum]);
					if(temp[location.y][location.x] == 3 || temp[location.y][location.x] == 4){
						player.hurt(10);
						cout << "You have walked into a trap! You have taken 10 damage." << endl;
						cout << "Health" << endl;
						cout << "======" << endl;
						cout << player.getName() << " :" << player.getHealth() << "/" << player.getTotalhp() << endl;
					}
				}
			}

			// Move right
			else if(input == 'D' && location.x != 4){
				if(temp[location.y][location.x+1] != 2){
					previous.x = location.x;
					previous.y = location.y;
					location.x += 1;
					rooms[roomNum].changeLocation(location, previous);
					rooms[roomNum].display(visited[roomNum]);
					if(temp[location.y][location.x] == 3 || temp[location.y][location.x] == 4){
						player.hurt(10);
						cout << "You have walked into a trap! You have taken 10 damage." << endl;
						cout << "Health" << endl;
						cout << "======" << endl;
						cout << player.getName() << " :" << player.getHealth() << "/" << player.getTotalhp() << endl;
					}
				}
			} 

			// Moving to a room above and engaging combat
			else if(input == 'W' && location.x == 2 && location.y == 0 && rooms[roomNum].northexit()){
				// Specific switching because of room layout
				if(roomNum == 1){
					roomNum = 3;
				}
				else if(roomNum == 7){
					roomNum = 6;
				}
				else{
					roomNum++;
				}
				// Change into the new room
				location.x = 2;
				location.y = 4;
				previous.x = 0;
				previous.y = 0;
				rooms[roomNum].changeLocation(location, previous);
				previous.x = 2;
				previous.y = 4;

				// Check if a monster is already spawned in the room, and if not, spawn one.
				if(!spawned[roomNum] && roomNum != 6 && roomNum!= 7){
					randmon = rand() % 5;
					monsternum[roomNum] = randmon;
					rooms[roomNum].spawnMonster(randmon);
					spawned[roomNum] = true;
				}

				// Check if traps have already been spawned in the room, and if not, spawn some.
				if(!trapspawned[roomNum] && roomNum != 0){
					rooms[roomNum].spawnTrap();
					trapspawned[roomNum] = true;
				}

				// Display the room
				rooms[roomNum].display(visited[roomNum]);
				visited[roomNum] = true;

				// Specification A2 - Combat
				// COMBAT
				bool fight = true;
				int choice = 0;
				
				// Check if the monsters in the room are already dead, and if not, engage combat with them
				if(!slain[roomNum] && fight){
					cout << monsters[monsternum[roomNum]].getName() << " appeared! Do you:" << endl;
					cout << "1. Fight" << endl;
					cout << "2. Run" << endl;
					cout << "Health" << endl;
					cout << "======" << endl;
					cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
					cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
					choice  = validInput(1,2);
					
					// Fight
					if(choice == 1){

						while(fight && player.getHealth() != 0 && monsters[monsternum[roomNum]].getHealth() != 0){
							system("clear");
							rooms[roomNum].display(visited[roomNum]);
							cout << "Health" << endl;
							cout << "======" << endl;
							cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
							cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
							cout << "\nPress 1 to attack the monster. Press 2 to run!" << endl;
							choice = validInput(1,2);
							

							// Fight
							if(choice == 1){
								system("clear");
								
								int playertemp = player.getDamage();
								int monstertemp = monsters[monsternum[roomNum]].getDamage();
								
								monsters[monsternum[roomNum]].hurt(playertemp);
								player.hurt(monstertemp);
								
								rooms[roomNum].display(visited[roomNum]);
								
								cout << "You attacked the monster for " << playertemp << " damage!" << endl;
								cout << "The monster attacked back and dealt " << monstertemp << " damage!" << endl;
								
								cout << "Health" << endl;
								cout << "======" << endl;
								cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
								cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
								
								cout << "\n Press 1 to continue" << endl;
								
								choice = validInput(1,1);
								
							}

							// Run
							else if(choice == 2){
								monsters[monsternum[roomNum]].resetHealth();
								fight = false;
							}
						}
						
					}

					// Checks if player is dead
					if(player.getHealth() == 0){
						cout << "You died!" << endl;
						invalid = false;
					}

					// Checks if monster is dead
					else if(monsters[monsternum[roomNum]].getHealth() == 0){
						system("clear");

						// Gets the current array from the current room
						for(int i = 0; i < 5; i++){
							for(int j = 0; j < 5; j++){
								temp[i][j] = rooms[roomNum].getVal(i, j);
							}
						}

						// Sets all values that were occupied by the monster to 0, signifying its death.
						for(int a = 0; a < 5; a++){
							for(int b = 0; b < 5; b++){
								if(temp[a][b] == 2){
									temp[a][b] = 0;
								}
							}
						}

						// Changes the Room object's array 
						rooms[roomNum].changeArray(temp);	

						rooms[roomNum].display(visited[roomNum]);

						cout << "You have slain the " << monsters[monsternum[roomNum]].getName() << "!\n" << endl;
						
						// Random Item Drops
						// Randomly selects a weapon and healing item
						randweapon = rand() % 4 + 1;
						inventoryweaponsname[weaponloc] = weaponrandom[randweapon].name;
						inventoryweaponsdesc[weaponloc] = weaponrandom[randweapon].description;

						randhealing = rand() % 4;
						inventoryhealingname[healingloc] = healingrandom[randhealing].name;
						inventoryhealingdesc[healingloc] = healingrandom[randhealing].description;
						
						// Notifies Player what was dropped by the monster
						cout << "The " << monsters[monsternum[roomNum]].getName() << " dropped: " << endl;
						cout << weaponrandom[randweapon].name << endl;
						cout << healingrandom[randhealing].name << endl;
						
						// Room 5 is Key Room
						if(roomNum == 5){
							key = true;
							cout << "Key to the Forbidden Chamber" << endl;
						}

						cout << "\n\n" << endl;

						// Checks if the dropped weapon is better than the current weapon, and selects the better weapon. 
						if(weaponrandom[randweapon].damage > weapondmg){
							player.increaseDamage(weaponrandom[randweapon].damage);
							currentwep = weaponloc;
							weapondmg = weaponrandom[randweapon].damage;
						}

						// Checks if the dropped healing item is better than the current healing item, and selects the better healing item.
						if(healingrandom[randhealing].health > healinghth){
							player.increaseHealth(healingrandom[randhealing].health);
							currenthealth = healingloc;
							healinghth = healingrandom[randhealing].health;
						}

						// Heals player back to full health in case damage wasn't reset by a better healing item.
						player.heal();
						// Resets monster's health in case the monster spawns in another room. 
						monsters[monsternum[roomNum]].resetHealth();

						// Increments to the next open location of the weapon and healing inventory arrays
						weaponloc++;
						healingloc++;

						slain[roomNum] = true;

						cout << "You may now move. \n" << endl;
					}
				}

				// END COMBAT
			}				
			
			// Moving to a room below and engaging combat
			else if(input == 'S' && location.x == 2 && location.y == 4 && rooms[roomNum].southexit()){
				
				// Used to check if the entire dungeon is cleared before ending game.
				int valid = true;
				if(valid){
					if(roomNum == 3){
						roomNum = 1;
					}
					else if(roomNum == 6){
						roomNum = 7;
					}
					// Checks if dungeon is all clear
					else if(roomNum == 7 && dead){
						cout << "You win!" << endl;
						break;
					}
					// Doesnt allow you to end if dungeon still has monsters.
					else if(roomNum == 7){
						cout << "You must slay all the monsters in the dungeon first!" << endl;
						valid = false;
						continue;
					}
					else{
						roomNum--;
					}
					location.x = 2;
					location.y = 0;
					previous.x = 0;
					previous.y = 0;
					rooms[roomNum].changeLocation(location, previous);
					previous.x = 2;
					previous.y = 0;

					if(!spawned[roomNum] && roomNum != 6 && roomNum!= 7){
						randmon = rand() % 5;
						monsternum[roomNum] = randmon;
						rooms[roomNum].spawnMonster(randmon);
						spawned[roomNum] = true;
					}
					else if(!spawned[roomNum] && roomNum == 7){
						monsternum[roomNum] = 6;
						rooms[roomNum].spawnMonster(6);
						spawned[roomNum] = true;
					}

					if(!trapspawned[roomNum] && roomNum != 0){
						rooms[roomNum].spawnTrap();
						trapspawned[roomNum] = true;
					}

					rooms[roomNum].display(visited[roomNum]);
					visited[roomNum] = true;

					// COMBAT
					bool fight = true;
					int choice = 0;
					
					if(!slain[roomNum] && fight){
						cout << monsters[monsternum[roomNum]].getName() << " appeared! Do you:" << endl;
						cout << "1. Fight" << endl;
						cout << "2. Run" << endl;
						cout << "Health" << endl;
						cout << "======" << endl;
						cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
						cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
						choice  = validInput(1,2);
						
						if(choice == 1){

							while(fight && player.getHealth() != 0 && monsters[monsternum[roomNum]].getHealth() != 0){
								system("clear");
								rooms[roomNum].display(visited[roomNum]);
								cout << "Health" << endl;
								cout << "======" << endl;
								cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
								cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
								cout << "\nPress 1 to attack the monster. Press 2 to run!" << endl;
								choice = validInput(1,2);
								
								if(choice == 1){
									system("clear");
									
									int playertemp = player.getDamage();
									int monstertemp = monsters[monsternum[roomNum]].getDamage();
									
									monsters[monsternum[roomNum]].hurt(playertemp);
									player.hurt(monstertemp);
									
									rooms[roomNum].display(visited[roomNum]);
									
									cout << "You attacked the monster for " << playertemp << " damage!" << endl;
									cout << "The monster attacked back and dealt " << monstertemp << " damage!" << endl;
									
									cout << "Health" << endl;
									cout << "======" << endl;
									cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
									cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
									
									cout << "\n Press 1 to continue" << endl;
									
									choice = validInput(1,1);
									
								}
								else if(choice == 2){
									monsters[monsternum[roomNum]].resetHealth();
									fight = false;
								}
							}
							
						}
						if(player.getHealth() == 0){
							cout << "You died!" << endl;
							invalid = false;
						}
						else if(monsters[monsternum[roomNum]].getHealth() == 0){
							system("clear");

							for(int i = 0; i < 5; i++){
								for(int j = 0; j < 5; j++){
									temp[i][j] = rooms[roomNum].getVal(i, j);
								}
							}

							for(int a = 0; a < 5; a++){
								for(int b = 0; b < 5; b++){
									if(temp[a][b] == 2){
										temp[a][b] = 0;
									}
								}
							}

							rooms[roomNum].changeArray(temp);	

							rooms[roomNum].display(visited[roomNum]);

							cout << "You have slain the " << monsters[monsternum[roomNum]].getName() << "!\n" << endl;
							
							// Random Item Drops
							randweapon = rand() % 4 + 1;
							inventoryweaponsname[weaponloc] = weaponrandom[randweapon].name;
							inventoryweaponsdesc[weaponloc] = weaponrandom[randweapon].description;

							randhealing = rand() % 4;
							inventoryhealingname[healingloc] = healingrandom[randhealing].name;
							inventoryhealingdesc[healingloc] = healingrandom[randhealing].description;
							
							cout << "The " << monsters[monsternum[roomNum]].getName() << " dropped: " << endl;
							cout << weaponrandom[randweapon].name << endl;
							cout << healingrandom[randhealing].name << endl;
							
							if(roomNum == 5){
								key = true;
								cout << "Key to the Forbidden Chamber" << endl;
							}

							cout << "\n\n" << endl;

							if(weaponrandom[randweapon].damage > weapondmg){
								player.increaseDamage(weaponrandom[randweapon].damage);
								currentwep = weaponloc;
								weapondmg = weaponrandom[randweapon].damage;
							}

							if(healingrandom[randhealing].health > healinghth){
								player.increaseHealth(healingrandom[randhealing].health);
								currenthealth = healingloc;
								healinghth = healingrandom[randhealing].health;
							} 

							player.heal();
							monsters[monsternum[roomNum]].resetHealth();

							weaponloc++;
							healingloc++;

							slain[roomNum] = true;

							cout << "You may now move. \n" << endl;
						}
					}

					// END COMBAT
				}

			}

			// Moving to a room to the right and engaging combat
			else if(input == 'D' && location.x == 4 && location.y == 2 && rooms[roomNum].eastexit()){
				if(roomNum == 2 && key){
					roomNum = 6;
				}
				else if(roomNum == 2 && !key){
					cout << "This room is locked. Find the key to proceed." << endl;
					continue;
				}
				else{
					roomNum++;
				}
				location.x = 0;
				location.y = 2;
				previous.x = 0;
				previous.y = 0;
				rooms[roomNum].changeLocation(location, previous);
				previous.x = 0;
				previous.y = 2;

				if(!spawned[roomNum] && roomNum != 6 && roomNum!= 7){
					randmon = rand() % 5;
					monsternum[roomNum] = randmon;
					rooms[roomNum].spawnMonster(randmon);
					spawned[roomNum] = true;
				}
				else if(!spawned[roomNum] && roomNum == 6){
					monsternum[roomNum] = 5;
					rooms[roomNum].spawnMonster(5);
					spawned[roomNum] = true;
				}

				if(!trapspawned[roomNum] && roomNum != 0){
					rooms[roomNum].spawnTrap();
					trapspawned[roomNum] = true;
				}

				rooms[roomNum].display(visited[roomNum]);
				visited[roomNum] = true;


				// COMBAT
				bool fight = true;
				int choice = 0;
				
				if(!slain[roomNum] && fight){
					cout << monsters[monsternum[roomNum]].getName() << " appeared! Do you:" << endl;
					cout << "1. Fight" << endl;
					cout << "2. Run" << endl;
					cout << "Health" << endl;
					cout << "======" << endl;
					cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
					cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
					choice  = validInput(1,2);
					
					if(choice == 1){

						while(fight && player.getHealth() != 0 && monsters[monsternum[roomNum]].getHealth() != 0){
							system("clear");
							rooms[roomNum].display(visited[roomNum]);
							cout << "Health" << endl;
							cout << "======" << endl;
							cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
							cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
							cout << "\nPress 1 to attack the monster. Press 2 to run!" << endl;
							choice = validInput(1,2);
							
							if(choice == 1){
								system("clear");
								
								int playertemp = player.getDamage();
								int monstertemp = monsters[monsternum[roomNum]].getDamage();
								
								monsters[monsternum[roomNum]].hurt(playertemp);
								player.hurt(monstertemp);
								
								rooms[roomNum].display(visited[roomNum]);
								
								cout << "You attacked the monster for " << playertemp << " damage!" << endl;
								cout << "The monster attacked back and dealt " << monstertemp << " damage!" << endl;
								
								cout << "Health" << endl;
								cout << "======" << endl;
								cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
								cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
								
								cout << "\n Press 1 to continue" << endl;
								
								choice = validInput(1,1);
								
							}
							else if(choice == 2){
								monsters[monsternum[roomNum]].resetHealth();
								fight = false;
							}
						}
						
					}
					if(player.getHealth() == 0){
						cout << "You died!" << endl;
						invalid = false;
					}
					else if(monsters[monsternum[roomNum]].getHealth() == 0){
						system("clear");

						for(int i = 0; i < 5; i++){
							for(int j = 0; j < 5; j++){
								temp[i][j] = rooms[roomNum].getVal(i, j);
							}
						}

						for(int a = 0; a < 5; a++){
							for(int b = 0; b < 5; b++){
								if(temp[a][b] == 2){
									temp[a][b] = 0;
								}
							}
						}

						rooms[roomNum].changeArray(temp);	

						rooms[roomNum].display(visited[roomNum]);
						

						cout << "You have slain the " << monsters[monsternum[roomNum]].getName() << "!\n" << endl;
						
						// Random Item Drops
						randweapon = rand() % 4 + 1;
						inventoryweaponsname[weaponloc] = weaponrandom[randweapon].name;
						inventoryweaponsdesc[weaponloc] = weaponrandom[randweapon].description;

						randhealing = rand() % 4;
						inventoryhealingname[healingloc] = healingrandom[randhealing].name;
						inventoryhealingdesc[healingloc] = healingrandom[randhealing].description;
						
						cout << "The " << monsters[monsternum[roomNum]].getName() << " dropped: " << endl;
						cout << weaponrandom[randweapon].name << endl;
						cout << healingrandom[randhealing].name << endl;

						if(roomNum == 5){
							key = true;
							cout << "Key to the Forbidden Chamber" << endl;
						}

						cout << "\n\n" << endl;

						if(weaponrandom[randweapon].damage > weapondmg){
							player.increaseDamage(weaponrandom[randweapon].damage);
							currentwep = weaponloc;
							weapondmg = weaponrandom[randweapon].damage;
						}

						if(healingrandom[randhealing].health > healinghth){
							player.increaseHealth(healingrandom[randhealing].health);
							currenthealth = healingloc;
							healinghth = healingrandom[randhealing].health;
						}

						player.heal();
						monsters[monsternum[roomNum]].resetHealth();

						weaponloc++;
						healingloc++;

						slain[roomNum] = true;

						cout << "You may now move. \n" << endl;
					}
				}

				// END COMBAT
			}

			// Moving to a room to the left and engaging combat
			else if(input == 'A' && location.x == 0 && location.y == 2 && rooms[roomNum].westexit()){
				if(roomNum == 6){
					roomNum = 2;
				}
				else{
					roomNum--;
				}
				location.x = 4;
				location.y = 2;
				previous.x = 0;
				previous.y = 0;
				rooms[roomNum].changeLocation(location, previous);
				previous.x = 4;
				previous.y = 2;

				if(!spawned[roomNum] && roomNum != 0 && roomNum != 6 && roomNum!= 7){
					randmon = rand() % 5;
					monsternum[roomNum] = randmon;
					rooms[roomNum].spawnMonster(randmon);
					spawned[roomNum] = true;
				}

				if(!trapspawned[roomNum] && roomNum != 0){
					rooms[roomNum].spawnTrap();
					trapspawned[roomNum] = true;
				}

				rooms[roomNum].display(visited[roomNum]);
				visited[roomNum] = true;


				// COMBAT
				bool fight = true;
				int choice = 0;
				
				if(!slain[roomNum] && fight){
					cout << monsters[monsternum[roomNum]].getName() << " appeared! Do you:" << endl;
					cout << "1. Fight" << endl;
					cout << "2. Run" << endl;
					cout << "Health" << endl;
					cout << "======" << endl;
					cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
					cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
					choice  = validInput(1,2);
					
					if(choice == 1){

						while(fight && player.getHealth() != 0 && monsters[monsternum[roomNum]].getHealth() != 0){
							system("clear");
							rooms[roomNum].display(visited[roomNum]);
							cout << "Health" << endl;
							cout << "======" << endl;
							cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
							cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
							cout << "\nPress 1 to attack the monster. Press 2 to run!" << endl;
							choice = validInput(1,2);
							
							if(choice == 1){
								system("clear");
								
								int playertemp = player.getDamage();
								int monstertemp = monsters[monsternum[roomNum]].getDamage();
								
								monsters[monsternum[roomNum]].hurt(playertemp);
								player.hurt(monstertemp);
								
								rooms[roomNum].display(visited[roomNum]);
								
								cout << "You attacked the monster for " << playertemp << " damage!" << endl;
								cout << "The monster attacked back and dealt " << monstertemp << " damage!" << endl;
								
								cout << "Health" << endl;
								cout << "======" << endl;
								cout << player.getName() << ": " << player.getHealth() << "/" << player.getTotalhp() << endl;
								cout << monsters[monsternum[roomNum]].getName() << ": " << monsters[monsternum[roomNum]].getHealth() << "/" << monsters[monsternum[roomNum]].getTotalhp() << endl;
								
								cout << "\n Press 1 to continue" << endl;
								
								choice = validInput(1,1);
								
							}
							else if(choice == 2){
								monsters[monsternum[roomNum]].resetHealth();
								fight = false;
							}
						}
						
					}
					if(player.getHealth() == 0){
						cout << "You died!" << endl;
						invalid = false;
					}
					else if(monsters[monsternum[roomNum]].getHealth() == 0){
						system("clear");

						for(int i = 0; i < 5; i++){
							for(int j = 0; j < 5; j++){
								temp[i][j] = rooms[roomNum].getVal(i, j);
							}
						}

						for(int a = 0; a < 5; a++){
							for(int b = 0; b < 5; b++){
								if(temp[a][b] == 2){
									temp[a][b] = 0;
								}
							}
						}

						rooms[roomNum].changeArray(temp);	

						rooms[roomNum].display(visited[roomNum]);
							

						cout << "You have slain the " << monsters[monsternum[roomNum]].getName() << "!\n" << endl;
						
						// Random Item Drops
						randweapon = rand() % 4 + 1;
						inventoryweaponsname[weaponloc] = weaponrandom[randweapon].name;
						inventoryweaponsdesc[weaponloc] = weaponrandom[randweapon].description;

						randhealing = rand() % 4;
						inventoryhealingname[healingloc] = healingrandom[randhealing].name;
						inventoryhealingdesc[healingloc] = healingrandom[randhealing].description;
						
						cout << "The " << monsters[monsternum[roomNum]].getName() << " dropped: " << endl;
						cout << weaponrandom[randweapon].name << endl;
						cout << healingrandom[randhealing].name << endl;

						if(roomNum == 5){
							key = true;
							cout << "Key to the Forbidden Chamber" << endl;
						}

						cout << "\n\n" << endl;

						if(weaponrandom[randweapon].damage > weapondmg){
							player.increaseDamage(weaponrandom[randweapon].damage);
							currentwep = weaponloc;
							weapondmg = weaponrandom[randweapon].damage;
						}

						if(healingrandom[randhealing].health > healinghth){
							player.increaseHealth(healingrandom[randhealing].health);
							currenthealth = healingloc;
							healinghth = healingrandom[randhealing].health;
						}

						player.heal();
						monsters[monsternum[roomNum]].resetHealth();

						weaponloc++;
						healingloc++;

						slain[roomNum] = true;

						cout << "You may now move. \n" << endl;
					}
				}

				// END COMBAT
			}

			// Specification C5 - Detailed Look
			else if(input == 'L'){
				// If the room has been visited, temporarily sets the room as unvisited, and shows the description.
				if(visited[roomNum]){
					visited[roomNum] = false;
					rooms[roomNum].display(visited[roomNum]);
					visited[roomNum] = true;
				}
				
			}


			// Specification A5 - Inventory
			else if(input == 'I'){
				cout << "\n                                         Inventory                                             " << endl;
				cout << "===============================================================================================" << endl;
				cout << setw(1) << left << " " << setw(35) << left << "Item" << setw(60) << left << "Description" << endl;
				cout << setw(1) << left << " " << setw(35) << left << "====" << setw(60) << left << "===========" << endl;

				for(int a = 0; a < 7; a++){
					// Currently 'equipped' weapon shows a star in front
					if(inventoryweaponsname[a] != "" && a == currentwep){
						cout << setw(1) << left << "*" << setw(35) << left << inventoryweaponsname[a] << setw(60) << left << inventoryweaponsdesc[a] << endl;
					}
					else if(inventoryweaponsname[a] != ""){
						cout << setw(1) << left << " " << setw(35) << left << inventoryweaponsname[a] << setw(60) << left << inventoryweaponsdesc[a] << endl;
					}
					// If empty, no more items remain
					else if(inventoryweaponsname[a] == ""){
						break;
					}
				}

				for(int b = 0; b < 7; b++){
					// Currently 'equipped' healing item shows a star in front
					if(inventoryhealingname[b] != "" && b == currenthealth){
						cout << setw(1) << left << "*" << setw(35) << left << inventoryhealingname[b] << setw(60) << left << inventoryhealingdesc[b] << endl;
					}
					else if(inventoryhealingname[b] != ""){
						cout << setw(1) << left << " " << setw(35) << left << inventoryhealingname[b] << setw(60) << left << inventoryhealingdesc[b] << endl;
					}
					// If empty, no more items remain
					else if(inventoryhealingname[b] == ""){
						break;
					}
				}

				if(key){
					cout << setw(1) << left << " " << setw(35) << left << "Key to Forbidden Chamber" << setw(60) << left << "The key to the giant lock in the Lock Room" << endl;
				}

				cout << "\n* = currently equipped items" << endl;
				cout << "===============================================================================================" << endl;
				cout << "\nCurrent Health: " << player.getHealth() << "/" << player.getTotalhp() << endl;
				cout << "Current Damage: " << player.getTrueDamage() << endl;
				cout << "\n\nEnter one of the movement keys to return to the game." << endl;
			}

			if(player.getHealth() == 0){
				cout << "You died!" << endl;
				break;
			}

			else if(input == 'H'){
				cout << "                       Controls and Directions                    " << endl;
				cout << "==================================================================" << endl;
				cout << "W - move up" << endl;
				cout << "S - move down" << endl;
				cout << "A - move left" << endl;
				cout << "D - move right" << endl;
				cout << "L - room information" << endl;
				cout << "I - inventory" << endl;
				cout << "H - help\n" << endl;
				cout << "All commands must be followed by clicking the 'Enter' key.\n" << endl;
				cout << "You are represented by 'O', traps by 'U' and monsters are 'X'." << endl;
				cout << "Monsters can take up more than one space. Traps are invisible " << endl;
				cout << "first, but will be revealed when walked over, and they will deal " << endl;
				cout << "10 damage everytime you walk over them. Traps will turn into an " << endl;
				cout << "'0' when you are standing on them. When you enter a room, you  " << endl;
				cout << "will have to choose to either attack the monsters inside or run. " << endl;
				cout << "To attack after you choose to run, you must leave the room and "<< endl;
				cout << "come back. Choose wisely! Kill the monsters and get to the end " << endl;
				cout << "of the dungeon to win! You also have a chance to do extra damage " << endl;
				cout << "with every attack. After killing a monster, you will gain a " << endl;
				cout << "healing item that will increase your health, and heal all lost " << endl;
				cout << "health. Only one weapon and healing item may be equipped at a " << endl;
				cout << "time, and this will be automatically done. You must kill all " << endl;
				cout << "the monsters to clear the dungeon! Make sure you are prepared!\n" << endl;
				cout << "==================================================================" << endl;
				cout << "Enter one of the movement keys to return to the game." << endl;
			}
		}


		// End or Play again
		invalid = true;
		//system("clear");
		cout << "GAME OVER" << endl;
		cout << "Would you like to play again? Y/N" << endl;
		player.increaseHealth(0);
		char temp;
		while(invalid){
			cin >> temp;
			temp = toupper(temp);
			if(temp == 'Y'){
				invalid = false;
			}
			else if(temp == 'N'){
				invalid = false;
				cont = false;
			}
			else{
				cout << "Invalid input." << endl;
			}
		}

 	}
 	cout << "Thanks for Playing!" << endl;
}


void ProgramGreeting() {
	cout << "This program is a simple text based adventure game.\n\n" << endl;
}

void ProgramMenu() {
	cout << "  Menu" << endl;
	cout << "========" << endl;
	cout << "1. Play" << endl;
	cout << "2. Explain Game" << endl;
	cout << "3. Quit" << endl;
} 

int validInput(int low, int high){
	int choice = 0;
	char in;

	// Take input as char, -48 is to convert from ASCII to integer value, 0 is 48 in ASCII.
	cin >> in;
	choice = in - 48;

	// Checks if choice is within the specified bounds.
	while(choice < low || choice > high){
		cout << "Invalid Input.\n" << endl;
		cin >> in;
		choice = in - 48;
	}
	return choice;
}