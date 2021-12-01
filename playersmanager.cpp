#include "library1.h"
#include "group.h"
#include "player.h"
#include "avl.h"
#include "pair.h" //Need to create this
#include "playersmanager.h"

using namespace ds;

namespace ds {
    PlayersManager::PlayersManager() : max_level(-1), id_max_level(-1), count(0), count_not_empty(0) {}

    PlayersManager::PlayersManager(PlayersManager& pm) {} //Copy c'tor - ??

    PlayersManager::StatusType AddGroup(int GroupID) {
        if (GroupID <= 0) {
            return INVALID_INPUT;
        }

        std::shared_ptr<Group> new_group(new Group(GroupID));
        //need to check allocation error
        return groups.insert(new_group, GroupID);
    }

    StatusType AddPlayer(int PlayerID, int GroupID, int Level) {
        if (PlayerID <=0 || GroupID <= 0 || Level < 0) {
            return INVALID_INPUT;
        }

        //need to write search
        else if (players_by_id.search(PlayerID) || !(groups.search(GroupID))) {
            return FAILURE;
        }

        //get the group's ptr
        std::shared_ptr<Group> group_ptr = groups.getData(GroupID); //need to write getData
        std::shared_ptr<Player> new_player(new Player(PlayerID, Level, group_ptr));
        Pair player_key = new Pair(Level, PlayerID);
        //need to check allocation error

        //insert the player to groups tree
        StatusType st = group_ptr->AddPlayer(new_player, player_key);
        if (st != SUCCESS) {
            return st;
        }

        //insert the group to not_empty_groups tree if needed
        if (group_ptr->count == 1) {
            st = not_empty_groups.insert(group_ptr, GroupID);
            if (st != SUCCESS) {
                return st;
            }
            count_not_empty++;
        }

        //insert the player to players_by_id tree
        st = players_by_id.insert(new_player, PlayerID);
        if (st != SUCCESS) {
            return st;
        }

        //insert the player to players_by_level tree
        st = players_by_level.insert(new_player, player_key);
        if (st != SUCCESS) {
            return st;
        }

        //update max level
        if(max_level < Level || (max_level == Level && id_max_level > PlayerID)) {
            max_level = Level;
            id_max_level = PlayerID;
        }

        count++;
        return SUCCESS;
    
    }

    StatusType RemovePlayer(int PlayerID) {
        if (PlayerID <= 0) {
            return INVALID_INPUT;
        }
        std::shared_ptr<Group> pg = players_by_id.getData()->player_group; //get the group ptr
        Pair player_key = new Pair(players_by_id.getData()->level, PlayerID); //get the player key

        //Remove from players_by_id
        StatusType st = players_by_id.remove(PlayerID);
        if(st != SUCCESS) {
            return st;
        }

        int cur_count = pg->count;

        //Remove from player's group tree
        st = pg->RemovePlayer(player_key);
        if(st != SUCCESS) {
            return st;
        }

        //Check if the group got empty and remove from not_empty_groups
        if (pg->count == 0) {
            st = not_empty_groups.Remove(pg->id);
            if (st != SUCCESS) {
                return st;
            }
            count_not_empty--;
        }

        //Remove from players_by_level
        st = players_by_level.remove(player_key);
        if (st != SUCCESS) {
            return st;
        }

        if (PlayerID == id_max_level) {
            Pair max = players.GetMaxKey(); //update the max in group - GetMaxKey in AVL
            max_level = max.level;
            id_max_level = max.id;
        }

        count--;
        return SUCCESS;
    }

    //need to complete
    StatusType ReplaceGroup(int GroupID, int ReplacementID);

    StatusType IncreaseLevel(int PlayerID, int LevelIncrease) {
        if (PlayerID <= 0 || LevelIncrease <= 0) {
            return INVALID_INPUT;
        }
        else if (!(players_by_id.search(PlayerID))) {
            return FAILURE;
        }

        std::shared_ptr<Player> p = players_by_id.getData(PlayerID);
        std::shared_ptr<Group> g = p->player_group;
        int new_level = p->level + LevelIncrease;
        Pair old_key = new Pair(p->level, PlayerID);
        Pair new_key = new Pair(new_level, PlayerID);

        //need to validate?
        g->RemovePlayer(old_key);
        players_by_id.remove(PlayerID);
        players_by_level.remove(old_key);

        p->level = new_level;
        g->AddPlayer(p, new_key);
        players_by_id.insert(p, PlayerID);
        players_by_level.insert(p, new_key);

    }

    StatusType GetHighestLevel(int GroupID, int *PlayerID) {
        if (PlayerID == nullptr || GroupID == 0) {
            return INVALID_INPUT;
        }

        if (GroupID < 0) {
            *PlayerID = id_max_level;
            return SUCCESS;
        }
        else if (!(groups.search(GroupID))) {
            return FAILURE;
        }

        std::shared_ptr<Group> g = groups.getData(GroupID);
        *PlayerID = g->id_max_level;
        return SUCCESS;
    }

    //need to complete
    StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers) {
        if (GroupID == 0 || Players == nullptr || numOfPlayers == nullptr) {
            return INVALID_INPUT;
        }
        else if (GroupID < 0) {
            *numOfPlayers = count;
            *Players = new int[*numOfPlayers]; //syntax??
            if (*Players == nullptr) {
                return ALLOCATION_ERROR;
            }

            //need to insert all players from players_by_level
        }
        else if (!(groups.search(GroupID))) {
            return FAILURE;
        }

        std::shared_ptr<Group> g = groups->getData(GroupID);
        *numOfPlayers = g->count;
        *Players = new int[*numOfPlayers]; //syntax??

        //need to insert all players from group's players tree
    }

    //need to complete
    StatusType GetGroupsHighestLevel(int numOfGroups, int **Players) {
        if (numOfGroups < 1 || Players == nullptr) {
            return INVALID_INPUT;
        }
        else if (numOfGroups > count_not_empty) {
            return FAILURE;
        }
        *Players = new int[numOfGroups];
        if (*Players == nullptr) {
            return ALLOCATION_ERROR;
        }

        //need to insert the groups from not_empty_groups
    }

    void Quit(void** DS); //why pointer to pointer
}