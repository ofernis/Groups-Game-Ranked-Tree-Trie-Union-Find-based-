#include "Player.hpp"


  void player_t::icreaseLevel(PlayerLevel level_upgrade) {
      level += level_upgrade;
  }

    const PlayerID player_t::getId() const {
        return id;
    }
    PlayerLevel player_t::getLevel() const {
        return level;
    }
    void player_t::setLevel(PlayerLevel new_level) {
        level = new_level;
    }
    GroupID player_t::getGroupId() const {
        return group_id;
    }
    void player_t::setGroupId(GroupID id) {
        group_id = id;
    }
    void player_t::setScore(int new_score){
        score = new_score;
    }

    Score player_t::getScore() {
        return score;    
    }

