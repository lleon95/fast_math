inline __m256d abs_pd(__m256d x) {
	static const __m256d sign_mask = _mm256_set1_pd(-0.); // -0. = 1 << 63
	return _mm256_andnot_pd(sign_mask, x); // !sign_mask & x
}