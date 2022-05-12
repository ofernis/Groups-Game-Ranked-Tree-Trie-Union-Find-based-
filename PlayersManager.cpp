#include "PlayersManager.hpp"

enum Mode {GROUP, SYSTEM};

PlayersManager::PlayersManager(int k, int scale) : num_of_groups(k), scale(scale) {
    if (k <= 0 || scale > MAX_SCALE || scale < MIN_SCALE) {
        return;
    }
    Groups = std::unique_ptr<UnionFind<GroupData>>(new UnionFind<GroupData>(k));
    for (int i = 1; i <= k ; i++){
        Groups->setData(i, GroupData(new groupData(scale)));
    }
    PlayersIDs = std::unique_ptr<Trie<Player>>(new Trie<Player>());
    PlayersLevels = std::unique_ptr<LevelsTree>(new LevelsTree());
    PlayersLevelsByScore = new std::shared_ptr<LevelsTree>[scale + 2];

    for (int i = 1; i <= scale ; i++){
        PlayersLevelsByScore[i] = std::shared_ptr<LevelsTree>(new LevelsTree);
    }
}

PlayersManager::~PlayersManager(){
    delete[] PlayersLevelsByScore;
}

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2) {
    
    if (GroupID1 <= 0 || GroupID1 > num_of_groups ||
        GroupID2 <= 0 || GroupID2 > num_of_groups){
            return INVALID_INPUT;
        }
    if (Groups->findId(GroupID1) == Groups->findId(GroupID2)){
        return SUCCESS;
    }
    try{
        int bigger_group_id = (Groups->getGroupSize(GroupID1) >= Groups->getGroupSize(GroupID2))
                               ? GroupID1 : GroupID2;
        int smaller_group_id = (Groups->getGroupSize(GroupID1) < Groups->getGroupSize(GroupID2))
                               ? GroupID1 : GroupID2;
        GroupData bigger_group_data = Groups->findData(bigger_group_id);
        GroupData smaller_group_data = Groups->findData(smaller_group_id);

        bigger_group_data->PlayersLevels->merge(*smaller_group_data->PlayersLevels);
        for (int i = 1; i <= scale ; i++){
            bigger_group_data->PlayersLevelsByScore[i]->merge(*smaller_group_data->PlayersLevelsByScore[i]);
        }
        
        Groups->makeUnion(GroupID1, GroupID2);
    }   
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS; 
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    if (GroupID > num_of_groups || GroupID <= 0 || 
        PlayerID <= 0 || score > scale || score <= 0) {
            return INVALID_INPUT;
    }
    try{
        Player new_player = Player(new player_t(PlayerID, GroupID, score));
        string player_id_str = std::to_string(PlayerID);
        if (PlayersIDs->find(player_id_str) == true) {
            return FAILURE;
        }
        PlayersIDs->insert(player_id_str, new_player);
        PlayersLevels->insert(0);
        PlayersLevelsByScore[score]->insert(0);

        Groups->findData(GroupID)->PlayersLevels->insert(0);
        Groups->findData(GroupID)->PlayersLevelsByScore[score]->insert(0);
    }   
    catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {
    if (PlayerID <= 0) {
        return INVALID_INPUT;
    }
    string player_id_str = std::to_string(PlayerID);
    if (PlayersIDs->find(player_id_str) == false) {
        return FAILURE;
    }
    Player player = PlayersIDs->getObject(player_id_str);
    int level = player->getLevel();
    int score = player->getScore();
    int group_id = player->getGroupId();
    PlayersIDs->remove(player_id_str);
    PlayersLevels->remove(level);
    PlayersLevelsByScore[score]->remove(level);
    Groups->findData(group_id)->PlayersLevels->remove(level);
    Groups->findData( group_id)->PlayersLevelsByScore[score]->remove(level);
    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if(PlayerID <= 0 || LevelIncrease <= 0) {
        return INVALID_INPUT;
    }
    string player_id_str = std::to_string(PlayerID);
    if (PlayersIDs->find(player_id_str) == false) {
        return FAILURE;
    }
    Player player = PlayersIDs->getObject(player_id_str);
    int level = player->getLevel();
    int new_level = level + LevelIncrease;
    int score = player->getScore();
    int group_id = player->getGroupId();
    player->setLevel(new_level);
    PlayersLevels->update(level, new_level);
    PlayersLevelsByScore[score]->update(level, new_level);
    Groups->findData(group_id)->PlayersLevels->update(level, new_level);
    Groups->findData( group_id)->PlayersLevelsByScore[score]->update(level, new_level);
    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore) {
    if (PlayerID <= 0 || NewScore <= 0 || NewScore > scale){
        return INVALID_INPUT;
    }
    string player_id_str = std::to_string(PlayerID);
    if (PlayersIDs->find(player_id_str) == false){
        return FAILURE;
    }
    //get data
    Player player = PlayersIDs->getObject(player_id_str);
    int level = player->getLevel();
    int score = player->getScore();
    int group_id = player->getGroupId();

    if (score == NewScore){
        return SUCCESS;
    }

    //change general tree
    PlayersLevelsByScore[score]->remove(level);
    PlayersLevelsByScore[NewScore]->insert(level);

    //change group tree
    GroupData data = Groups->findData(group_id);
    data->PlayersLevelsByScore[score]->remove(level);
    data->PlayersLevelsByScore[NewScore]->insert(level);

    //change player data
    player->setScore(NewScore);


    return SUCCESS;
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players) {
    if (players == nullptr || GroupID > num_of_groups || GroupID < 0) {
        return INVALID_INPUT;
    }

    if (lowerLevel > higherLevel){
        return FAILURE;
    }

    if (score < 1 || score > scale){
        *players = 0;
        return SUCCESS;
    }

    double general_num_of_players_in_bonuds =0;
    double score_num_of_players_in_bounds= 0;

    Mode op_mode = (GroupID == 0) ? SYSTEM : GROUP;
    switch (op_mode) {
    case GROUP: {
        GroupData group_data = Groups->findData(GroupID);
        general_num_of_players_in_bonuds = 
            group_data->PlayersLevels->getNumOfPlayersInRange(lowerLevel,higherLevel);
        score_num_of_players_in_bounds = 
            group_data->PlayersLevelsByScore[score]->getNumOfPlayersInRange(lowerLevel,higherLevel);
            break;
        }
    case SYSTEM: {
        general_num_of_players_in_bonuds = 
            PlayersLevels->getNumOfPlayersInRange(lowerLevel,higherLevel);
        score_num_of_players_in_bounds = 
            PlayersLevelsByScore[score]->getNumOfPlayersInRange(lowerLevel,higherLevel);
            break;
        }
    }
    if (general_num_of_players_in_bonuds == 0) {
        return FAILURE;
    }
    *players = (score_num_of_players_in_bounds / general_num_of_players_in_bonuds) * 100;
    assert(*players != 200);
    return SUCCESS;
}

StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level) {
    if (level == nullptr || GroupID < 0 || GroupID > num_of_groups || m <= 0){
        return INVALID_INPUT;
    }
    if (GroupID == 0){
        if (PlayersLevels->getNumberOfPlayers() < m){
            return FAILURE;
        }
        *level = PlayersLevels->getAveragelevel(m);
        
    }
    else {
        GroupData data = Groups->findData(GroupID);
        if (data->PlayersLevels->getNumberOfPlayers() < m ){
            return FAILURE;
        }
        *level =  data->PlayersLevels->getAveragelevel(m);
    }
    return SUCCESS;
}

StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                                        int * LowerBoundPlayers, int * HigherBoundPlayers) {
return SUCCESS;
                                        }