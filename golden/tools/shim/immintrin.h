/* Empty stand-in for <immintrin.h>.
 *
 * The HQC reference headers (src/ref/hqc.h, vector.h, parsing.h) include
 * <immintrin.h> although the ref code uses no x86 intrinsics. On a non-x86
 * host (Apple Silicon) the real header refuses to compile, so this directory
 * is put first on the include path when building the golden dumper.
 */
