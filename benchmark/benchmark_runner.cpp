#include "../tree/binary_search_tree.hpp"
#include "../containers/dynamic_array.hpp"

#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <iomanip>

struct BenchmarkResult {
    std::size_t size;
    double insertTimeMs;
    double searchTimeMs;
    double removeTimeMs;
};

BenchmarkResult runBenchmarkForSize(std::size_t n, std::mt19937_64& rng) {
    BinarySearchTree<uint64_t> tree;
    DynamicArray<uint64_t> insertedKeys;

    std::uniform_int_distribution<uint64_t> dist(1, n * 10);

    // 1. Замер времени вставки (Insert)
    auto startInsert = std::chrono::high_resolution_clock::now();
    while (tree.size() < n) {
        uint64_t val = dist(rng);
        if (tree.insert(val)) {
            insertedKeys.push_back(val);
        }
    }
    auto endInsert = std::chrono::high_resolution_clock::now();
    double insertMs = std::chrono::duration<double, std::milli>(endInsert - startInsert).count();

    // 2. Замер времени поиска (Search) — 10000 случайных запросов
    const std::size_t searchQueries = 10000;
    auto startSearch = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < searchQueries; ++i) {
        uint64_t query = (i % 2 == 0) ? insertedKeys[i % insertedKeys.size()] : dist(rng);
        tree.contains(query);
    }
    auto endSearch = std::chrono::high_resolution_clock::now();
    double searchMs = std::chrono::duration<double, std::milli>(endSearch - startSearch).count();

    // 3. Замер времени удаления (Remove) — удаление половины элементов
    const std::size_t removeCount = n / 2;
    auto startRemove = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < removeCount; ++i) {
        tree.remove(insertedKeys[i]);
    }
    auto endRemove = std::chrono::high_resolution_clock::now();
    double removeMs = std::chrono::duration<double, std::milli>(endRemove - startRemove).count();

    return {n, insertMs, searchMs, removeMs};
}

int main() {
    std::cout << "==============================================================\n";
    std::cout << "         BENCHMARK: BINARY SEARCH TREE PERFORMANCE            \n";
    std::cout << "==============================================================\n\n";

    std::mt19937_64 rng(1337);
    DynamicArray<std::size_t> testSizes = {
        10000, 25000, 50000, 75000, 100000, 250000, 500000, 750000, 1000000
    };

    std::ofstream csvFile("benchmark_results.csv");
    if (!csvFile.is_open()) {
        std::cerr << "Error: failed to create benchmark_results.csv\n";
        return 1;
    }

    csvFile << "Size,InsertTimeMs,SearchTimeMs,RemoveTimeMs\n";

    std::cout << std::setw(12) << "Elements"
              << std::setw(18) << "Insert (ms)"
              << std::setw(18) << "Search x10k (ms)"
              << std::setw(18) << "Remove N/2 (ms)\n";
    std::cout << std::string(66, '-') << "\n";

    for (std::size_t i = 0; i < testSizes.size(); ++i) {
        std::size_t n = testSizes[i];
        BenchmarkResult res = runBenchmarkForSize(n, rng);

        std::cout << std::setw(12) << res.size
                  << std::setw(18) << std::fixed << std::setprecision(2) << res.insertTimeMs
                  << std::setw(18) << std::fixed << std::setprecision(2) << res.searchTimeMs
                  << std::setw(18) << std::fixed << std::setprecision(2) << res.removeTimeMs << "\n";

        csvFile << res.size << ","
                << res.insertTimeMs << ","
                << res.searchTimeMs << ","
                << res.removeTimeMs << "\n";
    }

    csvFile.close();
    std::cout << "\nResults successfully saved to: benchmark_results.csv\n";
    std::cout << "To plot graphs, run: python3 benchmark/plot_results.py\n";
    return 0;
}
