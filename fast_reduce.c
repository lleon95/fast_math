inline double reduce_pd(__m256d & v) {
	__m128d vlow = _mm256_castpd256_pd128(v);
	__m128d vhigh = _mm256_extractf128_pd(v, 1); // high 128
	vlow = _mm_add_pd(vlow, vhigh);     // reduce down to 128

	__m128d high64 = _mm_unpackhi_pd(vlow, vlow);
	return  _mm_cvtsd_f64(_mm_add_sd(vlow, high64));  // reduce to scalar
}