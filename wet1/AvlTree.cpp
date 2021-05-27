#include "library.h"
#include "AvlTree.h"
#include <iostream>
using namespace std;

int max(int num1, int num2){
    return (num1>num2 ? num1 : num2);
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

void AvlDeleteTree(Node** root){
    Avl_Tree tree;
    tree.root=*root;
    tree.DestroyTree(*root);
}

void setNextPrev(Avl_Result* result){
    Node* node=result->result;
    if (node==nullptr){
        return;
    }
    //no parent so prev is max in left side, next is min in right side
    if (node->parent==nullptr){
        result->prev=TreeFindMax(node->left);
        result->next=TreeFindMin(node->right);
        return;
    }
    //next is either parent or right side min
    //prev is either left side max, or if no left side then prev is left grandparent 
    else if((node->id)<(node->parent->id)){ 
        if (node->right==nullptr){ //checks next
            result->next=node->parent;
        }
        else{
            Node* min=TreeFindMin(node->right);
            if (min->id<node->parent->id){
                result->next=min;
            }
            else{
                result->next=node->parent;
            }
        }
        if (node->left==nullptr){ //checks prev
            Node* grandparent;
            grandparent=node->parent;
            while (grandparent->parent!=nullptr){
                if (grandparent->parent->right==grandparent){
                    break;
                }
                else{
                    grandparent=grandparent->parent;
                }
            }
            result->prev=grandparent->parent;
        }
        else{
            Node* max=TreeFindMax(node->left);
            result->prev=max;
        }
    }
    //prev is either parent or left side max
    //next is either right side min, or if no right side then next is right grandparent 
    else if((node->id)>(node->parent->id)){ 
        if (node->left==nullptr){ //checks prev
            result->prev=node->parent;
        }
        else{
            Node* max=TreeFindMax(node->left);
            if (max->id>node->parent->id){
                result->prev=max;
            }
            else{
                result->prev=node->parent;
            }
        }
        if (node->right==nullptr){ //checks next
            Node* grandparent;
            grandparent=node->parent;
            while (grandparent->parent!=nullptr){
                if (grandparent->parent->left==grandparent){
                    break;
                }
                else{
                    grandparent=grandparent->parent;
                }
            }
            result->next=grandparent->parent;
        }
        else{
            Node* min=TreeFindMin(node->right);
            result->next=min;
        }
    }
}

// function to make "to" node have same parents and sons as "from" node,
// used for replacing "from" node by "to" node, while keeping "to"'s data
// before removing "from" from the tree.
void SetParentAndSons(Node* to, Node* from){
    to->parent=from->parent;
    to->right=from->right;
    to->left=from->left;
    if (to->parent!=nullptr){
        if (to->id<to->parent->id){
            to->parent->left=to;
        }
        else{
            to->parent->right=to;
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

Avl_Result AvlRemove (Node** node, Node* remove){
    Avl_Result result;
    result=AvlRemove(node,remove->id);
    return result;
}

Avl_Result AvlRemove (Node** node, s64 id){
    Avl_Result result;
    Avl_Tree tree;
    tree.root=*node;
    result.result=tree.RemoveAux(*node,id);
    if (result.result!=nullptr){
        result.found=true;
        result.result->left=nullptr;
        result.result->right=nullptr;
        result.result->parent=nullptr;
        result.result->height=1;
    }
    if (tree.root!=*node){
        *node=tree.root;
    }
    return result;
}

Avl_Result AvlInsert (Node** root, Node* node){
    Avl_Result result = AvlFind(*root, node->id);
    if(result.found == true) {
        return result;
    }
    
    *root = InsertAux(*root, node);
    result.result=node;
    setNextPrev(&result);
    
    return result;
}

Avl_Result AvlFind (Node* node, s64 id){
    Avl_Result result;
    if (node==nullptr){
        result.found=false;
        result.result=nullptr;
        return result;
    }
    else if (node->id==id){
        result.found=true;
        result.result=node;
        return result;
    }
    else if (node->id>id){
        result=(AvlFind(node->left,id));
    }
    else if (node->id<id){
        result=(AvlFind(node->right,id));
    }
    return result;
}



void Avl_Tree::DestroyTree(Node* node){
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
    else if (new_node->id<root->id){
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
    else if (new_node->id>root->id){
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
    return root;
}

Node* Avl_Tree::RemoveAux(Node* node, s64 id){
    Node* result=nullptr;
    if (node==nullptr){
        return nullptr;
    }
    else if (id!=node->id)
    {
        if (id < node->id){
            result=RemoveAux(node->left,id);
        }
        else if (id > node->id){
            result=RemoveAux(node->right,id);
        }
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
                    if (node->id<node->parent->id){
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
                    if (node->id<node->parent->id){
                        node->parent->left=node->left;
                    }
                    else{
                        node->parent->right=node->left;
                    }
                    node->parent->height=CalcHeight(node->parent);
                }
            }
            else if ((node->right==nullptr)&&(node->left==nullptr)){ //both sons are nullptr
                if (node->parent==nullptr){
                    this->root=nullptr;
                }
                else{
                    if (node->id<node->parent->id){
                        node->parent->left=nullptr;
                    }
                    else{
                        node->parent->right=nullptr;
                    }
                    node->parent->height=CalcHeight(node->parent);
                }  
            }
            return result;
        }
        else if (node->right!=nullptr&&node->left!=nullptr){ //both sons are not nullptr
            Node* temp;
            Node* replace_node;
            if (TreeDiff(node)>0){
                temp=TreeFindMax(node->left);
                replace_node=RemoveAux(node->left,temp->id); // remove the son to replace node
                SetParentAndSons(replace_node, node); // replace node
            }
            else{
                temp=TreeFindMin(node->right);
                replace_node=RemoveAux(node->right,temp->id);
                SetParentAndSons(replace_node, node);
            }
            replace_node->height=CalcHeight(replace_node);
            if (replace_node->parent==nullptr){
                this->root=replace_node;
            }
            result=node;
        }
    }
    node->height=CalcHeight(node);
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
            if (node->id<temp_parent->id){
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
void Avl_Tree::Inorder(Node* node){
    if (node==nullptr){
        return;
    }
    Inorder(node->left);
    cout << node->id << endl;
    Inorder(node->right);
}

void Avl_Tree::Destroy(){
    this->DestroyTree(this->root);
}

void Avl_Tree::Insert(s64 Insert_id){
    Node* node=new Node();
    node->id=Insert_id;
    this->root=InsertAux(root,node);
}

Node* Avl_Tree::Remove(s64 remove_id){
    Node* result;
    result=RemoveAux(this->root,remove_id);
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

void Avl_Tree::Print(){
    DebugDisplay(this->root,0);
    cout<<endl;
    cout<<endl;
}



// void AvlFindNode(Node *root, s64 id);

// void AvlInsertNode(Node *root, s64 id);

// void AvlRemoveNode(Node *root, s64 id);


// int main(){
//     cout << "beginning"<<endl;
//     Avl_Tree one;
//     Node** root;
//     root=&one.root;
//     one.Insert(2);
//     one.Insert(3);
//     // AvlRemove(root,2);
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
//     // result=one.Remove(20);
//     Avl_Result res;
//     // result=one.Remove(6);
//     AvlRemove(root,6);
//     one.root=*root;
//     one.Print();
//     // result=one.Remove(13);
//     // one.Print();
//     AvlRemove(root,13);
//     one.root=*root;
//     one.Print();
//     res=AvlRemove(root,14);
//     one.root=*root;
//     one.Print();
//     res=AvlInsert(root,res.result);
//     one.Print();
//     res=AvlRemove(&res.result,res.result);
//     one.root=*root;
//     one.Print();
//     one.Insert(1);
//     one.Print();
//     res=AvlRemove(root,39);
//     one.root=*root;
//     one.Print();
//     res=AvlRemove(root,37);
//     one.root=*root;
//     one.Print();
//     res=AvlRemove(root,32);
//     one.root=*root;
//     one.Print();
//     res=AvlRemove(root,30);
//     one.root=*root;
//     one.Print();
//     res=AvlRemove(root,7);
//     one.root=*root;
//     one.Print();
//     // result=one.Remove(2);
//     AvlRemove(root,2);
//     one.root=*root;
//     one.Print();
//     AvlDeleteTree(root);
//     cout<<"end"<<endl;
// }

