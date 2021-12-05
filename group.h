#ifndef GROUP_H
#define GROUP_H

#include "avl.h"
#include "player.h"
#include "pair.h"

namespace ds
{
    class Player;

    class Group
    {
    private:
        int id;
        int max_level;
        int id_max_level;
        Avl<std::shared_ptr<Player>, Pair> players;
        int count;

    public:
        Group(int id); //C'tor
       // Group(const Group& group); //Copy c'tor
        ~Group() = default; //D'tor
        StatusType AddPlayer(std::shared_ptr<Player> p, Pair key);
        StatusType RemovePlayer(Pair p);

        friend class Player;
        friend class PlayersManager;
    };
}
#endif