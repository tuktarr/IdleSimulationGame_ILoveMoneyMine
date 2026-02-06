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
	~Player();
	// 내가 가진 자산이 광산의 구입비용보다 많거나 같은지 비교
	bool TryPurchase(int cost);

	// 내가 소유한 광산 목록에 추가
	void AddMineList(Mine* mine) { ownedMines.emplace_back(mine); }

	// 내가 소유한 광산 목록 꺼내기
	const std::vector<Mine*>& GetOwnedMines() const { return ownedMines; }

	// 자산 관리
	long long GetGold() const { return gold; }
	void AddGold(long long amount) { gold += amount; }
	void SetGold(long long Ingold) { gold = Ingold; }

private:
	// 소유한 자산
	long long gold;

	// 소유한 광산 목록
	std::vector<Mine*> ownedMines;
};

