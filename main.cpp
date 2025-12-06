#pragma once
#include <iostream>
#include <cstdint>
/*Variable Definition*/
typedef std::uint16_t hptype;
/* Health point class*/
class HP {
private:
	hptype CurrentHP, MaxHP;
public:
	/* Set Max Health */
	bool SetMaxHP(hptype hp) {
		if (hp < 1) { return false; }
		MaxHP = hp;
		if (CurrentHP > MaxHP) { CurrentHP = MaxHP; }
		return true;
	}
	/* Get Max HP */
	hptype GetHP() { return MaxHP; }
	/* Get Current HP */
	hptype GetCurrentHP() { return CurrentHP; }
	/* Calculate HP based on Damage */
	void TakeDamage(hptype dmg) {
		if (dmg > CurrentHP) {
			CurrentHP = 0;
			return;
		}
		CurrentHP -= dmg;
	}
	/* Healing */
	void heal(hptype healing) {
		if (CurrentHP + healing > MaxHP) {
			CurrentHP = MaxHP;
		}
		CurrentHP += healing;
	}
}


int main() {
	return 0;
}