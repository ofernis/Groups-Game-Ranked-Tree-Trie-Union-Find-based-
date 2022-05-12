#ifndef TRIE_H
#define TRIE_H

#include <memory>
#include <string>
#include <iostream>

#define ALPHABET_SIZE 10

template <class T>
class Trie {
    private:
        // trie node
        class node {
            public:
            std::shared_ptr<node> children[ALPHABET_SIZE];
            bool is_end_of_word;
            T object;

            node() {
                 object = nullptr ;
                is_end_of_word = false;
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    children[i] = nullptr;
                }
            }
        };

        using Node = std::shared_ptr<node>;

        bool haveChildren(Node &current_node);
        bool remove_util(Node &current_node, std::string &key);
        void merge_util(std::string &prefix_str, Node &current_node);
        void print_util(std::string &prefix_str, Node &current_node);


        Node root;
        int size; //needed?

    public:
        Trie() : root(Node(new node)), size(0) {
            assert(root != nullptr);
        }
        Trie(const Trie &other) = default;
        Trie& operator=(const Trie &other) = default;
        ~Trie() = default;

        // if not present, inserts key into trie
        // if the key is prefix of trie node, just marks leaf node
        void insert(std::string &key, const T &object);

        void remove(std::string &key);

        // returns true if key presents in trie, else
        // false
        bool find(std::string &key) const;

        // merges another trie into this trie
        void merge(Trie &trie);  

        void print(); 

        int getSize() {
            return size;
        }

        T getObject(std::string &key);    

};

/////////////////////////////
// methods implementations //
/////////////////////////////


    template <class T>
    void Trie<T>::insert(std::string &key, const T &object) {
        Node current = root;

        for (int i = 0; i < key.length(); i++) {
            int index = key[i] - '0';
            if (current->children[index] == nullptr) {
                current->children[index] = Node(new node);
            }
            current = current->children[index];
        }
        if (current->is_end_of_word == true) {
            throw std::runtime_error("key already exists");
        }
        size++;
        // mark last node as leaf
        current->is_end_of_word = true;
        current->object = object;
    }

    template <class T>
    void Trie<T>::remove(std::string &key) {
        if (remove_util(root, key) == true) {
            size--;
        }
        assert(root != nullptr);            
    }

    template <class T>
    bool Trie<T>::haveChildren(Node &current_node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (current_node->children[i] != nullptr) { 
                return true;
            }
        }
        return false;
    }

    template <class T>
    bool Trie<T>::remove_util(Node &current_node, std::string &key) {
        if (current_node == nullptr) {
            return false;
        }

        // if the end of the key is not reached
        if (key.length() != 0) {
            // recur for the node corresponding to the next character in the key
            // and if it returns true, delete the current node (if it is non-leaf)
            std::string sub_key = key.substr(1);
            if (current_node->children[key[0] - '0'] != nullptr &&
                remove_util(current_node->children[key[0] - '0'], sub_key) == true &&
                current_node->is_end_of_word == false) {
                    if (haveChildren(current_node) == false) {
                        if (current_node != root){
                            //delete current_node; 
                            current_node = nullptr;
                        }
                        return true; 
                    }
                    else {
                        return false;
                    }
                }
        }
        // if the end of the key is reached
        if (key.length() == 0 && current_node->is_end_of_word == true) {
            if (haveChildren(current_node) == false) {
                //delete current_node;
                current_node = nullptr;
                // delete the non-leaf parent nodes
                return true;
            }
            // if the current node is a leaf node and has children
            else {
                // mark the current node as a non-leaf node (DON'T DELETE IT)
                current_node->is_end_of_word = false;
    
                // don't delete its parent nodes
                return false;
            }
        }
        return false;
    }

    template <class T>
    bool Trie<T>::find(std::string &key) const {
        Node current = root;

        for (int i = 0; i < key.length(); i++) {
            int index = key[i] - '0';
            if (current->children[index] == nullptr) {
                return false;
            }
            current = current->children[index];
        }

        return (current->is_end_of_word);
    }

    template <class T>
    void Trie<T>::merge(Trie &trie) {
        if (trie.size == 0) {
            return;
        }
        std::string prefix_str;
        merge_util(prefix_str, trie.root);
    }

    template <class T>
    void Trie<T>::merge_util(std::string &prefix_str, Node &current_node) {
        if (current_node->is_end_of_word == true) {
            this->insert(prefix_str, current_node->object);
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            char next_char = '0' + i; 
            Node child = current_node->children[i];
            if (child != nullptr) {
                prefix_str.push_back(next_char);
                merge_util(prefix_str, child);
                prefix_str.pop_back();
            }
        }
    }

    template <class T>
    T Trie<T>::getObject(std::string &key) {
        Node current = root;

        for (int i = 0; i < key.length(); i++) {
            int index = key[i] - '0';
            if (current->children[index] == nullptr) {
                return nullptr;
            }
            current = current->children[index];
        }

        if (current->is_end_of_word == false) {
            return nullptr;
        }
        return current->object;
    }

    template <class T>
    void Trie<T>::print() {
        std::string prefix_str;
        print_util(prefix_str, this->root);
    }

    template <class T>
    void Trie<T>::print_util(std::string &prefix_str, Node &current_node) {
        if (current_node->is_end_of_word == true) {
            std::cout << prefix_str << std::endl;
        }

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            char next_char = '0' + i; 
            Node child = current_node->children[i];
            if (child != nullptr) {
                prefix_str.push_back(next_char);
                print_util(prefix_str, child);
                prefix_str.pop_back();
            }
        }
    }

#endif