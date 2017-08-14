class FloydWarshall {
  // Computes the length of the shortest path between
  // all pairs of vertices with distances dist[i][j]
  // and stores them in the matrix dist[][] where  dist is a square matrix
  // and if i and j are not connected dist[i][j] must equal Integer.MAX_VALUE
  void apply(long[][] dist) {
    int N = dist.length;

    for (int k = 0; k < N; k++) {
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          dist[i][j] = Math.min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }
}
