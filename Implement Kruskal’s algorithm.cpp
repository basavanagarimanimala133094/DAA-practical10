#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent a weighted edge in the graph
struct Edge {
    int src, dest, weight;

    // Custom comparator to sort edges by weight ascendingly
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Disjoint Set Union (DSU) / Union-Find class
class DisjointSet {
private:
    vector<int> parent;
    vector<int> rank;

public:
    // Initialize DSU for V vertices
    DisjointSet(int V) {
        parent.resize(V);
        rank.resize(V, 0);
        // Initially, every vertex is its own parent
        for (int i = 0; i < V; i++) {
            parent[i] = i;
        }
    }

    // Find the representative of the set containing element 'i'
    // Uses path compression optimization
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]); 
    }

    // Combine two subsets using union by rank optimization
    // Returns true if union was successful, false if they were already in the same set
    bool unionSets(int u, int v) {
        int root_u = find(u);
        int root_v = find(v);

        if (root_u != root_v) {
            // Attach smaller rank tree under root of high rank tree
            if (rank[root_u] < rank[root_v]) {
                parent[root_u] = root_v;
            } else if (rank[root_u] > rank[root_v]) {
                parent[root_v] = root_u;
            } else {
                parent[root_v] = root_u;
                rank[root_u]++;
            }
            return true;
        }
        return false; // Cycle detected
    }
};

// Function to find and print the MST using Kruskal's algorithm
void kruskalMST(int V, vector<Edge>& edges) {
    // 1. Sort all the edges in non-decreasing order of their weight
    sort(edges.begin(), edges.end());

    DisjointSet dsu(V);
    vector<Edge> mst; // To store the resulting MST edges
    int totalMSTWeight = 0;

    // 2. Iterate through all sorted edges
    for (const auto& edge : edges) {
        // Check if including this edge creates a cycle
        if (dsu.unionSets(edge.src, edge.dest)) {
            mst.push_back(edge);
            totalMSTWeight += edge.weight;
        }

        // Optimization: MST of V vertices always contains exactly V-1 edges
        if (mst.size() == V - 1) {
            break;
        }
    }

    // Print the constructed MST
    cout << "Edges in the Minimum Spanning Tree:\n";
    cout << "Source -- Destination : Weight\n";
    for (const auto& edge : mst) {
        cout << edge.src << " -- " << edge.dest << " : " << edge.weight << "\n";
    }
    cout << "\nTotal Weight of Minimum Spanning Tree: " << totalMSTWeight << "\n";
}

int main() {
    /* Let us create the following weighted graph:
             10
        0 -------- 1

        |  \       |
       6|   5\     |15

        |      \   |
        2 -------- 3
             4       */
    int V = 4; // Number of vertices
    vector<Edge> edges = {
        {0, 1, 10},
        {0, 2, 6},
        {0, 3, 5},
        {1, 3, 15},
        {2, 3, 4}
    };

    kruskalMST(V, edges);

    return 0;
}
