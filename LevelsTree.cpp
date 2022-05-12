#include "LevelsTree.hpp"


//new_classes
class  PlayersCounters {
    int self_counter; 
    int sub_tree_counter; // including himself
    int sub_tree_levels_sum; // = level1*self_counter1 + level2*self_counter2...
    friend class LevelsTree;

    public:
    PlayersCounters(int level) : self_counter(1), sub_tree_counter(1), sub_tree_levels_sum(level){}
};

class ComapreInt{
    public:
    bool operator()(const int& a,const int& b){
        return a > b;
    }
};
class LevelsTree::UpdateNodeData{
    public:
    void operator()(Node& current_node){

        assert(current_node != nullptr);
        assert(current_node->value != nullptr);

        int left_size = getSubTreeSize(current_node->left_child);
        int right_size = getSubTreeSize(current_node->right_child);
        int self_size = current_node->value->self_counter;
        current_node->value->sub_tree_counter = left_size + right_size + self_size;

        int left_sub_tree_levels_sum  =  getSubTreeLevelsSum(current_node->left_child);
        int right_sub_tree_levels_sum = getSubTreeLevelsSum(current_node->right_child);
        int self_counter_levels_sum = (current_node->value->self_counter) * (current_node->key);
        current_node->value->sub_tree_levels_sum = left_sub_tree_levels_sum + right_sub_tree_levels_sum + self_counter_levels_sum;
    }
};

class LevelsTree::Print {
    public:
    void operator()(Node& current_node){
        int level = current_node->key;
        int counter1 = current_node->value->self_counter;
        int counter2 = current_node->value->sub_tree_counter;
        int counter3 = current_node->value->sub_tree_levels_sum;
        int height = current_node->height;
        std::cout << "key: " << level << ", "<< "self_counter: " << counter1 << ", " <<"height: " <<height <<", "
            <<"sub_tree_counter: "<< counter2 << ", " << "sub_tree_levels_sum: " << counter3 <<"\n";
    }
};

//rank_tree_functions
void LevelsTree::correctionAfterInsertOrEasyRemove(Node& current_node, int level){
    assert(current_node != nullptr);

    if (level == current_node->key){
        UpdateNodeData update_node_data;
        update_node_data(current_node);
        return;
    }
    if (level > current_node->key){
        correctionAfterInsertOrEasyRemove(current_node->right_child, level);
        UpdateNodeData update_node_data;
        update_node_data(current_node);
    }
    else {
        correctionAfterInsertOrEasyRemove(current_node->left_child, level);
        UpdateNodeData update_node_data;
        update_node_data(current_node);
    }
}
void LevelsTree::correctionAfterHardRemoveLeaf(Node& current_node, int level){

}

int LevelsTree::getSubTreeSize(Node& node){
    if (node == nullptr){
        return 0;
    }
    assert(node->value != nullptr);

    return node->value->sub_tree_counter;
}
int LevelsTree::getSubTreeLevelsSum(Node& node){
    if (node == nullptr){
        return 0;
    }
    assert(node->value != nullptr);
    
    return node->value->sub_tree_levels_sum;
}
void LevelsTree::resetCountersToZero(Node& current_node){
    if (current_node == nullptr){
        return;
    }
    if (current_node->value == nullptr){
        return;
    }
    current_node->value->self_counter = 0;
    current_node->value->sub_tree_counter = 0;
    current_node->value ->sub_tree_levels_sum = 0;
}
void LevelsTree::calculateSum(Node& current_node,int level, int& sum){
    if (current_node == nullptr){
        return;
    }
    if (level == current_node->key){
        sum += getSubTreeSize(current_node->left_child) + current_node->value->self_counter;
        return;

    }
    if (level > current_node->key){
        sum += getSubTreeSize(current_node->left_child) + current_node->value->self_counter;
        calculateSum(current_node->right_child, level, sum);
    }
    else {
        calculateSum(current_node->left_child, level, sum);
    }
}
void LevelsTree::calculateSumOfLevels(Node& current_node, int& players_to_sum,int& levels_sum){
    if (current_node == nullptr){
        return;
    }
    assert(current_node->value !=nullptr);
    int right_num = getSubTreeSize(current_node->right_child);
    if (right_num <= players_to_sum && (right_num + current_node->value->self_counter) >= players_to_sum){
        int self_levels_sum = (players_to_sum - right_num) * (current_node->key);
        int right_levels_sum = getSubTreeLevelsSum(current_node->right_child);
        levels_sum += right_levels_sum + self_levels_sum;
        return;
    }
    if (right_num > players_to_sum ){
        calculateSumOfLevels(current_node->right_child , players_to_sum, levels_sum);
        return;
    
    }
    if ((right_num + current_node->value->self_counter) < players_to_sum) {
        int self_levels_sum = current_node->value->self_counter * (current_node->key);
        int right_levels_sum = getSubTreeLevelsSum(current_node->right_child);
        levels_sum += right_levels_sum + self_levels_sum;
        players_to_sum -= (current_node->value->self_counter + right_num);
        calculateSumOfLevels(current_node->left_child , players_to_sum, levels_sum);
        return;
    }
}
/*
void LevelsTree::select(Node& current_node, int index, int& level){
    if (current_node == nullptr){
        return;
    }
    assert(current_node->value !=nullptr);
    if ((getSubTreeSize(current_node->left_child) <= index) && (getSubTreeSize(current_node->left_child) + current_node->value->self_counter >= index) ){
        level = current_node->key;
        return;
    }
    if (getSubTreeSize(current_node->left_child) > index){
        select(current_node->left_child, index, level);
    }
    else {
        select(current_node->right_child, index - getSubTreeSize(current_node->left_child) - current_node->value->self_counter , level);
    } 
}
*/
    
//added_general_tree_functions
void LevelsTree::postOrder(Node& current_node, UpdateNodeData& update_node_data){
    if (current_node == nullptr){
        return;
    }
    postOrder(current_node->left_child, update_node_data);
    postOrder(current_node->right_child, update_node_data);
    update_node_data(current_node);
}

//override_inner_functions
LevelsTree::Node LevelsTree::rotateRight_LevelsTree(Node current_node) {
    Node A = current_node->left_child;
    Node B = current_node;
    B->left_child = A->right_child;
    A->right_child = B;
    current_node->height = 1 + max(getNodeHeight(current_node->left_child), getNodeHeight(current_node->right_child));
    A->height = 1 + max(getNodeHeight(A->left_child), getNodeHeight(A->right_child));
    //new_code
    UpdateNodeData update_node_data;
    update_node_data(B);
    update_node_data(A);
    //end
    return A;
}
LevelsTree::Node LevelsTree::rotateLeft_LevelsTree(Node current_node) {
    Node A = current_node->right_child;
    Node B = current_node;
    B->right_child = A->left_child;
    A->left_child = B;
    current_node->height = 1 + max(getNodeHeight(current_node->left_child), getNodeHeight(current_node->right_child));
    A->height = 1 + max(getNodeHeight(A->left_child), getNodeHeight(A->right_child));
    //new_code
    UpdateNodeData update_node_data;
    update_node_data(B);
    update_node_data(A);
    //end
    return A;
}
LevelsTree::Node LevelsTree::rebalance_LevelsTree(Node current_node){
    
    //same as old rebalnce but calls the updated rotation

    if (balanceFactor(current_node) > 1) {
        // LL rotation
        if (balanceFactor(current_node->left_child) >= 0) {
            return rotateRight_LevelsTree(current_node);
        }
        // LR rotation
        else {
            current_node->left_child = rotateLeft_LevelsTree(current_node->left_child);
            return rotateRight_LevelsTree(current_node);
        }
    }
    else if (balanceFactor(current_node) < -1) {
        // RR rotation
        if (balanceFactor(current_node->right_child) <= 0) {
            return rotateLeft_LevelsTree(current_node);
        }
        // RL rotation
        else {
            current_node->right_child = rotateRight_LevelsTree(current_node->right_child);
            return rotateLeft_LevelsTree(current_node);
        }
    } 
    return current_node;



    
}
LevelsTree::Node LevelsTree::insertUtil_LevelsTree(Node& current_node, int level) {
    if (current_node == nullptr) {
        PlayersCountersPtr data(new PlayersCounters(level));
        Node tmp(new LevelsTree::node(level, data));
        return tmp;
    }
    if ((level > current_node->key) == false) {
        current_node->left_child = insertUtil_LevelsTree(current_node->left_child, level);
    }
    else {
        current_node->right_child = insertUtil_LevelsTree(current_node->right_child, level);
    }
    int left_child_height = getNodeHeight(current_node->left_child);
    int right_child_height = getNodeHeight(current_node->right_child);
    current_node->height = 1 + max(left_child_height, right_child_height);
    correctionAfterInsertOrEasyRemove(root, level);
    return rebalance_LevelsTree(current_node);
}
LevelsTree::Node LevelsTree::removeUtil_LevelsTree(Node current_node,int level) {

    assert(current_node != nullptr);

    // in case the matching node to remove was found
    if (current_node->key == level) {
        // leaf 
        if (!(current_node->left_child) && !(current_node->right_child)) {
             if (num_of_nodes == 0){
                num_of_nodes++;
                num_of_nodes--;
            }
            resetCountersToZero(current_node);
            correctionAfterInsertOrEasyRemove(root, level);
            if (num_of_nodes == 0){
                num_of_nodes++;
                num_of_nodes--;
            }
            current_node = nullptr;
            num_of_nodes--;
            assert(num_of_nodes >= 0);
            return nullptr;
        }
        // has two children
        else if (current_node->left_child && current_node->right_child) {
            Node lowest_right_child_node = findMinUtil(current_node->right_child);
            int saved_key = lowest_right_child_node->key;
            int saved_counter = lowest_right_child_node->value->self_counter;
            current_node->right_child = removeUtil_LevelsTree(current_node->right_child, saved_key);
            current_node->key =saved_key;
            current_node->value->self_counter = saved_counter;
            correctionAfterInsertOrEasyRemove(root, saved_key);
                       
        }
        // has only one child
        else {
            oneChildNodeRemove_LevelsTree(current_node, level);
        } 
    }
    else {
        if (level > current_node->key) {
            current_node->right_child = removeUtil_LevelsTree(current_node->right_child, level);
        }
        else {
            current_node->left_child = removeUtil_LevelsTree(current_node->left_child, level);
        }
    }
    current_node->height = 1 + max(getNodeHeight(current_node->left_child), getNodeHeight(current_node->right_child));
    return rebalance_LevelsTree(current_node);
}
void LevelsTree::oneChildNodeRemove_LevelsTree(Node current_node, int level) {
    Node child_node = (current_node->left_child != nullptr) ?
                        current_node->left_child : current_node->right_child;
    int saved_key = child_node->key;
    int saved_counter = child_node->value->self_counter;

    if (child_node == current_node->left_child) {
        current_node->left_child = removeUtil_LevelsTree(current_node->left_child, saved_key);
    }
    else {
        current_node->right_child = removeUtil_LevelsTree(current_node->right_child, saved_key);
    }
    current_node->key = saved_key;
    current_node->value->self_counter = saved_counter;
    correctionAfterInsertOrEasyRemove(root, saved_key);
}

//merge
void LevelsTree::merge_LevelsTree(LevelsTree &tree) {
    if (tree.isEmpty() == true) {
        return;
    }

    int n1 = this->getSize();
    int n2 = tree.getSize();
    Node* arr_t1 = new Node[n1+1];
    Node* arr_t2 = new Node[n2+1];

    TreeToArray tree1_to_array(arr_t1, 0); 
    this->inOrderNodes(tree1_to_array);
    TreeToArray tree2_to_array(arr_t2, 0); 
    tree.inOrderNodes(tree2_to_array);
    int same_node=0;
    Node* merged_array = arrayMerge_LevelsTree(arr_t1, n1, arr_t2, n2, &same_node);
    root = nullptr;
    num_of_nodes = n1 + n2 - same_node;
    root = makeTreeFromArray(merged_array, 0 , n1 + n2 - same_node - 1);
    
    

    delete[] arr_t1;
    delete[] arr_t2;
    delete[] merged_array;
}
LevelsTree::Node* LevelsTree::arrayMerge_LevelsTree(Node* arr1, int n1, Node* arr2, int n2, int* same_node) {
    Node* merged_arr = new Node[n1 + n2 +1];
    int array_size_1 = n1;
    int array_size_2 = n2;
    Node* merged = merged_arr;
    while (array_size_1 != 0 && array_size_2 != 0) {
        
        if (**arr1 < **arr2) {
            *merged = *arr1;
            arr1++;
            array_size_1--;
        }
        else if (**arr2 < **arr1){
            *merged = *arr2;
            arr2++;
            array_size_2--;
        }
        else {
           (*arr2)->value->self_counter += (*arr1)->value->self_counter ; 
            *merged = *arr2;
            arr1++;
            arr2++;
            array_size_1--;
            array_size_2--;
            *same_node +=1 ;
        }
        merged++;
    }
    if (array_size_1 == 0) {
        while (array_size_2 != 0) {
            *merged = *arr2;
            arr2++;
            array_size_2--;
            merged++;
        }
    }
    else {
        while (array_size_1 != 0) {
            *merged = *arr1;
            arr1++;
            array_size_1--;
            merged++;
        }
    } 
    return merged_arr;
}


//interface
void LevelsTree::insert(int level){
     if (level == 0) {
        num_of_level_zero_players++;
        return;
    }
    PlayersCountersPtr data = this->find(level);
    if (data != nullptr){
        data->self_counter++;
        correctionAfterInsertOrEasyRemove(root, level);
        return;
    }
    root = insertUtil_LevelsTree(root, level);
    num_of_nodes++;
    //insert without rotation
    //data correction 
    //rotation
    //data correction
}
void LevelsTree::remove(int level){
    if (level == 0) {
         num_of_level_zero_players--;
         assert(num_of_level_zero_players >= 0);
         return;
    }
    PlayersCountersPtr data = this->find(level);
    if (data == nullptr){
        return;
    }
    if (data->self_counter > 1){
        data->self_counter--;
        correctionAfterInsertOrEasyRemove(root, level);
        return;
    }
    root = removeUtil_LevelsTree(root, level);
}
void LevelsTree::update(int old_level, int new_level){
    if (old_level == new_level){
        return;
    }
    this->remove(old_level);
    this->insert(new_level);
}
void LevelsTree::merge (LevelsTree& tree){
    this->num_of_level_zero_players += tree.num_of_level_zero_players;
    this->merge_LevelsTree(tree);
    UpdateNodeData update_node_data;
    this->postOrder(this->root, update_node_data);
}
int LevelsTree::getNumOfPlayersInRange (int lower_level, int higher_level){

    if (higher_level < lower_level){
        return 0;
    }

    if (higher_level < 0 ){
        return 0;
    }
    if (higher_level == 0 ){
        return num_of_level_zero_players;
    }
    int sum_higher_level = 0 ;
    calculateSum(root, higher_level, sum_higher_level);
    int sum_lower_level = 0 ;
    calculateSum(root, lower_level, sum_lower_level);
    
    int lower_level_self_counter = 0; 
    PlayersCountersPtr lower_level_data = this->find(lower_level);
    if (lower_level_data != nullptr){
        lower_level_self_counter = lower_level_data->self_counter;
    }
    int res = sum_higher_level - sum_lower_level + lower_level_self_counter;
    if (lower_level <= 0){
        res += num_of_level_zero_players;
    }
    return res;
    
    
    
}
double LevelsTree::getAveragelevel(int m){
    if (root == nullptr){
        return 0;
    }
    assert(root->value != nullptr);
    if ( m == 0 ){
        return 0;
    }
    int players_to_sum = m;
    int levels_sum = 0;
    int num_of_players_with_positive_level =root->value->sub_tree_counter;
    if (m > num_of_players_with_positive_level){
        calculateSumOfLevels(root,num_of_players_with_positive_level, levels_sum);
    }
    else{
        calculateSumOfLevels(root,players_to_sum, levels_sum);
    }
    assert(levels_sum >= 0);
    double sum = levels_sum;
    return sum / m ; 
    /*
    int total_players_num = root->value->sub_tree_counter;
    int lower_player_index = total_players_num - m + 1 ;
    int level_of_lower_players_index = 0;
    select(root, lower_player_index,level_of_lower_players_index);
    assert(level_of_lower_players_index > 0);
    int players_to_sum = m;
    */ 
}




void LevelsTree::printInOrder(){
    Print print;
    int meaningless;
    inOrderNodesUtil(root,print, &meaningless);
}
void LevelsTree::printPostOrder(){
    Print print;
    printPostOrderUtil(root, print);
}

void LevelsTree::printPostOrderUtil(Node& current_node,Print print){
    if (current_node == nullptr){
        return;
    }
    printPostOrderUtil(current_node->left_child, print);
    printPostOrderUtil(current_node->right_child, print);
    print(current_node);
}

int LevelsTree::getNumberOfPlayers(){
    if (root == nullptr){
        return num_of_level_zero_players;
    }
    return root->value->sub_tree_counter + num_of_level_zero_players;
}