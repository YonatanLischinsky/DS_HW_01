#ifndef PLAYER_H
#define PLAYER_H

#include "avl.h"
#include "pair.h"
#include "group.h"
//#include "playersmanager.h"


namespace ds
{
    class Group;

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

        friend class Group;
        friend class PlayersManager;
    };
}
#endif