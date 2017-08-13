import java.util.Set;
import java.util.HashSet;

class PrimeFactors {
  // Returns the prime factors of N
  // in ~ O(sqrt(N)) time complexity
  static Set<Integer> apply(int N) {
    Set<Integer> factors = new HashSet<>();

    while (N % 2 == 0) {
      factors.add(2);
      N /= 2;
    }

    // iterates i until the floor(sqrt(N))
    for (int i = 3; i*i <= N; i += 2) { 
      while (N % i == 0) {
        factors.add(i);
        N /= i;
      }
    }

   // handles case where N is prime > 2
   if (N > 2) 
      factors.add(N);

    return factors;
  }
}
