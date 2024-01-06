/* TomsFastMath, a fast ISO C bignum library. -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */
#ifndef TFM_PRIVATE_H_
#define TFM_PRIVATE_H_

#include "tfm.h"

/* Compiler compatibility macros */
#if defined(__has_attribute)
#  if __has_attribute(always_inline)
#    define ATTR_ALWAYS_INLINE static inline __attribute__((always_inline))
#  endif
#elif defined(__GNUC__)
#  define ATTR_ALWAYS_INLINE static inline __attribute__((always_inline))
#endif

#ifndef ATTR_ALWAYS_INLINE
#  define ATTR_ALWAYS_INLINE static inline
#endif

#if defined(__has_attribute)
#  if __has_attribute(optimize) && !defined(__clang__)
#    define ATTR_OPTIMIZE_O3 __attribute__((optimize("O3")))
#  endif
#elif defined(__GNUC__) && !defined(__clang__) && \
      (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4))
#  define ATTR_OPTIMIZE_O3 __attribute__((optimize("O3")))
#endif

#ifndef ATTR_OPTIMIZE_O3
#  define ATTR_OPTIMIZE_O3
#endif

#if defined(__has_builtin)
#  if __has_builtin(__builtin_constant_p)
#    define CONSTANT_P(N) __builtin_constant_p((N))
#  endif
#elif defined(__GNUC__)
#  define CONSTANT_P(N) __builtin_constant_p((N))
#endif

#ifndef CONSTANT_P
#  define CONSTANT_P(n) (0)
#endif

/*
 * Private symbols
 * ---------------
 *
 * On Unix symbols can be marked as hidden if tomsfastmath is compiled
 * as a shared object. By default, symbols are visible.
 */
#if defined(__GNUC__) && __GNUC__ >= 4 && !defined(_WIN32) && !defined(__CYGWIN__)
#   define FP_PRIVATE __attribute__ ((visibility ("hidden")))
#else
#   define FP_PRIVATE
#endif

/* VARIOUS LOW LEVEL STUFFS */
#ifdef TFM_NO_STDLIB
FP_PRIVATE void fp_memcpy(void *restrict dst, const void *restrict src, size_t n);
FP_PRIVATE void fp_memset(void *restrict dst, unsigned char c, size_t n);

#define toupper(C) _toupper_impl((C))
ATTR_ALWAYS_INLINE int _toupper_impl(int c) {
   return 'a' <= c && 'z' >= c ? c - ((int)'a' - (int)'A') : c;
}

#define memcpy(D, S, N) _memcpy_impl((D), (S), (N))
ATTR_ALWAYS_INLINE void* _memcpy_impl(void *restrict _d, const void *restrict _s, size_t _n) {
  uint8_t *dst = (uint8_t*)(_d); \
  const uint8_t *src = (uint8_t*)(_s); \
  size_t n = (size_t)(_n);
  if (CONSTANT_P(n)) {
    for (size_t i = 0; i < n; ++i) dst[i] = src[i];
    return (void *)dst;
  }

  fp_memcpy(dst, src, n);
  return (void *)dst;
}

/* The loop increment of 128 bytes was determined experimentally - it results
 * in good inline code optimizations in both GCC and clang. */
#define memset(D, C, N) _memset_impl((D), (C), (N))
ATTR_ALWAYS_INLINE void *_memset_impl(void *_d, int _c, size_t _n) {
   uint8_t *dst = (uint8_t*)(_d);
   uint8_t c = (uint8_t)(_c);
   size_t n = (size_t)(_n);
   if (CONSTANT_P(n)) {
      size_t b = n >> 7, r = n & 127;
      for (size_t i = 0; i < b; ++i) {
         size_t j = 0;
         for (;j <  64; ++j) *dst++ = c;
         for (;j < 128; ++j) *dst++ = c;
      }
      size_t i = 0;
      if (r >  0) for (;i <  64 && i < r; ++i) *dst++ = c;
      if (r > 64) for (;i < 128 && i < r; ++i) *dst++ = c;
      return (void *)dst;
   }

   fp_memset(dst, c, n);
   return (void *)dst;
}
#endif

FP_PRIVATE void s_fp_add(const fp_int *a, const fp_int *b, fp_int *c);
FP_PRIVATE void s_fp_sub(const fp_int *a, const fp_int *b, fp_int *c);
FP_PRIVATE void fp_reverse(unsigned char *s, int len);

FP_PRIVATE void fp_mul_comba(const fp_int *A, const fp_int *B, fp_int *C);

#ifdef TFM_SMALL_SET
FP_PRIVATE void fp_mul_comba_small(const fp_int *A, const fp_int *B, fp_int *C);
#endif

#ifdef TFM_MUL3
FP_PRIVATE void fp_mul_comba3(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL4
FP_PRIVATE void fp_mul_comba4(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL6
FP_PRIVATE void fp_mul_comba6(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL7
FP_PRIVATE void fp_mul_comba7(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL8
FP_PRIVATE void fp_mul_comba8(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL9
FP_PRIVATE void fp_mul_comba9(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL12
FP_PRIVATE void fp_mul_comba12(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL17
FP_PRIVATE void fp_mul_comba17(const fp_int *A, const fp_int *B, fp_int *C);
#endif

#ifdef TFM_MUL20
FP_PRIVATE void fp_mul_comba20(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL24
FP_PRIVATE void fp_mul_comba24(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL28
FP_PRIVATE void fp_mul_comba28(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL32
FP_PRIVATE void fp_mul_comba32(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL48
FP_PRIVATE void fp_mul_comba48(const fp_int *A, const fp_int *B, fp_int *C);
#endif
#ifdef TFM_MUL64
FP_PRIVATE void fp_mul_comba64(const fp_int *A, const fp_int *B, fp_int *C);
#endif

FP_PRIVATE void fp_sqr_comba(const fp_int *A, fp_int *B);

#ifdef TFM_SMALL_SET
FP_PRIVATE void fp_sqr_comba_small(const fp_int *A, fp_int *B);
#endif

#ifdef TFM_SQR3
FP_PRIVATE void fp_sqr_comba3(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR4
FP_PRIVATE void fp_sqr_comba4(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR6
FP_PRIVATE void fp_sqr_comba6(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR7
FP_PRIVATE void fp_sqr_comba7(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR8
FP_PRIVATE void fp_sqr_comba8(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR9
FP_PRIVATE void fp_sqr_comba9(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR12
FP_PRIVATE void fp_sqr_comba12(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR17
FP_PRIVATE void fp_sqr_comba17(const fp_int *A, fp_int *B);
#endif

#ifdef TFM_SQR20
FP_PRIVATE void fp_sqr_comba20(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR24
FP_PRIVATE void fp_sqr_comba24(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR28
FP_PRIVATE void fp_sqr_comba28(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR32
FP_PRIVATE void fp_sqr_comba32(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR48
FP_PRIVATE void fp_sqr_comba48(const fp_int *A, fp_int *B);
#endif
#ifdef TFM_SQR64
FP_PRIVATE void fp_sqr_comba64(const fp_int *A, fp_int *B);
#endif
FP_PRIVATE extern const char *fp_s_rmap;

#endif
