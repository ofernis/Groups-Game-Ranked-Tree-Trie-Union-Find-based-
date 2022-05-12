#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <iostream>
#include <memory>
#include <cmath>
#include <new>
#include <assert.h>
#include <typeinfo>


template <class Data>
class UnionFind {
    class group {
        public:
        int id;
        int size_of_up_tree;
        Data group_data;
        std::shared_ptr<group> parent;
        group(int id = -1): id(id), size_of_up_tree(1),group_data(nullptr), parent(nullptr){} 
        bool operator==(const group& group2){
            return this->id == group2.id;
        }
    };

    typedef std::shared_ptr<group> Group;
    Group* groups;
    int num_of_groups;
    
    Group findUtil(int id);
    void shrinkRoutes(Group& start, Group& root);


    public:
        UnionFind (int groups_number);
        ~UnionFind();
        int findId(int id);
        Data findData(int id);
        void setData(int id, Data data);
        void makeUnion(int id1, int id2);
        int getGroupSize(int id);
};

template <class Data>
UnionFind<Data>::UnionFind (int groups_number){ 
        num_of_groups = groups_number;
        groups = new Group[groups_number +1];
        groups[0] = nullptr;
        for (int i = 1; i <= num_of_groups;i++){
            groups[i] = Group(new group(i));
        }
    }

template <class Data>
UnionFind<Data>::~UnionFind(){
        delete[] groups;
    }

template <class Data>
typename UnionFind<Data>::Group UnionFind<Data>::findUtil(int id){

    Group start = groups[id];
    Group current_group = start;
    int counter =0;
    while (current_group->parent != nullptr){
        assert((*current_group->parent == *current_group) == false);
        Group temp = current_group->parent;
        current_group = temp;
        
        
        
        counter++;
        assert(counter < 1000);   
    }
    shrinkRoutes(start, current_group);
    return current_group;
}

template <class Data>
void UnionFind<Data>::shrinkRoutes(Group& start, Group& root){
    if (*start == *root){
        return;
    }
    Group current_group = start;
    while ((*(current_group->parent) == *root) == false ){
        Group temp = current_group->parent;
        current_group->parent = root;
        current_group = temp;
    }
}

template <class Data>
int UnionFind<Data>::findId(int id){
    if (id < 1 || id > num_of_groups)
        return -1;
    return findUtil(id)->id;
}
template <class Data>
Data UnionFind<Data>::findData(int id){
    if (id < 1 || id > num_of_groups)
        return nullptr;
    return findUtil(id)->group_data;
}

template <class Data>
void UnionFind<Data>::makeUnion(int id1, int id2){
    if (id1 == id2){
        return;
    }
    Group first_group = findUtil(id1);
    Group second_group = findUtil(id2);
    if (*first_group == *second_group){
        return;
    }
    if (first_group->size_of_up_tree >= second_group->size_of_up_tree){
        second_group->parent = first_group;
        first_group->size_of_up_tree += second_group->size_of_up_tree;
    }
    else {
        first_group->parent = second_group;
        second_group->size_of_up_tree += first_group->size_of_up_tree;
    }
}

template <class Data>
int UnionFind<Data>::getGroupSize(int id) {
    return findUtil(id)->size_of_up_tree;
}
template <class Data>
void UnionFind<Data>::setData(int id, Data data){
    Group group = findUtil(id);
    if (group == nullptr){
        return;
    }
    group->group_data = data;
}

#endif