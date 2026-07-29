import matplotlib.pyplot as plt

# --- Data ---
runs = ['Run 1', 'Run 2', 'Run 3']

# Int data (100,000,000 elements)
int_data = {
    'find(int) - Hit Middle': [36.6, 37.7, 38.9],
    'find(int) - Miss': [59.4, 60.7, 59.0],
    'find_if(int) - Hit Middle': [30.7, 28.8, 27.5],
    'find_if(int) - Miss': [75.7, 71.6, 74.9],
}

# String data (1,000,000 elements)
string_data = {
    'find(string) - Hit Middle': [4.1, 4.2, 6.8],
    'find(string) - Miss': [10.4, 10.6, 8.9],
}

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
fig.suptitle('Benchmark Runtime Variance Across Runs', fontsize=15, fontweight='bold')

# --- Plot 1: vector<int> (100M elements) ---
ax1.plot(runs, int_data['find(int) - Hit Middle'], label='find() - Hit Middle', color='#1f77b4', marker='o', linewidth=2)
ax1.plot(runs, int_data['find(int) - Miss'], label='find() - Miss', color='#1f77b4', marker='s', linestyle='--', linewidth=2)
ax1.plot(runs, int_data['find_if(int) - Hit Middle'], label='find_if() - Hit Middle', color='#2ca02c', marker='^', linewidth=2)
ax1.plot(runs, int_data['find_if(int) - Miss'], label='find_if() - Miss', color='#2ca02c', marker='d', linestyle='--', linewidth=2)

ax1.set_title('vector<int> (N = 100,000,000)', fontsize=12, pad=10)
ax1.set_ylabel('Runtime (ms)', fontsize=10)
ax1.set_ylim(20, 85)  # Zoomed to fit int data nicely
ax1.grid(True, linestyle='--', alpha=0.5)
ax1.legend(loc='upper right', fontsize=9)

# --- Plot 2: vector<string> (1M elements) ---
ax2.plot(runs, string_data['find(string) - Hit Middle'], label='find() - Hit Middle', color='#ff7f0e', marker='o', linewidth=2)
ax2.plot(runs, string_data['find(string) - Miss'], label='find() - Miss', color='#ff7f0e', marker='s', linestyle='--', linewidth=2)

ax2.set_title('vector<string> (N = 1,000,000)', fontsize=12, pad=10)
ax2.set_ylabel('Runtime (ms)', fontsize=10)
ax2.set_ylim(0, 14)   # Zoomed to fit string data nicely
ax2.grid(True, linestyle='--', alpha=0.5)
ax2.legend(loc='upper right', fontsize=9)

plt.tight_layout()
plt.show()