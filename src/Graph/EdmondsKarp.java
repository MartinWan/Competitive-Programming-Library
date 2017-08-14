import java.util.Queue;
import java.util.Arrays;
import java.util.LinkedList;


class EdmondsKarp {
  static boolean augmentingPath(int[][] Gf, int s, int t, int[] parent) {
    // standard BFS starting at s on Gf, storing parent of vertices in parent array

    int V = Gf.length;

    boolean[] visited = new boolean[V];
    Arrays.fill(visited, false);

    Queue<Integer> Q = new LinkedList<>();
    Q.add(s);
    visited[s] = true;

    while (!Q.isEmpty()) {
      int u = Q.poll();

      for (int v = 0; v < V; v++) {
        if (Gf[u][v] > 0 && !visited[v]) {
          Q.add(v);
          parent[v] = u;
          visited[v] = true;
        }
      }
    }

    return visited[t];
  }

  // Computes the max flow in the flow network Gf
  // where Gf is an adjacency-matrix representation of the network
  // Note Gf is mutated by this procedure.
  //
  // Implementation based off of wikipedia: https://en.wikipedia.org/wiki/Ford%E2%80%93Fulkerson_algorithm
  // Time Complexity: O(VE^2)
  static int apply(int[][] Gf, int source, int sink) {
    int V = Gf.length;
    int[] parent = new int[V];

    int maxflow = 0;

    while (augmentingPath(Gf, source, sink, parent)) {
      long pathflow = Integer.MAX_VALUE;
      int s = sink;

      // find augmenting path's bottleneck
      while (s != source) {
        pathflow = Math.min(pathflow, Gf[parent[s]][s]);
        s = parent[s];
      }

      maxflow += pathflow;

      // update residual capacities in Gf along augmenting path
      int v = sink;
      while (v != source) {
        Gf[parent[v]][v] -= pathflow;
        Gf[v][parent[v]] += pathflow;
        v = parent[v];
      }
    }

    return maxflow;
  }
}
