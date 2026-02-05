#pragma once
#include "Actor/Actor.h"
#include <vector>

using namespace Wanted;

class Mine;

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor);

public:
	Player() : gold(0) {}

	bool TryPurchase(int cost);
	void AddMineList(Mine* mine) { ownedMines.emplace_back(mine); }
	const std::vector<Mine*>& GetOwnedMines() const { return ownedMines; }

	// 자산 관리
	long long GetGold() const { return gold; }
	void AddGold(int amount) { gold += amount; }

private:
	// 소유한 자산
	long long gold;

	// 소유한 광산 목록
	std::vector<Mine*> ownedMines;
};

