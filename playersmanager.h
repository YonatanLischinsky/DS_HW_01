#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include "library1.h"
#include "group.h"
#include "player.h"
#include "avl.h"
#include "pair.h"

namespace ds
{
    class PlayersManager
    {
        private:
            Avl<std::shared_ptr<Player>, int> players_by_id;
            Avl<std::shared_ptr<Player>, Pair> players_by_level;
            Avl<std::shared_ptr<Player>, int> groups;
            Avl<std::shared_ptr<Player>, int> not_empty_groups;
            int max_level;
            int id_max_level;
            int count;
            int count_not_empty;

        public:
            PlayersManager(); //C'tor
            PlayersManager(PlayersManager& pm); //Copy c'tor - ??
            ~PlayersManager() = default; //D'tor - ??
            StatusType AddGroup(int GroupID);
            StatusType AddPlayer(int PlayerID, int GroupID, int Level);
            StatusType RemovePlayer(int PlayerID);
            StatusType ReplaceGroup(int GroupID, int ReplacementID);
            StatusType IncreaseLevel(int PlayerID, int LevelIncrease);
            StatusType GetHighestLevel(int GroupID, int *PlayerID);
            StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers);
            StatusType GetGroupsHighestLevel(int numOfGroups, int **Players);
            void Quit(void** DS); //why pointer to pointer //yontan: nobody fucking know :O
    };
}

#endif