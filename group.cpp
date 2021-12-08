#include "group.h"
#include "avl.h"

Group::Group(int id) : id(id), max_level(-1), id_max_level(-1), count(0)
{
    std::shared_ptr < Avl<std::shared_ptr<Player>, Pair> > p( new Avl<std::shared_ptr<Player>, Pair>());
    this->players = p;
}

StatusType Group::AddPlayer(std::shared_ptr<Player> p, Pair key)
{
    StatusType st = players->insert(p, key);
    if (st == SUCCESS)
    {
        count++;
        if ((key.levels > max_level) || (key.levels == max_level && key.id < id_max_level))
        {
            max_level = key.levels;
            id_max_level = key.id;
        }
    }

    return st;
}

StatusType Group::RemovePlayer(Pair p)
{
    StatusType st = players->remove(p);
    if (st == SUCCESS)
    {
        count--;
        if (count == 0)
        {
            max_level = -1;
            id_max_level = -1;
        }
        else if (p.id == id_max_level)
        {
            Pair max = players->GetMaxKey();
            max_level = max.levels;
            id_max_level = max.id;
        }
    }
    return st;
}