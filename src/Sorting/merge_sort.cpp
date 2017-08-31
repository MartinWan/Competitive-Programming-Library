#include<vector>
#include<iostream>
using namespace std;


void merge_sort(vector<int>& a, int l, int r) {
  int size = r - l + 1;
  if (size <= 1) return; // consider size arrays of size 0, 1 sorted

  int mid = (l + r) / 2;

  merge_sort(a, l, mid);
  merge_sort(a, mid+1, r);
  
  vector<int> temp(size); // temp array to merge a[l...mid] and a[mid+1...r]

  int i = l; // current index of a[l...mid]
  int j = mid+1; // curent index of a[mid+1...r]
  for (int k = 0; k < size; k++) {
    int x;
    if (i > mid) 
      x = a[j++];
    else if (j > r)
      x = a[i++];
    else {
      if (a[i] < a[j]) 
        x = a[i++];
      else
        x = a[j++];
    }
    temp[k] = x;
  }
  
  // copy merged array into a[l...r]
  for (int i = l, k = 0; i <= r; i++, k++) {
    a[i] = temp[k];
  }
}


void merge_sort(vector<int>& a) {
  merge_sort(a, 0, a.size()-1);
}



