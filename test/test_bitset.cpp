#include <iostream>
#include <vector>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>

extern "C" {
    #include "bitset.h"
}

#define UNIV_SIZE 100000000 
#define NUM_QUERIES 5000000

using namespace std::chrono;

void print_row(const char* op, double vec_time, double static_time, double boost_time, double c_time) {
    double min_time = std::min({vec_time, static_time, boost_time, c_time});
    const char* winner = (min_time == c_time) ? "EALib" : 
                         (min_time == boost_time) ? "boost::dynamic_bitset" :
                         (min_time == static_time) ? "std::bitset" : "std::vector";
                         
    std::cout << std::left << std::setw(14) << op << " | "
              << std::right << std::setw(11) << vec_time << " | "
              << std::setw(11) << static_time << " | "
              << std::setw(21) << boost_time << " | "
              << std::setw(11) << c_time << " | "
              << winner << "\n";
}

int main() {
    std::cout << "\nStarting Fixed Universe Set Benchmarks (-O3 -flto required)\n";
    std::cout << "====================================================================================================\n";
    std::cout << "    Operation  | std::vector | std::bitset | boost::dynamic_bitset |       EALib | Winner\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";

    std::vector<bool> cpp_vec1(UNIV_SIZE, false);
    std::vector<bool> cpp_vec2(UNIV_SIZE, false);
    std::vector<bool> cpp_dest(UNIV_SIZE, false);

    auto* cpp_static1 = new std::bitset<UNIV_SIZE>();
    auto* cpp_static2 = new std::bitset<UNIV_SIZE>();
    auto* cpp_static_dest = new std::bitset<UNIV_SIZE>();

    boost::dynamic_bitset<> boost_set1(UNIV_SIZE);
    boost::dynamic_bitset<> boost_set2(UNIV_SIZE);
    boost::dynamic_bitset<> boost_dest(UNIV_SIZE);

    struct bitset *c_set1 = nullptr, *c_set2 = nullptr, *c_dest = nullptr;
    init_bitset(&c_set1, UNIV_SIZE);
    init_bitset(&c_set2, UNIV_SIZE);
    init_bitset(&c_dest, UNIV_SIZE);

    std::mt19937 gen(42);
    std::uniform_int_distribution<size_t> dist(0, UNIV_SIZE - 1);
    std::vector<size_t> random_indices(NUM_QUERIES);
    for (size_t i = 0; i < NUM_QUERIES; ++i) random_indices[i] = dist(gen);

    // ==========================================
    // BENCHMARK 1: Bulk Insert
    // ==========================================
    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) cpp_vec1[random_indices[i]] = true;
    auto end = high_resolution_clock::now();
    double vec_insert = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) (*cpp_static1)[random_indices[i]] = true;
    end = high_resolution_clock::now();
    double static_insert = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) boost_set1[random_indices[i]] = true;
    end = high_resolution_clock::now();
    double boost_insert = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) add_element(c_set1, random_indices[i]);
    end = high_resolution_clock::now();
    double c_insert = duration_cast<microseconds>(end - start).count();
    
    print_row("Insert x5M", vec_insert, static_insert, boost_insert, c_insert);

    for (size_t i = 0; i < NUM_QUERIES; ++i) {
        size_t idx = dist(gen);
        cpp_vec2[idx] = true;
        (*cpp_static2)[idx] = true;
        boost_set2[idx] = true;
        add_element(c_set2, idx);
    }

    // ==========================================
    // BENCHMARK 2: Point Queries
    // ==========================================
    volatile int dummy = 0; 
    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) dummy = cpp_vec1[random_indices[i]];
    end = high_resolution_clock::now();
    double vec_query = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) dummy = (*cpp_static1)[random_indices[i]];
    end = high_resolution_clock::now();
    double static_query = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) dummy = boost_set1[random_indices[i]];
    end = high_resolution_clock::now();
    double boost_query = duration_cast<microseconds>(end - start).count();

    int is_in = 0;
    start = high_resolution_clock::now();
    for (size_t i = 0; i < NUM_QUERIES; ++i) {
        is_element_in(c_set1, random_indices[i], &is_in);
        dummy = is_in;
    }
    end = high_resolution_clock::now();
    double c_query = duration_cast<microseconds>(end - start).count();

    print_row("Query x5M", vec_query, static_query, boost_query, c_query);

    // ==========================================
    // BENCHMARK 3: Intersection 
    // ==========================================
    start = high_resolution_clock::now();
    for (size_t i = 0; i < UNIV_SIZE; ++i) cpp_dest[i] = cpp_vec1[i] && cpp_vec2[i];
    end = high_resolution_clock::now();
    double vec_intersect = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    *cpp_static_dest = *cpp_static1;
    *cpp_static_dest &= *cpp_static2;
    end = high_resolution_clock::now();
    double static_intersect = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    boost_dest = boost_set1;
    boost_dest &= boost_set2;
    end = high_resolution_clock::now();
    double boost_intersect = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    bitset_intersect(c_dest, c_set1, c_set2);
    end = high_resolution_clock::now();
    double c_intersect = duration_cast<microseconds>(end - start).count();

    dummy += c_dest->arr[0] + cpp_static_dest->test(0) + boost_dest.test(0) + cpp_dest[0];
    print_row("Intersection", vec_intersect, static_intersect, boost_intersect, c_intersect);

    // ==========================================
    // BENCHMARK 4: Union 
    // ==========================================
    start = high_resolution_clock::now();
    for (size_t i = 0; i < UNIV_SIZE; ++i) cpp_dest[i] = cpp_vec1[i] || cpp_vec2[i];
    end = high_resolution_clock::now();
    double vec_union = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    *cpp_static_dest = *cpp_static1;
    *cpp_static_dest |= *cpp_static2;
    end = high_resolution_clock::now();
    double static_union = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    boost_dest = boost_set1;
    boost_dest |= boost_set2;
    end = high_resolution_clock::now();
    double boost_union = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    bitset_union(c_dest, c_set1, c_set2);
    end = high_resolution_clock::now();
    double c_union = duration_cast<microseconds>(end - start).count();

    dummy += c_dest->arr[0] + cpp_static_dest->test(0) + boost_dest.test(0) + cpp_dest[0];
    print_row("Union", vec_union, static_union, boost_union, c_union);

    // ==========================================
    // BENCHMARK 5: Difference
    // ==========================================
    start = high_resolution_clock::now();
    for (size_t i = 0; i < UNIV_SIZE; ++i) cpp_dest[i] = cpp_vec1[i] && !cpp_vec2[i];
    end = high_resolution_clock::now();
    double vec_diff = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    *cpp_static_dest = *cpp_static2;
    cpp_static_dest->flip(); 
    *cpp_static_dest &= *cpp_static1;
    end = high_resolution_clock::now();
    double static_diff = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    boost_dest = boost_set2;
    boost_dest.flip();
    boost_dest &= boost_set1;
    end = high_resolution_clock::now();
    double boost_diff = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    bitset_difference(c_dest, c_set1, c_set2);
    end = high_resolution_clock::now();
    double c_diff = duration_cast<microseconds>(end - start).count();

    dummy += c_dest->arr[0] + cpp_static_dest->test(0) + boost_dest.test(0) + cpp_dest[0]; 
    print_row("Difference", vec_diff, static_diff, boost_diff, c_diff);

    // ==========================================
    // BENCHMARK 6: Population Count
    // ==========================================
    start = high_resolution_clock::now();
    size_t vec_pop = std::count(cpp_vec1.begin(), cpp_vec1.end(), true);
    end = high_resolution_clock::now();
    double vec_pop_time = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    size_t static_pop = cpp_static1->count();
    end = high_resolution_clock::now();
    double static_pop_time = duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    size_t boost_pop = boost_set1.count();
    end = high_resolution_clock::now();
    double boost_pop_time = duration_cast<microseconds>(end - start).count();

    size_t c_pop = 0;
    start = high_resolution_clock::now();
    get_population(c_set1, &c_pop);
    end = high_resolution_clock::now();
    double c_pop_time = duration_cast<microseconds>(end - start).count();

    print_row("Pop Count", vec_pop_time, static_pop_time, boost_pop_time, c_pop_time);
    
    std::cout << "====================================================================================================\n\n";

    if (vec_pop == 0 || static_pop == 0 || boost_pop == 0 || c_pop == 0 || dummy == -1) std::cout << ""; 

    delete cpp_static1;
    delete cpp_static2;
    delete cpp_static_dest;
    free_bitset(&c_set1);
    free_bitset(&c_set2);
    free_bitset(&c_dest);

    return 0;
}



