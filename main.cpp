
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>   // for std::round, std::pow
#include <algorithm>
#include <random>  // Enemy stat

/* Weapons class*/
enum class Weapons{
    Sword,
    Shield,
    Armor,
    Bow
};

// Convert weapon enum to string
static std::string WeaponString(Weapons equipment) {
    switch (equipment) {
        case Weapons::Sword : return "Sword";
        case Weapons::Shield : return "Shield";
        case Weapons::Armor  : return "Armor";
        case Weapons::Bow    : return "Bow";
        default: return "Unknown";
    }
}

/* Player stats Class*/
class Player_stats {
private:
    /* Player Stats */
    int Health, Melee_Dmg, Ranged_Dmg, Defense;
    std::string PlayerName;
    std::vector<std::string> Skills;

    /* Level Progression */
    int level;
    int xp;            // accumulated xp at current level
    int nextlevelxp;   // xp needed to reach the next level from current level
    double Scaling;    // scaling factor for nextlevelxp

    /* Weapons unlock */
    std::vector<Weapons> unlock_weapons;
    std::vector<std::pair<int,Weapons>> available_weapons = {
        {1, Weapons::Sword},
        {2, Weapons::Shield},
        {3, Weapons::Bow},
        {4, Weapons::Armor}
    };

public:
    // Get current level
    int GetLevel(){ return level; }

    // Unlock weapons for current level
    void weapons_unlock(){
        for (const auto& [reqLevel,equip]: available_weapons){
            if(reqLevel <= level){
                if(std::find(unlock_weapons.begin(), unlock_weapons.end(), equip) == unlock_weapons.end()){
                    unlock_weapons.push_back(equip);
                    std::cout << PlayerName << " has reached level " << level << " and unlocked "
                              << WeaponString(equip) << "\n";
                }
            }
        }
    }

    // Get Unlocked Weapons
    std::vector<Weapons>& GetUnlockedWeapons(){ return unlock_weapons; }

    // Compute max xp for next level
    void max_xp_per_level() {
        int baseXP = 100;
        nextlevelxp = static_cast<int>(std::round(baseXP * std::pow(Scaling, std::max(0, level - 1))));
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

        Health    += HP_stats;
        Melee_Dmg += Melee_stats;
        Ranged_Dmg+= Ranged_stats;
        Defense   += Def_stats;

        // Xp progression
        xp -= nextlevelxp;
        if (xp < 0) xp = 0;

        // Scale xp requirement for the new level
        max_xp_per_level();

        // unlock weapons for new level
        weapons_unlock();

        std::cout << "[Level Up] " << PlayerName << " reached Level " << level
                  << " \n New XP target: " << nextlevelxp << "\n";
    }

    // Xp gains
    void To_next_level(int exp) {
        if (exp <= 0) { return; }
        xp += exp;
        std::cout << PlayerName << " gained " << exp << " xp \n";
        int xp_limit = 100; // set XP limit (safety cap)
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
        weapons_unlock();
    }

    // Display Player Stats
    void player_status() {
        std::cout << "Player: " << PlayerName
                  << " \n Level: " << level
                  << " \n XP: " << xp << "/" << nextlevelxp
                  << " \n HP: " << Health
                  << " \n Melee: " << Melee_Dmg
                  << " \n Ranged: " << Ranged_Dmg
                  << " \n Def: " << Defense
                  << "\n";
    }

    // Display Unlocked Weapons
    void display_unlocked_weapons(){
        if(unlock_weapons.empty()){
            std::cout << "No weapons unlocked yet.\n";
            return;
        }
        for(size_t i = 0; i < unlock_weapons.size(); ++i){
            std::cout << WeaponString(unlock_weapons[i]);
            if( i + 1 < unlock_weapons.size()) std::cout << ", ";
        }
        std::cout << "\n";
    }
};

// Enemy Structure
struct Enemy{
    std::string Name;
    int Health;
    int Attack;
    int Defense;
    int XpReward;
};

// Stage Definition
class levelsetup{
private:
    int CurrentStage = 1;     // <-- initialize to a valid stage
    const int Max_stages = 6;

public:
    levelsetup() = default;

    int GetCurrentStage(){ return CurrentStage; }
    int GetMaxStages(){ return Max_stages; }

    // Set Stage
    bool SetStage(int stage){
        if( stage >= 1 && stage <= Max_stages){
            CurrentStage = stage;
            std::cout << "On Lvl " << CurrentStage << "\n";
            return true;
        }
        std::cerr << "Invalid stage: " << stage << "\n";
        return false; // <-- always return a value
    }

    // Create Enemy
    Enemy CreateEnemy(int stage,int spawn){
        Enemy e;

        // set base stats
        int baseHP       = 30 + stage * 10;
        int baseAtk      = 5  + stage * 2;
        int baseDef      = 5  + stage * 3;
        int baseXpReward = 15 + stage * 2;

        // random stats
        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> var(-2,4);

        // Assign stats with rng
        e.Name    = "Enemy" + std::to_string(stage) + "-" + std::to_string(spawn);
        e.Health  = std::max(1, baseHP       + var(rng));
        e.Attack  = std::max(1, baseAtk      + var(rng));
        e.Defense = std::max(1, baseDef      + var(rng));
        e.XpReward= std::max(1, baseXpReward + var(rng));
        return e;
    }

    // Spawn Enemies for stage
    std::vector<Enemy> SpawnEnemy(){
        // Guard against invalid CurrentStage
        if (CurrentStage < 1 || CurrentStage > Max_stages) {
            std::cerr << "Cannot spawn: invalid CurrentStage = " << CurrentStage << "\n";
            return {};
        }

        int limit = CurrentStage;
        std::vector<Enemy> enemies;
        enemies.reserve(limit);

        for( int spawn = 0; spawn < limit; ++spawn){
            Enemy Spawn_Enemy = CreateEnemy(CurrentStage, spawn);
            enemies.push_back(Spawn_Enemy);
        }
        std::cout << " Spawning " << limit << " enemies for stage " << CurrentStage << " \n";
        return enemies;
    }

    // Advance to next stage
    bool NextStage(){
        if(CurrentStage < Max_stages){
            CurrentStage++;
            std::cout << " Advancing to Stage " << CurrentStage << "\n";
            return true;
        } else {
            std::cout << " Already at Final stage " << Max_stages << "\n";
            return false;
        }
    }

    // Reset Stage
    void ResetStage(){
        CurrentStage = 1;
        std::cout << " Resetting Stage \n";
    }
};

int main() {
    // Create Player
    std::vector<std::string> player_skills = { "Slash", "Block", "Shoot" };
    Player_stats player("Hero", 100, 20, 15, 10, player_skills, 1, 0, 1.0);
    levelsetup stages;
    // Level and Enemy Setup
    for(int lvl = 1; lvl <= stages.GetMaxStages(); ++lvl) {     
        //guard invalid stage
        if (!stages.SetStage(lvl)) {
            continue; // 
        }

        auto enemies = stages.SpawnEnemy();
        int TotalXP = 0;

        for(const auto& enemy: enemies){
            TotalXP += enemy.XpReward;
            std::cout << " Defeated " << enemy.Name
                      << " XP Reward: " << enemy.XpReward << "\n";
        }

        // Player stat update after defeating enemies
        player.To_next_level(TotalXP);
        player.player_status();
        player.display_unlocked_weapons();

        // advances lvl
        stages.NextStage();
    }

    stages.ResetStage();
    return 0;
}
