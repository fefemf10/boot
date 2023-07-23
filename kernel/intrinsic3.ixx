export module intrinsic3;
export import intrinsicTypes;
export extern "C"
{
	__m256d _mm256_add_pd(__m256d, __m256d);
	__m256 _mm256_add_ps(__m256, __m256);
	__m256d _mm256_addsub_pd(__m256d, __m256d);
	__m256 _mm256_addsub_ps(__m256, __m256);
	__m256d _mm256_and_pd(__m256d, __m256d);
	__m256 _mm256_and_ps(__m256, __m256);
	__m256d _mm256_andnot_pd(__m256d, __m256d);
	__m256 _mm256_andnot_ps(__m256, __m256);
	__m256d _mm256_blend_pd(__m256d, __m256d, const int);
	__m256 _mm256_blend_ps(__m256, __m256, const int);
	__m256d _mm256_blendv_pd(__m256d, __m256d, __m256d);
	__m256 _mm256_blendv_ps(__m256, __m256, __m256);
	__m256d _mm256_div_pd(__m256d, __m256d);
	__m256 _mm256_div_ps(__m256, __m256);
	__m256 _mm256_dp_ps(__m256, __m256, const int);
	__m256d _mm256_hadd_pd(__m256d, __m256d);
	__m256 _mm256_hadd_ps(__m256, __m256);
	__m256d _mm256_hsub_pd(__m256d, __m256d);
	__m256 _mm256_hsub_ps(__m256, __m256);
	__m256d _mm256_max_pd(__m256d, __m256d);
	__m256 _mm256_max_ps(__m256, __m256);
	__m256d _mm256_min_pd(__m256d, __m256d);
	__m256 _mm256_min_ps(__m256, __m256);
	__m256d _mm256_mul_pd(__m256d, __m256d);
	__m256 _mm256_mul_ps(__m256, __m256);
	__m256d _mm256_or_pd(__m256d, __m256d);
	__m256 _mm256_or_ps(__m256, __m256);
	__m256d _mm256_shuffle_pd(__m256d, __m256d, const int);
	__m256 _mm256_shuffle_ps(__m256, __m256, const int);
	__m256d _mm256_sub_pd(__m256d, __m256d);
	__m256 _mm256_sub_ps(__m256, __m256);
	__m256d _mm256_xor_pd(__m256d, __m256d);
	__m256 _mm256_xor_ps(__m256, __m256);
	__m128d _mm_cmp_pd(__m128d, __m128d, const int);
	__m256d _mm256_cmp_pd(__m256d, __m256d, const int);
	__m128 _mm_cmp_ps(__m128, __m128, const int);
	__m256 _mm256_cmp_ps(__m256, __m256, const int);
	__m128d _mm_cmp_sd(__m128d, __m128d, const int);
	int _mm_comi_sd(__m128d, __m128d, const int);
	__m128 _mm_cmp_ss(__m128, __m128, const int);
	int _mm_comi_ss(__m128, __m128, const int);
	__m256d _mm256_cvtepi32_pd(__m128i);
	__m256  _mm256_cvtepi32_ps(__m256i);
	__m128  _mm256_cvtpd_ps(__m256d);
	__m256i _mm256_cvtps_epi32(__m256);
	__m256d _mm256_cvtps_pd(__m128);
	__m128i _mm256_cvttpd_epi32(__m256d);
	__m128i _mm256_cvtpd_epi32(__m256d);
	__m256i _mm256_cvttps_epi32(__m256);
	__m128  _mm256_extractf128_ps(__m256, const int);
	__m128d _mm256_extractf128_pd(__m256d, const int);
	__m128i _mm256_extractf128_si256(__m256i, const int);
	void _mm256_zeroall(void);
	void _mm256_zeroupper(void);
	__m256  _mm256_permutevar_ps(__m256, __m256i);
	__m128  _mm_permutevar_ps(__m128, __m128i);
	__m256  _mm256_permute_ps(__m256, int);
	__m128  _mm_permute_ps(__m128, int);
	__m256d _mm256_permutevar_pd(__m256d, __m256i);
	__m128d _mm_permutevar_pd(__m128d, __m128i);
	__m256d _mm256_permute_pd(__m256d, int);
	__m128d _mm_permute_pd(__m128d, int);
	__m256  _mm256_permute2f128_ps(__m256, __m256, int);
	__m256d _mm256_permute2f128_pd(__m256d, __m256d, int);
	__m256i _mm256_permute2f128_si256(__m256i, __m256i, int);
	__m256  _mm256_broadcast_ss(float const*);
	__m128  _mm_broadcast_ss(float const*);
	__m256d _mm256_broadcast_sd(double const*);
	__m256  _mm256_broadcast_ps(__m128 const*);
	__m256d _mm256_broadcast_pd(__m128d const*);
	__m256  _mm256_insertf128_ps(__m256, __m128, int);
	__m256d _mm256_insertf128_pd(__m256d, __m128d, int);
	__m256i _mm256_insertf128_si256(__m256i, __m128i, int);
	__m256d _mm256_load_pd(double const*);
	void    _mm256_store_pd(double*, __m256d);
	__m256  _mm256_load_ps(float const*);
	void    _mm256_store_ps(float*, __m256);
	__m256d _mm256_loadu_pd(double const*);
	void    _mm256_storeu_pd(double*, __m256d);
	__m256  _mm256_loadu_ps(float const*);
	void    _mm256_storeu_ps(float*, __m256);
	__m256i _mm256_load_si256(__m256i const*);
	void    _mm256_store_si256(__m256i*, __m256i);
	__m256i _mm256_loadu_si256(__m256i const*);
	void    _mm256_storeu_si256(__m256i*, __m256i);
	__m256d _mm256_maskload_pd(double const*, __m256i);
	void    _mm256_maskstore_pd(double*, __m256i, __m256d);
	__m128d _mm_maskload_pd(double const*, __m128i);
	void    _mm_maskstore_pd(double*, __m128i, __m128d);
	__m256  _mm256_maskload_ps(float const*, __m256i);
	void    _mm256_maskstore_ps(float*, __m256i, __m256);
	__m128  _mm_maskload_ps(float const*, __m128i);
	void    _mm_maskstore_ps(float*, __m128i, __m128);
	__m256  _mm256_movehdup_ps(__m256);
	__m256  _mm256_moveldup_ps(__m256);
	__m256d _mm256_movedup_pd(__m256d);
	__m256i _mm256_lddqu_si256(__m256i const*);
	void    _mm256_stream_si256(__m256i*, __m256i);
	void    _mm256_stream_pd(double*, __m256d);
	void    _mm256_stream_ps(float*, __m256);
	__m256  _mm256_rcp_ps(__m256);
	__m256  _mm256_rsqrt_ps(__m256);
	__m256d _mm256_sqrt_pd(__m256d);
	__m256  _mm256_sqrt_ps(__m256);
	__m256d _mm256_round_pd(__m256d, int);
	__m256  _mm256_round_ps(__m256, int);
	__m256d _mm256_unpackhi_pd(__m256d, __m256d);
	__m256  _mm256_unpackhi_ps(__m256, __m256);
	__m256d _mm256_unpacklo_pd(__m256d, __m256d);
	__m256  _mm256_unpacklo_ps(__m256, __m256);
	int _mm256_testz_si256(__m256i, __m256i);
	int _mm256_testc_si256(__m256i, __m256i);
	int _mm256_testnzc_si256(__m256i, __m256i);
	int _mm256_testz_pd(__m256d, __m256d);
	int _mm256_testc_pd(__m256d, __m256d);
	int _mm256_testnzc_pd(__m256d, __m256d);
	int _mm_testz_pd(__m128d, __m128d);
	int _mm_testc_pd(__m128d, __m128d);
	int _mm_testnzc_pd(__m128d, __m128d);
	int _mm256_testz_ps(__m256, __m256);
	int _mm256_testc_ps(__m256, __m256);
	int _mm256_testnzc_ps(__m256, __m256);
	int _mm_testz_ps(__m128, __m128);
	int _mm_testc_ps(__m128, __m128);
	int _mm_testnzc_ps(__m128, __m128);
	int _mm256_movemask_pd(__m256d);
	int _mm256_movemask_ps(__m256);
	__m256d _mm256_setzero_pd(void);
	__m256  _mm256_setzero_ps(void);
	__m256i _mm256_setzero_si256(void);
	__m256d _mm256_set_pd(double, double, double, double);
	__m256  _mm256_set_ps(float, float, float, float, float, float, float, float);
	__m256i _mm256_set_epi8(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char);
	__m256i _mm256_set_epi16(short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short);
	__m256i _mm256_set_epi32(int, int, int, int, int, int, int, int);
	__m256i _mm256_set_epi64x(__int64, __int64, __int64, __int64);
	__m256d _mm256_setr_pd(double, double, double, double);
	__m256  _mm256_setr_ps(float, float, float, float, float, float, float, float);
	__m256i _mm256_setr_epi8(char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char);
	__m256i _mm256_setr_epi16(short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short);
	__m256i _mm256_setr_epi32(int, int, int, int, int, int, int, int);
	__m256i _mm256_setr_epi64x(__int64, __int64, __int64, __int64);
	__m256d _mm256_set1_pd(double);
	__m256  _mm256_set1_ps(float);
	__m256i _mm256_set1_epi8(char);
	__m256i _mm256_set1_epi16(short);
	__m256i _mm256_set1_epi32(int);
	__m256i _mm256_set1_epi64x(long long);
	__m256  _mm256_castpd_ps(__m256d);
	__m256d _mm256_castps_pd(__m256);
	__m256i _mm256_castps_si256(__m256);
	__m256i _mm256_castpd_si256(__m256d);
	__m256  _mm256_castsi256_ps(__m256i);
	__m256d _mm256_castsi256_pd(__m256i);
	__m128  _mm256_castps256_ps128(__m256);
	__m128d _mm256_castpd256_pd128(__m256d);
	__m128i _mm256_castsi256_si128(__m256i);
	__m256  _mm256_castps128_ps256(__m128);
	__m256d _mm256_castpd128_pd256(__m128d);
	__m256i _mm256_castsi128_si256(__m128i);
	__m128  _mm_cvtph_ps(__m128i);
	__m256  _mm256_cvtph_ps(__m128i);
	__m128i _mm_cvtps_ph(__m128 /* m1 */, const int /* imm */);
	__m128i _mm256_cvtps_ph(__m256, int);
	unsigned __int64 _xgetbv(unsigned int);
	void _xsetbv(unsigned int, unsigned __int64);
	void _xsave64(void*, unsigned __int64);
	void _xsaveopt64(void*, unsigned __int64);
	void _xsavec64(void*, unsigned __int64);
	void _xrstor64(void const*, unsigned __int64);
	void _xsaves64(void*, unsigned __int64);
	void _xrstors64(void const*, unsigned __int64);
	void _fxsave64(void*);
	void _fxrstor64(void const*);
	int _rdrand16_step(unsigned short*);
	int _rdrand32_step(unsigned int*);
	int _rdrand64_step(unsigned __int64*);
	unsigned int_readfsbase_u32(void);
	unsigned int_readgsbase_u32(void);
	unsigned __int64 _readfsbase_u64(void);
	unsigned __int64 _readgsbase_u64(void);
	void _writefsbase_u32(unsigned int);
	void _writegsbase_u32(unsigned int);
	void _writefsbase_u64(unsigned __int64);
	void _writegsbase_u64(unsigned __int64);

	__m128  _mm_fmadd_ps(__m128, __m128, __m128);
	__m128d _mm_fmadd_pd(__m128d, __m128d, __m128d);
	__m128  _mm_fmadd_ss(__m128, __m128, __m128);
	__m128d _mm_fmadd_sd(__m128d, __m128d, __m128d);
	__m128  _mm_fmsub_ps(__m128, __m128, __m128);
	__m128d _mm_fmsub_pd(__m128d, __m128d, __m128d);
	__m128  _mm_fmsub_ss(__m128, __m128, __m128);
	__m128d _mm_fmsub_sd(__m128d, __m128d, __m128d);
	__m128  _mm_fnmadd_ps(__m128, __m128, __m128);
	__m128d _mm_fnmadd_pd(__m128d, __m128d, __m128d);
	__m128  _mm_fnmadd_ss(__m128, __m128, __m128);
	__m128d _mm_fnmadd_sd(__m128d, __m128d, __m128d);
	__m128  _mm_fnmsub_ps(__m128, __m128, __m128);
	__m128d _mm_fnmsub_pd(__m128d, __m128d, __m128d);
	__m128  _mm_fnmsub_ss(__m128, __m128, __m128);
	__m128d _mm_fnmsub_sd(__m128d, __m128d, __m128d);
	__m256  _mm256_fmadd_ps(__m256, __m256, __m256);
	__m256d _mm256_fmadd_pd(__m256d, __m256d, __m256d);
	__m256  _mm256_fmsub_ps(__m256, __m256, __m256);
	__m256d _mm256_fmsub_pd(__m256d, __m256d, __m256d);
	__m256  _mm256_fnmadd_ps(__m256, __m256, __m256);
	__m256d _mm256_fnmadd_pd(__m256d, __m256d, __m256d);
	__m256  _mm256_fnmsub_ps(__m256, __m256, __m256);
	__m256d _mm256_fnmsub_pd(__m256d, __m256d, __m256d);
	__m128  _mm_fmaddsub_ps(__m128, __m128, __m128);
	__m128d _mm_fmaddsub_pd(__m128d, __m128d, __m128d);
	__m128  _mm_fmsubadd_ps(__m128, __m128, __m128);
	__m128d _mm_fmsubadd_pd(__m128d, __m128d, __m128d);
	__m256  _mm256_fmaddsub_ps(__m256, __m256, __m256);
	__m256d _mm256_fmaddsub_pd(__m256d, __m256d, __m256d);
	__m256  _mm256_fmsubadd_ps(__m256, __m256, __m256);
	__m256d _mm256_fmsubadd_pd(__m256d, __m256d, __m256d);
	__m256i _mm256_cmpeq_epi8(__m256i, __m256i);
	__m256i _mm256_cmpeq_epi16(__m256i, __m256i);
	__m256i _mm256_cmpeq_epi32(__m256i, __m256i);
	__m256i _mm256_cmpeq_epi64(__m256i, __m256i);
	__m256i _mm256_cmpgt_epi8(__m256i, __m256i);
	__m256i _mm256_cmpgt_epi16(__m256i, __m256i);
	__m256i _mm256_cmpgt_epi32(__m256i, __m256i);
	__m256i _mm256_cmpgt_epi64(__m256i, __m256i);
	__m256i _mm256_max_epi8(__m256i, __m256i);
	__m256i _mm256_max_epi16(__m256i, __m256i);
	__m256i _mm256_max_epi32(__m256i, __m256i);
	__m256i _mm256_max_epu8(__m256i, __m256i);
	__m256i _mm256_max_epu16(__m256i, __m256i);
	__m256i _mm256_max_epu32(__m256i, __m256i);
	__m256i _mm256_min_epi8(__m256i, __m256i);
	__m256i _mm256_min_epi16(__m256i, __m256i);
	__m256i _mm256_min_epi32(__m256i, __m256i);
	__m256i _mm256_min_epu8(__m256i, __m256i);
	__m256i _mm256_min_epu16(__m256i, __m256i);
	__m256i _mm256_min_epu32(__m256i, __m256i);
	__m256i _mm256_and_si256(__m256i, __m256i);
	__m256i _mm256_andnot_si256(__m256i, __m256i);
	__m256i _mm256_or_si256(__m256i, __m256i);
	__m256i _mm256_xor_si256(__m256i, __m256i);
	__m256i _mm256_abs_epi8(__m256i);
	__m256i _mm256_abs_epi16(__m256i);
	__m256i _mm256_abs_epi32(__m256i);
	__m256i _mm256_add_epi8(__m256i, __m256i);
	__m256i _mm256_add_epi16(__m256i, __m256i);
	__m256i _mm256_add_epi32(__m256i, __m256i);
	__m256i _mm256_add_epi64(__m256i, __m256i);
	__m256i _mm256_adds_epi8(__m256i, __m256i);
	__m256i _mm256_adds_epi16(__m256i, __m256i);
	__m256i _mm256_adds_epu8(__m256i, __m256i);
	__m256i _mm256_adds_epu16(__m256i, __m256i);
	__m256i _mm256_sub_epi8(__m256i, __m256i);
	__m256i _mm256_sub_epi16(__m256i, __m256i);
	__m256i _mm256_sub_epi32(__m256i, __m256i);
	__m256i _mm256_sub_epi64(__m256i, __m256i);
	__m256i _mm256_subs_epi8(__m256i, __m256i);
	__m256i _mm256_subs_epi16(__m256i, __m256i);
	__m256i _mm256_subs_epu8(__m256i, __m256i);
	__m256i _mm256_subs_epu16(__m256i, __m256i);
	__m256i _mm256_avg_epu8(__m256i, __m256i);
	__m256i _mm256_avg_epu16(__m256i, __m256i);
	__m256i _mm256_hadd_epi16(__m256i, __m256i);
	__m256i _mm256_hadd_epi32(__m256i, __m256i);
	__m256i _mm256_hadds_epi16(__m256i, __m256i);
	__m256i _mm256_hsub_epi16(__m256i, __m256i);
	__m256i _mm256_hsub_epi32(__m256i, __m256i);
	__m256i _mm256_hsubs_epi16(__m256i, __m256i);
	__m256i _mm256_madd_epi16(__m256i, __m256i);
	__m256i _mm256_maddubs_epi16(__m256i, __m256i);
	__m256i _mm256_mulhi_epi16(__m256i, __m256i);
	__m256i _mm256_mulhi_epu16(__m256i, __m256i);
	__m256i _mm256_mullo_epi16(__m256i, __m256i);
	__m256i _mm256_mullo_epi32(__m256i, __m256i);
	__m256i _mm256_mul_epu32(__m256i, __m256i);
	__m256i _mm256_mul_epi32(__m256i, __m256i);
	__m256i _mm256_sign_epi8(__m256i, __m256i);
	__m256i _mm256_sign_epi16(__m256i, __m256i);
	__m256i _mm256_sign_epi32(__m256i, __m256i);
	__m256i _mm256_mulhrs_epi16(__m256i, __m256i);
	__m256i _mm256_sad_epu8(__m256i, __m256i);
	__m256i _mm256_mpsadbw_epu8(__m256i, __m256i, const int);
	__m256i _mm256_slli_si256(__m256i, const int);
	__m256i _mm256_srli_si256(__m256i, const int);
	__m256i _mm256_sll_epi16(__m256i, __m128i);
	__m256i _mm256_sll_epi32(__m256i, __m128i);
	__m256i _mm256_sll_epi64(__m256i, __m128i);
	__m256i _mm256_slli_epi16(__m256i, int);
	__m256i _mm256_slli_epi32(__m256i, int);
	__m256i _mm256_slli_epi64(__m256i, int);
	__m256i _mm256_sllv_epi32(__m256i, __m256i);
	__m256i _mm256_sllv_epi64(__m256i, __m256i);
	__m128i _mm_sllv_epi32(__m128i, __m128i);
	__m128i _mm_sllv_epi64(__m128i, __m128i);
	__m256i _mm256_sra_epi16(__m256i, __m128i);
	__m256i _mm256_sra_epi32(__m256i, __m128i);
	__m256i _mm256_srai_epi16(__m256i, int);
	__m256i _mm256_srai_epi32(__m256i, int);
	__m256i _mm256_srav_epi32(__m256i, __m256i);
	__m128i _mm_srav_epi32(__m128i, __m128i);
	__m256i _mm256_srl_epi16(__m256i, __m128i);
	__m256i _mm256_srl_epi32(__m256i, __m128i);
	__m256i _mm256_srl_epi64(__m256i, __m128i);
	__m256i _mm256_srli_epi16(__m256i, int);
	__m256i _mm256_srli_epi32(__m256i, int);
	__m256i _mm256_srli_epi64(__m256i, int);
	__m256i _mm256_srlv_epi32(__m256i, __m256i);
	__m256i _mm256_srlv_epi64(__m256i, __m256i);
	__m128i _mm_srlv_epi32(__m128i, __m128i);
	__m128i _mm_srlv_epi64(__m128i, __m128i);
	__m128i _mm_blend_epi32(__m128i, __m128i, const int);
	__m256i _mm256_blend_epi32(__m256i, __m256i, const int);
	__m256i _mm256_alignr_epi8(__m256i, __m256i, const int);
	__m256i _mm256_blendv_epi8(__m256i, __m256i, __m256i);
	__m256i _mm256_blend_epi16(__m256i, __m256i, const int);
	__m256i _mm256_packs_epi16(__m256i, __m256i);
	__m256i _mm256_packs_epi32(__m256i, __m256i);
	__m256i _mm256_packus_epi16(__m256i, __m256i);
	__m256i _mm256_packus_epi32(__m256i, __m256i);
	__m256i _mm256_unpackhi_epi8(__m256i, __m256i);
	__m256i _mm256_unpackhi_epi16(__m256i, __m256i);
	__m256i _mm256_unpackhi_epi32(__m256i, __m256i);
	__m256i _mm256_unpackhi_epi64(__m256i, __m256i);
	__m256i _mm256_unpacklo_epi8(__m256i, __m256i);
	__m256i _mm256_unpacklo_epi16(__m256i, __m256i);
	__m256i _mm256_unpacklo_epi32(__m256i, __m256i);
	__m256i _mm256_unpacklo_epi64(__m256i, __m256i);
	__m256i _mm256_shuffle_epi8(__m256i, __m256i);
	__m256i _mm256_shuffle_epi32(__m256i, const int);
	__m256i _mm256_shufflehi_epi16(__m256i, const int);
	__m256i _mm256_shufflelo_epi16(__m256i, const int);
	__m128i _mm256_extracti128_si256(__m256i, const int);
	__m256i _mm256_inserti128_si256(__m256i, __m128i, const int);
	__m128  _mm_broadcastss_ps(__m128);
	__m128d _mm_broadcastsd_pd(__m128d);
	__m128i _mm_broadcastb_epi8(__m128i);
	__m128i _mm_broadcastw_epi16(__m128i);
	__m128i _mm_broadcastd_epi32(__m128i);
	__m128i _mm_broadcastq_epi64(__m128i);
	__m256  _mm256_broadcastss_ps(__m128);
	__m256d _mm256_broadcastsd_pd(__m128d);
	__m256i _mm256_broadcastb_epi8(__m128i);
	__m256i _mm256_broadcastw_epi16(__m128i);
	__m256i _mm256_broadcastd_epi32(__m128i);
	__m256i _mm256_broadcastq_epi64(__m128i);
	__m256i _mm256_broadcastsi128_si256(__m128i);
	__m256i _mm256_cvtepi8_epi16(__m128i);
	__m256i _mm256_cvtepi8_epi32(__m128i);
	__m256i _mm256_cvtepi8_epi64(__m128i);
	__m256i _mm256_cvtepi16_epi32(__m128i);
	__m256i _mm256_cvtepi16_epi64(__m128i);
	__m256i _mm256_cvtepi32_epi64(__m128i);
	__m256i _mm256_cvtepu8_epi16(__m128i);
	__m256i _mm256_cvtepu8_epi32(__m128i);
	__m256i _mm256_cvtepu8_epi64(__m128i);
	__m256i _mm256_cvtepu16_epi32(__m128i);
	__m256i _mm256_cvtepu16_epi64(__m128i);
	__m256i _mm256_cvtepu32_epi64(__m128i);
	int _mm256_movemask_epi8(__m256i);
	__m128i _mm_maskload_epi32(int const*, __m128i);
	__m128i _mm_maskload_epi64(__int64 const*, __m128i);
	void _mm_maskstore_epi32(int*, __m128i,	__m128i);
	void _mm_maskstore_epi64(__int64*, __m128i, __m128i);
	__m256i _mm256_maskload_epi32(int const*, __m256i  );
	__m256i _mm256_maskload_epi64(__int64 const*, __m256i );
	void _mm256_maskstore_epi32(int*  , __m256i, __m256i);
	void _mm256_maskstore_epi64(__int64*, __m256i, __m256i);
	__m256i _mm256_permutevar8x32_epi32(__m256i, __m256i);
	__m256  _mm256_permutevar8x32_ps(__m256, __m256i);
	__m256i _mm256_permute4x64_epi64(__m256i, const int);
	__m256d _mm256_permute4x64_pd(__m256d, const int);
	__m256i _mm256_permute2x128_si256(__m256i, __m256i, const int);
	__m256i  _mm256_stream_load_si256(__m256i const*);
	__m256d _mm256_mask_i32gather_pd(__m256d, double const*, __m128i, __m256d, const int);
	__m256  _mm256_mask_i32gather_ps(__m256 , float const* , __m256i, __m256 , const int);
	__m256d _mm256_mask_i64gather_pd(__m256d, double const*, __m256i, __m256d, const int);
	__m128  _mm256_mask_i64gather_ps(__m128 , float const* , __m256i, __m128 , const int);
	__m128d _mm_mask_i32gather_pd(__m128d, double const*, __m128i, __m128d, const int);
	__m128  _mm_mask_i32gather_ps(__m128 , float const* , __m128i, __m128 , const int);
	__m128d _mm_mask_i64gather_pd(__m128d, double const*, __m128i, __m128d, const int);
	__m128  _mm_mask_i64gather_ps(__m128 , float const* , __m128i, __m128 , const int);
	__m256i _mm256_mask_i32gather_epi32(__m256i, int const*, __m256i, __m256i  , const int);
	__m256i _mm256_mask_i32gather_epi64(__m256i, __int64 const*, __m128i, __m256i  , const int);
	__m128i _mm256_mask_i64gather_epi32(__m128i, int const*, __m256i, __m128i  , const int);
	__m256i _mm256_mask_i64gather_epi64(__m256i, __int64 const*, __m256i, __m256i  , const int);
	__m128i _mm_mask_i32gather_epi32(__m128i , int const*    , __m128i , __m128i , const int );
	__m128i _mm_mask_i32gather_epi64(__m128i , __int64 const*, __m128i , __m128i , const int );
	__m128i _mm_mask_i64gather_epi32(__m128i , int const*, __m128i , __m128i , const int );
	__m128i _mm_mask_i64gather_epi64(__m128i , __int64 const*, __m128i , __m128i , const int );
	__m256d _mm256_i32gather_pd(double const*, __m128i, const int);
	__m256  _mm256_i32gather_ps(float  const*, __m256i, const int);
	__m256d _mm256_i64gather_pd(double const*, __m256i, const int);
	__m128  _mm256_i64gather_ps(float  const*, __m256i, const int);

	__m128d _mm_i32gather_pd(double const*, __m128i, const int);
	__m128  _mm_i32gather_ps(float  const*, __m128i, const int);
	__m128d _mm_i64gather_pd(double const*, __m128i, const int);
	__m128  _mm_i64gather_ps(float  const*, __m128i, const int);

	__m256i _mm256_i32gather_epi32(int const*    , __m256i , const int );
	__m256i _mm256_i32gather_epi64(__int64 const*, __m128i , const int );
	__m128i _mm256_i64gather_epi32(int const*    , __m256i , const int );
	__m256i _mm256_i64gather_epi64(__int64 const*, __m256i , const int );

	__m128i _mm_i32gather_epi32(int const*    , __m128i , const int);
	__m128i _mm_i32gather_epi64(__int64 const*, __m128i , const int);
	__m128i _mm_i64gather_epi32(int const*, __m128i , const int);
	__m128i _mm_i64gather_epi64(__int64 const*, __m128i , const int);
	unsigned int     _bextr_u32(unsigned int, unsigned int, unsigned int);
	unsigned int     _bextr2_u32(unsigned int, unsigned int);
	unsigned int     _blsi_u32(unsigned int);
	unsigned int     _blsmsk_u32(unsigned int);
	unsigned int     _blsr_u32(unsigned int);
	unsigned int     _bzhi_u32(unsigned int, unsigned int);
	unsigned int     _mulx_u32(unsigned int, unsigned int, unsigned int*);
	unsigned int     _pdep_u32(unsigned int, unsigned int);
	unsigned int     _pext_u32(unsigned int, unsigned int);
	unsigned int     _rorx_u32(unsigned int, const unsigned int);
	int              _sarx_i32(int, unsigned int);
	unsigned int     _shlx_u32(unsigned int, unsigned int);
	unsigned int     _shrx_u32(unsigned int, unsigned int);
	unsigned __int64 _bextr_u64(unsigned __int64, unsigned int, unsigned int);
	unsigned __int64 _bextr2_u64(unsigned __int64, unsigned __int64);
	unsigned __int64 _blsi_u64(unsigned __int64);
	unsigned __int64 _blsmsk_u64(unsigned __int64);
	unsigned __int64 _blsr_u64(unsigned __int64);
	unsigned __int64 _bzhi_u64(unsigned __int64, unsigned int);
	unsigned __int64 _mulx_u64(unsigned __int64, unsigned __int64, unsigned __int64*);
	unsigned __int64 _pdep_u64(unsigned __int64, unsigned __int64);
	unsigned __int64 _pext_u64(unsigned __int64, unsigned __int64);
	unsigned __int64 _rorx_u64(unsigned __int64, const unsigned int);
	__int64          _sarx_i64(__int64, unsigned int);
	unsigned __int64 _shlx_u64(unsigned __int64, unsigned int);
	unsigned __int64 _shrx_u64(unsigned __int64, unsigned int);
	unsigned int     _lzcnt_u32(unsigned int);
	unsigned __int64 _lzcnt_u64(unsigned __int64);
	unsigned __int16 _tzcnt_u16(unsigned __int16);
	unsigned int     _tzcnt_u32(unsigned int);
	unsigned __int64 _tzcnt_u64(unsigned __int64);
	void _invpcid(unsigned int, void*);

	void _Store_HLERelease(long volatile*, long);
	void _StorePointer_HLERelease(void* volatile*, void*);
	long _InterlockedExchange_HLEAcquire(long volatile*, long);
	long _InterlockedExchange_HLERelease(long volatile*, long);
	void* _InterlockedExchangePointer_HLEAcquire(void* volatile*, void*);
	void* _InterlockedExchangePointer_HLERelease(void* volatile*, void*);
	long _InterlockedCompareExchange_HLEAcquire(long volatile*, long, long);
	long _InterlockedCompareExchange_HLERelease(long volatile*, long, long);
	__int64 _InterlockedCompareExchange64_HLEAcquire(__int64 volatile*, __int64, __int64);
	__int64 _InterlockedCompareExchange64_HLERelease(__int64 volatile*, __int64, __int64);
	void* _InterlockedCompareExchangePointer_HLEAcquire(void* volatile*, void*, void*);
	void* _InterlockedCompareExchangePointer_HLERelease(void* volatile*, void*, void*);
	long _InterlockedExchangeAdd_HLEAcquire(long volatile*, long);
	long _InterlockedExchangeAdd_HLERelease(long volatile*, long);
	long _InterlockedAnd_HLEAcquire(long volatile*, long);
	long _InterlockedAnd_HLERelease(long volatile*, long);
	long _InterlockedOr_HLEAcquire(long volatile*, long);
	long _InterlockedOr_HLERelease(long volatile*, long);
	long _InterlockedXor_HLEAcquire(long volatile*, long);
	long _InterlockedXor_HLERelease(long volatile*, long);
	unsigned char _interlockedbittestandset_HLEAcquire(long*, long);
	unsigned char _interlockedbittestandset_HLERelease(long*, long);
	unsigned char _interlockedbittestandreset_HLEAcquire(long*, long);
	unsigned char _interlockedbittestandreset_HLERelease(long*, long);
	void _Store64_HLERelease(__int64 volatile*, __int64);
	__int64 _InterlockedExchange64_HLEAcquire(__int64 volatile*, __int64);
	__int64 _InterlockedExchange64_HLERelease(__int64 volatile*, __int64);
	__int64 _InterlockedExchangeAdd64_HLEAcquire(__int64 volatile*, __int64);
	__int64 _InterlockedExchangeAdd64_HLERelease(__int64 volatile*, __int64);
	__int64 _InterlockedAnd64_HLEAcquire(__int64 volatile*, __int64);
	__int64 _InterlockedAnd64_HLERelease(__int64 volatile*, __int64);
	__int64 _InterlockedOr64_HLEAcquire(__int64 volatile*, __int64);
	__int64 _InterlockedOr64_HLERelease(__int64 volatile*, __int64);
	__int64 _InterlockedXor64_HLEAcquire(__int64 volatile*, __int64);
	__int64 _InterlockedXor64_HLERelease(__int64 volatile*, __int64);
	unsigned char _interlockedbittestandset64_HLEAcquire(__int64*, __int64);
	unsigned char _interlockedbittestandset64_HLERelease(__int64*, __int64);
	unsigned char _interlockedbittestandreset64_HLEAcquire(__int64*, __int64);
	unsigned char _interlockedbittestandreset64_HLERelease(__int64*, __int64);
	int _rdseed16_step(unsigned short*);
	int _rdseed32_step(unsigned int*);
	int _rdseed64_step(unsigned __int64*);
	unsigned char _addcarryx_u32(unsigned char, unsigned int, unsigned int, unsigned int*);
	unsigned char _addcarryx_u64(unsigned char, unsigned __int64, unsigned __int64, unsigned __int64*);
	unsigned short   _load_be_u16(void const*);
	unsigned int     _load_be_u32(void const*);
	unsigned __int64 _load_be_u64(void const*);
	void _store_be_u16(void*, unsigned short);
	void _store_be_u32(void*, unsigned int);
	void _store_be_u64(void*, unsigned __int64);
	__m128i _mm_sha1msg1_epu32(__m128i, __m128i);
	__m128i _mm_sha1msg2_epu32(__m128i, __m128i);
	__m128i _mm_sha1nexte_epu32(__m128i, __m128i);
	__m128i _mm_sha1rnds4_epu32(__m128i, __m128i, const int);
	__m128i _mm_sha256msg1_epu32(__m128i, __m128i);
	__m128i _mm_sha256msg2_epu32(__m128i, __m128i);
	__m128i _mm_sha256rnds2_epu32(__m128i, __m128i, __m128i);
	void* _bnd_set_ptr_bounds(const void*, size_t);
	void* _bnd_narrow_ptr_bounds(const void*, const void*, size_t);
	void* _bnd_copy_ptr_bounds(const void*, const void*);
	void* _bnd_init_ptr_bounds(const void*);
	void _bnd_store_ptr_bounds(const void**, const void*);
	void _bnd_chk_ptr_lbounds(const void*);
	void _bnd_chk_ptr_ubounds(const void*);
	void _bnd_chk_ptr_bounds(const void*, size_t);
	void* _bnd_load_ptr_bounds(const void**, const void*);
	const void* _bnd_get_ptr_lbound(const void*);
	const void* _bnd_get_ptr_ubound(const void*);
	__m256i _mm256_insert_epi8(__m256i, int, const int);
	__m256i _mm256_insert_epi16(__m256i, int, const int);
	__m256i _mm256_insert_epi32(__m256i, int, const int);
	__m256i _mm256_insert_epi64(__m256i, __int64, const int);
	int _mm256_extract_epi8(__m256i, const int);
	int _mm256_extract_epi16(__m256i, const int);
	int _mm256_extract_epi32(__m256i, const int);
	__int64 _mm256_extract_epi64(__m256i, const int);
	__m256d _mm256_zextpd128_pd256(__m128d);
	__m256  _mm256_zextps128_ps256(__m128);
	__m256i _mm256_zextsi128_si256(__m128i);
	unsigned int _rdpid_u32(void);
	void         _ptwrite32(unsigned int);
	void         _ptwrite64(unsigned __int64);
	__m128i _mm_dpbusd_avx_epi32(__m128i, __m128i, __m128i);
	__m256i _mm256_dpbusd_avx_epi32(__m256i, __m256i, __m256i);
	__m128i _mm_dpbusds_avx_epi32(__m128i, __m128i, __m128i);
	__m256i _mm256_dpbusds_avx_epi32(__m256i, __m256i, __m256i);
	__m128i _mm_dpwssd_avx_epi32(__m128i, __m128i, __m128i);
	__m256i _mm256_dpwssd_avx_epi32(__m256i, __m256i, __m256i);
	__m128i _mm_dpwssds_avx_epi32(__m128i, __m128i, __m128i);
	__m256i _mm256_dpwssds_avx_epi32(__m256i, __m256i, __m256i);
	unsigned int _pconfig_u32(const int, size_t __data[]);
	void _wbnoinvd(void);
	unsigned int _encls_u32(const int, size_t __data[]);
	unsigned int _enclu_u32(const int, size_t __data[]);
	unsigned int _enclv_u32(const int, size_t __data[]);
	unsigned __int64 _udiv128(unsigned __int64, unsigned __int64, unsigned __int64, unsigned __int64*);
	__int64 _div128(__int64, __int64, __int64, __int64*);
	unsigned _udiv64(unsigned __int64, unsigned, unsigned*);
	int _div64(__int64, int, int*);
	unsigned char _mm_aesdec128kl_u8(__m128i*, __m128i, const void*);
	unsigned char _mm_aesdec256kl_u8(__m128i*, __m128i, const void*);
	unsigned char _mm_aesdecwide128kl_u8(__m128i*, const __m128i*, const void*);
	unsigned char _mm_aesdecwide256kl_u8(__m128i*, const __m128i*, const void*);
	unsigned char _mm_aesenc128kl_u8(__m128i*, __m128i, const void*);
	unsigned char _mm_aesenc256kl_u8(__m128i*, __m128i, const void*);
	unsigned char _mm_aesencwide128kl_u8(__m128i*, const __m128i*, const void*);
	unsigned char _mm_aesencwide256kl_u8(__m128i*, const __m128i*, const void*);
	unsigned int  _mm_encodekey128_u32(unsigned int, __m128i, void*);
	unsigned int  _mm_encodekey256_u32(unsigned int, __m128i, __m128i, void*);
	void _mm_loadiwkey(unsigned int, __m128i, __m128i, __m128i);
	unsigned int __cdecl _rdpkru_u32(void);
	void __cdecl _wrpkru(unsigned int);
	int __cdecl _enqcmd(void*, const void*);
	int __cdecl _enqcmds(void*, const void*);
	void __cdecl _incsspd(unsigned int);
	unsigned int __cdecl _rdsspd(void);
	void __cdecl _saveprevssp(void);
	void __cdecl _rstorssp(void*);
	void __cdecl _wrssd(unsigned int, void*);
	void __cdecl _wrussd(unsigned int, void*);
	void __cdecl _setssbsy(void);
	void __cdecl _clrssbsy(void*);
	void* __cdecl _switchssp(void*);
	void __cdecl _incsspq(unsigned __int64);
	unsigned __int64 __cdecl _rdsspq(void);
	void __cdecl _wrssq(unsigned __int64, void*);
	void __cdecl _wrussq(unsigned __int64, void*);

	__m128i _mm_div_epi8(__m128i, __m128i);
	__m128i _mm_div_epi16(__m128i, __m128i);
	__m128i _mm_div_epi32(__m128i, __m128i);
	__m128i _mm_div_epi64(__m128i, __m128i);
	__m128i _mm_div_epu8(__m128i, __m128i);
	__m128i _mm_div_epu16(__m128i, __m128i);
	__m128i _mm_div_epu32(__m128i, __m128i);
	__m128i _mm_div_epu64(__m128i, __m128i);
	__m128i _mm_rem_epi8(__m128i, __m128i);
	__m128i _mm_rem_epi16(__m128i, __m128i);
	__m128i _mm_rem_epi32(__m128i, __m128i);
	__m128i _mm_rem_epi64(__m128i, __m128i);
	__m128i _mm_rem_epu8(__m128i, __m128i);
	__m128i _mm_rem_epu16(__m128i, __m128i);
	__m128i _mm_rem_epu32(__m128i, __m128i);
	__m128i _mm_rem_epu64(__m128i, __m128i);
	__m256i _mm256_div_epi8(__m256i, __m256i);
	__m256i _mm256_div_epi16(__m256i, __m256i);
	__m256i _mm256_div_epi32(__m256i, __m256i);
	__m256i _mm256_div_epi64(__m256i, __m256i);
	__m256i _mm256_div_epu8(__m256i, __m256i);
	__m256i _mm256_div_epu16(__m256i, __m256i);
	__m256i _mm256_div_epu32(__m256i, __m256i);
	__m256i _mm256_div_epu64(__m256i, __m256i);
	__m256i _mm256_rem_epi8(__m256i, __m256i);
	__m256i _mm256_rem_epi16(__m256i, __m256i);
	__m256i _mm256_rem_epi32(__m256i, __m256i);
	__m256i _mm256_rem_epi64(__m256i, __m256i);
	__m256i _mm256_rem_epu8(__m256i, __m256i);
	__m256i _mm256_rem_epu16(__m256i, __m256i);
	__m256i _mm256_rem_epu32(__m256i, __m256i);
	__m256i _mm256_rem_epu64(__m256i, __m256i);
	__m128i _mm_divrem_epi32(__m128i*, __m128i, __m128i);
	__m128i _mm_divrem_epu32(__m128i*, __m128i, __m128i);
	__m256i _mm256_divrem_epi32(__m256i*, __m256i, __m256i);
	__m256i _mm256_divrem_epu32(__m256i*, __m256i, __m256i);

	__m128  _mm_sin_ps(__m128);
	__m128d _mm_sin_pd(__m128d);
	__m256  _mm256_sin_ps(__m256);
	__m256d _mm256_sin_pd(__m256d);
	__m128  _mm_cos_ps(__m128);
	__m128d _mm_cos_pd(__m128d);
	__m256  _mm256_cos_ps(__m256);
	__m256d _mm256_cos_pd(__m256d);
	__m128  _mm_sincos_ps(__m128*, __m128);
	__m128d _mm_sincos_pd(__m128d*, __m128d);
	__m256  _mm256_sincos_ps(__m256*, __m256);
	__m256d _mm256_sincos_pd(__m256d*, __m256d);
	__m128  _mm_tan_ps(__m128);
	__m128d _mm_tan_pd(__m128d);
	__m256  _mm256_tan_ps(__m256);
	__m256d _mm256_tan_pd(__m256d);
	__m128  _mm_asin_ps(__m128);
	__m128d _mm_asin_pd(__m128d);
	__m256  _mm256_asin_ps(__m256);
	__m256d _mm256_asin_pd(__m256d);
	__m128  _mm_acos_ps(__m128);
	__m128d _mm_acos_pd(__m128d);
	__m256  _mm256_acos_ps(__m256);
	__m256d _mm256_acos_pd(__m256d);
	__m128  _mm_atan_ps(__m128);
	__m128d _mm_atan_pd(__m128d);
	__m256  _mm256_atan_ps(__m256);
	__m256d _mm256_atan_pd(__m256d);
	__m128  _mm_atan2_ps(__m128, __m128);
	__m128d _mm_atan2_pd(__m128d, __m128d);
	__m256  _mm256_atan2_ps(__m256, __m256);
	__m256d _mm256_atan2_pd(__m256d, __m256d);
	__m128  _mm_sind_ps(__m128);
	__m128d _mm_sind_pd(__m128d);
	__m256  _mm256_sind_ps(__m256);
	__m256d _mm256_sind_pd(__m256d);
	__m128  _mm_cosd_ps(__m128);
	__m128d _mm_cosd_pd(__m128d);
	__m256  _mm256_cosd_ps(__m256);
	__m256d _mm256_cosd_pd(__m256d);
	__m128  _mm_tand_ps(__m128);
	__m128d _mm_tand_pd(__m128d);
	__m256  _mm256_tand_ps(__m256);
	__m256d _mm256_tand_pd(__m256d);
	__m128  _mm_sinh_ps(__m128);
	__m128d _mm_sinh_pd(__m128d);
	__m256  _mm256_sinh_ps(__m256);
	__m256d _mm256_sinh_pd(__m256d);
	__m128  _mm_cosh_ps(__m128);
	__m128d _mm_cosh_pd(__m128d);
	__m256  _mm256_cosh_ps(__m256);
	__m256d _mm256_cosh_pd(__m256d);
	__m128  _mm_tanh_ps(__m128);
	__m128d _mm_tanh_pd(__m128d);
	__m256  _mm256_tanh_ps(__m256);
	__m256d _mm256_tanh_pd(__m256d);
	__m128  _mm_asinh_ps(__m128);
	__m128d _mm_asinh_pd(__m128d);
	__m256  _mm256_asinh_ps(__m256);
	__m256d _mm256_asinh_pd(__m256d);
	__m128  _mm_acosh_ps(__m128);
	__m128d _mm_acosh_pd(__m128d);
	__m256  _mm256_acosh_ps(__m256);
	__m256d _mm256_acosh_pd(__m256d);
	__m128  _mm_atanh_ps(__m128);
	__m128d _mm_atanh_pd(__m128d);
	__m256  _mm256_atanh_ps(__m256);
	__m256d _mm256_atanh_pd(__m256d);
	__m128  _mm_log_ps(__m128);
	__m128d _mm_log_pd(__m128d);
	__m256  _mm256_log_ps(__m256);
	__m256d _mm256_log_pd(__m256d);
	__m128  _mm_log1p_ps(__m128);
	__m128d _mm_log1p_pd(__m128d);
	__m256  _mm256_log1p_ps(__m256);
	__m256d _mm256_log1p_pd(__m256d);
	__m128  _mm_log10_ps(__m128);
	__m128d _mm_log10_pd(__m128d);
	__m256  _mm256_log10_ps(__m256);
	__m256d _mm256_log10_pd(__m256d);
	__m128  _mm_log2_ps(__m128);
	__m128d _mm_log2_pd(__m128d);
	__m256  _mm256_log2_ps(__m256);
	__m256d _mm256_log2_pd(__m256d);
	__m128  _mm_logb_ps(__m128);
	__m128d _mm_logb_pd(__m128d);
	__m256  _mm256_logb_ps(__m256);
	__m256d _mm256_logb_pd(__m256d);
	__m128  _mm_exp_ps(__m128);
	__m128d _mm_exp_pd(__m128d);
	__m256  _mm256_exp_ps(__m256);
	__m256d _mm256_exp_pd(__m256d);
	__m128  _mm_exp10_ps(__m128);
	__m128d _mm_exp10_pd(__m128d);
	__m256  _mm256_exp10_ps(__m256);
	__m256d _mm256_exp10_pd(__m256d);
	__m128  _mm_exp2_ps(__m128);
	__m128d _mm_exp2_pd(__m128d);
	__m256  _mm256_exp2_ps(__m256);
	__m256d _mm256_exp2_pd(__m256d);
	__m128  _mm_expm1_ps(__m128);
	__m128d _mm_expm1_pd(__m128d);
	__m256  _mm256_expm1_ps(__m256);
	__m256d _mm256_expm1_pd(__m256d);
	__m128  _mm_pow_ps(__m128, __m128);
	__m128d _mm_pow_pd(__m128d, __m128d);
	__m256  _mm256_pow_ps(__m256, __m256);
	__m256d _mm256_pow_pd(__m256d, __m256d);
	__m128  _mm_trunc_ps(__m128);
	__m128d _mm_trunc_pd(__m128d);
	__m256  _mm256_trunc_ps(__m256);
	__m256d _mm256_trunc_pd(__m256d);
	__m128  _mm_svml_floor_ps(__m128);
	__m128d _mm_svml_floor_pd(__m128d);
	__m256  _mm256_svml_floor_ps(__m256);
	__m256d _mm256_svml_floor_pd(__m256d);
	__m128  _mm_svml_ceil_ps(__m128);
	__m128d _mm_svml_ceil_pd(__m128d);
	__m256  _mm256_svml_ceil_ps(__m256);
	__m256d _mm256_svml_ceil_pd(__m256d);
	__m128  _mm_svml_round_ps(__m128);
	__m128d _mm_svml_round_pd(__m128d);
	__m256  _mm256_svml_round_ps(__m256);
	__m256d _mm256_svml_round_pd(__m256d);
	__m128  _mm_fmod_ps(__m128, __m128);
	__m128d _mm_fmod_pd(__m128d, __m128d);
	__m256  _mm256_fmod_ps(__m256, __m256);
	__m256d _mm256_fmod_pd(__m256d, __m256d);
	__m128  _mm_svml_sqrt_ps(__m128);
	__m128d _mm_svml_sqrt_pd(__m128d);
	__m256  _mm256_svml_sqrt_ps(__m256);
	__m256d _mm256_svml_sqrt_pd(__m256d);
	__m128  _mm_invsqrt_ps(__m128);
	__m128d _mm_invsqrt_pd(__m128d);
	__m256  _mm256_invsqrt_ps(__m256);
	__m256d _mm256_invsqrt_pd(__m256d);
	__m128  _mm_cbrt_ps(__m128);
	__m128d _mm_cbrt_pd(__m128d);
	__m256  _mm256_cbrt_ps(__m256);
	__m256d _mm256_cbrt_pd(__m256d);
	__m128  _mm_invcbrt_ps(__m128);
	__m128d _mm_invcbrt_pd(__m128d);
	__m256  _mm256_invcbrt_ps(__m256);
	__m256d _mm256_invcbrt_pd(__m256d);
	__m128  _mm_hypot_ps(__m128, __m128);
	__m128d _mm_hypot_pd(__m128d, __m128d);
	__m256  _mm256_hypot_ps(__m256, __m256);
	__m256d _mm256_hypot_pd(__m256d, __m256d);
	__m128  _mm_cdfnorm_ps(__m128);
	__m128d _mm_cdfnorm_pd(__m128d);
	__m256  _mm256_cdfnorm_ps(__m256);
	__m256d _mm256_cdfnorm_pd(__m256d);
	__m128  _mm_cdfnorminv_ps(__m128);
	__m128d _mm_cdfnorminv_pd(__m128d);
	__m256  _mm256_cdfnorminv_ps(__m256);
	__m256d _mm256_cdfnorminv_pd(__m256d);
	__m128  _mm_cexp_ps(__m128);
	__m256  _mm256_cexp_ps(__m256);
	__m128  _mm_clog_ps(__m128);
	__m256  _mm256_clog_ps(__m256);
	__m128  _mm_csqrt_ps(__m128);
	__m256  _mm256_csqrt_ps(__m256);
	__m128  _mm_erf_ps(__m128);
	__m128d _mm_erf_pd(__m128d);
	__m256  _mm256_erf_ps(__m256);
	__m256d _mm256_erf_pd(__m256d);
	__m128  _mm_erfc_ps(__m128);
	__m128d _mm_erfc_pd(__m128d);
	__m256  _mm256_erfc_ps(__m256);
	__m256d _mm256_erfc_pd(__m256d);
	__m128  _mm_erfcinv_ps(__m128);
	__m128d _mm_erfcinv_pd(__m128d);
	__m256  _mm256_erfcinv_ps(__m256);
	__m256d _mm256_erfcinv_pd(__m256d);
	__m128  _mm_erfinv_ps(__m128);
	__m128d _mm_erfinv_pd(__m128d);
	__m256  _mm256_erfinv_ps(__m256);
	__m256d _mm256_erfinv_pd(__m256d);
	void _mm_cldemote(void const*);
	void _directstoreu_u32(void*, unsigned int);
	void _directstoreu_u64(void*, unsigned __int64);
	void _movdir64b(void*, void const*);
	void __cdecl _serialize(void);
	void __cdecl _xsusldtrk(void);
	void __cdecl _xresldtrk(void);
	void _umonitor(void*);
	unsigned char _umwait(unsigned int, unsigned __int64);
	unsigned char _tpause(unsigned int, unsigned __int64);
	void _clui(void);
	void _stui(void);
	unsigned char _testui(void);
	void _senduipi(unsigned __int64);
	void _hreset(unsigned __int32);
	__m128 _mm_svml_cvtepu32_ps(__m128i);
	__m256 _mm256_svml_cvtepu32_ps(__m256i);
	__m128d _mm_svml_cvtepu32_pd(__m128i);
	__m256d _mm256_svml_cvtepu32_pd(__m128i);
	__m128d _mm_svml_cvtepi64_pd(__m128i);
	__m256d _mm256_svml_cvtepi64_pd(__m256i);
	__m128d _mm_svml_cvtepu64_pd(__m128i);
	__m256d _mm256_svml_cvtepu64_pd(__m256i);
	unsigned __int32 _castf32_u32(float);
	unsigned __int64 _castf64_u64(double);
	float _castu32_f32(unsigned __int32);
	double _castu64_f64(unsigned __int64);
	__m128i __cdecl _mm_madd52hi_avx_epu64(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_madd52hi_avx_epu64(__m256i, __m256i, __m256i);
	__m128i __cdecl _mm_madd52lo_avx_epu64(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_madd52lo_avx_epu64(__m256i, __m256i, __m256i);
	__m128 __cdecl _mm_bcstnebf16_ps(const __bfloat16*);
	__m256 __cdecl _mm256_bcstnebf16_ps(const __bfloat16*);
	__m128 __cdecl _mm_bcstnesh_ps(const void*);
	__m256 __cdecl _mm256_bcstnesh_ps(const void*);
	__m128 __cdecl _mm_cvtneebf16_ps(const __m128bh*);
	__m256 __cdecl _mm256_cvtneebf16_ps(const __m256bh*);
	__m128 __cdecl _mm_cvtneeph_ps(const __m128h*);
	__m256 __cdecl _mm256_cvtneeph_ps(const __m256h*);
	__m128 __cdecl _mm_cvtneobf16_ps(const __m128bh*);
	__m256 __cdecl _mm256_cvtneobf16_ps(const __m256bh*);
	__m128 __cdecl _mm_cvtneoph_ps(const __m128h*);
	__m256 __cdecl _mm256_cvtneoph_ps(const __m256h*);
	__m128bh __cdecl _mm_cvtneps_avx_pbh(__m128);
	__m128bh __cdecl _mm256_cvtneps_avx_pbh(__m256);
	__m128i __cdecl _mm_dpbssd_epi32(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_dpbssd_epi32(__m256i, __m256i, __m256i);
	__m128i __cdecl _mm_dpbssds_epi32(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_dpbssds_epi32(__m256i, __m256i, __m256i);
	__m128i __cdecl _mm_dpbsud_epi32(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_dpbsud_epi32(__m256i, __m256i, __m256i);
	__m128i __cdecl _mm_dpbsuds_epi32(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_dpbsuds_epi32(__m256i, __m256i, __m256i);
	__m128i __cdecl _mm_dpbuud_epi32(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_dpbuud_epi32(__m256i, __m256i, __m256i);
	__m128i __cdecl _mm_dpbuuds_epi32(__m128i, __m128i, __m128i);
	__m256i __cdecl _mm256_dpbuuds_epi32(__m256i, __m256i, __m256i);
	int __cdecl _cmpccxadd_epi32(void*, int, int, const int);
	__int64 __cdecl _cmpccxadd_epi64(void*, __int64, __int64, const int);
}