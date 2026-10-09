/* Quick Sort on package weights - Part (a) basic (Lomuto), Part (b) stable, with trace */
#include <stdio.h>
#include <string.h>

typedef struct { char id[8]; int w; } Pkg;
static long cmp_count = 0;
static int step = 0;

static void show(const char *label, Pkg *a, int lo, int hi) {
    printf("  %-22s [", label);
    for (int i = lo; i <= hi; i++) printf("%s(%d)%s", a[i].id, a[i].w, i < hi ? " " : "");
    printf("]\n");
}

/* ---------- Part (a): basic Quick Sort, Lomuto partition, last element pivot ---------- */
static int partition(Pkg *a, int lo, int hi) {
    int pivot = a[hi].w, i = lo - 1;
    for (int j = lo; j < hi; j++) {
        cmp_count++;
        if (a[j].w <= pivot) { i++; Pkg t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    Pkg t = a[i + 1]; a[i + 1] = a[hi]; a[hi] = t;
    return i + 1;
}
static void qsort_basic(Pkg *a, int lo, int hi) {
    if (lo >= hi) return;
    printf("  Step %2d range [%d..%d] pivot=%s(%d)\n", ++step, lo, hi, a[hi].id, a[hi].w);
    int p = partition(a, lo, hi);
    show("after partition:", a, lo, hi);
    printf("  %-22s pivot placed at index %d\n", "", p);
    qsort_basic(a, lo, p - 1);
    qsort_basic(a, p + 1, hi);
}

/* ---------- Part (b): stable Quick Sort (stable 3-way partition using auxiliary arrays) ---------- */
static void qsort_stable(Pkg *a, int lo, int hi) {
    if (lo >= hi) return;
    int pivot = a[hi].w;
    Pkg L[64], E[64], G[64]; int nl = 0, ne = 0, ng = 0;
    printf("  Step %2d range [%d..%d] pivot value=%d\n", ++step, lo, hi, pivot);
    for (int i = lo; i <= hi; i++) {        /* scan left->right keeps original order */
        cmp_count++;
        if (a[i].w < pivot) L[nl++] = a[i];
        else if (a[i].w == pivot) E[ne++] = a[i];
        else G[ng++] = a[i];
    }
    int k = lo;
    for (int i = 0; i < nl; i++) a[k++] = L[i];
    int eStart = k;
    for (int i = 0; i < ne; i++) a[k++] = E[i];
    int eEnd = k - 1;
    for (int i = 0; i < ng; i++) a[k++] = G[i];
    show("after partition:", a, lo, hi);
    printf("  %-22s equal block at [%d..%d]\n", "", eStart, eEnd);
    qsort_stable(a, lo, eStart - 1);
    qsort_stable(a, eEnd + 1, hi);
}

static int load(Pkg *a) {
    FILE *f = fopen("input.txt", "r");
    if (!f) { perror("input.txt"); return 0; }
    int n; fscanf(f, "%d", &n);
    for (int i = 0; i < n; i++) fscanf(f, "%7s %d", a[i].id, &a[i].w);
    fclose(f);
    return n;
}

static void run(int stable) {
    Pkg a[64]; int n = load(a);
    cmp_count = 0; step = 0;
    printf("\n=== QUICK SORT (%s) ===\n", stable ? "Part b: STABLE 3-way partition"
                                                   : "Part a: BASIC Lomuto");
    show("Input:", a, 0, n - 1);
    if (stable) qsort_stable(a, 0, n - 1); else qsort_basic(a, 0, n - 1);
    show("Sorted:", a, 0, n - 1);
    printf("  Comparisons = %ld\n", cmp_count);
    int ok = 1;
    for (int i = 0; i + 1 < n; i++)
        if (a[i].w == a[i+1].w && strcmp(a[i].id, a[i+1].id) > 0) ok = 0;
    printf("  Stable? %s\n", ok ? "YES (equal weights keep original ID order)" : "NO (equal weights reordered)");
}

int main(void) { run(0); run(1); return 0; }
