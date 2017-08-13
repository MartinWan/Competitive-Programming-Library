class EulersTotientFunction {
  
  // Returns phi(N) where phi(N) 
  // is the product over distinct prime numbers
  // that divide N
  public static int apply(int N) {
    int result = N;

    for (int p: PrimeFactors.apply(N)) 
      result *= 1 - 1.0/p;

    return result;
  }
}
