#include "Player.h"
#include "Actor/Mine.h"

bool Player::TryPurchase(int cost)
{
	if (gold >= cost)
	{
		gold -= cost;
		return true;
	}
	return false;
}
