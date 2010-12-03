/*
 * FixedMath.h
 *
 *  Created on: 01/12/2010
 *      Author: coma
 */

#ifndef FIXEDMATH_H_
#define FIXEDMATH_H_

const int64_t powersOfTwo[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

//#define ToFixed(x, f)									(((x) + (1.0 / powersOfTwo[(f - 1)])) * powersOfTwo[(f)]) // Attempt to fix rounding error in calculation 123 * 0.1992 = 24.5016
#define ToFixed(x, f)										((int64_t)((x) * powersOfTwo[(f)]))
#define ToInt(x, f)											((((int32_t)x) + powersOfTwo[(f - 1)]) / powersOfTwo[(f)])
#define FixedAdd(a, b, f)								(((int64_t)a) + ((int64_t)b))
#define FixedSub(a, b, f)								(((int64_t)a) - ((int64_t)b))
#define FixedMul(a, b, f)								((((int64_t)a) * ((int64_t)b)) / powersOfTwo[(f)])
#define FixedDiv(a, b, f)								(((((int64_t)a) * powersOfTwo[(f)]) / ((int64_t)b)))

typedef int32_t fixed7_8;
typedef int64_t fixed15_16;

#define ToFixed7_8(x)										ToFixed(x, 8)
#define ToInt7_8(x)											ToInt(x, 8)
#define Fixed7_8Add(a, b)								FixedAdd(a, b, 8)
#define Fixed7_8Sub(a, b)								FixedSub(a, b, 8)
#define Fixed7_8Mul(a, b)								FixedMul(a, b, 8)
#define Fixed7_8Div(a, b)								FixedDiv(a, b, 8)

#define ToFixed15_16(x)										ToFixed(x, 16)
#define ToInt15_16(x)											ToInt(x, 16)
#define Fixed15_16Add(a, b)								FixedAdd(a, b, 16)
#define Fixed15_16Sub(a, b)								FixedSub(a, b, 16)
#define Fixed15_16Mul(a, b)								FixedMul(a, b, 16)
#define Fixed15_16Div(a, b)								FixedDiv(a, b, 16)

#endif /* FIXEDMATH_H_ */
