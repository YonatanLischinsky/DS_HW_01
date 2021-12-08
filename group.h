#ifndef GROUP_H
#define GROUP_H

#include "avl.h"
#include "player.h"
#include "pair.h"

class Player;

class Group
{
private:
    int id;
    int max_level;
    int id_max_level;
    std::shared_ptr < Avl<std::shared_ptr<Player>, Pair> > players;
    int count;

public:
    Group(int id); //C'tor
    ~Group()
    {
        players->deleteAllTree();
        players = nullptr;
    }
    StatusType AddPlayer(std::shared_ptr<Player> p, Pair key);
    StatusType RemovePlayer(Pair p);

    friend class Player;
    friend class PlayersManager;
};
#endif