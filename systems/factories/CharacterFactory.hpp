#pragma once

namespace ks {

class Player;
class Enemy;

class CharacterFactory {
public:
    Player* createPlayer();
    Enemy* createEnemy(const char* typeId);
};

} // namespace ks
