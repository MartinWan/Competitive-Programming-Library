#include<cstdlib>
#define RED true
#define BLACK false
using namespace std;

/*
 * An order statistic tree is a variant of a binary search tree
 * that supports:
 *  -Select(i): finding the ith smallest element 
 *  -Rank(Key) finding the rank of an element in the tree, its index in the sorted list of elements
 *
 * Implementation based off of red black tree from Sedgewick Algorithms 4th edition: http://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
 * Note select and rank operations are O(logN)
 */
template<typename Key, typename Value> class order_statistic_tree {
 private:
   class Node {
      public:
        Key key;
        Value val;
        Node* left;
        Node* right;
        bool color;
        int size;

        Node(Key key, Value val, bool color, int size) {
          this->key = key;
          this->val = val;
          this->color = color;
          this->size = size;
          this->left = this->right = NULL;
        }
        ~Node() {
          delete left;
          delete right;
        }
    };

  Node* root; // root of the tree
  
  bool is_red(Node* x) {
    if (x == NULL) return false;
    return x->color == RED;
  }

  int size(Node* x) {
    if (x == NULL) return 0;
    return x->size;
  }
   
  Node* rotate_left(Node* h) {
    Node* x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = RED;
    x->size = h->size;
    h->size = size(h->left) + size(h->right) + 1;

    return x;
  }

  Node* rotate_right(Node* h) {
    Node* x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = RED;
    x->size = h->size;
    h->size = size(h->left) + size(h->right) + 1;
    
    return x;
  }

  void flip_colors(Node* h) {
    h->color = !h->color;
    h->left->color = !h->left->color;
    h->right->color = !h->right->color;
  }

  Value get(Node* x, Key key) {
    while (x != NULL) {
      if (key < x->key) x = x->left;
      else if (key > x->key) x = x->right;
      else return x->val;
    }
    return Value();
  }

  Node* put(Node* h, Key key, Value val) {
    if (h == NULL) return new Node(key, val, RED, 1);
    
    if (key < h->key) h->left = put(h->left, key, val);
    else if (key > h->key) h->right = put(h->right, key, val);
    else h->val = val;

    // fix up any right leaning links
    if (is_red(h->right) && !is_red(h->left)) h = rotate_left(h);
    if (is_red(h->left) && is_red(h->left->left)) h = rotate_right(h);
    if (is_red(h->left) && is_red(h->right)) flip_colors(h);
    h->size = size(h->left) + size(h->right) + 1;

    return h;
  }
  
  int rank(Key key, Node* x) {
    if (x == NULL) return 0;
    if (key < x->key) return rank(key, x->left);
    else if (key > x->key) return 1 + size(x->left) + rank(key, x->right);
    else return size(x->left);
  }

  Node* select(Node* x, int k) {
    int t = size(x->left);
    if (t > k) return select(x->left, k);
    else if (t < k) return select(x->right, k-t-1);
    else return x;
  }

 public:
    order_statistic_tree() {
      this->root = NULL;
    }
    ~order_statistic_tree() {
      delete root;
    }
  
    Value get(Key key) {
      return get(root, key);
    }

    void put(Key key, Value val) {
      root = put(root, key, val);
      root->color = BLACK;
    }

    int rank(Key key) {
      return rank(key, root);
    }

    Key select(int k) {
      if (k < 0 || k >= size())
        throw "SELECT_OUT_OF_BOUNDS";  
      Node* x = select(root, k);
      return x->key;
    }

    int size() {
      return size(root);
    }
};



