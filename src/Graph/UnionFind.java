import java.util.Arrays;

/**
  * Weighted quick union with path compression.
  * Unions N elements in O(Nlog*(N)) ~ O(N)
  * Where log*(N) is the inverse of the super exponential
  * e.g. 2^(2^(2^(2...)))
  */
class UnionFind {
  private int N;
  private int[] size;
  private int[] id;

  public UnionFind(int N) {
    this.N = N;
    this.size = new int[N];
    this.id = new int[N];

    for (int i = 0; i < N; i++) {
      this.size[i] = 1;
      this.id[i] = i;
    }
  }

  public boolean connected(int v1, int v2) {
    return find(v1) == find(v2);
  }

  public void union(int v1, int v2) {
    if (v1 < 0 || v1 >= N) return;
    if (v2 < 0 || v2 >= N) return;

    int i = find(v1);
    int j = find(v2);

    if (i != j) { // v1, v2 not already unioned
      // smaller tree always merged into larger tree to maintain balance
      if (size[i] < size[j]) {
        id[i] = id[j];
        size[j] += size[i];
      } else {
        id[j] = id[i];
        size[i] += size[j];
      }
    }
  }

  private int find(int v) {
    int p = id[v];
    while (p != id[p]) {
      id[p] = id[id[p]]; // path compression
      p = id[p];
    }
    return p;
  }
}
