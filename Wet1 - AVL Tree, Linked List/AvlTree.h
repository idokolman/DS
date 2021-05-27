#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "library.h"
#include "node.h"
#include <iostream>

struct Avl_Result {
    Node *result = nullptr;
    Node *next = nullptr;
    Node *prev = nullptr;
    
    bool found = false;
};

//Avl_Result
Avl_Result AvlInsert (Node** root, Node* node);
Node* InsertAux(Node* root, Node* new_node);
Avl_Result AvlRemove (Node** node, s64 id);
Avl_Result AvlRemove (Node** node, Node* remove);
Avl_Result AvlFind (Node* node, s64 id);
void AvlDeleteTree(Node** root);

class Avl_Tree{
    public:
    Node* root;
    Avl_Tree(){
        root=nullptr;
    }
    void Destroy();
    Node* RemoveAux(Node* node, s64 id);
    void Insert(s64 insert_id);
    Node* Remove(s64 remove_id);
    void Print();
    void DestroyTree(Node* node);
    void Inorder(Node* node);
    // void ChangeID(s64 new_id){
    //     this->root->ChangeID(new_id);
    // }
};

#endif //AVL_TREE_H