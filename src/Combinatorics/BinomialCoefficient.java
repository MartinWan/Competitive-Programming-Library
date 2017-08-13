class BinomialCoefficient {
  // returns n choose k
  // where n choose k is n*(n-1)*...(n-k+1) / k!
  // This implementation is fast, but may not be
  // precise for extremely large values 
  public static long apply(long n, long k) {
    double result = 1.0; 

    for (long i = 1; i <= k; i++) {
      result *= n-i+1;
      result /= i;
    }

    return (long) result;
  }
}
