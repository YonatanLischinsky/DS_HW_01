#ifndef PLAYER_H
#define PLAYER_H

#include "avl.h"
#include "group.h"

using namespace std;
using namespace ds;

class Player
{
    private:
        int id;
        int level;
        std::shared_ptr<Group> player_group;

    public:
        Player(int id, int lvl, std::shared_ptr<Group> g) : id(id), level(lvl), player_group(g)
        { } //C'tor

        Player(Player& p) = default; //Copy c'tor
        ~Player() = default; //D'tor

        friend class PlayersManager;
};

// Player::Player(int id, int lvl, std::shared_ptr<Group> g) : id(id), level(lvl), player_group(g) { }

#endif