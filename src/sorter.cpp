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
    int i=0;
    while(infile>>word && i<count){ vec.push_back(word); ++i; }
}
void Sorter::print(ostream & out){
    for(const auto &word : vec){ out << word << " "; }
}
bool Sorter::verify_sorted(){
    for(size_t i=1; i<vec.size(); ++i){ if (vec[i-1]>vec[i]) return false; }
    return true;
}

// Insertion
void InsertionSorter::insertionsort(vector<string>& vec, int low, int high) {
    for (int i=low + 1; i<=high; ++i){
        string key = vec[i];
        int j = i-1;
        while (j>=low && vec[j]>key) {
            vec[j+1] = vec[j];
            j--;
        }
        vec[j+1] = key;
    }
}
void InsertionSorter::sort() {
    if (vec.empty()) return;
    insertionsort(vec, 0, vec.size()-1);
}

// Quick
string QuickSorter::select_pivot(vector<string>& vec, int low, int high) { (void)low; return vec[high]; }

int QuickSorter::partition(vector<string>& vec, int low, int high) {
    string pivot = select_pivot(vec, low, high);
    int i = low;
    for (int j = low; j < high; j++) {
        if (vec[j] < pivot) {
            std::swap(vec[i], vec[j]);
            i++;
        }
    }
    std::swap(vec[i], vec[high]);
    return i;
}
void QuickSorter::quicksort(vector<string>& vec, int low, int high){
    if (low < high) {
        int pi = partition(vec, low, high);
        quicksort(vec, low, pi-1);
        quicksort(vec, pi+1, high);
    }
}
void QuickSorter::sort(){
    if (vec.empty()) return;
    quicksort(vec, 0, vec.size() - 1);
}

// Heap
void HeapSorter::heapify(vector<string>& vec, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && vec[left] > vec[largest])
        largest = left;
    if (right < n && vec[right] > vec[largest])
        largest = right;
    if (largest != i) {
        std::swap(vec[i], vec[largest]);
        heapify(vec, n, largest);
    }
}

void HeapSorter::heapsort(vector<string>& vec, int low, int high) {
    (void)low; (void)high;
    int n = vec.size();
    for(int i=n/2-1; i >= 0; --i){ heapify(vec, n, i); }
    for(int i=n-1; i > 0; --i){
        std::swap(vec[0], vec[i]);
        heapify(vec, i, 0);
    }
}
void HeapSorter::sort() {
    if(vec.empty()) return;
    heapsort(vec, 0, vec.size() - 1);
}

static void heapify_range(vector<string>& vec, int low, int high, int i) {
    int left = 2*(i-low)+1+low;
    int right = 2*(i-low)+2+low;
    int largest = i;
    if (left <= high && vec[left] > vec[largest]) largest = left;
    if (right <= high && vec[right] > vec[largest]) largest = right;
    if (largest != i) {
        std::swap(vec[i], vec[largest]);
        heapify_range(vec, low, high, largest);
    }
}

static void heap_sort_range(vector<string>& vec, int low, int high) {
    int n = high-low+1;
    for (int i = low+n/2-1; i>=low; --i){ heapify_range(vec, low, high, i); }
    for (int i = high; i > low; i--) {
        std::swap(vec[low], vec[i]);
        heapify_range(vec, low, i - 1, low);
    }
}

// Intro
static void introsort_helper(vector<string>& vec, int low, int high, int depth_limit) {
    int size = high-low+1;
    if (size <= 16) {
        InsertionSorter::insertionsort(vec, low, high);
        return;
    }
    if (depth_limit==0) {
        heap_sort_range(vec, low, high);
        return;
    }
    int p = QuickSorter::partition(vec, low, high);
    introsort_helper(vec, low, p - 1, depth_limit - 1);
    introsort_helper(vec, p + 1, high, depth_limit - 1);
}
void IntroSorter::introsort(vector<string>& vec, int low, int high) {
    int n = high-low+1;
    int depth_limit = 2*log(n);
    introsort_helper(vec, low, high, depth_limit);
}
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
    int sz = avector.size();
    for (int i = start + gap; i < sz; i += gap) {
        string currentvalue = avector[i];
        int position = i;
        while (position >= gap && avector[position - gap] > currentvalue) {
            avector[position] = avector[position - gap];
            position -= gap;
        }
        avector[position] = currentvalue;
    }
}
void ShellSorter::shellSort(vector<string> & avector) {
    int n = avector.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = 0; i < gap; i++) {
            gapInsertionSort(avector, i, gap);
        }
    }
}
void ShellSorter::sort() {
    if (vec.empty()) return;
    shellSort(vec); 
}


// extra
ostream & operator << (ostream &out, Sorter &L){ L.print(out); return out; }
void error(string word, string msg) { std::cerr << "Error: " << word << " - " << msg << std::endl; }
void measure_partition(int k, string file_name, Sorter & L) {
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
void measure_partitions(string file_name, Sorter & L) {
    std::cout << L.name << ":" << std::endl;
    for (int k = 1; k <= 10; k++) {
        measure_partition(k, file_name, L);
    }
}

void measure_sorters(string input_file) {
    std::cout << "File: " << input_file << std::endl;
    QuickSorter qs;
    HeapSorter hs;
    InsertionSorter is;
    IntroSorter ins;
    STLSorter stl;
    StableSorter ss;
    //ShellSorter sh;

    //measure_partitions(input_file, qs);
    //measure_partitions(input_file, hs);
    //measure_partitions(input_file, is);
    measure_partitions(input_file, ins);
    //measure_partitions(input_file, stl);
    //measure_partitions(input_file, ss);
    // measure_partitions(input_file, sh);
}
