#include "RankedTree.h"
#include <cassert>
#include <iostream>
using namespace std;

int max(int num1, int num2){
    return (num1>num2 ? num1 : num2);
}

bool BiggerThan(Node* left, Node* right){
    if (left->time>right->time){
        return true;
    }
    else if (right->time>left->time){
        return false;
    }
    else if (left->id<right->id){
        return true;
    }
    else {
        return false;
    }
}

int GetHeight(Node* node){
    if (node==nullptr){
        return 0;
    }
    return node->height;
}

int CalcHeight(Node* node){
    if (node==nullptr){
        return 0;
    }
    return (max(GetHeight(node->left),GetHeight(node->right))+1);
}

int CalcRank(Node* node){
    if (node==nullptr){
        return 0;
    }
    int rank_left=0,rank_right=0;
    if (node->left){
        rank_left = node->left->rank;
    }
    if (node->right){
        rank_right = node->right->rank;
    }
    return (rank_left + rank_right + 1);
}

Node* TreeFindMin(Node* node){
    if (node==nullptr){
        return nullptr;
    }
    if (node->left==nullptr){
        return node;
    }
    return (TreeFindMin(node->left));
}

Node* TreeFindMax(Node* node){
    if (node==nullptr){
        return nullptr;
    }
    if (node->right==nullptr){
        return node;
    }
    return (TreeFindMax(node->right));
}

int TreeDiff(Node* node){
    return (GetHeight(node->left)-GetHeight(node->right));
}

void RankedDeleteTree(Node** root){
    Ranked_Tree tree;
    tree.root=*root;
    tree.DestroyTree(*root);
}

// void setNextPrev(Node** result){
//     Node* node=result->result;
//     if (node==nullptr){
//         return;
//     }
//     //no parent so prev is max in left side, next is min in right side
//     if (node->parent==nullptr){
//         result->prev=TreeFindMax(node->left);
//         result->next=TreeFindMin(node->right);
//         return;
//     }
//     //next is either parent or right side min
//     //prev is either left side max, or if no left side then prev is left grandparent 
//     else if(BiggerThan(node, node->parent)){ 
//         if (node->right==nullptr){ //checks next
//             result->next=node->parent;
//         }
//         else{
//             Node* min=TreeFindMin(node->right);
//             if (BiggerThan(min, node->parent)){
//                 result->next=min;
//             }
//             else{
//                 result->next=node->parent;
//             }
//         }
//         if (node->left==nullptr){ //checks prev
//             Node* grandparent;
//             grandparent=node->parent;
//             while (grandparent->parent!=nullptr){
//                 if (grandparent->parent->right==grandparent){
//                     break;
//                 }
//                 else{
//                     grandparent=grandparent->parent;
//                 }
//             }
//             result->prev=grandparent->parent;
//         }
//         else{
//             Node* max=TreeFindMax(node->left);
//             result->prev=max;
//         }
//     }
//     //prev is either parent or left side max
//     //next is either right side min, or if no right side then next is right grandparent 
//     else if(BiggerThan(node, node->parent)){ 
//         if (node->left==nullptr){ //checks prev
//             result->prev=node->parent;
//         }
//         else{
//             Node* max=TreeFindMax(node->left);
//             if (BiggerThan(max, node->parent)){
//                 result->prev=max;
//             }
//             else{
//                 result->prev=node->parent;
//             }
//         }
//         if (node->right==nullptr){ //checks next
//             Node* grandparent;
//             grandparent=node->parent;
//             while (grandparent->parent!=nullptr){
//                 if (grandparent->parent->left==grandparent){
//                     break;
//                 }
//                 else{
//                     grandparent=grandparent->parent;
//                 }
//             }
//             result->next=grandparent->parent;
//         }
//         else{
//             Node* min=TreeFindMin(node->right);
//             result->next=min;
//         }
//     }
// }

// function to make "to" node have same parents and sons as "from" node,
// used for replacing "from" node by "to" node, while keeping "to"'s data
// before removing "from" from the tree.
void SetParentAndSons(Node* to, Node* from){
    to->parent=from->parent;
    to->right=from->right;
    to->left=from->left;
    if (to->parent!=nullptr){
        if (BiggerThan(to, to->parent)){
            to->parent->right=to;
        }
        else{
            to->parent->left=to;
        }
    }
    if (to->right!=nullptr){
        to->right->parent=to;
    }
    if (to->left!=nullptr){
        to->left->parent=to;
    }
}

Node* LLrotation(Node* &node){
    Node* new_node = node->left;
    node->left = new_node->right;
    if (node->left!=nullptr){
        node->left->parent=node;
    }
    new_node->right=node;
    node->parent=new_node;
    new_node->parent=nullptr;
    node->height=CalcHeight(node);
    new_node->height=CalcHeight(new_node);
    node->rank=CalcRank(node);
    new_node->rank=CalcRank(new_node);
    return new_node;
}

Node* RRrotation(Node* &node){
    Node* new_node = node->right;
    node->right = new_node->left;
    new_node->left=node;
    node->parent=new_node;
    if (node->right!=nullptr){
        node->right->parent=node;
    }
    new_node->parent=nullptr;
    node->height=CalcHeight(node);
    new_node->height=CalcHeight(new_node);
    node->rank=CalcRank(node);
    new_node->rank=CalcRank(new_node);
    return new_node;
}

Node* LRrotation(Node* &node){
    if (node->left!=nullptr){
        node->left=RRrotation(node->left);
    }
    node=LLrotation(node);
    return node;
}

Node* RLrotation(Node* &node){
    if (node->right!=nullptr){
        node->right=LLrotation(node->right);
    }
    node=RRrotation(node);
    return node;
}

Node* RankedRemove (Node** root, Node* remove){
    if (root == nullptr || remove == nullptr){
        return nullptr;
    }
    return RankedRemove(root,remove->id,remove->time);
}

Node* RankedRemove (Node** root, s64 id, s32 time){
    Node* result;
    Ranked_Tree tree;
    tree.root=*root;
    result=tree.RemoveAux(*root,id,time);
    if (result!=nullptr){
        result->left=nullptr;
        result->right=nullptr;
        result->parent=nullptr;
        result->height=1;
        result->rank=1;
    }
    if (tree.root!=*root){
        *root=tree.root;
    }
    return result;
}

Node* RankedInsert (Node** root, Node* node){
    *root = InsertAux(*root, node);
    
    return node;
}

Node* RankedFind (Node* root, s64 id, s32 time){
    if(root == nullptr) return nullptr;
    
    if (root->id == id) {
        assert(root->time == time);
        return root;
    }
    
    Node* result = nullptr;
    if (time < root->time) {
        result = RankedFind(root->left, id, time);
    } else if (time > root->time) {
        result = RankedFind(root->right, id, time);
    } else { // time == root->time
        if (id < root->id) {
            result = RankedFind(root->right, id, time);
        } else {
            result = RankedFind(root->left, id, time);
        }
    }
    
    return result;
}

Node* RankedFind (Node* root, Node* node) {
    if ((root == nullptr) || (node == nullptr)) return nullptr;
    return RankedFind(root, node->id, node->time);
}


void Ranked_Tree::DestroyTree(Node* node){
    if (node==nullptr){
        return;
    }
    DestroyTree(node->left);
    DestroyTree(node->right);
    delete node; // NOTE(IK): do we need new desturctor for the new node?
}

Node* InsertAux(Node* root, Node* new_node){
    if (root==nullptr){
        root=new_node;
    }
    else if (BiggerThan(root, new_node)){
        if (root->left==nullptr){
            root->left=new_node;
            new_node->parent=root;
        }
        else{
            root->left=InsertAux(root->left, new_node);
        }
        if(TreeDiff(root)==2)
        {
            if (TreeDiff(root->left)>=0){
                root=LLrotation(root);
            }
            else if(TreeDiff(root->left)==-1){
                root=LRrotation(root);
            }
        }
        root->left->parent=root;
    }
    else if (BiggerThan(new_node, root)){
        if (root->right==nullptr){
            root->right=new_node;
            new_node->parent=root;
        }
        else{
            root->right=InsertAux(root->right, new_node);
        }
        if(TreeDiff(root)==-2)
        {
            if (TreeDiff(root->right)<=0){
                root=RRrotation(root);
            }
            else if(TreeDiff(root->right)==1){
                root=RLrotation(root);
            }
        }
        root->right->parent=root;
    }
    root->height=CalcHeight(root);
    root->rank=CalcRank(root);
    return root;
}

Node* Ranked_Tree::RemoveAux(Node* node, s64 id, s32 time){
    Node* result=nullptr;
    if (node==nullptr){
        return nullptr;
    }
    else if (id!=node->id)
    {
        if (time < node->time){
            result=RemoveAux(node->left,id,time);
        }
        else if (time > node->time){
            result=RemoveAux(node->right,id,time);
        }
        else if (time==node->time)
        {
            if (id < node->id){
                result=RemoveAux(node->right,id,time);
            }
            else {
                result=RemoveAux(node->left,id,time);
            }
        }
        //node->rank=CalcRank(node);
    }
    else if (id==node->id)
    {
        result=node;
        if (node->right==nullptr || node->left==nullptr){ // if doesn't have 2 sons
            if ((node->left==nullptr)&&(node->right!=nullptr)){ //only have right son
                node->right->parent=node->parent;
                if (node->parent==nullptr){
                    this->root=node->right;
                }
                else{
                    if (BiggerThan(node->parent, node)){
                        node->parent->left=node->right;
                    }
                    else{
                        node->parent->right=node->right;
                    }
                }
            }
            else if ((node->right==nullptr)&&(node->left!=nullptr)){ //only have left son
                node->left->parent=node->parent;
                if (node->parent==nullptr){
                    this->root=node->left;
                }
                else{
                    if (BiggerThan(node->parent, node)){
                        node->parent->left=node->left;
                    }
                    else{
                        node->parent->right=node->left;
                    }
                    node->parent->height=CalcHeight(node->parent);
                    node->parent->rank=CalcRank(node->parent);
                }
            }
            else if ((node->right==nullptr)&&(node->left==nullptr)){ //both sons are nullptr
                if (node->parent==nullptr){
                    this->root=nullptr;
                }
                else{
                    if (BiggerThan(node->parent, node)){
                        node->parent->left=nullptr;
                    }
                    else{
                        node->parent->right=nullptr;
                    }
                    node->parent->height=CalcHeight(node->parent);
                    node->parent->rank=CalcRank(node->parent);
                }  
            }
            return result;
        }
        else if (node->right!=nullptr&&node->left!=nullptr){ //both sons are not nullptr
            Node* temp;
            Node* replace_node;
            if (TreeDiff(node)>0){
                temp=TreeFindMax(node->left);
                replace_node=RemoveAux(node->left,temp->id,time); // remove the son to replace node
                SetParentAndSons(replace_node, node); // replace node
            }
            else{
                temp=TreeFindMin(node->right);
                replace_node=RemoveAux(node->right,temp->id,time);
                SetParentAndSons(replace_node, node);
            }
            replace_node->height=CalcHeight(replace_node);
            replace_node->rank=CalcRank(replace_node);
            if (replace_node->parent==nullptr){
                this->root=replace_node;
            }
            result=node;
        }
    }
    node->height=CalcHeight(node);
    node->rank=CalcRank(node);
    if (TreeDiff(node)==2||TreeDiff(node)==-2){
        Node* temp_parent=node->parent;
        if(TreeDiff(node)==2){
            if (TreeDiff(node->left)>=0){
                node=LLrotation(node);
            }
            else if(TreeDiff(node->left)==-1){
                node=LRrotation(node);
            }
            node->left->parent=node;
        }
        if(TreeDiff(node)==-2){
            if (TreeDiff(node->right)<=0){
                node=RRrotation(node);
            }
            else if(TreeDiff(node->right)==1){
                node=RLrotation(node);
            }
            node->right->parent=node;
        }
        node->parent=temp_parent;
        if (temp_parent!=nullptr){
            if (BiggerThan(temp_parent, node)){
                temp_parent->left=node;
            }
            else{
                temp_parent->right=node;
            }
        }
        else{
            this->root=node;
        }
    }
    return result;
}

// prints inorder
void Ranked_Tree::Inorder(Node* node){
    if (node==nullptr){
        return;
    }
    Inorder(node->left);
    std::cout << node->id << endl;
    Inorder(node->right);
}

void Ranked_Tree::Destroy(){
    this->DestroyTree(this->root);
}

void Ranked_Tree::Insert(s64 Insert_id){
    Node* node=new Node();
    node->id=Insert_id;
    this->root=InsertAux(root,node);
}

Node* Ranked_Tree::Remove(s64 remove_id){
    Node* result;
    result=RemoveAux(this->root,remove_id,0);
    return result;
}

void DebugDisplay(Node* node, int level)
{
    int i;
    if (node!=NULL)
    {
        DebugDisplay(node->right, level + 1);
        printf("\n");
        if (level == 0)
            cout<<"Root -> ";
        if (level!=0){
            for (i = 0; i < level+1; i++)
                cout<<"        ";
        }
        cout<<node->id;
        DebugDisplay(node->left, level + 1);
    }
}

void Ranked_Tree::Print(){
    DebugDisplay(this->root,0);
    cout<<endl;
    cout<<endl;
}




// int main(){
//     cout << "beginning"<<endl;
//     Ranked_Tree one;
//     Node** root;
//     root=&one.root;
//     one.Insert(2);
//     one.Insert(3);
//     // RankedRemove(root,2);
//     // one.root=*root;
//     // one.Print();
//     one.Insert(4);
//     one.Insert(6);
//     one.Insert(7);
//     one.Insert(9);
//     one.Insert(12);
//     one.Insert(13);
//     one.Insert(14);
//     one.Insert(17);
//     one.Insert(19);
//     one.Insert(20);
//     one.Insert(21);
//     one.Insert(22);
//     one.Insert(33);
//     one.Insert(34);
//     one.Insert(37);
//     one.Insert(39);
//     one.Insert(30);
//     one.Insert(31);
//     one.Insert(32);
//     one.Print();
//     Node* result;
//     result=one.Remove(20);
//     one.Print();
//     Node* res;
//     // result=one.Remove(6);
//     RankedRemove(root,6,0);
//     one.root=*root;
//     one.Print();
//     // result=one.Remove(13);
//     // one.Print();
//     RankedRemove(root,13,0);
//     one.root=*root;
//     one.Print();
//     res=RankedRemove(root,14,0);
//     one.root=*root;
//     one.Print();
//     res=RankedInsert(root,res);
//     one.Print();
//     res=RankedRemove(&one.root,res);
//     one.root=*root;
//     one.Print();
//     one.Insert(1);
//     one.Print();
//     // res=RankedRemove(root,39);
//     // one.root=*root;
//     // one.Print();
//     // res=RankedRemove(root,37);
//     // one.root=*root;
//     // one.Print();
//     // res=RankedRemove(root,32);
//     // one.root=*root;
//     // one.Print();
//     // res=RankedRemove(root,30);
//     // one.root=*root;
//     // one.Print();
//     // res=RankedRemove(root,7);
//     // one.root=*root;
//     // one.Print();
//     // // result=one.Remove(2);
//     // RankedRemove(root,2);
//     // one.root=*root;
//     // one.Print();
//     res=RankedRemove(root,22,0);
//     one.root=*root;
//     one.Print();
//     RankedDeleteTree(root);
//     cout<<"end"<<endl;
// }

