export module intrinsic;

export
{
    typedef union __declspec(intrin_type) __declspec(align(16)) __m128 {
        float               m128_f32[4];
        unsigned __int64    m128_u64[2];
        __int8              m128_i8[16];
        __int16             m128_i16[8];
        __int32             m128_i32[4];
        __int64             m128_i64[2];
        unsigned __int8     m128_u8[16];
        unsigned __int16    m128_u16[8];
        unsigned __int32    m128_u32[4];
    } __m128;
    typedef union __declspec(intrin_type) __declspec(align(16)) __m128i {
        __int8              m128i_i8[16];
        __int16             m128i_i16[8];
        __int32             m128i_i32[4];
        __int64             m128i_i64[2];
        unsigned __int8     m128i_u8[16];
        unsigned __int16    m128i_u16[8];
        unsigned __int32    m128i_u32[4];
        unsigned __int64    m128i_u64[2];
    } __m128i;

    typedef struct __declspec(intrin_type) __declspec(align(16)) __m128d {
        double              m128d_f64[2];
    } __m128d;
    
}