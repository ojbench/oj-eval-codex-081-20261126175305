# Solution

By the max-flow/min-cut theorem, the requested value for a pair is its
minimum edge cut. The degree bound makes every such value at most three.

Build a Gomory-Hu tree using n-1 minimum-cut computations. Each undirected
edge is represented by a pair of reverse residual arcs, initially both of
capacity one. Each augmenting path sends one unit, so there are at most
three successful breadth-first searches per computation. The final failed
search gives the source side of a minimum cut, which is used for the
standard Gomory-Hu parent updates. Disconnected vertices are supported:
their separating cuts, and corresponding tree edges, have weight zero.

The Gomory-Hu property says that the minimum edge weight on the tree path
between any two vertices equals their minimum cut in the original graph.
Process tree edges in decreasing weight using a disjoint-set structure.
When an edge of weight w joins components of sizes a and b, exactly a*b
new pairs become connected. Each such pair has minimum path weight w,
so its contribution is w*a*b. Every pair with positive value is counted
once. Zero-weight edges need not be processed.

Each minimum-cut computation takes O(n+m) time; parent updates take O(n).
Total time is O(n(n+m)), and space is O(n+m). The answer uses 64-bit
arithmetic.

## Validation

Build with `cmake -S . -B build && cmake --build build`.
Run `python3 tests/check.py ./build/code` for exhaustive graphs through
four vertices and 500 deterministic random graphs, comparing every pair
against exhaustive enumeration of cuts.

The statement sample returns 36. Maximum-size checks include an empty
graph (0), a path (4498500), and a random cycle plus matching (13495500).
The supplied-data directory documented in README.md was unavailable.
