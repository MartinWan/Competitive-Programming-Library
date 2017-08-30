#include<climits>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;

bool augmenting_path(vector<vector<int>>& Gf, int s, int t, vector<int>& parent) {
  // standard BFS starting at s on Gf, storing parent vertices in parent vector
  int V = Gf.size();

  vector<bool> visited(V);
  queue<int> Q;
  Q.push(s);
  visited[s] = true;

  while (!Q.empty()) {
    int u = Q.front();
    Q.pop();

    for (int v = 0; v < V; v++) {
      if (Gf[u][v] > 0 && !visited[v]) {
        Q.push(v);
        parent[v] = u;
        visited[v] = true;
      }
    }
  }

  return visited[t];
}

/**
 * Computes the max flow in the flow network Gf
 * where Gf[u][v] is the residual capcity of edge (u, v).
 * Note Gf is mutated by this procedure.
 *
 * Implementation based off of wikipedia.
 * Time Complexity: O(|V|*|E|^2)
 */
int edmonds_karp(vector<vector<int>>& Gf, int source, int sink) {
  int V = Gf.size();
  vector<int> parent(V);

  int maxflow = 0;

  while (augmenting_path(Gf, source, sink, parent)) {
    long long pathflow = INT_MAX;
    int s = sink;

    // find augmenting path's bottleneck
    while (s != source) {
      pathflow = min(pathflow, (long long) Gf[parent[s]][s]);
      s = parent[s];
    }

    maxflow += pathflow;

    // update residual capacities along augmenting path
    int v = sink;
    while (v != source) {
      Gf[parent[v]][v] -= pathflow;
      Gf[v][parent[v]] += pathflow;
      v = parent[v];
    }
  }

  return maxflow;
}
