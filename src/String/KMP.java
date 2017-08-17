class KMP {

  // Returns an array pi[] such that
  // pi[q] = "length of longest prefix that is a proper suffix of P[0...q]"
  // That is, pi[q] = max { k < q : P[0...k] is a suffix of P[0...q] }
  static int[] computePrefixFunction(String P) {
    int m = P.length();
    int[] pi = new int[m];
    pi[0] = 0;
    int k = 0;

    for (int q = 1; q < m; q++) {
      while (k > 0 && P.charAt(k) != P.charAt(q))
        k = pi[k-1];
      if (P.charAt(k) == P.charAt(q))
        k++;
      pi[q] = k;
    }
    
    return pi;
  }
 
  // returns index i such that
  // P occurs in T starting at index i
  // returns -1 if no such index exists
  // Time Complexity: O(|T| + |P|)
  public static int match(String T, String P) {
    int n = T.length();
    int m = P.length();
    int[] pi = computePrefixFunction(P);
    int matches = 0;

    for (int i = 0; i < n; i++) {
      while (matches > 0 && P.charAt(matches) != T.charAt(i))
        matches = pi[matches-1];
      if (P.charAt(matches) == T.charAt(i))
        matches++;
      if (matches == m)
        return i - m + 1;
    }

    return -1; // not found
  }
}
