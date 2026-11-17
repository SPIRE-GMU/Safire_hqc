/* Timing / statistics helpers for the fafft-c profiling harness: aarch64 Linux port.
 * Derived from the x86 version used for the i9 profile. Only the timer and the CPU pinning are new:
 *   - rdtsc  ->  Arm generic timer, virtual count CNTVCT_EL0 read between ISB barriers;
 *     if user space cannot read it (SIGILL), fall back to clock_gettime(CLOCK_MONOTONIC_RAW) in ns;
 *   - taskset -> sched_setaffinity in a constructor: env PIN_CPU, default CPU 1 when 2+ CPUs are online.
 * Stage bookkeeping, statistics (min/p25/median/p75/mean) and the report format are unchanged. */
#ifndef PROF_COMMON_H
#define PROF_COMMON_H
#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#if !defined(__aarch64__)
#error "this common.h is the aarch64 port; use the x86 version elsewhere"
#endif

#define CAT2_(a, b) a##b
#define CAT2(a, b) CAT2_(a, b)
#define H(name) CAT2(HQCNS, name)   /* HQCNS = PQCLEAN_HQC128_CLEAN_ etc. */

#ifndef SET_NAME
#define SET_NAME "unknown"
#endif
#ifndef OPT_NAME
#define OPT_NAME "unknown"
#endif

/* ---- timer ---------------------------------------------------------------------------- */
static int g_timer_raw = 0;          /* 0: CNTVCT_EL0 ticks, 1: CLOCK_MONOTONIC_RAW ns */
static uint64_t g_cntfrq = 0;        /* CNTFRQ_EL0, Hz (0 if unreadable) */

static inline uint64_t cntvct_read(void) {
    uint64_t v;
    __asm__ volatile("isb\n\tmrs %0, cntvct_el0\n\tisb" : "=r"(v) : : "memory");
    return v;
}
static inline uint64_t raw_ns(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}
static inline uint64_t tsc_begin(void) { return g_timer_raw ? raw_ns() : cntvct_read(); }
static inline uint64_t tsc_end(void) { return g_timer_raw ? raw_ns() : cntvct_read(); }
static inline uint64_t now_ns(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}
/* Timer frequency (ticks per ns, i.e. GHz) from a 500 ms wall-clock interval (CLOCK_MONOTONIC),
 * exactly as the x86 version calibrates the TSC. */
static double tsc_ghz_measure(void) {
    uint64_t n0 = now_ns(), t0 = tsc_begin();
    struct timespec r = {0, 500000000}; nanosleep(&r, NULL);
    uint64_t t1 = tsc_end(), n1 = now_ns();
    return (double)(t1 - t0) / (double)(n1 - n0);
}

static sigjmp_buf g_trap_env;
static void g_trap(int sig) { (void)sig; siglongjmp(g_trap_env, 1); }

__attribute__((constructor)) static void prof_arm_init(void) {
    /* 1) Can user space read the generic timer? Try once with SIGILL caught. */
    struct sigaction sa, old_ill;
    memset(&sa, 0, sizeof sa); sa.sa_handler = g_trap; sigemptyset(&sa.sa_mask);
    volatile int ok = 0;
    volatile uint64_t frq = 0;
    if (sigaction(SIGILL, &sa, &old_ill) == 0) {
        if (sigsetjmp(g_trap_env, 1) == 0) {
            uint64_t f;
            __asm__ volatile("mrs %0, cntfrq_el0" : "=r"(f));
            frq = f;
            (void)cntvct_read();
            ok = 1;
        }
        sigaction(SIGILL, &old_ill, NULL);
    }
    g_cntfrq = frq;
    g_timer_raw = ok ? 0 : 1;

    /* 2) Pin to one CPU (replaces taskset). */
    long online = sysconf(_SC_NPROCESSORS_ONLN);
    int cpu = online >= 2 ? 1 : 0;
    const char *env = getenv("PIN_CPU");
    if (env && *env) cpu = atoi(env);
    cpu_set_t set; CPU_ZERO(&set); CPU_SET(cpu, &set);
    int pin_rc = sched_setaffinity(0, sizeof set, &set);
    int pin_errno = pin_rc ? errno : 0;

    fprintf(stderr, "# arm-port: timer=%s cntfrq_hz=%llu pin_cpu=%d pin=%s%s%s running_on=%d online=%ld\n",
            g_timer_raw ? "clock_monotonic_raw_ns" : "cntvct_el0", (unsigned long long)g_cntfrq, cpu,
            pin_rc ? "failed:" : "ok", pin_rc ? strerror(pin_errno) : "", "", sched_getcpu(), online);
}

/* ---- stages and statistics (unchanged from the x86 version) ---------------------------- */
typedef struct { char name[128]; char cat[16]; uint64_t *s; size_t n, cap; } stage_t;
#define MAX_STAGES 256
static stage_t g_stages[MAX_STAGES];
static int g_nstages = 0;

static int stage_id(const char *name, const char *cat) {
    for (int i = 0; i < g_nstages; i++) if (!strcmp(g_stages[i].name, name)) return i;
    if (g_nstages >= MAX_STAGES) { fprintf(stderr, "too many stages\n"); exit(1); }
    stage_t *st = &g_stages[g_nstages++];
    snprintf(st->name, sizeof st->name, "%s", name);
    snprintf(st->cat, sizeof st->cat, "%s", cat);
    st->cap = 4096; st->s = malloc(st->cap * sizeof(uint64_t)); st->n = 0;
    return g_nstages - 1;
}
static inline void stage_add(int id, uint64_t v) {
    stage_t *st = &g_stages[id];
    if (st->n == st->cap) { st->cap *= 2; st->s = realloc(st->s, st->cap * sizeof(uint64_t)); }
    st->s[st->n++] = v;
}
static int cmp_u64(const void *a, const void *b) { uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b; return x < y ? -1 : x > y; }
typedef struct { uint64_t min, p25, med, p75; double mean; } stats_t;
static stats_t stage_stats(stage_t *st) {
    stats_t r = {0, 0, 0, 0, 0};
    if (!st->n) return r;
    qsort(st->s, st->n, sizeof(uint64_t), cmp_u64);
    r.min = st->s[0]; r.p25 = st->s[st->n / 4]; r.med = st->s[st->n / 2]; r.p75 = st->s[(3 * st->n) / 4];
    double acc = 0; for (size_t i = 0; i < st->n; i++) acc += (double)st->s[i];
    r.mean = acc / (double)st->n;
    return r;
}

/* Per-statement timer: PROF_START() then PROF_NEXT("op/NN name", "cat") after each statement. */
#define PROF_DECL uint64_t _pt = 0
#define PROF_START() (_pt = tsc_begin())
#define PROF_NEXT(name, cat) do { uint64_t _e = tsc_end(); static int _id = -1; \
        if (_id < 0) { _id = stage_id(name, cat); } stage_add(_id, _e - _pt); _pt = tsc_begin(); } while (0)
#define PROF_STOP(name, cat) do { uint64_t _e = tsc_end(); static int _id = -1; \
        if (_id < 0) { _id = stage_id(name, cat); } stage_add(_id, _e - _pt); } while (0)

/* Report: markdown to stdout, CSV rows appended to csv_path. Ticks are timer ticks; the header
 * names the timer, its measured rate and CNTFRQ_EL0. "median us" = median / ghz / 1000 as before. */
static void print_report(double ghz, const char *csv_path, const char *kind) {
    FILE *csv = csv_path ? fopen(csv_path, "a") : NULL;
    printf("\n### %s / %s / %s   (timer %s %.6f GHz, cntfrq %llu Hz)\n\n", kind, SET_NAME, OPT_NAME,
           g_timer_raw ? "clock_monotonic_raw_ns" : "cntvct_el0", ghz, (unsigned long long)g_cntfrq);
    printf("| stage | cat | n | min | p25 | median | p75 | mean | median us |\n|---|---|---:|---:|---:|---:|---:|---:|---:|\n");
    for (int i = 0; i < g_nstages; i++) {
        stage_t *st = &g_stages[i]; stats_t s = stage_stats(st);
        printf("| %s | %s | %zu | %llu | %llu | %llu | %llu | %.0f | %.3f |\n", st->name, st->cat, st->n,
               (unsigned long long)s.min, (unsigned long long)s.p25, (unsigned long long)s.med,
               (unsigned long long)s.p75, s.mean, (double)s.med / ghz / 1000.0);
        if (csv) fprintf(csv, "%s,%s,%s,\"%s\",%s,%zu,%llu,%llu,%llu,%llu,%.0f,%.9f\n", kind, SET_NAME, OPT_NAME,
                         st->name, st->cat, st->n, (unsigned long long)s.min, (unsigned long long)s.p25,
                         (unsigned long long)s.med, (unsigned long long)s.p75, s.mean, ghz);
    }
    if (csv) fclose(csv);
}

/* Deterministic filler for test inputs (not security relevant). */
static uint64_t g_lcg = 0x9E3779B97F4A7C15ULL;
static inline uint64_t lcg_next(void) { g_lcg = g_lcg * 6364136223846793005ULL + 1442695040888963407ULL; return g_lcg ^ (g_lcg >> 29); }
static void fill_random_bytes(uint8_t *p, size_t n) { for (size_t i = 0; i < n; i++) p[i] = (uint8_t)(lcg_next() >> 24); }
#endif
