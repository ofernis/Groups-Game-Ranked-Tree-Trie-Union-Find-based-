#ifndef PLAYERS_MANAGER_H
#define PLAYERS_MANAGER_H

#include "library2.h"
#include "Player.hpp" //necessary?
#include "trie.hpp"
#include "LevelsTree.hpp"
#include "LevelsTree.hpp"
#include "UnionFind.hpp"
#include <string>
#include <memory>

#define MAX_SCALE 200
#define MIN_SCALE 1

using std::string;

class PlayersManager { 
    private:

        class groupData{
            public:
            std::shared_ptr<LevelsTree> PlayersLevels;
            std::shared_ptr<LevelsTree>* PlayersLevelsByScore;
            groupData(int scale) : PlayersLevels(std::shared_ptr<LevelsTree>(new LevelsTree)), PlayersLevelsByScore(new std::shared_ptr<LevelsTree>[scale + 2]) {
                for (int i = 1; i <= scale ; i++){
                    PlayersLevelsByScore[i] = std::shared_ptr<LevelsTree>(new LevelsTree);
                }
            }
            ~groupData(){
                delete[] PlayersLevelsByScore;
            }
        };

        typedef std::shared_ptr<groupData> GroupData;

        std::unique_ptr<UnionFind<GroupData>> Groups;
        std::unique_ptr<Trie<Player>> PlayersIDs;
        std::unique_ptr<LevelsTree> PlayersLevels;
        std::shared_ptr<LevelsTree>* PlayersLevelsByScore;
        int num_of_groups;
        int scale;

    public: 
        PlayersManager(int k, int scale);
        ~PlayersManager();
        PlayersManager(const PlayersManager&) = delete;
        PlayersManager& operator=(const PlayersManager&) = delete;

        StatusType MergeGroups(int GroupID1, int GroupID2);
        StatusType AddPlayer(int PlayerID, int GroupID, int score);
        StatusType RemovePlayer(int PlayerID);
        StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
        StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
        StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double * players);
        StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
        StatusType GetPlayersBound(int GroupID, int score, int m,
                                                int * LowerBoundPlayers, int * HigherBoundPlayers);

};

#endif
