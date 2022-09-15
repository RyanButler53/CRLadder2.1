#ifndef TREEQUEUE_INCLUDED_HPP
#define TREEQUEUE_INCLUDED_HPP

#include "player.hpp"
#include <stdint.h>
#include <vector>

class TreeQueue{
    private:
        struct Node{
            size_t arrIndex_;
            Node* left_;
            Node* right_;
            long long enteredQueue_;

            // Constructors and destructor
            Node() = delete;
            ~Node() = default;
            Node(size_t arrayIndex, long long enter);
            Node(const Node&) = delete;
            Node &operator=(const Node &) = delete;
        };

        size_t size_;
        Node *root_;
        std::vector<Player*>& playerVec_;        

        // helper methods
        void destructorHelper(Node*& root);
        void addPlayerHelper(const size_t& index, Node*& root, long long enterTime);
        size_t findOpponentHelper(Node* tree, size_t ind, u_int8_t mmType) const;
        long long deleteHelper(size_t arrInd, Node*& tree);

    public: 
        // Constructors and Destructor
        TreeQueue(std::vector<Player*>& playerVector);
        ~TreeQueue();
        TreeQueue(const TreeQueue &toCopy) = delete;
        TreeQueue &operator=(const TreeQueue &other) = delete;

        // Methods

        /**
         * @brief Find the size of the tree
         * 
         * @return size_t Queue size
         */
        size_t size() const;

        /**
         * @brief Adds a player to the tree
         * 
         * @param arrInd Index of PlayerVec vector where the player is
         * @param queueEnter  Number of battles played when the player enters queue
         */
        void addPlayer(const size_t& arrInd, long long queueEnter);

        /**
         * @brief Find a suitable opponent for player at playerVec[arrInd]
         * 
         * @param arrInd Vector index where the player is
         * @param mmType Matchmaking type (King Tower, Card Level None)
         * @return size_t The index of an opponent. 
         * @note Returns arrInd if there is no opponent per the mm type
         */
        size_t findOpponent(size_t arrInd, u_int8_t mmType) const;

        /**
         * @brief Checks if a player is in the tree. O(1)
         * 
         * @param arrInd Array index of player to check
         * @return true Player is in the tree
         * @return false Player is not in the tree
         */
        bool exists(size_t arrInd) const;

        /**
         * @brief Removes a player from the tree
         * 
         * @param arrInd Vector Index where the player to be removed is
         * @return Returns the time the player entered the queue. 
         */
        long long deletePlayer(size_t arrInd);
};
#endif
