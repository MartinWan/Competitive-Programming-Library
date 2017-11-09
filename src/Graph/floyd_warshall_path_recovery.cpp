#include<cstring>
#include<cstdio>
#include<vector>
using namespace std;

const int V_MAX = 1000;
const int INF = 2000000000;
const int UNDEFINED = -1;

int V;

// dist[u][v] = shortest path distance from u->v, initialized to edge weights (INF if no edge)
int dist[V_MAX][V_MAX];

// next vertex in shortest path from u->v, initialized to v for each edge (u,v)
int after[V_MAX][V_MAX];

/*
 * Floyd warshall with path reconstruction. Implementation based off of wikipedia 
 * https://en.wikipedia.org/wiki/Floyd-Warshall_algorithm
 */
void floydwarshall() {
   for (int k = 0; k < V; k++)
     for (int i = 0; i < V; i++)
       for (int j = 0; j < V; j++)
         if (dist[i][j] > dist[i][k] + dist[k][j])
           dist[i][j] = dist[i][k] + dist[k][j], after[i][j] = after[i][k];
}

vector<int> Path(int u, int v) {
  if (after[u][v] == UNDEFINED)
    return vector<int>();

  vector<int> path; path.push_back(u);

  while (u != v)
    u = after[u][v], path.push_back(u);

  return path;
}

