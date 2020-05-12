__m256d atan_ps(__m256d x)
{
	static const __m256d kSignMask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x8000000000000000));
	static const __m256d kInvSignMask = _mm256_castsi256_pd(_mm256_set1_epi64x(~0x8000000000000000));
	static const __m256d kPI2 = _mm256_set1_pd(1.5707963267948966192);
	static const __m256d kPI4 = _mm256_set1_pd(0.7853981633974483096);
	static const __m256d kOne = _mm256_set1_pd(1.);
	
	__m256d sign_bit, y;

	sign_bit = x;
	/* take the absolute value */
	x = _mm256_and_pd(x, kInvSignMask);
	/* extract the sign bit (upper one) */
	sign_bit = _mm256_and_pd(sign_bit, kSignMask);

	/* range reduction, init x and y depending on range */
	/* x > 2.414213562373095 */
	__m256d cmp0 = _mm256_cmp_pd(x, _mm256_set1_pd(2.414213562373095), 30);
	/* x > 0.4142135623730950 */
	__m256d cmp1 = _mm256_cmp_pd(x, _mm256_set1_pd(0.4142135623730950), 30);

	/* x > 0.4142135623730950 && !( x > 2.414213562373095 ) */
	__m256d cmp2 = _mm256_andnot_pd(cmp0, cmp1);

	/* -( 1.0/x ) */
	__m256d y0 = _mm256_and_pd(cmp0, kPI2);
	__m256d x0 = _mm256_div_pd(kOne, x);
	x0 = _mm256_xor_pd(x0, kSignMask);

	__m256d y1 = _mm256_and_pd(cmp2, kPI4);
	/* (x-1.0)/(x+1.0) */
	__m256d x1_o = _mm256_sub_pd(x, kOne);
	__m256d x1_u = _mm256_add_pd(x, kOne);
	__m256d x1 = _mm256_div_pd(x1_o, x1_u);

	__m256d x2 = _mm256_and_pd(cmp2, x1);
	x0 = _mm256_and_pd(cmp0, x0);
	x2 = _mm256_or_pd(x2, x0);
	cmp1 = _mm256_or_pd(cmp0, cmp2);
	x2 = _mm256_and_pd(cmp1, x2);
	x = _mm256_andnot_pd(cmp1, x);
	x = _mm256_or_pd(x2, x);

	y = _mm256_or_pd(y0, y1);

	__m256d zz = _mm256_mul_pd(x, x);
	__m256d acc = _mm256_set1_pd(8.05374449538e-2);
	acc = _mm256_mul_pd(acc, zz);
	acc = _mm256_sub_pd(acc, _mm256_set1_pd(1.38776856032E-1));
	acc = _mm256_mul_pd(acc, zz);
	acc = _mm256_add_pd(acc, _mm256_set1_pd(1.99777106478E-1));
	acc = _mm256_mul_pd(acc, zz);
	acc = _mm256_sub_pd(acc, _mm256_set1_pd(3.33329491539E-1));
	acc = _mm256_mul_pd(acc, zz);
	acc = _mm256_mul_pd(acc, x);
	acc = _mm256_add_pd(acc, x);
	y = _mm256_add_pd(y, acc);

	/* update the sign */
	y = _mm256_xor_pd(y, sign_bit);

	return y;
}
