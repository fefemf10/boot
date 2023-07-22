export module intrinsicTypes;

export
{
	union __declspec(intrin_type) __declspec(align(16)) __m128
	{
		float               m128_f32[4];
		unsigned __int64    m128_u64[2];
		__int8              m128_i8[16];
		__int16             m128_i16[8];
		__int32             m128_i32[4];
		__int64             m128_i64[2];
		unsigned __int8     m128_u8[16];
		unsigned __int16    m128_u16[8];
		unsigned __int32    m128_u32[4];
	};
	union __declspec(intrin_type) __declspec(align(16)) __m128i
	{
		__int8              m128i_i8[16];
		__int16             m128i_i16[8];
		__int32             m128i_i32[4];
		__int64             m128i_i64[2];
		unsigned __int8     m128i_u8[16];
		unsigned __int16    m128i_u16[8];
		unsigned __int32    m128i_u32[4];
		unsigned __int64    m128i_u64[2];
	};

	struct __declspec(intrin_type) __declspec(align(16)) __m128d
	{
		double              m128d_f64[2];
	};

	union __declspec(intrin_type) __declspec(align(32)) __m256
	{
		float m256_f32[8];
	};

	struct __declspec(intrin_type) __declspec(align(32)) __m256d
	{
		double m256d_f64[4];
	};

	union  __declspec(intrin_type) __declspec(align(32)) __m256i
	{
		__int8              m256i_i8[32];
		__int16             m256i_i16[16];
		__int32             m256i_i32[8];
		__int64             m256i_i64[4];
		unsigned __int8     m256i_u8[32];
		unsigned __int16    m256i_u16[16];
		unsigned __int32    m256i_u32[8];
		unsigned __int64    m256i_u64[4];
	};

	constexpr auto _CMP_EQ_OQ = 0x00;  /* Equal (ordered, nonsignaling)               */
	constexpr auto _CMP_LT_OS = 0x01;  /* Less-than (ordered, signaling)              */
	constexpr auto _CMP_LE_OS = 0x02;  /* Less-than-or-equal (ordered, signaling)     */
	constexpr auto _CMP_UNORD_Q = 0x03;  /* Unordered (nonsignaling)                    */
	constexpr auto _CMP_NEQ_UQ = 0x04;  /* Not-equal (unordered, nonsignaling)         */
	constexpr auto _CMP_NLT_US = 0x05;  /* Not-less-than (unordered, signaling)        */
	constexpr auto _CMP_NLE_US = 0x06;  /* Not-less-than-or-equal (unordered,
															signaling)          */
	constexpr auto _CMP_ORD_Q = 0x07;  /* Ordered (nonsignaling)                      */
	constexpr auto _CMP_EQ_UQ = 0x08;  /* Equal (unordered, non-signaling)            */
	constexpr auto _CMP_NGE_US = 0x09;  /* Not-greater-than-or-equal (unordered,
															   signaling)       */
	constexpr auto _CMP_NGT_US = 0x0A;  /* Not-greater-than (unordered, signaling)     */
	constexpr auto _CMP_FALSE_OQ = 0x0B;  /* False (ordered, nonsignaling)               */
	constexpr auto _CMP_NEQ_OQ = 0x0C;  /* Not-equal (ordered, non-signaling)          */
	constexpr auto _CMP_GE_OS = 0x0D;  /* Greater-than-or-equal (ordered, signaling)  */
	constexpr auto _CMP_GT_OS = 0x0E;  /* Greater-than (ordered, signaling)           */
	constexpr auto _CMP_TRUE_UQ = 0x0F;  /* True (unordered, non-signaling)             */
	constexpr auto _CMP_EQ_OS = 0x10;  /* Equal (ordered, signaling)                  */
	constexpr auto _CMP_LT_OQ = 0x11;  /* Less-than (ordered, nonsignaling)           */
	constexpr auto _CMP_LE_OQ = 0x12;  /* Less-than-or-equal (ordered, nonsignaling)  */
	constexpr auto _CMP_UNORD_S = 0x13;  /* Unordered (signaling)                       */
	constexpr auto _CMP_NEQ_US = 0x14;  /* Not-equal (unordered, signaling)            */
	constexpr auto _CMP_NLT_UQ = 0x15;  /* Not-less-than (unordered, nonsignaling)     */
	constexpr auto _CMP_NLE_UQ = 0x16;  /* Not-less-than-or-equal (unordered,
															nonsignaling)       */
	constexpr auto _CMP_ORD_S = 0x17;  /* Ordered (signaling)                         */
	constexpr auto _CMP_EQ_US = 0x18;  /* Equal (unordered, signaling)                */
	constexpr auto _CMP_NGE_UQ = 0x19;  /* Not-greater-than-or-equal (unordered,
															   nonsignaling)    */
	constexpr auto _CMP_NGT_UQ = 0x1A;  /* Not-greater-than (unordered, nonsignaling)  */
	constexpr auto _CMP_FALSE_OS = 0x1B;  /* False (ordered, signaling)                  */
	constexpr auto _CMP_NEQ_OS = 0x1C;  /* Not-equal (ordered, signaling)              */
	constexpr auto _CMP_GE_OQ = 0x1D;  /* Greater-than-or-equal (ordered,
														   nonsignaling)        */
	constexpr auto _CMP_GT_OQ = 0x1E;  /* Greater-than (ordered, nonsignaling)        */
	constexpr auto _CMP_TRUE_US = 0x1F;  /* True (unordered, signaling)                 */

	using __mmask8 = unsigned char;
	using __mmask16 = unsigned short;
	using __mmask32 = unsigned int;
	using __mmask64 = unsigned __int64;

	union __declspec(intrin_type) __declspec(align(64)) __m512
	{
		float m512_f32[16];
	};

	struct __declspec(intrin_type) __declspec(align(64)) __m512d
	{
		double m512d_f64[8];
	};

	union  __declspec(intrin_type) __declspec(align(64)) __m512i
	{
		__int8              m512i_i8[64];
		__int16             m512i_i16[32];
		__int32             m512i_i32[16];
		__int64             m512i_i64[8];
		unsigned __int8     m512i_u8[64];
		unsigned __int16    m512i_u16[32];
		unsigned __int32    m512i_u32[16];
		unsigned __int64    m512i_u64[8];
	};
	using __m128bh = __m128i;
	using __m256bh = __m256i;
	using __m512bh = __m512i;
	using __bfloat16 = unsigned short;
	using __m128h = __m128i;
	using __m256h = __m256i;
	using __m512h = __m512i;
}