# Q10 – Package Sorting: Merge Sort vs Quick Sort (in C)

**Input:** weights `20, 15, 20, 10, 15, 20, 25, 10` with unique IDs `P1 … P8` (assigned in arrival order, see `input.txt`).

| File | Purpose |
|---|---|
| `merge_sort.c`, `quick_sort.c` | Source code (Part a basic + Part b stable) |
| `input.txt` | Input data |
| `output_merge_sort.txt`, `output_quick_sort.txt` | Actual program output |
| `README.md` | Trace tables, complexity analysis, comparison, conclusion |

Build/run: `gcc merge_sort.c -o merge_sort && ./merge_sort` (same for `quick_sort`).

---
## Part (a) – Trace tables (basic versions)

### Merge Sort (ties take right element → not stable)
| Step | Merge | Result |
|---|---|---|
| 1 | [0]+[1] | P2(15) P1(20) |
| 2 | [2]+[3] | P4(10) P3(20) |
| 3 | [0..1]+[2..3] | P4(10) P2(15) **P3(20) P1(20)** |
| 4 | [4]+[5] | P5(15) P6(20) |
| 5 | [6]+[7] | P8(10) P7(25) |
| 6 | [4..5]+[6..7] | P8(10) P5(15) P6(20) P7(25) |
| 7 | [0..3]+[4..7] | **P8 P4** P5 P2 **P6 P3 P1** P7 → final |

Final: `P8(10) P4(10) P5(15) P2(15) P6(20) P3(20) P1(20) P7(25)` – **17 comparisons**

### Quick Sort (Lomuto, last element as pivot)
| Step | Range | Pivot | Array after partition | Pivot index |
|---|---|---|---|---|
| 1 | 0..7 | P8(10) | P4 P8 P3 P1 P5 P6 P7 P2 | 1 |
| 2 | 2..7 | P2(15) | P5 P2 P3 P6 P7 P1 | 3 |
| 3 | 4..7 | P1(20) | P3 P6 P1 P7 | 6 |
| 4 | 4..5 | P6(20) | P3 P6 | 5 |

Final: `P4(10) P8(10) P5(15) P2(15) P3(20) P6(20) P1(20) P7(25)` – **16 comparisons**

Both sort correctly by weight, but equal weights are reordered (original order: P4<P8, P2<P5, P1<P3<P6).

---
## Part (b) – Stable versions

* **Merge Sort fix:** in `merge()`, take the left element when `left <= right`.
* **Quick Sort fix:** stable 3-way partition – scan left→right copying elements `< pivot`, `== pivot`, `> pivot` into auxiliary arrays, then write back `L, E, G` (O(n) extra space).

### Stable Merge Sort trace
| Step | Merge | Result |
|---|---|---|
| 1 | [0]+[1] | P2(15) P1(20) |
| 2 | [2]+[3] | P4(10) P3(20) |
| 3 | [0..1]+[2..3] | P4(10) P2(15) P1(20) P3(20) |
| 4 | [4]+[5] | P5(15) P6(20) |
| 5 | [6]+[7] | P8(10) P7(25) |
| 6 | [4..5]+[6..7] | P8(10) P5(15) P6(20) P7(25) |
| 7 | [0..3]+[4..7] | P4 P8 P2 P5 P1 P3 P6 P7 |

### Stable Quick Sort trace
| Step | Range | Pivot | Result (L \| E \| G) | Equal block |
|---|---|---|---|---|
| 1 | 0..7 | 10 | P4 P8 \| P1 P2 P3 P5 P6 P7 (original order kept) | [0..1] |
| 2 | 2..7 | 25 | P1 P2 P3 P5 P6 \| P7 | [7] |
| 3 | 2..6 | 20 | P2 P5 \| P1 P3 P6 | [4..6] |
| 4 | 2..3 | 15 | P2 P5 | [2..3] |

### Verification using package IDs
Final (both stable versions): `P4(10) P8(10) P2(15) P5(15) P1(20) P3(20) P6(20) P7(25)`

| Weight | Original ID order | Sorted ID order | Preserved? |
|---|---|---|---|
| 10 | P4, P8 | P4, P8 | ✔ |
| 15 | P2, P5 | P2, P5 | ✔ |
| 20 | P1, P3, P6 | P1, P3, P6 | ✔ |
| 25 | P7 | P7 | ✔ |

---
## Part (c) – Analysis

**Duplicate values.** The input has 5 repeated values (10×2, 15×2, 20×3). Merge Sort is unaffected by duplicates (always splits in the middle). Basic Quick Sort with `<=` partitioning is also correct, but heavy duplicates cause unbalanced partitions (all-equal input → O(n²)); a 3-way partition handles duplicates well.

**Stability.** Merge Sort is stable when ties take the left element. Basic Quick Sort is **not** stable because partition swaps move elements across each other (e.g. P1 ended after P3/P6). It can be made stable only with extra memory.

**Number of comparisons (n = 8, measured).**
| Version | Comparisons |
|---|---|
| Merge Sort (basic) | 17 |
| Merge Sort (stable) | 16 |
| Quick Sort (basic) | 16 |
| Quick Sort (stable, 3-way) | 21 |

Theory: Merge Sort needs between ~(n·log₂n)/2 and n·log₂n − n + 1 comparisons (at most 17 for n = 8); Quick Sort ≈ 1.39·n·log₂n average, n(n−1)/2 worst.

**Time and space complexity.**
| | Best | Average | Worst | Space | Stable |
|---|---|---|---|---|---|
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick Sort (basic) | O(n log n) | O(n log n) | O(n²) | O(log n) stack (O(n) worst-case stack) | No |
| Quick Sort (stable, aux arrays) | O(n log n) | O(n log n) | O(n²) | O(n) | Yes |

## Comparison table
| Criterion | Merge Sort | Quick Sort |
|---|---|---|
| Handles duplicates | Well | Well only with 3-way partition |
| Stable | **Yes** (natural) | No (needs O(n) extra to fix) |
| Comparisons (measured) | 16 (stable) | 16 (basic) / 21 (stable) |
| Worst-case time | O(n log n) | O(n²) |
| Extra space | O(n) | O(log n) in-place basic |
| Preserves package order | **Guaranteed** | Only in modified version |

## Final conclusion
When the original order of equal-weight packages must be preserved, **Merge Sort is the more suitable algorithm.** It is naturally stable, has guaranteed O(n log n) time even with many duplicate weights, and gave the correct ID order (P1,P3,P6 for weight 20, etc.) without extra changes. Quick Sort is faster in-place in practice and uses less memory, but its basic form breaks the order of equal packages, and making it stable removes its memory advantage while keeping its O(n²) worst case.
