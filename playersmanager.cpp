#include "library1.h"
#include "group.h"
#include "player.h"
#include "avl.h"
#include "pair.h"
#include "playersmanager.h"

using namespace ds;

namespace ds
{
    PlayersManager::PlayersManager() : max_level(-1), id_max_level(-1), count(0), count_not_empty(0)
    {
       players_by_id = Avl<std::shared_ptr<Player>, int> ();
       players_by_level = Avl<std::shared_ptr<Player>, Pair)> ();
       groups = Avl<std::shared_ptr<Player>, int> ();
       not_empty_groups = Avl<std::shared_ptr<Player>, int> ();
    }

    StatusType PlayersManager::AddGroup(int GroupID)
    {
        if (GroupID <= 0)
        {
            return INVALID_INPUT;
        }

        if (groups.search(GroupID)) {
            return FAILURE;
        }

        std::shared_ptr<Group> new_group(new Group(GroupID));
        if(new_group == nullptr)
        {
            return ALLOCATION_ERROR;
        }

        return groups.insert(new_group, GroupID);
    }

    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level)
    {
        if (PlayerID <=0 || GroupID <= 0 || Level < 0)
        {
            return INVALID_INPUT;
        }

        if (players_by_id.search(PlayerID) || !(groups.search(GroupID)))
        {
            return FAILURE;
        }

        //get the group's ptr
        std::shared_ptr<Group> group_ptr = groups.getData(GroupID);
        std::shared_ptr<Player> new_player (new Player(PlayerID, Level, group_ptr));
        Pair player_key = Pair(Level, PlayerID); //maybe use 'new' and use pointers.
        if(new_player == nullptr)
        {
            return ALLOCATION_ERROR;
        }

        //insert the player to groups tree
        StatusType st = group_ptr->AddPlayer(new_player, player_key);
        if (st != SUCCESS)
        {
            return st;
        }

        //insert the group to not_empty_groups tree if needed
        if (group_ptr->count == 1)
        {
            st = not_empty_groups.insert(group_ptr, GroupID);
            if (st != SUCCESS)
            {
                return st;
            }
            count_not_empty++;
        }

        //insert the player to players_by_id tree
        st = players_by_id.insert(new_player, PlayerID);
        if (st != SUCCESS)
        {
            return st;
        }

        //insert the player to players_by_level tree
        st = players_by_level.insert(new_player, player_key);
        if (st != SUCCESS)
        {
            return st;
        }

        //update max level
        if(max_level < Level || (max_level == Level && id_max_level > PlayerID))
        {
            max_level = Level;
            id_max_level = PlayerID;
        }

        count++;
        return SUCCESS;
    }

    StatusType PlayersManager::RemovePlayer(int PlayerID)
    {
        if (PlayerID <= 0)
        {
            return INVALID_INPUT;
        }

        std::shared_ptr<Group> pg = players_by_id.getData(PlayerID)->player_group; //get the group ptr
        Pair player_key = Pair(players_by_id.getData(PlayerID)->level, PlayerID); //get the player key

        //Remove from players_by_id
        StatusType st = players_by_id.remove(PlayerID);
        if(st != SUCCESS)
        {
            return st;
        }

        //Remove from player's group tree
        st = pg->RemovePlayer(player_key);
        if(st != SUCCESS) {
            return st;
        }

        //Check if the group got empty and remove from not_empty_groups
        if (pg->count == 0)
        {
            st = not_empty_groups.Remove(pg->id);
            if (st != SUCCESS)
            {
                return st;
            }
            count_not_empty--;
        }

        //Remove from players_by_level
        st = players_by_level.remove(player_key);
        if (st != SUCCESS)
        {
            return st;
        }

        if (PlayerID == id_max_level)
        {
            Pair max = players_by_level.GetMaxKey(); //update the max in group - GetMaxKey in AVL

            max_level = max.level;
            id_max_level = max.id;
        }

        count--;
        return SUCCESS;
    }

    //need to complete
    StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID)
    {
        if (GroupID <= 0 || ReplacementID <= 0) {
            return INVALID_INPUT;
        }
        if(!(groups.search(GroupID)) || !(groups.search(ReplacementID))) { //logk
            return FAILURE;
        }

        std::shared_ptr<Group> group_to_delete = groups.getData(GroupID); //logk
        std::shared_ptr<Group> group_to_insert = groups.getData(ReplacementID); //logk

        std::shared_ptr<Player> arr1[] = new std::shared_ptr<Player>[group_to_delete->count];
        if(arr1 == nullptr)
            return ALLOCATION_ERROR;
        std::shared_ptr<Player> arr2[] = new std::shared_ptr<Player>[group_to_insert->count];
        if(arr2 == nullptr) {
            delete[] arr1;
            return ALLOCATION_ERROR;
        }
        std::shared_ptr<Player> arr3[] = new std::shared_ptr<Player>[group_to_delete->count + group_to_insert->count];
        if(arr3 == nullptr) {
            delete[] arr1;
            delete[] arr2;
            return ALLOCATION_ERROR;
        }

        (group_to_delete->players).InOrderFillArrData(arr1, group_to_delete->count);
        (group_to_insert->players).InOrderFillArrData(arr2, group_to_insert->count);
        

        //Merge the arrays into arr3 //log(n1+n2)
        MergeGroups(arr1, group_to_delete->count, arr2, group_to_insert->count, arr3);

        //Build an almost full tree - build a full tree and then delete leaves (reverse inorder) //log(n1+n2)???
        //By inorder - inserting each player //log(n1+n2)

        return SUCCESS;
    }

    StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease)
    {
        if (PlayerID <= 0 || LevelIncrease <= 0)
        {
            return INVALID_INPUT;
        }
        if ( !players_by_id.search(PlayerID) )
        {
            return FAILURE;
        }

        std::shared_ptr<Player> p = players_by_id.getData(PlayerID); //log n
        std::shared_ptr<Group> g = p->player_group;
        int new_level = p->level + LevelIncrease;
        Pair old_key = Pair(p->level, PlayerID);
        Pair new_key = Pair(new_level, PlayerID);

        g->RemovePlayer(old_key);
        players_by_level.remove(old_key);

        p->level = new_level;
        g->AddPlayer(p, new_key);
        return players_by_level.insert(p, new_key);
    }

    StatusType PlayersManager::GetHighestLevel(int GroupID, int *PlayerID)
    {
        if (PlayerID == nullptr || GroupID == 0)
        {
            return INVALID_INPUT;
        }

        if (GroupID < 0)
        {
            *PlayerID = id_max_level;
            return SUCCESS;
        }
        if ( !groups.search(GroupID) )
        {
            return FAILURE;
        }

        std::shared_ptr<Group> g = groups.getData(GroupID);
        *PlayerID = g->id_max_level;
        return SUCCESS;
    }

    StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers)
    {
        if (GroupID == 0 || Players == nullptr || numOfPlayers == nullptr)
        {
            return INVALID_INPUT;
        }

        if (GroupID < 0)
        {
            *numOfPlayers = count;
            *Players = new int[*numOfPlayers]; //syntax??
            if (*Players == nullptr)
            {
                return ALLOCATION_ERROR;
            }

           
            Pair arr[] = new Pair[count];
            if(arr == nullptr)
                return ALLOCATION_ERROR;
            players_by_level.reverseInOrderFillArr(arr);

            for(int i = 0; i < count; i++)
            {
                *Players[i] = arr[i].id;
            }
            delete arr;
            return SUCCESS;
        }

        if (!(groups.search(GroupID)))
        {
            return FAILURE;
        }

        std::shared_ptr<Group> g = groups->getData(GroupID);
        *numOfPlayers = g->count;
        *Players = new int[*numOfPlayers]; //syntax??

        Pair arr[] = new Pair[g->count];
        if(arr == nullptr)
            return ALLOCATION_ERROR;

        g->players.reverseInOrderFillArr(arr);

        for(int i = 0; i < g->count; i++)
        {
            *Players[i] = arr[i].id;
        }
        delete[] arr;
        return SUCCESS;
    }

    StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int **Players)
    {
        if (numOfGroups < 1 || Players == nullptr)
        {
            return INVALID_INPUT;
        }
        if (numOfGroups > count_not_empty)
        {
            return FAILURE;
        }
        *Players = new int[numOfGroups];
        if (*Players == nullptr) {
            return ALLOCATION_ERROR;
        }
        std::shared_ptr<Group> arr[] = new std::shared_ptr<Group>[numOfGroups];
        if(arr == nullptr)
            return ALLOCATION_ERROR;
        not_empty_groups.InOrderFillArrData(arr, numOfGroups);

        for(int i = 0; i < numOfGroups; i++)
        {
            *Players[i] = arr[i]->id_max_level;
        }
        
        delete[] arr;
        return SUCCESS;
    }

    void PlayersManager::MergeGroups(std::shared_ptr<Player> a[], int na, std::shared_ptr<Player> b[], int nb, std::shared_ptr<Player> c[])
    {
        int ia = 0;
        int ib = 0;
        int ic = 0;
        while (ia < na && ib < nb)
        {
            //maybe insert a pair to every player - to make comparison
        }
        
        
    }
}