import csv
import matplotlib.pyplot as plt
from collections import defaultdict
from pathlib import Path

results_file = Path("experiment_results/results.csv")
output_dir = Path("experiment_results")
output_dir.mkdir(exist_ok=True)

rows = []
with open(results_file, newline="") as f:
    reader = csv.DictReader(f)
    for row in reader:
        row["m"] = int(row["m"]) if row["m"] else None
        row["n"] = int(row["n"]) if row["n"] else None
        row["mispred_rate"] = float(row["mispred_rate"])
        rows.append(row)

#bimodal graphs
for benchmark in ["gcc_trace.txt", "jpeg_trace.txt"]:
    data = [r for r in rows if r["predictor"] == "bimodal" and r["benchmark"] == benchmark]
    data.sort(key=lambda r: r["m"])

    x = [r["m"] for r in data]
    y = [r["mispred_rate"] for r in data]

    plt.figure()
    plt.plot(x, y, marker="o")
    plt.xlabel("m")
    plt.ylabel("Misprediction Rate (%)")
    plt.title(f"{benchmark}, bimodal")
    plt.xticks(x)
    plt.tight_layout()
    plt.savefig(output_dir / f"{benchmark.replace('.txt','')}_bimodal.png")
    plt.close()

#gshare graphs
for benchmark in ["gcc_trace.txt", "jpeg_trace.txt"]:
    grouped = defaultdict(list)
    data = [r for r in rows if r["predictor"] == "gshare" and r["benchmark"] == benchmark]

    for r in data:
        grouped[r["n"]].append(r)

    plt.figure()

    for n in sorted(grouped.keys()):
        points = sorted(grouped[n], key=lambda r: r["m"])
        x = [r["m"] for r in points]
        y = [r["mispred_rate"] for r in points]
        plt.plot(x, y, marker="o", label=f"n={n}")

    plt.xlabel("m")
    plt.ylabel("Misprediction Rate (%)")
    plt.title(f"{benchmark}, gshare")
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_dir / f"{benchmark.replace('.txt','')}_gshare.png")
    plt.close()

print("graphs saved in experiment_results/")
