/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

 #ifdef __cplusplus
 extern "C" {
 #endif
 
 #ifndef __CC_H
 #define __CC_H
 
 #include <inttypes.h>
 #include <stdbool.h>
 #include <stdlib.h> 
 #include <stdint.h>
 #include <stdio.h>
 #include <string.h>
 #include <time.h>
 #include <unistd.h>
 
 /**
  * test_value_type_t contains all possible types that can be under test.
  */
 typedef enum {
     test_type_char,
     test_type_string,
     test_type_bool,
     test_type_float,
     test_type_double,
     test_type_long,
     test_type_long_long,
     test_type_int,
     test_type_int8,
     test_type_int16,
     test_type_int32,
     test_type_int64,
     test_type_uint,
     test_type_uint8,
     test_type_uint16,
     test_type_uint32,
     test_type_uint64,
 } test_value_type_t;
 
 /**
  * test_values_t holds all possible values that can be under test.
  */
 typedef union {
     int int_val;
     unsigned int uint_val;
     float float_val;
     double double_val;
     long long_val;
     long long long_long_val;
     int8_t int8_val;
     int16_t int16_val;
     int32_t int32_val;
     int64_t int64_val;
     uint8_t uint8_val;
     uint16_t uint16_val;
     uint32_t uint32_val;
     uint64_t uint64_val;
     char char_val;
     char *string_val;
     bool bool_val;
 } test_values_t;
 
 /**
  * cc_result_t holds the state of a test run, most importantly whether
  * the test has failed or succeeded.
  */
 typedef struct {
     char *filename;
     char *function;
     test_value_type_t type;
     test_values_t exp;
     test_values_t act;
     bool result;
     uint64_t line;
 } cc_result_t;
 
 /**
  * cc_func_t is the function definition for a unit test to be passed into the
  * cc_run function for execution.
  */
 typedef cc_result_t (*cc_func_t)();
 
 /**
  * CC_SUCCESS is a convenience macro to be included at the end of the 
  * test functions indicating a successful run.
  */
 #define CC_SUCCESS return (cc_result_t) { \
     .function = (char*)__FUNCTION__,      \
     .result = true,                       \
 };
 
 /**
  * __CC_VALUES_ASSIGN handles copying the values under test into the result
  * struct for error output.
  */
 #define __CC_VALUES_ASSIGN(actual, expected, test_type)                      \
     if ((test_type) == test_type_char) {                                     \
         ccrt.exp = (test_values_t) {.char_val = (char)(expected)};           \
         ccrt.act = (test_values_t) {.char_val = (char)(actual)};             \
     } else if ((test_type) == test_type_float) {                             \
         ccrt.exp = (test_values_t) {.float_val = (float)(expected)};         \
         ccrt.act = (test_values_t) {.float_val = (float)(actual)};           \
     } else if ((test_type) == test_type_double) {                            \
         ccrt.exp = (test_values_t) {.double_val = (double)(expected)};       \
         ccrt.act = (test_values_t) {.double_val = (double)(actual)};         \
     } else if ((test_type) == test_type_long) {                              \
         ccrt.exp = (test_values_t) {.long_val = (long)(expected)};           \
         ccrt.act = (test_values_t) {.long_val = (long)(actual)};             \
     } else if ((test_type) == test_type_long_long) {                         \
         ccrt.exp = (test_values_t) {.long_long_val = (long long)(expected)}; \
         ccrt.act = (test_values_t) {.long_long_val = (long long)(actual)};   \
     } else if ((test_type) == test_type_int) {                               \
         ccrt.exp = (test_values_t) {.int_val = (int)(expected)};             \
         ccrt.act = (test_values_t) {.int_val = (int)(actual)};               \
     } else if ((test_type) == test_type_int8) {                              \
         ccrt.exp = (test_values_t) {.int8_val = (int8_t)(expected)};         \
         ccrt.act = (test_values_t) {.int8_val = (int8_t)(actual)};           \
     } else if ((test_type) == test_type_int16) {                             \
         ccrt.exp = (test_values_t) {.int16_val = (int16_t)(expected)};       \
         ccrt.act = (test_values_t) {.int16_val = (int16_t)(actual)};         \
     } else if ((test_type) == test_type_int32) {                             \
         ccrt.exp = (test_values_t) {.int32_val = (int32_t)(expected)};       \
         ccrt.act = (test_values_t) {.int32_val = (int32_t)(actual)};         \
     } else if ((test_type) == test_type_int64) {                             \
         ccrt.exp = (test_values_t) {.int64_val = (int64_t)(expected)};       \
         ccrt.act = (test_values_t) {.int64_val = (int64_t)(actual)};         \
     } else if ((test_type) == test_type_uint) {                              \
         ccrt.exp = (test_values_t) {.uint_val = (unsigned int)(expected)};   \
         ccrt.act = (test_values_t) {.uint_val = (unsigned int)(actual)};     \
     } else if ((test_type) == test_type_uint8) {                             \
         ccrt.exp = (test_values_t) {.uint8_val = (uint8_t)(expected)};       \
         ccrt.act = (test_values_t) {.uint8_val = (uint8_t)(actual)};         \
     } else if ((test_type) == test_type_uint16) {                            \
         ccrt.exp = (test_values_t) {.uint16_val = (uint16_t)(expected)};     \
         ccrt.act = (test_values_t) {.uint16_val = (uint16_t)(actual)};       \
     } else if ((test_type) == test_type_uint32) {                            \
         ccrt.exp = (test_values_t) {.uint32_val = (uint32_t)(expected)};     \
         ccrt.act = (test_values_t) {.uint32_val = (uint32_t)(actual)};       \
     } else if ((test_type) == test_type_uint64) {                            \
         ccrt.exp = (test_values_t) {.uint64_val = (uint64_t)(expected)};     \
         ccrt.act = (test_values_t) {.uint64_val = (uint64_t)(actual)};       \
     }
 
 /**
  * __CC_ASSERT_NUMBER_EQUAL_TYPE is the "unexported" check for number
  * value equality.
  */
 #define __CC_ASSERT_NUMBER_EQUAL_TYPE(actual, expected, test_type) \
     do {                                                           \
         if ((actual) != (expected)) {                              \
             cc_result_t ccrt = (cc_result_t) {                     \
                 .filename = __FILE__,                              \
                 .function = (char*)__FUNCTION__,                   \
                 .type = (test_type),                               \
                 .result = false,                                   \
                 .line = __LINE__                                   \
             };                                                     \
             __CC_VALUES_ASSIGN(actual, expected, test_type);       \
             return ccrt;                                           \
         }                                                          \
     } while (0)
 
 /**
  * __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE is the "unexported" check for number
  * value in-equality.
  */
 #define __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE(actual, expected, test_type) \
     do {                                                               \
         if ((actual) == (expected)) {                                  \
             cc_result_t ccrt = (cc_result_t) {                         \
                 .filename = __FILE__,                                  \
                 .function = (char*)__FUNCTION__,                       \
                 .type = (test_type),                                   \
                 .result = false,                                       \
                 .line = __LINE__                                       \
             };                                                         \
             __CC_VALUES_ASSIGN(actual, expected, test_type);           \
             return ccrt;                                               \
         }                                                              \
     } while (0)
 
 /* Type specific wrapper macros for equality */
 #define CC_ASSERT_CHAR_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_char)
 #define CC_ASSERT_FLOAT_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_float)
 #define CC_ASSERT_DOUBLE_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_double)
 #define CC_ASSERT_LONG_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_long)
 #define CC_ASSERT_LONG_LONG_EQUAL(actual, expected)                          \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_long_long)
 #define CC_ASSERT_INT_EQUAL(actual, expected)                                \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_int)
 #define CC_ASSERT_INT8_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_int8)
 #define CC_ASSERT_INT16_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_int16)
 #define CC_ASSERT_INT32_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_int32)
 #define CC_ASSERT_INT64_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_int64)
 #define CC_ASSERT_UINT_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_uint)
 #define CC_ASSERT_UINT8_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_uint8)
 #define CC_ASSERT_UINT16_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_uint16)
 #define CC_ASSERT_UINT32_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_uint32)
 #define CC_ASSERT_UINT64_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_EQUAL_TYPE((actual), (expected), test_type_uint64)
 
 /* Type specific wrapper macros for in-equality */
 #define CC_ASSERT_CHAR_NOT_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_char)
 #define CC_ASSERT_FLOAT_NOT_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_float)
 #define CC_ASSERT_DOUBLE_NOT_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_double)
 #define CC_ASSERT_LONG_NOT_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_long)
 #define CC_ASSERT_LONG_LONG_NOT_EQUAL(actual, expected)                          \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_long_long)
 #define CC_ASSERT_INT_NOT_EQUAL(actual, expected)                                \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((int)(actual), (int)(expected), test_type_int)
 #define CC_ASSERT_INT8_NOT_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_int8)
 #define CC_ASSERT_INT16_NOT_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_int16)
 #define CC_ASSERT_INT32_NOT_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_int32)
 #define CC_ASSERT_INT64_NOT_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_int64)
 #define CC_ASSERT_UINT_NOT_EQUAL(actual, expected)                               \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_uint)
 #define CC_ASSERT_UINT8_NOT_EQUAL(actual, expected)                              \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_uint8)
 #define CC_ASSERT_UINT16_NOT_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_uint16)
 #define CC_ASSERT_UINT32_NOT_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_uint32)
 #define CC_ASSERT_UINT64_NOT_EQUAL(actual, expected)                             \
     __CC_ASSERT_NUMBER_NOT_EQUAL_TYPE((actual), (expected), test_type_uint64)
 
 /**
  * __CC_STRING_VAL_COPY copies the string values under test.
  */
 #define __CC_STRING_VAL_COPY(actual, expected)                   \
     ccrt.exp = (test_values_t) {                                 \
         .string_val = calloc(strlen((expected))+1, sizeof(char)) \
     };                                                           \
     strcpy(ccrt.exp.string_val, (expected));                     \
     ccrt.act = (test_values_t) {                                 \
         .string_val = calloc(strlen((actual))+1, sizeof(char))   \
     };                                                           \
     strcpy(ccrt.act.string_val, (actual));
 
 /**
  * CC_ASSERT_STRING_EQUAL takes 2 strings and reports on their inequality. 
  */
 #define CC_ASSERT_STRING_EQUAL(actual, expected)    \
     do {                                            \
         if (strcmp(actual, expected) != 0) {        \
             cc_result_t ccrt = (cc_result_t) {      \
                 .filename = __FILE__,               \
                 .function = (char*)__FUNCTION__,    \
                 .type = test_type_string,           \
                 .result = false,                    \
                 .line = __LINE__                    \
             };                                      \
             __CC_STRING_VAL_COPY(actual, expected); \
             return ccrt;                            \
         } \
     } while (0)
 
 /**
  * CC_ASSERT_STRING_NOT_EQUAL takes 2 strings and reports on their inequality. 
  */
 #define CC_ASSERT_STRING_NOT_EQUAL(actual, expected) \
     do {                                             \
         if (strcmp(actual, expected) == 0) {         \
             cc_result_t ccrt = (cc_result_t) {       \
                 .filename = __FILE__,                \
                 .function = (char*)__FUNCTION__,     \
                 .type = test_type_string,            \
                 .result = false,                     \
                 .line = __LINE__                     \
             };                                       \
             __CC_STRING_VAL_COPY(actual, expected);  \
             return ccrt;                             \
         }                                            \
     } while (0)
 
 
 #define __CC_ASSIGN_BOOL(actual)                     \
     cc_result_t ccrt = (cc_result_t) {               \
         .filename = __FILE__,                        \
         .function = (char*)__FUNCTION__,             \
         .type = test_type_bool,                      \
         .act = (test_values_t) {.bool_val = actual}, \
         .result = false,                             \
         .line = __LINE__                             \
     };
 
 /**
  * CC_ASSERT_TRUE checks to see if the given value is true.
  */
 #define CC_ASSERT_TRUE(actual)        \
     do {                              \
         if (actual == false) {        \
             __CC_ASSIGN_BOOL(actual); \
             return ccrt;              \
         }                             \
     } while (0)
 
 /**
  * CC_ASSERT_FALSE checks to see if the given value is false.
  */
 #define CC_ASSERT_FALSE(actual)   \
 do {                              \
     if (actual != false) {        \
         __CC_ASSIGN_BOOL(actual); \
         return ccrt;              \
     }                             \
 } while (0)

#define __CC_NULL_RES cc_result_t ccrt = (cc_result_t) { \
    .filename = __FILE__,                                \
    .function = (char*)__FUNCTION__,                     \
    .type = test_type_string,                            \
    .result = false,                                     \
    .line = __LINE__                                     \
};

/**
* CC_ASSERT_NULL checks to see if the given value is NULL.
*/
#define CC_ASSERT_NULL(actual)                           \
do {                                                     \
    if (actual != NULL) {                                \
        __CC_NULL_RES;                                   \
        __CC_STRING_VAL_COPY("NULL", "not NULL");        \
        return ccrt;                                     \
    }                                                    \
} while (0)

/**
* CC_ASSERT_NOT_NULL checks to see if the given value is not NULL.
*/
#define CC_ASSERT_NOT_NULL(actual)                       \
do {                                                     \
    if (actual == NULL) {                                \
        __CC_NULL_RES;                                   \
        __CC_STRING_VAL_COPY("not NULL", "NULL");        \
        return ccrt;                                     \
    }                                                    \
} while (0)

 /**
  * cc_setup is a function that needs to be implemented by the consumer of the 
  * library and will be ran before every test.
  */
 void
 cc_setup();
 
 /**
  * cc_tear_down is a function that needs to be implemented by the consumer of
  * the library and will be ran before every test.
  */
 void
 cc_tear_down();
 
 /**
  * cc_init initializes the library and needed memory.
  */
 void
 cc_init();
 
 /**
  * cc_complete cleans up used resources and prints results.
  */
 uint64_t
 cc_complete();
 
 /**
  * Run the given test.
  */
 bool
 cc_run(cc_func_t func);
 
 #define CC_INIT cc_init();
 #define CC_RUN(func) cc_run(func);
 #define CC_COMPLETE return cc_complete();
 
 #endif /* end __CC_H */
 #ifdef __cplusplus
 }
 #endif
 