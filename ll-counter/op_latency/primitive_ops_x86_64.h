#ifndef PRIMITIVE_OPS_X86_64H
#define PRIMITIVE_OPS_X86_64H

/** Implementation of primitive ops for x86_64
 *
 *  Should be included automatically from primitive_ops.h and not directly by
 *  other code.
 */

template <>
struct primitive_op<arith_op::add> {
    ALWAYS_INLINE static void run(float &a1,float a2,...) {
        asm volatile ("addss %0,%1\n\t" :"+x"(a1) :"x"(a2));
    }
    ALWAYS_INLINE static void run(double &a1,double a2,...) {
        asm volatile ("addsd %0,%1\n\t" :"+x"(a1) :"x"(a2));
    }
    static constexpr bool is_specialized=true;
};

template <>
struct primitive_op<arith_op::mul> {
    ALWAYS_INLINE static void run(float &a1,float a2,...) {
        asm volatile ("mulss %0,%1\n\t" :"+x"(a1) :"x"(a2));
    }
    ALWAYS_INLINE static void run(double &a1,double a2,...) {
        asm volatile ("mulsd %0,%1\n\t" :"+x"(a1) :"x"(a2));
    }
    static constexpr bool is_specialized=true;
};

template <>
struct primitive_op<arith_op::div> {
    ALWAYS_INLINE static void run(float &a1,float a2,...) {
        asm volatile ("divss %0,%1\n\t" :"+x"(a1) :"x"(a2));
    }
    ALWAYS_INLINE static void run(double &a1,double a2,...) {
        asm volatile ("divsd %0,%1\n\t" :"+x"(a1) :"x"(a2));
    }
    static constexpr bool is_specialized=true;
};

template <>
struct primitive_op<arith_op::sqrt> {
    ALWAYS_INLINE static void run(float &a1,...) {
        asm volatile ("sqrtss %0,%1\n\t" :"=x"(a1) :"0"(a1));
    }
    ALWAYS_INLINE static void run(double &a1,...) {
        asm volatile ("sqrtsd %0,%1\n\t" :"=x"(a1) :"0"(a1));
    }
    static constexpr bool is_specialized=true;
};

inline void consume(float v)  { asm volatile ("" ::"x"(v)); }
inline void consume(double v) { asm volatile ("" ::"x"(v)); }


#endif // ndef PRIMITIVE_OPS_X86_64H
