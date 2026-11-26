"""Compare the executable with exhaustive cuts on small subcubic graphs."""
import random
import subprocess
import sys


def check(n, edges):
    expected = 0
    cuts = []
    for mask in range(1 << n):
        cuts.append(sum(((mask >> u) ^ (mask >> v)) & 1 for u, v in edges))
    for u in range(n):
        for v in range(u):
            expected += min(cuts[mask] for mask in range(1 << n)
                            if ((mask >> u) ^ (mask >> v)) & 1)
    data = f"{n} {len(edges)}\n" + "".join(f"{u+1} {v+1}\n" for u, v in edges)
    actual = int(subprocess.check_output([sys.argv[1]], input=data.encode()))
    assert actual == expected, (data, expected, actual)


random.seed(1761)
for n in range(2, 5):
    pairs = [(u, v) for u in range(n) for v in range(u)]
    for mask in range(1 << len(pairs)):
        check(n, [edge for i, edge in enumerate(pairs) if mask >> i & 1])
for _ in range(500):
    n = random.randint(2, 9)
    pairs = [(u, v) for u in range(n) for v in range(u)]
    random.shuffle(pairs)
    degrees = [0] * n
    edges = []
    for u, v in pairs:
        if degrees[u] < 3 and degrees[v] < 3 and random.random() < 0.6:
            edges.append((u, v))
            degrees[u] += 1
            degrees[v] += 1
    check(n, edges)
print("Passed exhaustive graphs through n=4 and 500 random graphs.")
