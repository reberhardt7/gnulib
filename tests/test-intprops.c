/* Test intprops.h.
   Copyright (C) 2011 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Paul Eggert.  */

#include <config.h>

#include "intprops.h"
#include "verify.h"

#include <stdbool.h>
#include <inttypes.h>

#include "macros.h"

/* Work around compiler bugs in HP-UX 11.23 cc, IRIX 6.5 cc, OSF/1 5.1
   cc, and Solaris 9 cc.  See
   <http://lists.gnu.org/archive/html/bug-gnulib/2011-05/msg00401.html>.  */
#undef UINT_MAX
#undef ULONG_MAX
#undef UINTMAX_MAX
#define UINT_MAX ((unsigned int) -1)
#define ULONG_MAX ((unsigned long int) -1)
#define UINTMAX_MAX ((uintmax_t) -1)
#define U0 ((unsigned int) 0)
#define U1 ((unsigned int) 1)

/* Integer representation.  */
verify (INT_MIN + INT_MAX < 0
        ? (TYPE_TWOS_COMPLEMENT (int)
           && ! TYPE_ONES_COMPLEMENT (int) && ! TYPE_SIGNED_MAGNITUDE (int))
        : (! TYPE_TWOS_COMPLEMENT (int)
           && (TYPE_ONES_COMPLEMENT (int) || TYPE_SIGNED_MAGNITUDE (int))));

/* TYPE_SIGNED.  */
/* verify (! TYPE_SIGNED (bool)); */ /* not guaranteed by gnulib substitute */
verify (TYPE_SIGNED (signed char));
verify (! TYPE_SIGNED (unsigned char));
verify (TYPE_SIGNED (short int));
verify (! TYPE_SIGNED (unsigned short int));
verify (TYPE_SIGNED (int));
verify (! TYPE_SIGNED (unsigned int));
verify (TYPE_SIGNED (long int));
verify (! TYPE_SIGNED (unsigned long int));
verify (TYPE_SIGNED (intmax_t));
verify (! TYPE_SIGNED (uintmax_t));

/* TYPE_MINIMUM, TYPE_MAXIMUM.  */
verify (TYPE_MINIMUM (char) == CHAR_MIN);
verify (TYPE_MAXIMUM (char) == CHAR_MAX);
verify (TYPE_MINIMUM (unsigned char) == 0);
verify (TYPE_MAXIMUM (unsigned char) == UCHAR_MAX);
verify (TYPE_MINIMUM (signed char) == SCHAR_MIN);
verify (TYPE_MAXIMUM (signed char) == SCHAR_MAX);
verify (TYPE_MINIMUM (short int) == SHRT_MIN);
verify (TYPE_MAXIMUM (short int) == SHRT_MAX);
verify (TYPE_MINIMUM (unsigned short int) == 0);
verify (TYPE_MAXIMUM (unsigned short int) == USHRT_MAX);
verify (TYPE_MINIMUM (int) == INT_MIN);
verify (TYPE_MAXIMUM (int) == INT_MAX);
verify (TYPE_MINIMUM (unsigned int) == 0);
verify (TYPE_MAXIMUM (unsigned int) == UINT_MAX);
verify (TYPE_MINIMUM (long int) == LONG_MIN);
verify (TYPE_MAXIMUM (long int) == LONG_MAX);
verify (TYPE_MINIMUM (unsigned long int) == 0);
verify (TYPE_MAXIMUM (unsigned long int) == ULONG_MAX);
verify (TYPE_MINIMUM (intmax_t) == INTMAX_MIN);
verify (TYPE_MAXIMUM (intmax_t) == INTMAX_MAX);
verify (TYPE_MINIMUM (uintmax_t) == 0);
verify (TYPE_MAXIMUM (uintmax_t) == UINTMAX_MAX);

/* INT_BITS_STRLEN_BOUND.  */
verify (INT_BITS_STRLEN_BOUND (1) == 1);
verify (INT_BITS_STRLEN_BOUND (2620) == 789);

/* INT_STRLEN_BOUND, INT_BUFSIZE_BOUND.  */
#ifdef INT32_MAX /* POSIX guarantees int32_t; this ports to non-POSIX hosts */
verify (INT_STRLEN_BOUND (int32_t) == sizeof ("-2147483648") - 1);
verify (INT_BUFSIZE_BOUND (int32_t) == sizeof ("-2147483648"));
#endif
#ifdef INT64_MAX
verify (INT_STRLEN_BOUND (int64_t) == sizeof ("-9223372036854775808") - 1);
verify (INT_BUFSIZE_BOUND (int64_t) == sizeof ("-9223372036854775808"));
#endif

/* All the INT_<op>_RANGE_OVERFLOW tests are equally valid as
   INT_<op>_OVERFLOW tests, so define a single macro to do both.  */
#define check_binop(op, a, b, min, max, overflow)                      \
  (INT_##op##_RANGE_OVERFLOW (a, b, min, max) == (overflow)            \
   && INT_##op##_OVERFLOW (a, b) == (overflow))
#define check_unop(op, a, min, max, overflow)                          \
  (INT_##op##_RANGE_OVERFLOW (a, min, max) == (overflow)               \
   && INT_##op##_OVERFLOW (a) == (overflow))

/* INT_<op>_RANGE_OVERFLOW, INT_<op>_OVERFLOW.  */
verify (INT_ADD_RANGE_OVERFLOW (INT_MAX, 1, INT_MIN, INT_MAX));
verify (INT_ADD_OVERFLOW (INT_MAX, 1));
verify (check_binop (ADD, INT_MAX, 1, INT_MIN, INT_MAX, true));
verify (check_binop (ADD, INT_MAX, -1, INT_MIN, INT_MAX, false));
verify (check_binop (ADD, INT_MIN, 1, INT_MIN, INT_MAX, false));
verify (check_binop (ADD, INT_MIN, -1, INT_MIN, INT_MAX, true));
verify (check_binop (ADD, UINT_MAX, U1, U0, UINT_MAX, true));
verify (check_binop (ADD, U0, U1, U0, UINT_MAX, false));

verify (check_binop (SUBTRACT, INT_MAX, 1, INT_MIN, INT_MAX, false));
verify (check_binop (SUBTRACT, INT_MAX, -1, INT_MIN, INT_MAX, true));
verify (check_binop (SUBTRACT, INT_MIN, 1, INT_MIN, INT_MAX, true));
verify (check_binop (SUBTRACT, INT_MIN, -1, INT_MIN, INT_MAX, false));
verify (check_binop (SUBTRACT, UINT_MAX, U1, U0, UINT_MAX, false));
verify (check_binop (SUBTRACT, U0, U1, U0, UINT_MAX, true));

verify (check_unop (NEGATE, INT_MIN, INT_MIN, INT_MAX,
                    TYPE_TWOS_COMPLEMENT (int)));
verify (check_unop (NEGATE, 0, INT_MIN, INT_MAX, false));
verify (check_unop (NEGATE, INT_MAX, INT_MIN, INT_MAX, false));
verify (check_unop (NEGATE, U0, U0, UINT_MAX, false));
verify (check_unop (NEGATE, U1, U0, UINT_MAX, true));
verify (check_unop (NEGATE, UINT_MAX, U0, UINT_MAX, true));

verify (check_binop (MULTIPLY, INT_MAX, INT_MAX, INT_MIN, INT_MAX, true));
verify (check_binop (MULTIPLY, INT_MAX, INT_MIN, INT_MIN, INT_MAX, true));
verify (check_binop (MULTIPLY, INT_MIN, INT_MAX, INT_MIN, INT_MAX, true));
verify (check_binop (MULTIPLY, INT_MIN, INT_MIN, INT_MIN, INT_MAX, true));
verify (check_binop (MULTIPLY, -1, INT_MIN, INT_MIN, INT_MAX,
                     INT_NEGATE_OVERFLOW (INT_MIN)));
verify (check_binop (MULTIPLY, LONG_MIN / INT_MAX, (long int) INT_MAX,
                     LONG_MIN, LONG_MIN, false));

verify (check_binop (DIVIDE, INT_MIN, -1, INT_MIN, INT_MAX,
                     INT_NEGATE_OVERFLOW (INT_MIN)));
verify (check_binop (DIVIDE, INT_MAX, 1, INT_MIN, INT_MAX, false));
verify (check_binop (DIVIDE, (unsigned int) INT_MIN,
                     -U1, U0, UINT_MAX, false));

verify (check_binop (REMAINDER, INT_MIN, -1, INT_MIN, INT_MAX,
                     INT_NEGATE_OVERFLOW (INT_MIN)));
verify (check_binop (REMAINDER, INT_MAX, 1, INT_MIN, INT_MAX, false));
verify (check_binop (REMAINDER, (unsigned int) INT_MIN,
                     -U1, U0, UINT_MAX, false));

verify (check_binop (LEFT_SHIFT, UINT_MAX, 1, U0, UINT_MAX, true));
verify (check_binop (LEFT_SHIFT, UINT_MAX / 2 + 1, 1, U0, UINT_MAX, true));
verify (check_binop (LEFT_SHIFT, UINT_MAX / 2, 1, U0, UINT_MAX, false));

/* INT_<op>_OVERFLOW with mixed types.  */
#define check_sum(a, b, overflow)                       \
  verify (INT_ADD_OVERFLOW (a, b) == (overflow));       \
  verify (INT_ADD_OVERFLOW (b, a) == (overflow))
check_sum (-1, LONG_MIN, true);
check_sum (-1, UINT_MAX, false);
check_sum (- (long) 1, INT_MIN, INT_MIN == LONG_MIN);
check_sum (U0, -1, true);
check_sum (U0, 0, false);
check_sum (U0, 1, false);
check_sum (1, LONG_MAX, true);
check_sum (1, UINT_MAX, true);
check_sum ((long) 1, INT_MAX, INT_MAX == LONG_MAX);
check_sum (U1, INT_MAX, INT_MAX == UINT_MAX);
check_sum (U1, INT_MIN, true);

verify (! INT_SUBTRACT_OVERFLOW (INT_MAX, U1));
verify (! INT_SUBTRACT_OVERFLOW (UINT_MAX, 1));
verify (! INT_SUBTRACT_OVERFLOW (U0, -1));
verify (INT_SUBTRACT_OVERFLOW (UINT_MAX, -1));
verify (INT_SUBTRACT_OVERFLOW (INT_MIN, U1));
verify (INT_SUBTRACT_OVERFLOW (-1, U0));

#define check_product(a, b, overflow)                   \
  verify (INT_MULTIPLY_OVERFLOW (a, b) == (overflow));   \
  verify (INT_MULTIPLY_OVERFLOW (b, a) == (overflow))

check_product (-1, U1, true);
check_product (-1, INT_MIN, INT_NEGATE_OVERFLOW (INT_MIN));
check_product (-1, UINT_MAX, true);
check_product (-12345, LONG_MAX / -12345 - 1, true);
check_product (-12345, LONG_MAX / -12345, false);
check_product (0, -1, false);
check_product (0, 0, false);
check_product (0, U0, false);
check_product (0, 1, false);
check_product (0, INT_MAX, false);
check_product (0, INT_MIN, false);
check_product (0, UINT_MAX, false);
check_product (U0, -1, false);
check_product (U0, 0, false);
check_product (U0, U0, false);
check_product (U0, 1, false);
check_product (U0, INT_MAX, false);
check_product (U0, INT_MIN, false);
check_product (U0, UINT_MAX, false);
check_product (1, INT_MAX, false);
check_product (1, INT_MIN, false);
check_product (1, UINT_MAX, false);
check_product (U1, INT_MIN, true);
check_product (U1, INT_MAX, UINT_MAX < INT_MAX);
check_product (INT_MAX, UINT_MAX, true);
check_product (INT_MAX, ULONG_MAX, true);
check_product (INT_MIN, LONG_MAX / INT_MIN - 1, true);
check_product (INT_MIN, LONG_MAX / INT_MIN, false);
check_product (INT_MIN, UINT_MAX, true);
check_product (INT_MIN, ULONG_MAX, true);

verify (INT_DIVIDE_OVERFLOW (INT_MIN, - (long) 1)
        == (TYPE_TWOS_COMPLEMENT (long int) && INT_MIN == LONG_MIN));
verify (! INT_DIVIDE_OVERFLOW (INT_MIN, UINT_MAX));
verify (! INT_DIVIDE_OVERFLOW (INTMAX_MIN, UINTMAX_MAX));
verify (! INT_DIVIDE_OVERFLOW (INTMAX_MIN, UINT_MAX));
verify (INT_DIVIDE_OVERFLOW (-11, (unsigned int) 10));
verify (INT_DIVIDE_OVERFLOW (-10, (unsigned int) 10));
verify (! INT_DIVIDE_OVERFLOW (-9, (unsigned int) 10));
verify (INT_DIVIDE_OVERFLOW ((unsigned int) 11, -10));
verify (INT_DIVIDE_OVERFLOW ((unsigned int) 10, -10));
verify (! INT_DIVIDE_OVERFLOW (9u, -10));

verify (INT_REMAINDER_OVERFLOW (INT_MIN, - (long) 1)
        == (TYPE_TWOS_COMPLEMENT (long int) && INT_MIN == LONG_MIN));
verify (INT_REMAINDER_OVERFLOW (-1, UINT_MAX));
verify (INT_REMAINDER_OVERFLOW ((intmax_t) -1, UINTMAX_MAX));
verify (INT_REMAINDER_OVERFLOW (INTMAX_MIN, UINT_MAX)
        == (INTMAX_MAX < UINT_MAX
            && - (unsigned int) INTMAX_MIN % UINT_MAX != 0));
verify (INT_REMAINDER_OVERFLOW (INT_MIN, ULONG_MAX)
        == (INT_MIN % ULONG_MAX != 1));
verify (! INT_REMAINDER_OVERFLOW (U1, -1));
verify (! INT_REMAINDER_OVERFLOW (37 * (unsigned int) 39, -39));
verify (INT_REMAINDER_OVERFLOW (37 * (unsigned int) 39 + 1, -39));
verify (INT_REMAINDER_OVERFLOW (37 * (unsigned int) 39 - 1, -39));
verify (! INT_REMAINDER_OVERFLOW (LONG_MAX, -INT_MAX));

int
main (void)
{
  /* Test expressions that might not be integer constant expressions.  */

  /* TYPE_IS_INTEGER.  */
  ASSERT (TYPE_IS_INTEGER (bool));
  ASSERT (TYPE_IS_INTEGER (char));
  ASSERT (TYPE_IS_INTEGER (signed char));
  ASSERT (TYPE_IS_INTEGER (unsigned char));
  ASSERT (TYPE_IS_INTEGER (short int));
  ASSERT (TYPE_IS_INTEGER (unsigned short int));
  ASSERT (TYPE_IS_INTEGER (int));
  ASSERT (TYPE_IS_INTEGER (unsigned int));
  ASSERT (TYPE_IS_INTEGER (long int));
  ASSERT (TYPE_IS_INTEGER (unsigned long int));
  ASSERT (TYPE_IS_INTEGER (intmax_t));
  ASSERT (TYPE_IS_INTEGER (uintmax_t));
  ASSERT (! TYPE_IS_INTEGER (float));
  ASSERT (! TYPE_IS_INTEGER (double));
  ASSERT (! TYPE_IS_INTEGER (long double));

  /* TYPE_SIGNED.  */
  ASSERT (TYPE_SIGNED (float));
  ASSERT (TYPE_SIGNED (double));
  ASSERT (TYPE_SIGNED (long double));

  return 0;
}