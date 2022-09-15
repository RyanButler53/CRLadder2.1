#include "treequeue.hpp"
#include <iostream>

TreeQueue::TreeQueue(std::vector<Player*>& playerVector):
    size_{0}, root_{nullptr}, playerVec_{playerVector}
    {
        // nothing to do here
    }

TreeQueue::~TreeQueue(){
    for (Player* p : playerVec_){
        if (p->queueStatus()){
            p->changeQueueStatus();
        }
    }
    destructorHelper(root_);
}

void TreeQueue::destructorHelper(Node*& root){
    if (root != nullptr) {
         //std::cout << "destructor called on " <<root->arrIndex_ << std::endl;
        if (root->left_ != nullptr){
            //std::cout << "going left to "<< root->left_->arrIndex_ << std::endl;
            destructorHelper(root->left_);
        }
        if (root->right_ != nullptr) {
            //std::cout << "going right to " << root->right_->arrIndex_ << std::endl;
            destructorHelper(root->right_);
        }
        // before deleting, remove player from queue
        size_t ind = root->arrIndex_;
        playerVec_[ind]->changeQueueStatus();
        //std::cout << "Destructor delete " << ind << std::endl;
        delete root;
    }
}

size_t TreeQueue::size() const{
    return size_;
}

void TreeQueue::addPlayer(const size_t& arrInd, long long enter){
    bool inTree = playerVec_[arrInd]->queueStatus();
    if (!inTree)
    {
        addPlayerHelper(arrInd, root_, enter);
        size_++;
    }
}

void TreeQueue::addPlayerHelper(const size_t& arrInd, Node*& root, long long enter){
    if (root == nullptr) {
        root = new Node(arrInd, enter);
        playerVec_[arrInd]->changeQueueStatus();
    } else {
        short addTrophies = playerVec_[arrInd]->getTrophies();
        short curTrophies = playerVec_[root->arrIndex_]->getTrophies();
        if (addTrophies > curTrophies){
            addPlayerHelper(arrInd, root->right_, enter);
        } else {
            addPlayerHelper(arrInd, root->left_, enter);
        }
    }
}

bool TreeQueue::exists(size_t arrInd) const {
    return playerVec_[arrInd]->queueStatus();
}

size_t TreeQueue::findOpponent(size_t arrInd, u_int8_t mmType) const {
    if (root_ == nullptr){
        return arrInd;
    }
    if (exists(arrInd)){
        return arrInd; // Player is already in queue
    } else {
        size_t opp = findOpponentHelper(root_, arrInd, mmType);
        return opp;
    }
}
size_t TreeQueue::findOpponentHelper(Node* tree, size_t ind,u_int8_t mmType) const {
    if (tree == nullptr){
        return ind; 
    } else { 
        short searchTrophies = playerVec_[ind]->getTrophies();
        short curTrophies = playerVec_[tree->arrIndex_]->getTrophies();
        Player &p1 = *playerVec_[ind];
        Player &p2 = *playerVec_[tree->arrIndex_];
        if (p1.matchAllowed(p2, mmType))
        {
            return tree->arrIndex_;
        }
        else if (curTrophies < searchTrophies)
        {
            return findOpponentHelper(tree->right_, ind, mmType);
        }
        else
        {
            return findOpponentHelper(tree->left_, ind, mmType);
        }
    }
}
//optimization is for find opp to return a node*. Just call delete Helper!
long long TreeQueue::deletePlayer(size_t arrInd){
    if (exists(arrInd))
    {
        long long enter = deleteHelper(arrInd, root_);
        playerVec_[arrInd]->changeQueueStatus();
        --size_;
        return enter;
    }
    return 0; //no changes if its in the tree. 
}

long long TreeQueue::deleteHelper(size_t arrInd, Node*& tree){
    short searchTrophies = playerVec_[arrInd]->getTrophies();
    short curTrophies = playerVec_[tree->arrIndex_]->getTrophies();
    if (arrInd == tree->arrIndex_) {
        // delete and change queue status
        long long enteredAt = tree->enteredQueue_;
        if (tree->left_ == nullptr and tree->right_ == nullptr)
        {
            delete tree;
            tree = nullptr;
        }
        else if (tree->left_ != nullptr and tree->right_ == nullptr)
        {
            //one child case.  Move the left child up.
            Node* temp = tree;
            tree = tree->left_;
            delete temp;
            temp = nullptr;
        } else if (tree->left_ == nullptr and tree->right_ != nullptr) {
            //one right child case
            Node *temp = tree;
            tree = tree->right_;
            delete temp;
            temp = nullptr; // prevents dangling pointer
        } else {
            //2 children. Find min greater than tree. 
            Node* temp = tree;
            Node* min = tree->right_;
            while (min->left_  != nullptr){
                min = min->left_;
            }
            size_t newArrInd = min->arrIndex_;
            long long minTrophiesEnter = deleteHelper(newArrInd, tree);
            tree->arrIndex_ = newArrInd;
            Node* newNode = new Node(newArrInd, minTrophiesEnter);
            newNode->left_ = tree->left_;
            newNode->right_ = tree->right_;
            delete temp;
            tree = newNode;
        }
        return enteredAt;
    }
    else if (curTrophies < searchTrophies)
    {
        return deleteHelper(arrInd, tree->right_);
    }
    else if (curTrophies >= searchTrophies)
    {
        return deleteHelper(arrInd, tree->left_);
    }
    return 0;
}

TreeQueue::Node::Node(size_t arrayIndex, long long enter) : 
    arrIndex_{arrayIndex},
    left_{nullptr},
    right_{nullptr},
    enteredQueue_{enter}
{
    // nothing to do here
}
    