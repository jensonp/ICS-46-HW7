#include "sorter.h"
#include "timer.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// Sorter
void Sorter::fill(int count, string file_name){
    vec.clear();
    std::ifstream infile(file_name);
    string word;
    for(int i=0; i<count && infile>>word; ++i){ vec.push_back(word); }
}
void Sorter::print(ostream & out){ for(const auto &word : vec){out<<word<<" ";} }
bool Sorter::verify_sorted(){
    for(size_t i=1; i<vec.size(); ++i){ 
        if(vec[i-1]>vec[i]) return false; 
    }
    return true;
}

// Quick
string QuickSorter::select_pivot(vector<string>& vec, int low, int high){ (void)low; return vec[high]; }

int QuickSorter::partition(vector<string>& vec, int low, int high){
    string pivot = select_pivot(vec, low, high);
    int i=low;
    for (int j=low; j<high; ++j) {
        if (vec[j] < pivot) {
            std::swap(vec[i], vec[j]);
            ++i;
        }
    }
    std::swap(vec[i], vec[high]);
    return i;
}
void QuickSorter::quicksort(vector<string>& vec, int low, int high){
    if (low<high){
        quicksort(vec, low, partition(vec, low, high)-1);
        quicksort(vec, partition(vec, low, high)+1, high);
    }
}
void QuickSorter::sort(){
    if (vec.empty()) return;
    quicksort(vec, 0, vec.size()-1);
}

// Heap
void HeapSorter::heapify(vector<string>& vec, int n, int i){
    int m=i, l=2*i+1, r=2*i+2;
    if (l<n && vec[l]>vec[m]) m=l;
    if (r<n && vec[r]>vec[m]) m=r;
    if (m!=i){ std::swap(vec[i], vec[m]); heapify(vec, n, m); }
}


void HeapSorter::heapsort(vector<string>& vec, int low, int high){
    (void)low; (void)high;
    int n = vec.size();
    for(int i=n/2-1; i >= 0; --i){ heapify(vec, n, i); }
    for(int i=n-1; i > 0; --i){ std::swap(vec[0], vec[i]); heapify(vec, i, 0); }
}
void HeapSorter::sort() {
    if(vec.empty()) return;
    heapsort(vec, 0, vec.size() - 1);
}

static void heapify_range(vector<string>& vec, int low, int high, int i){
    int l=2*(i-low)+1+low, r=2*(i-low)+2+low, lg=i;
    if (l<=high && vec[l]>vec[lg]) lg=l;
    if (r<=high && vec[r]>vec[lg]) lg=r;
    if (lg!=i){ std::swap(vec[i], vec[lg]); heapify_range(vec, low, high, lg); }
}

static void heap_sort_range(vector<string>& vec, int low, int high){
    int n=high-low+1;
    for (int i=low+n/2-1; i>=low; --i){ heapify_range(vec, low, high, i); }
    for (int i=high; i>low; --i){ std::swap(vec[low], vec[i]); heapify_range(vec, low, i-1, low); }
}

// Insertion
void InsertionSorter::insertionsort(vector<string>& vec, int low, int high){
    for(int i= low+1; i<=high; ++i){
        string key = vec[i];
        int j=i-1;
        while (j>=low && vec[j]>key){ vec[j+1]=vec[j]; --j; }
        vec[j+1] = key;
    }
}
void InsertionSorter::sort(){
    if (vec.empty()) return;
    insertionsort(vec, 0, vec.size()-1);
}

// Intro
static void introsort_helper(vector<string>& vec, int low, int high, int depth_limit){
    int size = high-low+1;
    if (size<=16){ InsertionSorter::insertionsort(vec, low, high); return; }
    if (depth_limit==0){ heap_sort_range(vec, low, high); return; }
    introsort_helper(vec, low, QuickSorter::partition(vec, low, high)-1, depth_limit-1);
    introsort_helper(vec, QuickSorter::partition(vec, low, high)+1, high, depth_limit-1);
}
void IntroSorter::introsort(vector<string>& vec, int low, int high) { introsort_helper(vec, low, high, 2*log(high-low+1)); }
void IntroSorter::sort() {
    if (vec.empty()) return;
    introsort(vec, 0, vec.size()-1);
}

// STL Sort
void STLSorter::sort() {
    if (vec.empty()) return;
    std::sort(vec.begin(), vec.end());
}

// Stable
void StableSorter::sort() {
    if (vec.empty()) return;
    std::stable_sort(vec.begin(), vec.end());
}

// Shell
void ShellSorter::gapInsertionSort(vector<string> & avector, int start, int gap) {
    int s=avector.size();
    for (int i=start+gap; i<s; i+=gap) {
        string c=avector[i];
        int p=i;
        while (p>=gap && avector[p-gap]>c){
            avector[p] = avector[p-gap];
            p -= gap;
        }
        avector[p]=c;
    }
}
void ShellSorter::shellSort(vector<string> & avector){
    for (int gap=avector.size()/2; gap>0; gap /= 2){
        for (int i=0; i<gap; ++i){ gapInsertionSort(avector, i, gap); }
    }
}
void ShellSorter::sort(){
    if (vec.empty()) return;
    shellSort(vec); 
}


// extra
ostream & operator << (ostream &out, Sorter &L){ L.print(out); return out; }
void error(string word, string msg){ std::cerr << "Error: " << word << " - " << msg << std::endl; }
void measure_partition(int k, string file_name, Sorter & L){
    int limit = k * NWORDS / 10;
    L.fill(limit, file_name);
    Timer t;
    double userTime;
    t.start();
    L.sort();
    t.elapsedUserTime(userTime);
    if (!L.verify_sorted())
        error(L.name, "sorting failed");
    std::cout << "\t" << userTime << std::endl;
}
void measure_partitions(string file_name, Sorter & L){
    std::cout << L.name << ":" << std::endl;
    for (int k = 1; k <= 10; k++) {
        measure_partition(k, file_name, L);
    }
}

void measure_sorters(string input_file){
    std::cout << "File: " << input_file << std::endl;
    QuickSorter qs;
    HeapSorter hs;
    InsertionSorter is;
    IntroSorter ins;
    STLSorter stl;
    StableSorter ss;
    //ShellSorter sh;

    measure_partitions(input_file, qs);
    measure_partitions(input_file, hs);
    measure_partitions(input_file, is);
    measure_partitions(input_file, ins);
    measure_partitions(input_file, stl);
    measure_partitions(input_file, ss);
    // measure_partitions(input_file, sh);
}
