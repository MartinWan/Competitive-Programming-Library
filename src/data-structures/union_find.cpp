#include<vector>
using namespace std;

class union_find {
  private:
    vector<int> id;
    vector<int> size;

    int find(int v) {
      int p = id[v];
      while (p != id[p]) {
        id[p] = id[id[p]]; // path compression
        p = id[p];
      }
      return p;
    }

  public:
    union_find(int N) {
      id = vector<int>(N);
      size = vector<int>(N, 1);
      for (int i = 0; i < N; i++) {
        id[i] = i;
      }
    }

    void join(int v1, int v2) {
      int p1 = find(v1);
      int p2 = find(v2);

      if (p1 != p2) {
        if (size[p1] < size[p2]) {
          id[p1] = id[p2];
          size[p2] += size[p1];
        } else {
          id[p2] = id[p1];
          size[p1] += size[p2];
        }
      }
    }

    bool connected(int v1, int v2) {
      return find(v1) == find(v2);
    }
};
