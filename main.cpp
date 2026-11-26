#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class FlowGraph {
    struct Edge {
        int to;
        int capacity;
    };
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adjacency;
    std::vector<int> previous, queue;

public:
    explicit FlowGraph(int n) : adjacency(n), previous(n), queue(n) {}

    void add_edge(int u, int v) {
        adjacency[u].push_back(static_cast<int>(edges.size()));
        edges.push_back({v, 1});
        adjacency[v].push_back(static_cast<int>(edges.size()));
        edges.push_back({u, 1});
    }

    int min_cut(int source, int sink) {
        for (auto &edge : edges) edge.capacity = 1;
        int flow = 0;
        while (true) {
            std::fill(previous.begin(), previous.end(), -1);
            previous[source] = -2;
            int head = 0, tail = 0;
            queue[tail++] = source;
            while (head < tail && previous[sink] == -1) {
                int u = queue[head++];
                for (int id : adjacency[u]) {
                    const auto &edge = edges[id];
                    if (edge.capacity == 0 || previous[edge.to] != -1) continue;
                    previous[edge.to] = id;
                    queue[tail++] = edge.to;
                }
            }
            // The final unsuccessful search identifies the source side of the cut.
            if (previous[sink] == -1) return flow;
            for (int v = sink; v != source;) {
                int id = previous[v];
                --edges[id].capacity;
                ++edges[id ^ 1].capacity;
                v = edges[id ^ 1].to;
            }
            ++flow;
        }
    }

    bool source_side(int v) const { return previous[v] != -1; }
};

class DisjointSet {
    std::vector<int> parent, size;
public:
    explicit DisjointSet(int n) : parent(n), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int v) {
        return parent[v] == v ? v : parent[v] = find(parent[v]);
    }
    long long merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return 0;
        if (size[u] < size[v]) std::swap(u, v);
        long long pairs = 1LL * size[u] * size[v];
        parent[v] = u;
        size[u] += size[v];
        return pairs;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    if (!(std::cin >> n >> m)) return 0;
    FlowGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        graph.add_edge(u - 1, v - 1);
    }

    // Gomory-Hu tree: each path's minimum edge is its endpoints' minimum cut.
    std::vector<int> parent(n, 0), weight(n, 0);
    for (int s = 1; s < n; ++s) {
        int t = parent[s];
        int cut = graph.min_cut(s, t);
        for (int v = s + 1; v < n; ++v) {
            if (parent[v] == t && graph.source_side(v)) parent[v] = s;
        }
        if (graph.source_side(parent[t])) {
            parent[s] = parent[t];
            parent[t] = s;
            weight[s] = weight[t];
            weight[t] = cut;
        } else {
            weight[s] = cut;
        }
    }

    DisjointSet components(n);
    long long answer = 0;
    // Joining in descending weight counts each pair at its path's minimum.
    for (int w = 3; w >= 1; --w) {
        for (int v = 1; v < n; ++v) {
            if (weight[v] == w) answer += w * components.merge(v, parent[v]);
        }
    }
    std::cout << answer << '\n';
}
