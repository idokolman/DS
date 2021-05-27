#ifndef _RANKED_TREE_H
#define _RANKED_TREE_H

#include "library2.h"
#include "node.h"
#include <iostream>


//Node*
Node* RankedInsert (Node** root, Node* node);
Node* InsertAux(Node* root, Node* new_node);
Node* RankedRemove (Node** root, s64 id, s32 time);
Node* RankedRemove (Node** root, Node* remove);
Node* RankedFind (Node* root, s64 id, s32 time);
Node* RankedFind (Node* root, Node* node);
void RankedDeleteTree(Node** root);

class Ranked_Tree{
    public:
    Node* root;
    Ranked_Tree(){
        root=nullptr;
    }
    void Destroy();
    Node* RemoveAux(Node* node, s64 id, s32 time);
    void Insert(s64 insert_id);
    Node* Remove(s64 remove_id);
    void Print();
    void DestroyTree(Node* node);
    void Inorder(Node* node);
    // void ChangeID(s64 new_id){
    //     this->root->ChangeID(new_id);
    // }
};


#endif // _RANKED_TREE_H