#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <tuple>

using namespace std;

// ==============================
// 資料結構定義
// ==============================
struct Edge {
    int u, v, w;
};

// 並查集 Disjoint Set Union
class DSU {
private:
    vector<int> parent, rankv;

public:
    DSU(int n) {
        parent.resize(n + 1);
        rankv.resize(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        // 路徑壓縮 (Path Compression)
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]); 
    }

    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB) return false;

        // 按秩合併 (Union by Rank)
        if (rankv[rootA] < rankv[rootB]) {
            parent[rootA] = rootB;
        } else if (rankv[rootA] > rankv[rootB]) {
            parent[rootB] = rootA;
        } else {
            parent[rootB] = rootA;
            rankv[rootA]++;
        }
        return true;
    }
};

// ==============================
// 工具函式
// ==============================
void printEdge(const Edge& e) {
    cout << e.u << " - " << e.v << " (w:" << e.w << ")";
}

void printMST(const vector<Edge>& mst) {
    int total = 0;
    cout << "\nMST edges:\n";
    for (const auto& e : mst) {
        printEdge(e);
        cout << "\n";
        total += e.w;
    }
    cout << "Total weight = " << total << "\n";
}

// ==============================
// 1. Kruskal Algorithm
// ==============================
void kruskalMST(int n, vector<Edge> edges) {
    cout << "==============================\n";
    cout << "Kruskal Algorithm\n";
    cout << "==============================\n";

    // Step 1. 依照權重排序
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.w < b.w;
    });

    DSU dsu(n);
    vector<Edge> mst;
    
    cout << "Selection steps:\n";

    for (const auto& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            printEdge(e); cout << " (selected)\n";
            if (mst.size() == n - 1) break;
        }
    }
    
    printMST(mst);
    cout << "\n";
}

// ==============================
// 2. Prim Algorithm
// ==============================
void primMST(int n, const vector<vector<pair<int, int>>>& adj, int start = 1) {
    cout << "==============================\n";
    cout << "Prim Algorithm\n";
    cout << "==============================\n";

    vector<bool> inMST(n + 1, false);
    vector<Edge> mst;

    // 最小堆 {權重, from, to}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;

    inMST[start] = true;
    for (auto& neighbor : adj[start]) {
        pq.push({neighbor.second, start, neighbor.first});
    }
    
    cout << "Selection steps:\n";

    while (!pq.empty() && mst.size() < n - 1) {
        auto [w, u, v] = pq.top(); pq.pop();

        if (inMST[v]) continue;

        inMST[v] = true;
        mst.push_back({u, v, w});
        cout << "Selected "; printEdge({ u, v, w }); cout << "\n";

        for (auto& next : adj[v]) {
            if (!inMST[next.first]) {
                pq.push({next.second, v, next.first});
            }
        }
    }
    
    printMST(mst);
    cout << "\n";
}

// ==============================
// 3. Boruvka (Sollin) Algorithm
// ==============================
void boruvkaMST(int n, const vector<Edge>& edges) {
    cout << "==============================\n";
    cout << "Boruvka Algorithm\n";
    cout << "==============================\n";

    DSU dsu(n);
    vector<Edge> mst;
    int numComponents = n;
    int round = 1;

    while (numComponents > 1) {
        cout << "Round " << round << ":\n";

        // cheapest[comp_id] 存儲該連通分量目前最便宜邊的索引
        vector<int> cheapest(n + 1, -1);

        for (int i = 0; i < edges.size(); i++) {
            int set1 = dsu.find(edges[i].u);
            int set2 = dsu.find(edges[i].v);
            if (set1 == set2) continue;

            if (cheapest[set1] == -1 || edges[i].w < edges[cheapest[set1]].w) cheapest[set1] = i;
            if (cheapest[set2] == -1 || edges[i].w < edges[cheapest[set2]].w) cheapest[set2] = i;
        }
        
        bool merged = false;
        for (int i = 1; i <= n; i++) {
            if (cheapest[i] != -1) {
                Edge e = edges[cheapest[i]];
                if (dsu.unite(e.u, e.v)) {
                    mst.push_back(e);
                    cout << "Merged component " << i << " using edge "; printEdge(e); cout << "\n";
                    numComponents--;
                    merged = true;
                }
            }
        }
        
        if (!merged) break;
        cout << "\n";
        round++;
    }

    printMST(mst);
    cout << "\n";
}

// ==============================
// 主程式
// ==============================
int main() {
    int n = 7; // 節點數量

    // 使用 Question 6 的數據作為範例
    vector<Edge> edges = {
        {1, 2, 28}, {1, 6, 10}, {2, 3, 16}, {2, 7, 14},
        {3, 4, 12}, {4, 5, 22}, {4, 7, 18}, {5, 6, 25}, {5, 7, 24}
    };

    vector<vector<pair<int, int>>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.u].push_back({e.v, e.w});
        adj[e.v].push_back({e.u, e.w});
    }

    kruskalMST(n, edges);
    primMST(n, adj, 1);
    boruvkaMST(n, edges);

    return 0;
}