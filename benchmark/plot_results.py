import csv
import matplotlib.pyplot as plt

def plot_benchmarks(filename="benchmark_results.csv"):
    sizes = []
    insert_times = []
    search_times = []
    remove_times = []

    try:
        with open(filename, mode='r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                sizes.append(int(row["Size"]))
                insert_times.append(float(row["InsertTimeMs"]))
                search_times.append(float(row["SearchTimeMs"]))
                remove_times.append(float(row["RemoveTimeMs"]))
    except FileNotFoundError:
        print(f"Error: {filename} not found. Please run the C++ benchmark executable first!")
        return

    plt.figure(figsize=(12, 7))

    plt.plot(sizes, insert_times, marker='o', linewidth=2, color='#1f77b4', label='Insert N elements (O(N log N))')
    plt.plot(sizes, remove_times, marker='s', linewidth=2, color='#d62728', label='Remove N/2 elements (O(N log N))')
    plt.plot(sizes, search_times, marker='^', linewidth=2, color='#2ca02c', label='10 000 Search Queries (O(k log N))')

    plt.title('Binary Search Tree: Operation Time vs Number of Elements', fontsize=14, fontweight='bold')
    plt.xlabel('Number of Elements (N)', fontsize=12)
    plt.ylabel('Execution Time (milliseconds)', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend(fontsize=11)
    plt.tight_layout()

    output_img = "benchmark_plot.png"
    plt.savefig(output_img, dpi=300)
    print(f"Plot successfully saved to: {output_img}")
    plt.show()

if __name__ == '__main__':
    plot_benchmarks()
