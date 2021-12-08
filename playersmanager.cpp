#include "playersmanager.h"

PlayersManager::PlayersManager() : max_level(-1), id_max_level(-1), count(0), count_not_empty(0)
{
    std::shared_ptr<Avl<std::shared_ptr<Player>,int>> ids(new Avl<std::shared_ptr<Player>, int>);
    std::shared_ptr<Avl<std::shared_ptr<Player>, Pair>> lvls(new Avl<std::shared_ptr<Player>, Pair>);
    std::shared_ptr<Avl<std::shared_ptr<Group>, int>> grps(new Avl<std::shared_ptr<Group>, int>);
    std::shared_ptr<Avl<std::shared_ptr<Group>, int>> not_empty_grps(new Avl<std::shared_ptr<Group>, int>);

    players_by_id = ids;
    players_by_level = lvls;
    groups = grps;
    not_empty_groups = not_empty_grps;
}

StatusType PlayersManager::AddGroup(int GroupID)
{
    if (GroupID <= 0)
    {
        return INVALID_INPUT;
    }

    if (groups->search(GroupID)) {
        return FAILURE;
    }

    std::shared_ptr<Group> new_group (new Group(GroupID));
    if (new_group == nullptr)
    {
        return ALLOCATION_ERROR;
    }

    return ((StatusType)(groups->insert(new_group, GroupID)) );
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int Level)
{
    if (PlayerID <= 0 || GroupID <= 0 || Level < 0)
    {
        return INVALID_INPUT;
    }

    if (players_by_id->search(PlayerID) || !(groups->search(GroupID)))
    {
        return FAILURE;
    }

    //get the group's ptr
    std::shared_ptr<Group> group_ptr = groups->getData(GroupID);
    std::shared_ptr<Player> new_player(new Player(PlayerID, Level, group_ptr));
    Pair player_key = Pair(Level, PlayerID); //maybe use 'new' and use pointers.
    if (new_player == nullptr)
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
        st = not_empty_groups->insert(group_ptr, GroupID);
        if (st != SUCCESS)
        {
            return st;
        }
        count_not_empty++;
    }

    //insert the player to players_by_id tree
    st = players_by_id->insert(new_player, PlayerID);
    if (st != SUCCESS)
    {
        return st;
    }

    //insert the player to players_by_level tree
    st = players_by_level->insert(new_player, player_key);
    if (st != SUCCESS)
    {
        return st;
    }

    //update max level
    if (max_level < Level || (max_level == Level && id_max_level > PlayerID))
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
    if(!players_by_id->search(PlayerID))
    {
        return FAILURE;
    }
    std::shared_ptr<Player> p = players_by_id->getData(PlayerID);
    std::shared_ptr<Group> pg = p->player_group; //get the group ptr
        
    Pair player_key (p->level, PlayerID); //get the player key

    //Remove from players_by_id
    StatusType st = players_by_id->remove(PlayerID);
    if (st != SUCCESS)
    {
        return st;
    }

    //Remove from player's group tree
    st = pg->RemovePlayer(player_key);
    if (st != SUCCESS) {
        return st;
    }

    //Check if the group got empty and remove from not_empty_groups
    if (pg->count == 0)
    {
        st = not_empty_groups->remove(pg->id);
        if (st != SUCCESS)
        {
            return st;
        }
        count_not_empty--;
    }

    //Remove from players_by_level
    st = players_by_level->remove(player_key);
    if (st != SUCCESS)
    {
        return st;
    }

    if (PlayerID == id_max_level)
    {
        Pair max = players_by_level->GetMaxKey();
        max_level = max.levels;
        id_max_level = max.id;
    }

    count--;
    return SUCCESS;
}

StatusType PlayersManager::ReplaceGroup(int GroupID, int ReplacementID)
{
    if (GroupID <= 0 || ReplacementID <= 0 || GroupID == ReplacementID) {
        return INVALID_INPUT;
    }
    if (!(groups->search(GroupID)) || !(groups->search(ReplacementID))) { //O(logk)
        return FAILURE;
    }


    std::shared_ptr<Group> group_to_delete = groups->getData(GroupID); //O(logk)
    std::shared_ptr<Group> group_to_insert = groups->getData(ReplacementID); //O(logk)

    int nD = group_to_delete->count;
    int nI = group_to_insert->count;

    if (nD == 0)
    {
        groups->remove(GroupID);
        return SUCCESS;
    }

    std::shared_ptr<Player>* arr1 = new std::shared_ptr<Player>[nD];
    if (arr1 == nullptr)
        return ALLOCATION_ERROR;

    std::shared_ptr<Player>* arr2 = new std::shared_ptr<Player>[(nI > 0 ? nI : 1)]; // if NI == 0 allocate array length of 1. (the logic should be the same...)
    if (arr2 == nullptr) {
        delete[] arr1;
        return ALLOCATION_ERROR;
    }

    std::shared_ptr<Player>* arrMergedData = new std::shared_ptr<Player>[nD + nI];
    if (arrMergedData == nullptr) {
        delete[] arr1;
        delete[] arr2;
        return ALLOCATION_ERROR;
    }

    Pair* arrMergedKey = new Pair[nD + nI];
    if (arrMergedKey == nullptr) {
        delete[] arr1;
        delete[] arr2;
        delete[] arrMergedData;
        return ALLOCATION_ERROR;
    }

    (group_to_delete->players)->InOrderFillArrData(arr1, nD, true);
    (group_to_insert->players)->InOrderFillArrData(arr2, nI, true);

    //Merge the arrays into arr3 //O(n1+n2)
    MergeGroups(arr1, nD, arr2, nI, arrMergedData);

    for (int i = 0; i < nI + nD; i++)
    {
        Pair p(arrMergedData[i]->level, arrMergedData[i]->id);
        arrMergedKey[i] = p;
    }

    int treeHeight = ceil(log2(nD + nI + 1)) - 1;      // [log(n1+n2+1)]-1
    int countRemoveFromRight = (pow(2, treeHeight + 1) - 1) - (nD + nI); // (2^(treeHeight+1)-1) - (n1+n2)





    std::shared_ptr<Avl<std::shared_ptr<Player>, Pair>> emptyTree (new Avl<std::shared_ptr<Player>, Pair>(treeHeight)); //O(nI + nD)
    emptyTree->reverseInOrderRemoveNodes(countRemoveFromRight);
    emptyTree->InOrderFillArrData(arrMergedData, nI + nD, false); // //O(nI + nD)
    emptyTree->InOrderFillArrKey(arrMergedKey, nI + nD, false); //O(nI + nD)

    //if(group_to_insert->players != nullptr) // 
    //    group_to_insert->players->deleteAllTree(); // O(nI)
    group_to_insert->players = emptyTree;

    if ((group_to_insert->max_level < group_to_delete->max_level) || group_to_insert->max_level == group_to_delete->max_level &&
        group_to_insert->id_max_level > group_to_delete->id_max_level)
    {
        group_to_insert->max_level = group_to_delete->max_level;
        group_to_insert->id_max_level = group_to_delete->id_max_level;
    }

    for (int i = 0; i < nD; i++)
    {
        arr1[i]->player_group = group_to_insert;
    }

    groups->remove(GroupID); // O(log K)
    not_empty_groups->remove(GroupID); // O(log K)

    if (group_to_insert->count == 0)
    {
        not_empty_groups->insert(group_to_insert, ReplacementID); // O(log k)
    }
    group_to_insert->count += nD;


    delete[] arr1;
    delete[] arr2;
    delete[] arrMergedData;
    delete[] arrMergedKey;
    return SUCCESS;
}

StatusType PlayersManager::IncreaseLevel(int PlayerID, int LevelIncrease)
{
    if (PlayerID <= 0 || LevelIncrease <= 0)
    {
        return INVALID_INPUT;
    }
    if (!players_by_id->search(PlayerID))
    {
        return FAILURE;
    }

    std::shared_ptr<Player> p = players_by_id->getData(PlayerID); //log n
    std::shared_ptr<Group> g = p->player_group;
    int new_level = p->level + LevelIncrease;
    Pair old_key = Pair(p->level, PlayerID);
    Pair new_key = Pair(new_level, PlayerID);

    g->RemovePlayer(old_key);
    players_by_level->remove(old_key);

    p->level = new_level;
    g->AddPlayer(p, new_key);

    if ((new_level > max_level) || (new_level == max_level && PlayerID < id_max_level))
    {
        max_level = new_level;
        id_max_level = PlayerID;
    }
    return players_by_level->insert(p, new_key);
}

StatusType PlayersManager::GetHighestLevel(int GroupID, int* PlayerID)
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
    if (!groups->search(GroupID))
    {
        return FAILURE;
    }

    std::shared_ptr<Group> g = groups->getData(GroupID);
    *PlayerID = g->id_max_level;
    return SUCCESS;
}

StatusType PlayersManager::GetAllPlayersByLevel(int GroupID, int** Players, int* numOfPlayers)
{
    if (GroupID == 0 || Players == nullptr || numOfPlayers == nullptr)
    {
        return INVALID_INPUT;
    }

    if (GroupID < 0)
    {
        *numOfPlayers = count;

        *Players = (int*)malloc(sizeof(int) * (*numOfPlayers));

        if (*Players == nullptr)
        {
            return ALLOCATION_ERROR;
        }


        Pair* arr = new Pair[count];
        if (arr == nullptr)
            return ALLOCATION_ERROR;
        players_by_level->reverseInOrderFillArr(arr);

        for (int i = 0; i < count; i++)
        {
            (*Players)[i] = arr[i].id;
        }
        delete[] arr;
        return SUCCESS;
    }

    if (!(groups->search(GroupID)))
    {
        return FAILURE;
    }

    std::shared_ptr<Group> g = groups->getData(GroupID);
    *numOfPlayers = g->count;

    *Players = (int*)malloc(sizeof(int) * (*numOfPlayers));

    Pair* arr = new Pair[g->count];
    if (arr == nullptr)
        return ALLOCATION_ERROR;

    g->players->reverseInOrderFillArr(arr);

    for (int i = 0; i < g->count; i++)
    {
        (*Players)[i] = arr[i].id;
    }
    delete[] arr;
    return SUCCESS;
}

StatusType PlayersManager::GetGroupsHighestLevel(int numOfGroups, int** Players)
{
    if (numOfGroups < 1 || Players == nullptr)
    {
        return INVALID_INPUT;
    }
    if (numOfGroups > count_not_empty)
    {
        return FAILURE;
    }

    *Players = (int*) malloc(sizeof(int)*numOfGroups);
    if (*Players == nullptr) {
        return ALLOCATION_ERROR;
    }
    std::shared_ptr<Group>* arr = new std::shared_ptr<Group>[numOfGroups];
    if (arr == nullptr)
        return ALLOCATION_ERROR;
    not_empty_groups->InOrderFillArrData(arr, numOfGroups, true);


    for (int i = 0; i < numOfGroups; i++)
    {
        (*Players)[i] = arr[i]->id_max_level;
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
        Pair p1(a[ia]->level, a[ia]->id);
        Pair p2(b[ib]->level, b[ib]->id);

        if (p1 < p2)
        {
            c[ic++] = a[ia++];
        }
        else
        {
            c[ic++] = b[ib++];
        }
    }

    while (ia < na)
    {
        c[ic++] = a[ia++];
    }
    while (ib < nb)
    {
        c[ic++] = b[ib++];
    }
}

void PlayersManager::ResetAllPlayersGroupPtr()
{
    std::shared_ptr<Player>* arr = new std::shared_ptr<Player>[count];
    players_by_id->InOrderFillArrData(arr, count, true);

    for (int i = 0; i < count; i++)
    {
        arr[i]->player_group = nullptr;
    }

    delete[] arr;
}
