#include <vector>
#include <list>
using namespace std;

/**
 * Bellmand-Ford algorithm to compute shortest paths to all vertices 
 * from source vertex s in O(|V|*|E|) time. Throws error if negative cycle found.
 *
 * @E is a list of edges
 * @w is a matrix with w[u][v] being the weight of edge (u, v). (INT_MAX if no edge exists)
 * @s is the source vertex
 */
pair<vector<long long>, vector<int> > bellman_ford(list<pair<int, int> >& E, vector<vector<long long> >& w, int s) {
  int V = w.size();
  vector<long long> distance(V);
  vector<int> predecessor(V);

  // Initialize Graph
  for (int v = 0; v < V; v++) {
    distance[v] = INT_MAX;
  }

  distance[s] = 0;

  // Relax edges repeatedly
  for (int i = 1; i <= V - 1; i++) {
    for (pair<int, int> e : E) {
      int u = e.first;
      int v = e.second;

      if (distance[u] + w[u][v] < distance[v]) {
        distance[v] = distance[u] + w[u][v];
        predecessor[v] = u;
      }
    }
  }

  // Check for negative weight cycles
  for (pair<int, int> e : E) {
    int u = e.first;
    int v = e.second;

    if (distance[u] + w[u][v] < distance[v]) {
      throw "Negative Cycle Found";
    }
  }

  pair<vector<long long>, vector<int> > result;
  result.first = distance;
  result.second = predecessor;

  return result;
}

