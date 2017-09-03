#include<random>
#include<list>
#include<vector>
#include<algorithm>
using namespace std;

template <class Key>
struct interval {
  Key min, max;
  interval(Key min, Key max) {
    if (max < min) throw logic_error("Illegal argument to interval");
    this->min = min;
    this->max = max;
  }
  interval() {}

  bool contains(Key x) {
    return !(x < min) && !(max < x);
  }
};

template <class Key>
struct interval2d {
  interval<Key> intervalx, intervaly;
  interval2d(interval<Key> intervalx, interval<Key> intervaly) {
    this->intervalx = intervalx;
    this->intervaly = intervaly;
  }

  bool contains(Key x, Key y) {
    return intervalx.contains(x) && intervaly.contains(y);
  }
};

/*
 * A range search data structure implemented as a randomized binary search tree
 */
template <class Key, class Value>
class range_search {
  private:
    struct node {
      Key key;
      Value val;
      node* left;
      node* right;
      int N;
      node(Key key, Value val) {
        this->key = key;
        this->val = val;
        this->left = this->right = nullptr;
        this->N = 1;
      }
      ~node() {
        delete left; delete right;
      }
    };

    node* root;
    uniform_real_distribution<double> uniform;
    default_random_engine seed;

    Value get(node* x, Key key) {
      if (x == nullptr) return Value();
      if (key == x->key) return x->val;
      else if (key < x->key) return get(x->left, key);
      else return get(x->right, key);
    }

    node* put(node* x, Key key, Value val) {
      if (x == nullptr) return new node(key, val);
      if (key == x->key) {
        x->val = val;
        return x;
      }
      if (bernoulli(1.0 / (size(x) + 1.0))) return put_root(x, key, val);
      if (key < x->key) x->left = put(x->left, key, val);
      else x->right = put(x->right, key, val);
      fix(x);
      return x;
    }

    node* put_root(node* x, Key key, Value val) {
      if (x == nullptr) return new node(key, val);
      if (key == x->key) {
        x->val = val;
        return x;
      } else if (key < x->key) {
        x->left = put_root(x->left, key, val);
        x = rotate_right(x);
      } else {
        x->right = put_root(x->right, key, val);
        x = rotate_left(x);
      }
      return x;
    }

    node* rotate_right(node* h) {
      node* x = h->left;
      h->left = x->right;
      x->right = h;
      fix(h);
      fix(x);
      return x;
    }

    node* rotate_left(node* h) {
      node* x = h->right;
      h->right = x->left;
      x->left = h;
      fix(h);
      fix(x);
      return x;
    }

    void range(node* x, interval<Key>& interval, list<Key>& result) {
      if (x == nullptr) return;
      if (!(x->key < interval.min)) range(x->left, interval, result);
      if (interval.contains(x->key)) result.push_back(x->key);
      if (!(interval.max < x->key)) range(x->right, interval, result);
    }

    void fix(node* x) {
      if (x == nullptr) return;
      x->N = 1 + size(x->left) + size(x->right);
    }

    bool bernoulli(double p) {
      return uniform(seed) < p;
    }

    int size(node* x) {
      if (x == nullptr) return 0;
      else return x->N;
    }

  public:
    range_search(): uniform(0.0, 1.0) {
      root = nullptr;
    }
    ~range_search() {
      delete root;
    }

    Value get(Key key) {
      return get(root, key);
    }

    void put(Key key, Value val) {
      root = put(root, key, val);
    }

    list<Key> range(Key min, Key max) {
      return range(interval<Key>(min, max));
    }

    list<Key> range(interval<Key> interval) {
      list<Key> result;
      range(root, interval, result);
      return result;
    }
};


/*
 * A range tree is a data structure to find all k-dimensional points that 
 * lie within a k-dimensional box. This implementation is a 2-dimensional range
 * tree based off of http://algs4.cs.princeton.edu/92search/RangeTree.java.html
 *
 * On N data points, time complexity of query(range) is O((logN)^2 + K) where K is the 
 * points in the range. Tree also uses O(NlogN) space.
 */
template <class Key>
class range_tree {
  private:
    struct node {
        Key x, y;
        node* left;
        node* right;
        range_search<Key, Key> bst;
        node(Key x, Key y) {
          this->x = x;
          this->y = y;
          this->left = nullptr;
          this->right = nullptr;
          this->bst.put(y, x);
        }
        ~node() {
          delete left; delete right;
        }
    };

    node* root;

    node* insert(node* h, Key x, Key y) {
      if (h == nullptr) return new node(x, y);
      h->bst.put(y, x);
      if (x < h->x) h->left = insert(h->left, x, y);
      else h->right = insert(h->right, x, y);
      return h;
    }

    void query_left(node* h, interval2d<Key>& rect, list<pair<Key, Key>>& result) {
      if (h == nullptr) return;
      if (rect.contains(h->x, h->y)) {
        result.push_back(make_pair(h->x, h->y));
      }
      if (!(h->x < rect.intervalx.min)) {
        enumerate(h->right, rect, result);
        query_left(h->left, rect, result);
      } else {
        query_left(h->right, rect, result);
      }
    }

    void query_right(node* h, interval2d<Key>& rect, list<pair<Key, Key>>& result) {
      if (h == nullptr) return;
      if (rect.contains(h->x, h->y)) {
        result.push_back(make_pair(h->x, h->y));
      }
      if (!(rect.intervalx.max < h->x)) {
        enumerate(h->left, rect, result);
        query_right(h->right, rect, result);
      } else {
        query_right(h->left, rect, result);
      }
    }

    void enumerate(node* h, interval2d<Key>& rect, list<pair<Key, Key>>& result) {
      if (h == nullptr) return;
      list<Key> l = h->bst.range(rect.intervaly);
      for (Key& y : l) {
        Key x = h->bst.get(y);
        result.push_back(make_pair(x, y));
      }
    }
  
  public:
    range_tree() {
      root = nullptr;
    }
    ~range_tree() {
      delete root;
    }

    void insert(Key x, Key y) {
      root = insert(root, x, y);
    }

    list<pair<Key, Key>> query(interval2d<Key>& rect) {
      list<pair<Key, Key>> result;

      interval<Key>& intervalx = rect.intervalx;
      node* h = root;
      while (h != nullptr && !intervalx.contains(h->x)) {
        if (intervalx.max < h->x) h = h->left;
        else if (h->x < intervalx.min) h = h->right;
      }
      if (h == nullptr) return result;

      if (rect.contains(h->x, h->y)) {
        result.push_back(make_pair(h->x, h->y));
      }
      query_left(h->left, rect, result);
      query_right(h->right, rect, result);

      return result;
    }
};

