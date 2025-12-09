#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>    // for std::round, std::pow

/* Player stats Class*/
class Player_stats {
private:
    /* Player Stats */
    int Health, Melee_Dmg, Ranged_Dmg, Defense;
    std::string PlayerName;
    std::vector<std::string> Skills;

    /* Level Progression */
    int level;
    int xp;             // accumulated xp at current level
    int nextlevelxp;    // xp needed to reach the next level from current level
    double Scaling;     // scaling factor for nextlevelxp

    // Compute max xp for next level
    void max_xp_per_level() {
        int baseXP = 100;
        nextlevelxp = static_cast<int>(std::round(base * std::pow(Scaling, std::max(0, level - 1))));
        if (nextlevelxp < 1) { nextlevelxp = 1; }
    }

    // Level up logic
    void levelup() {
        // Increase Level
        level++;

        // Increase Player stats on leveling up
        const int HP_stats     = 10;
        const int Melee_stats  = 5;
        const int Ranged_stats = 5;
        const int Def_stats    = 5;

        Health     += HP_stats;
        Melee_Dmg  += Melee_stats;
        Ranged_Dmg += Ranged_stats;
        Defense    += Def_stats;

        // Xp progression
        xp -= nextlevelxp;
        if (xp < 0) xp = 0;

        // Scale xp requirement for the new level
        max_xp_per_level();
    }

public:
    // Xp gains
    void To_next_level(int exp) {
        if (exp <= 0) { return; }
        xp += exp;
        std::cout << PlayerName << " gained " << exp << " xp \n";
        int xp_limit = 100; //set XP limit
        while (xp >= nextlevelxp && xp_limit-- > 0) {
            levelup();
        }
    }

    // Player Constructor
    Player_stats(const std::string& name,
                 int HP,
                 int Melee,
                 int Ranged,
                 int def,
                 const std::vector<std::string>& sp,
                 int startlvl,
                 int startxp,
                 double xp_scaling)
        : Health(HP),
          Melee_Dmg(Melee),
          Ranged_Dmg(Ranged),
          Defense(def),
          PlayerName(name),
          Skills(sp),
          level(startlvl),
          xp(startxp),
          nextlevelxp(0),
          Scaling(xp_scaling){
                if (level < 1) level = 1;
                max_xp_per_level();
        }

    // Display Player Stats
    void Status() {
        std::cout << "Player: " << PlayerName
                  << " | Level: " << level
                  << " | XP: " << xp << "/" << nextlevelxp
                  << " | HP: " << Health
                  << " | Melee: " << Melee_Dmg
                  << " | Ranged: " << Ranged_Dmg
                  << " | Def: " << Defense
                  << "\n";
    }
};

int main() {
    // Create Player
    std::vector<std::string> player_skills = { "Slash", "Block", "Shoot" };
    Player_stats player("Hero", 100, 20, 15, 10, player_skills, 1, 0, 1.5);

    player.Status();

    // Simulate gaining experience
    player.To_next_level(150);

    player.Status();

    return 0;
}
