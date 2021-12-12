#ifndef PLAYERSMANAGER_H
#define PLAYERSMANAGER_H

#include "library1.h"
#include "avl.h"
#include "pair.h"
#include "group.h"
#include "player.h"

class PlayersManager
{
private:
    std::shared_ptr< Avl<std::shared_ptr<Player>, int>  > players_by_id;
    std::shared_ptr< Avl<std::shared_ptr<Player>, Pair> > players_by_level;
    std::shared_ptr< Avl<std::shared_ptr<Group>, int>   > groups;
    std::shared_ptr< Avl<std::shared_ptr<Group>, int>   > not_empty_groups;
    int max_level;
    int id_max_level;
    int count;
    int count_not_empty;

    void MergeGroups(std::shared_ptr<Player> a[], int na, std::shared_ptr<Player> b[], int nb, std::shared_ptr<Player> c[]);
    void ResetAllPlayersGroupPtr();

public:
    PlayersManager(); //C'tor
    PlayersManager(PlayersManager& pm) = default; //Copy c'tor
    ~PlayersManager()
    {
        ResetAllPlayersGroupPtr(); //Inorder

        players_by_id->deleteAllTree(); //PostOrder
        players_by_id = nullptr;

        players_by_level->deleteAllTree(); //PostOrder
        players_by_level = nullptr;

        groups->deleteAllTree(); //PostOrder
        groups = nullptr;

        not_empty_groups->deleteAllTree(); //PostOrder
        not_empty_groups = nullptr;
    }
    StatusType AddGroup(int GroupID);
    StatusType AddPlayer(int PlayerID, int GroupID, int Level);
    StatusType RemovePlayer(int PlayerID);
    StatusType ReplaceGroup(int GroupID, int ReplacementID);
    StatusType IncreaseLevel(int PlayerID, int LevelIncrease);
    StatusType GetHighestLevel(int GroupID, int* PlayerID);
    StatusType GetAllPlayersByLevel(int GroupID, int** Players, int* numOfPlayers);
    StatusType GetGroupsHighestLevel(int numOfGroups, int** Players);
};
#endif