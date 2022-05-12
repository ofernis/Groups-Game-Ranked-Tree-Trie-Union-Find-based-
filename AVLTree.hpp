#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <memory>
#include <cmath>
#include <new>
#include <assert.h>
#include <typeinfo>


template <class Key, class Value, class Compare>
class AVLTree {
    protected:
        class node {
            public:
                Key key; 
                Value value;
                std::shared_ptr<node> left_child; 
                std::shared_ptr<node> right_child;
                int height;
                node(const Key& key, const Value& value) : 
                key(key), value(value), left_child(nullptr), right_child(nullptr), height(0){}
                bool operator<(node& other_node){
                    return this->key < other_node.key;
                    //Compare isBIgger;
                    //return !isBIgger(this->key,other_node.key);
                } 
                void print(){
                    key->print();
                }
        };

        using Node = std::shared_ptr<node>;

        Node root; 
        int num_of_nodes; 
        
        Node rotateLeft(Node current_node);
        Node rotateRight(Node current_node);
        Node rebalance(Node current_node, Key key);
        int balanceFactor(Node current_node) ;
        Node insertUtil(Node current_node,const  Key& key,const  Value& value);
        Node removeUtil(Node current_node, const Key &key);
        void oneChildNodeRemove(Node current_node, Key key);
        Node findUtil(Node current_node, const Key& key) const;
        Node findMinUtil(Node current_node) const;
        int getNodeHeight(Node node) const;
        Node* arrayMerge(Node* arr1, int n1, Node* arr2, int n2);

        class TreeToArray{
            Node* array;
            int& index; 
            
            public:
                TreeToArray(Node* array, int index): array(array), index(index){}
                void operator()(Node node){
                    if (node == nullptr) {
                        return;
                    }
                    array[index] = node;
                    index++;
                }
        };

        template <typename Functor>
        void inOrderNodes(Functor &functor);

        template <typename Functor>
        void inOrderNodesUtil(Node& current_node, Functor &functor, int* counter);

        template <typename Functor>
        void inOrderUtil(Node current_node, Functor &functor);

        template <typename Functor>
        void reverseInOrderNodes(Functor &functor);

        template <typename Functor>
        void reverseInOrderNodesUtil(Node current_node, Functor &functor);

        template <typename Functor>
        void reverseInOrderUtil(Node current_node, Functor &functor);

        Node makeTreeFromArray(Node* array,int min_node, int max_node);

    public:
        AVLTree() : root(nullptr), num_of_nodes(0) {}
        AVLTree(const AVLTree& other) = delete;
        ~AVLTree() { root = nullptr; }
        AVLTree& operator=(const AVLTree& other) = default;
        Value find(const Key& key) const;
        void insert( const Key& key,const Value& value);
        void remove(const Key &key);
        int getSize() const;
        Value findMax() const;
        Value findMin() const;
        bool isEmpty() const;
        void merge(AVLTree &tree);

        template <typename Functor>
        void inOrder(Functor &functor);

        template <typename Functor>
        void reverseInOrder(Functor &functor);

        int getHeight() const {
            return root->height;
        }
};


/////////////////////////////
// methods implementations //
/////////////////////////////

static int max(int a, int b) {
    return (a > b) ? a : b;
}

template <class Key, class Value, class Compare>
int AVLTree<Key,Value,Compare>::getNodeHeight(Node node) const{
    if (node == nullptr){
        return -1;
    }
    return node->height;
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::rotateLeft(Node current_node) {
    Node A = current_node->right_child;
    Node B = current_node;
    B->right_child = A->left_child;
    A->left_child = B;
    current_node->height = 1 + max(getNodeHeight(current_node->left_child), getNodeHeight(current_node->right_child));
    A->height = 1 + max(getNodeHeight(A->left_child), getNodeHeight(A->right_child));
    return A;
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::rotateRight(Node current_node) {
    Node A = current_node->left_child;
    Node B = current_node;
    B->left_child = A->right_child;
    A->right_child = B;
    current_node->height = 1 + max(getNodeHeight(current_node->left_child), getNodeHeight(current_node->right_child));
    A->height = 1 + max(getNodeHeight(A->left_child), getNodeHeight(A->right_child));
    return A;
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::rebalance(Node current_node, Key key) {
    if (balanceFactor(current_node) > 1) {
        // LL rotation
        if (balanceFactor(current_node->left_child) >= 0) {
            return rotateRight(current_node);
        }
        // LR rotation
        else {
            current_node->left_child = rotateLeft(current_node->left_child);
            return rotateRight(current_node);
        }
    }
    else if (balanceFactor(current_node) < -1) {
        // RR rotation
        if (balanceFactor(current_node->right_child) <= 0) {
            return rotateLeft(current_node);
        }
        // RL rotation
        else {
            current_node->right_child = rotateRight(current_node->right_child);
            return rotateLeft(current_node);
        }
    } 
    return current_node; //verify right return value   
} 

template <class Key, class Value, class Compare>
int AVLTree<Key,Value,Compare>::balanceFactor(Node current_node) {
    int left_child_height = getNodeHeight(current_node->left_child);
    int right_child_height = getNodeHeight(current_node->right_child);
    return (left_child_height - right_child_height);
}

template <class Key, class Value, class Compare>
Value AVLTree<Key,Value,Compare>::find(const Key& key) const {
    Node node = findUtil(root, key);
    if (node == nullptr){
        return nullptr;
    }
    return node->value;
}


template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::findUtil(Node current_node, const Key& key) const {
    if (current_node != nullptr) {
        Compare IsBigger;
        if (current_node->key == key) {
            return current_node;
        }
        else if (IsBigger(key,current_node->key)==false) {
            return findUtil(current_node->left_child, key);
        }
        else {
            return findUtil(current_node->right_child, key);
        }
    } 
    return nullptr;
}

template <class Key, class Value, class Compare>
void AVLTree<Key,Value,Compare>::insert(const Key& key, const Value& value) {
    if (find(key) != nullptr) {
        return;
    }
    root = insertUtil(root, key, value);
    num_of_nodes++;
}


template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::insertUtil(Node current_node, const Key& key, const Value& value) {
    if (current_node == nullptr) {
        Node tmp(new AVLTree<Key,Value,Compare>::node(key, value));
        return tmp;
    }
    Compare IsBigger;
    if (IsBigger(key,current_node->key) == false) {
        current_node->left_child = insertUtil(current_node->left_child, key, value);
    }
    else {
        current_node->right_child = insertUtil(current_node->right_child, key, value);
    }
    int left_child_height = getNodeHeight(current_node->left_child);
    int right_child_height = getNodeHeight(current_node->right_child);
    current_node->height = 1 + max(left_child_height, right_child_height); //TODO max function
    return rebalance(current_node, key);
}

template <class Key, class Value, class Compare>
void AVLTree<Key,Value,Compare>::remove(const Key &key) {
    if (find(key) == nullptr) {
        return;
    }
    root = removeUtil(root, key);
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::removeUtil(Node current_node, const Key &key) {
    assert(current_node != nullptr);
    // in case the matching node to remove was found
    if (current_node->key == key) {
        // leaf 
        if (!(current_node->left_child) && !(current_node->right_child)) {
            current_node = nullptr;
            num_of_nodes--;
            return nullptr;
        }
        // has two children
        else if (current_node->left_child && current_node->right_child) {
            Node lowest_right_child_node = findMinUtil(current_node->right_child);
            current_node->value = lowest_right_child_node->value;
            current_node->key = lowest_right_child_node->key;  
            current_node->right_child = removeUtil(current_node->right_child, current_node->key);        
        }
        // has only one child
        else {
            oneChildNodeRemove(current_node, key);
        } 
    }
    else {
        Compare isBigger;
        if (isBigger(key, current_node->key)) {
            current_node->right_child = removeUtil(current_node->right_child, key);
        }
        else {
            current_node->left_child = removeUtil(current_node->left_child, key);
        }
    }
    current_node->height = 1 + max(getNodeHeight(current_node->left_child), getNodeHeight(current_node->right_child));
    return rebalance(current_node, key);
}

template <class Key, class Value, class Compare>
void AVLTree<Key,Value,Compare>::oneChildNodeRemove(Node current_node, Key key) {
    Node child_node = (current_node->left_child != nullptr) ?
                        current_node->left_child : current_node->right_child;
    current_node->value = child_node->value;
    current_node->key = child_node->key;

    if (child_node == current_node->left_child) {
        current_node->left_child = removeUtil(current_node->left_child, current_node->key);
    }
    else {
        current_node->right_child = removeUtil(current_node->right_child, current_node->key);
    }
}

template <class Key, class Value, class Compare>
int AVLTree<Key,Value,Compare>::getSize() const {
    return this->num_of_nodes;
}

template <class Key, class Value, class Compare>
Value AVLTree<Key,Value,Compare>::findMax() const {
    Node current_max = root;

    if (current_max == nullptr) {
        return nullptr;
    }
    while (current_max->right_child != nullptr)
    {
        current_max = current_max->right_child;
    }
    return current_max->value;
}

template <class Key, class Value, class Compare>
Value AVLTree<Key,Value,Compare>::findMin() const {
    Node min_node = findMinUtil(root);
    return min_node->value;
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::findMinUtil(Node current_node) const {
    if (current_node == nullptr) {
        return nullptr;
    }
    while (current_node->left_child != nullptr)
    {
        current_node = current_node->left_child;
    }
    return current_node;
}

template <class Key, class Value, class Compare>
bool AVLTree<Key,Value,Compare>::isEmpty() const {
    return (this->root == nullptr);
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node AVLTree<Key,Value,Compare>::makeTreeFromArray(Node* array,int min_node, int max_node){
    if (max_node == min_node){
        Node new_node(new node(array[max_node]->key, array[max_node]->value));
        new_node->height = 1 + max(getNodeHeight(new_node->left_child), getNodeHeight(new_node->right_child));;
        return new_node;
    }
    else if (min_node > max_node){
        return nullptr;
    }

    int mid = (max_node + min_node) / 2;
    Node new_node(new node(array[mid]->key, array[mid]->value));
    new_node->left_child = makeTreeFromArray(array, min_node, mid - 1);
    new_node->right_child = makeTreeFromArray(array, mid + 1, max_node);
    new_node->height = 1 + max(getNodeHeight(new_node->left_child), getNodeHeight(new_node->right_child));
    return new_node;
}

template <class Key, class Value, class Compare>
void AVLTree<Key,Value,Compare>::merge(AVLTree<Key,Value,Compare> &tree) {
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

    Node* merged_array = arrayMerge(arr_t1, n1, arr_t2, n2);
    root = nullptr;
    root = makeTreeFromArray(merged_array, 0 , n1 + n2 - 1);
    num_of_nodes = n1 + n2;

    delete[] arr_t1;
    delete[] arr_t2;
    delete[] merged_array;
}

template <class Key, class Value, class Compare>
typename AVLTree<Key,Value,Compare>::Node* AVLTree<Key,Value,Compare>::arrayMerge(Node* arr1, int n1, Node* arr2, int n2) {
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
        else{
            *merged = *arr2;
            arr2++;
            array_size_2--;
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

// internal function for operating in order over noodes (not values)
template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::inOrderNodes(Functor &functor) {
    int counter = 0;
    inOrderNodesUtil(root, functor, &counter);

}

// internal utility function for operating in order over noodes (not values)
template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::inOrderNodesUtil(Node& current_node, Functor &functor, int* counter) {
    (*counter)++;
    if (current_node == nullptr) {
        return;
    }
    inOrderNodesUtil(current_node->left_child, functor,counter);
    functor(current_node);
    inOrderNodesUtil(current_node->right_child, functor, counter);
}

template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::inOrder(Functor &functor) {
    inOrderUtil(root, functor);
}

template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::inOrderUtil(Node current_node, Functor &functor) {
    if (current_node == nullptr) {
        return;
    }
    inOrderUtil(current_node->left_child, functor);
    functor(current_node->value);
    inOrderUtil(current_node->right_child, functor);
}

// internal function for operating in order over noodes (not values)
template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::reverseInOrder(Functor &functor) {
    reverseInOrderUtil(root, functor);
}

// internal utility function for operating in order over noodes (not values)
template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::reverseInOrderUtil(Node current_node, Functor &functor) {
    if (current_node == nullptr) {
        return;
    }
    reverseInOrderUtil(current_node->right_child, functor);
    functor(current_node->value);
    reverseInOrderUtil(current_node->left_child, functor);
}

template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::reverseInOrderNodes(Functor &functor) {
    reverseInOrderNodesUtil(root, functor);
}

template <class Key, class Value, class Compare>
template <typename Functor>
void AVLTree<Key,Value,Compare>::reverseInOrderNodesUtil(Node current_node, Functor &functor) {
    if (current_node == nullptr) {
        return;
    }
    reverseInOrderNodesUtil(current_node->right_child, functor);
    functor(current_node);
    reverseInOrderNodesUtil(current_node->left_child, functor);
}


#endif