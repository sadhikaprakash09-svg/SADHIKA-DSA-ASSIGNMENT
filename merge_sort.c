/* Merge Sort on package weights - Part (a) basic, Part (b) stable, with trace */
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

/* stable=1 : take LEFT element on ties (<=)  -> stable
   stable=0 : take RIGHT element on ties (<)  -> basic version, NOT stable */
static void merge(Pkg *a, int lo, int mid, int hi, int stable) {
    Pkg tmp[64];
    int i = lo, j = mid + 1, k = 0;
    while (i <= mid && j <= hi) {
        cmp_count++;
        int takeLeft = stable ? (a[i].w <= a[j].w) : (a[i].w < a[j].w);
        tmp[k++] = takeLeft ? a[i++] : a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];
    for (k = 0; k < hi - lo + 1; k++) a[lo + k] = tmp[k];
    printf("  Step %2d MERGE [%d..%d] ->", ++step, lo, mid);
    printf(" [%d..%d]\n", mid + 1, hi);
    show("result:", a, lo, hi);
}

static void msort(Pkg *a, int lo, int hi, int stable) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    printf("  DIVIDE [%d..%d] -> [%d..%d] | [%d..%d]\n", lo, hi, lo, mid, mid + 1, hi);
    msort(a, lo, mid, stable);
    msort(a, mid + 1, hi, stable);
    merge(a, lo, mid, hi, stable);
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
    printf("\n=== MERGE SORT (%s) ===\n", stable ? "Part b: STABLE, ties take left (<=)"
                                                   : "Part a: BASIC, ties take right (<)");
    show("Input:", a, 0, n - 1);
    msort(a, 0, n - 1, stable);
    show("Sorted:", a, 0, n - 1);
    printf("  Comparisons = %ld\n", cmp_count);
    printf("  Stable? ");
    int ok = 1;
    for (int i = 0; i + 1 < n; i++)
        if (a[i].w == a[i+1].w && strcmp(a[i].id, a[i+1].id) > 0) ok = 0;
    printf("%s\n", ok ? "YES (equal weights keep original ID order)" : "NO (equal weights reordered)");
}

int main(void) { run(0); run(1); return 0; }
