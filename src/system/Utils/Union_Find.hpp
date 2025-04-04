#pragma once
#include <bits/stdc++.h>

// A class to represent a Union-Find data structure (also known as Disjoint Set Union)
// that provides efficient methods for connecting elements, and determining whether
// elements are connected. It's particularly useful for graph-related algorithms.
class UnionFind {
public:
    // Constructor initializes the Union-Find structure with 'size' elements.
    // Initially, each element is in a set by itself.
    UnionFind(int _size) : root(_size), rank(_size, 0) {
        size = _size;
        for (int i = 0; i < size; ++i) {
            root[i] = i; // Each element is its own root to start.
        }
    }

    // Reset the forest
    void reset(){
        for (int i = 0; i < size; ++i) {
            root[i] = i; // Each element is its own root to start.
        }
    }

    // Finds and returns the root of the set that element 'x' belongs to.
    // Implements path compression to make subsequent queries faster.
    int find(int x) {
        if (x == root[x]) {
            return x; // x is the root of itself.
        }
        // Recursively find the root of x, and update x's root to be the root of its set,
        // this is the path compression optimization.
        return root[x] = find(root[x]); 
    }

    // Connects the sets containing elements 'x' and 'y' together.
    // Uses union by rank to choose the new root, which keeps the tree flat.
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) { // Only unite if x and y are not already in the same set.
            if (rank[rootX] < rank[rootY]) {
                root[rootX] = rootY; // Make the root of the smaller rank tree point to the root of the higher rank tree.
            } else if (rank[rootX] > rank[rootY]) {
                root[rootY] = rootX;
            } else {
                root[rootY] = rootX; // If ranks are the same, arbitrarily choose one as the new root and increment its rank.
                rank[rootX] += 1;
            }
        }
    }

    // Returns true if elements 'x' and 'y' are part of the same set, false otherwise.
    bool connected(int x, int y) {
        return find(x) == find(y);
    }

    // Return rank value
    int getrank(int x) { return rank[x]; }

private:
    int size;   // set size
    std::vector<int> root; // Tracks the root of each element.
    std::vector<int> rank; // Used to keep the trees as flat as possible.
};


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


class ClusterFind {

private:
    int size;   // set size
    std::vector<int> root; // Tracks the root of each element.

public:
    ClusterFind() {};

    // Initialize the cluster
    void ini(int _size) {
        size = _size;
        root.resize(size);
        reset();
    }

    // Reset the cluster
    void reset(){
        for (int i = 0; i < size; ++i) {
            root[i] = -1; // set cluster size as 1
        }
    }

    // Finds and returns the root of the set that element 'x' belongs to.
    // Implements path compression to make subsequent queries faster.
    int find(int x) {
        if (root[x] < 0) {
            return x;   // negtive value means root node
        }

        return root[x] = find(root[x]); 
    }

    // Connects the sets containing elements 'x' and 'y' together.
    // Uses union by rank to choose the new root, which keeps the tree flat.
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) { 
            if (root[rootX] > root[rootY]) // due to negtive value for root node, rootY is large
            {
                root[rootY] += root[rootX]; // rootX merge into rootY
                root[rootX] = rootY;
            } 
            else  // otherwise, rootX is large or equal
            {
                root[rootX] += root[rootY]; // rootX merge into rootY
                root[rootY] = rootX;
            }
        }
    }

    // Returns true if elements 'x' and 'y' are part of the same set, false otherwise.
    bool clusterOrNot(int x, int y) {
        return find(x) == find(y);
    }

    // Return root value
    int getval(int x) { return root[x]; }

};