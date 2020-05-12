__m256d atan2_pd(__m256d y, __m256d x)
{
	static const __m256d kZero = _mm256_set1_pd(0.);
	static const __m256d kSignMask = _mm256_castsi256_pd(_mm256_set1_epi64x(0x8000000000000000));
	static const __m256d kPI2 = _mm256_set1_pd(1.5707963267948966192);
	static const __m256d kPI = _mm256_set1_pd(3.141592653589793238);

	__m256d x_eq_0 = _mm256_cmp_pd(x, kZero, 0);
	__m256d x_gt_0 = _mm256_cmp_pd(x, kZero, 30);
	__m256d x_le_0 = _mm256_cmp_pd(x, kZero, 18);
	__m256d y_eq_0 = _mm256_cmp_pd(y, kZero, 0);
	__m256d x_lt_0 = _mm256_cmp_pd(x, kZero, 17);
	__m256d y_lt_0 = _mm256_cmp_pd(y, kZero, 17);

	__m256d zero_mask = _mm256_and_pd(x_eq_0, y_eq_0);
	__m256d zero_mask_other_case = _mm256_and_pd(y_eq_0, x_gt_0);
	zero_mask = _mm256_or_pd(zero_mask, zero_mask_other_case);

	__m256d pio2_mask = _mm256_andnot_pd(y_eq_0, x_eq_0);
	__m256d pio2_mask_sign = _mm256_and_pd(y_lt_0, kSignMask);

	__m256d pio2_result = kPI2;
	pio2_result = _mm256_xor_pd(pio2_result, pio2_mask_sign);
	pio2_result = _mm256_and_pd(pio2_mask, pio2_result);

	__m256d pi_mask = _mm256_and_pd(y_eq_0, x_le_0);
	__m256d pi = kPI;
	__m256d pi_result = _mm256_and_pd(pi_mask, pi);

	__m256d swap_sign_mask_offset = _mm256_and_pd(x_lt_0, y_lt_0);
	swap_sign_mask_offset = _mm256_and_pd(swap_sign_mask_offset, kSignMask);

	__m256d offset0 = _mm256_setzero_pd();
	__m256d offset1 = kPI;
	offset1 = _mm256_xor_pd(offset1, swap_sign_mask_offset);

	__m256d offset = _mm256_andnot_pd(x_lt_0, offset0);
	offset = _mm256_and_pd(x_lt_0, offset1);

	__m256d arg = _mm256_div_pd(y, x);
	__m256d atan_result = atan_ps(arg);
	atan_result = _mm256_add_pd(atan_result, offset);

	/* select between zero_result, pio2_result and atan_result */

	__m256d result = _mm256_andnot_pd(zero_mask, pio2_result);
	atan_result = _mm256_andnot_pd(pio2_mask, atan_result);
	atan_result = _mm256_andnot_pd(pio2_mask, atan_result);
	result = _mm256_or_pd(result, atan_result);
	result = _mm256_or_pd(result, pi_result);

	return result;
}
