/*
 * atomic.h
 *
 * Created by ElioYang on 2022/2/17.
 * Email: jluelioyang2001@gmail.com
 *
 * MIT License
 * Copyright (c) 2021 Elio-yang
 *
 */

#ifndef EOS_DEV_ATOMIC_H
#define EOS_DEV_ATOMIC_H
#include "io.h"
#include "system.h"
#include "config.h"

/* atomic_t should be 32 bit signed type */

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#ifdef CONFIG_SMP
#define LOCK "lock ; "
#else
#define LOCK ""
#endif

/*
 * Make sure gcc doesn't try to be clever and move things around
 * on us. We need to use _exactly_ the address the user gave us,
 * not some alias that contains the same information.
 */
typedef struct { volatile int32_t counter; } atomic_t;

#define ATOMIC_INIT(i)	{ (i) }

/**
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_read(v)		((v)->counter)

/**
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#define atomic_set(v,i)		(((v)->counter) = (i))

/**
 * atomic_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v.  Note that the guaranteed useful range
 * of an atomic_t is only 24 bits.
 */
static __inline__ void atomic_add(int i, atomic_t *v)
{
        __ASM__(
        LOCK "addl %1,%0"
        :"=m" (v->counter)
        :"ir" (i), "m" (v->counter));
}

/**
 * atomic_sub - subtract the atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ void atomic_sub(int i, atomic_t *v)
{
        __ASM__(
        LOCK "subl %1,%0"
        :"=m" (v->counter)
        :"ir" (i), "m" (v->counter));
}

/**
 * atomic_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ int atomic_sub_and_test(int i, atomic_t *v)
{
        unsigned char c;

        __ASM__(
        LOCK "subl %2,%0; sete %1"
        :"=m" (v->counter), "=qm" (c)
        :"ir" (i), "m" (v->counter) : "memory");
        return c;
}

/**
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ void atomic_inc(atomic_t *v)
{
        __ASM__(
        LOCK "incl %0"
        :"=m" (v->counter)
        :"m" (v->counter));
}

/**
 * atomic_dec - decrement atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ void atomic_dec(atomic_t *v)
{
        __ASM__(
        LOCK "decl %0"
        :"=m" (v->counter)
        :"m" (v->counter));
}

/**
 * atomic_dec_and_test - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ int atomic_dec_and_test(atomic_t *v)
{
        unsigned char c;

        __ASM__(
        LOCK "decl %0; sete %1"
        :"=m" (v->counter), "=qm" (c)
        :"m" (v->counter) : "memory");
        return c != 0;
}

/**
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ int atomic_inc_and_test(atomic_t *v)
{
        unsigned char c;

        __ASM__(
        LOCK "incl %0; sete %1"
        :"=m" (v->counter), "=qm" (c)
        :"m" (v->counter) : "memory");
        return c != 0;
}

/**
 * atomic_add_negative - add and test if negative
 * @v: pointer of type atomic_t
 * @i: integer value to add
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
static __inline__ int atomic_add_negative(int i, atomic_t *v)
{
        unsigned char c;

        __ASM__(
        LOCK "addl %2,%0; sets %1"
        :"=m" (v->counter), "=qm" (c)
        :"ir" (i), "m" (v->counter) : "memory");
        return c;
}

#endif //EOS_DEV_ATOMIC_H
