#pragma once
#include <iostream>
#include <vector>
#include <string>

/*Gear
enum gear {
	Sword,
	Shield,
	Armor,
	Bow
};*/

/* Player stats Class*/
class Player_stats() {
private:
	/* Player Stats*/
	int Health, Melee_Dmg, Ranged_Dmg, Defense;
	std::string PlayerName;
	std::vector<std::string> Skills;

	/* Level Progression */
	int level, xp, nextlevelxp;
	double Scaling;

public:

	//Find Max xp for next level
	void max_xp_per_level() {
		nextlevelxp = std::round(xp * std::pow(scaling, level - 1));
		if (nextlevelxp < 1) { nextlevelxp = 1; }
	}

	void levelup() {
		//Increase Level
		level++;

		//Increase Player stats on leveling up
		int HP_stats = 10;
		int Melee_stats = 5;
		int Ranged_stats = 5;
		int Def_stats = 5;

		Health += HP_stats;
		Melee_dmg += Melee_stats;
		Ranged_Dmg += Ranged_stats;
		Defense += Def_stats;

		//Scale Xp for new level
		xp -= nextlevelxp;
		max_xp_per_level();
	}
	//Xp gains
	void To_next_level(int exp) {
		if (exp <= 0) { return 0; }
		xp += amount;
		std::cout << PlayerName << "gained" << exp << "\n";
		//exit loop if max xp for level is reached
		int xp_limit = 100
			while (xp >= nextlevelxp && xp_limit < 100) {
				levelup();
			}
	}
//Player Constructor Class
	Player_stats(std::string name, 
				 int HP, 
		         int Melee, 
		         int Ranged, 
		         int def, 
		         std::vector<std::string> sp,
		         int startlvl,
				 int startxp,
		         int xp_scaling ) :
					PlayerName(name), Health(HP), Melee_Dmg(Melee), Ranged_Dmg(Ranged), Defense(def), Skills(sp), level(startlvl),xp(startxp),scaling(xp_scaling) {
									max_xp_per_level();
				}

};

int main() {
	Player_stats player;
	//Simulate XP 
	player.To_next_level(10);
}
