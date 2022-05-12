#ifndef Player_H
#define Player_H

#include <memory>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef int PlayerID;
typedef int PlayerLevel;
typedef int GroupID;
typedef int Score;

class player_t;


typedef std::shared_ptr<player_t> Player;

class player_t{
    private:
        PlayerID id;
        PlayerLevel level; 
        GroupID group_id;
        Score score;  
        

    public:
        player_t(PlayerID id, GroupID group_id, Score score) : id(id), level(0),
                 group_id(group_id), score(score) {}
        void icreaseLevel(PlayerLevel level_upgrade);
        const PlayerID getId() const;
        PlayerLevel getLevel() const;
        void setLevel(PlayerLevel new_level);
        GroupID getGroupId() const;
        Score getScore();
        void setGroupId(GroupID id);
        void setScore(int new_score);
        
        //bool operator>(const player_t& player2) const; 
        /* void print(){
            std::cout<< id;
            std::cout<<"  ";
            std::cout << level ;
            std::cout<<"  ";
            std::cout<< group_id; 
            std::cout<<"  " << std::endl;
        } */
};
  
#endif