#ifndef LEVELS_TREE_H
#define LEVELS_TREE_H

#include "AVLTree.hpp"

class ComapreInt;
class PlayersCounters;
typedef std::shared_ptr<PlayersCounters> PlayersCountersPtr;

class LevelsTree : public AVLTree<int, PlayersCountersPtr,ComapreInt>{
    int num_of_level_zero_players;
    
    //new_classes
    class UpdateNodeData;
    class Print;
    
    //rank_tree_functions
    void correctionAfterInsertOrEasyRemove(Node& current_node,int level);
    void correctionAfterHardRemoveLeaf(Node& current_node, int level);

    static int getSubTreeSize(Node& node);
    static int getSubTreeLevelsSum(Node& node);
    void resetCountersToZero(Node& node);
    void calculateSum(Node& current_node, int level, int& sum);
    void calculateSumOfLevels(Node& current_node, int& players_to_sum,int& levels_sum);
    //void select(Node& current_node, int index, int& level);
    
    //added_general_tree_functions
    void postOrder(Node& current_node, UpdateNodeData& update_node_data);//recursia
    void printPostOrderUtil(Node& current_node,Print print);

    //override_inner_functions
    Node rotateRight_LevelsTree(Node current_node);
    Node rotateLeft_LevelsTree(Node current_node);
    Node rebalance_LevelsTree(Node current_node);
    Node insertUtil_LevelsTree(Node& current_node, int level);
    Node removeUtil_LevelsTree(Node current_node,int level);
    void oneChildNodeRemove_LevelsTree(Node current_node, int level);
    

    //merge 
    void merge_LevelsTree(LevelsTree &tree);
    LevelsTree::Node* arrayMerge_LevelsTree(Node* arr1, int n1, Node* arr2, int n2, int* same_node);

    //interface
    public:
    LevelsTree() : num_of_level_zero_players(0){} 
    void insert(int level);
    void remove(int level);
    void update(int old_level, int new_level);
    void merge(LevelsTree& tree);
    int getNumOfPlayersInRange(int lower_level, int higher_level);
    double getAveragelevel(int m);
    void printInOrder();
    void printPostOrder();
    int getNumberOfPlayers();

    
};


#endif
