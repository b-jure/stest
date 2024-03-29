#ifndef __S_TEST_H__
#define __S_TEST_H__

#include <stdbool.h>
#include <string.h>

/******************************************************************************/
/*              ADVANCED ASSERTIONS (USE MACROS DEFINED BELOW)                */
/******************************************************************************/
/*
 * If you wish to fiddle with your own custom metadata for each assertion
 * then use these two* functions, these can virtually represent any test case,
 * provided you give the correct function that returns 'bool' for the assertion
 * you are making.
 * Usually user of this library should use the macros defined below that
 * do the work for you, and provide with the correct preproccessor metadata.
 */

/* Internal assert_eq function */
extern void _stest_assert_eq(bool result, const char *left, const char *right,
                             const char *file, const int line,
                             const char *fn_name);

/* Internal assert function */
extern void _stest_assert(bool result, const char *assertion, const char *file,
                          const int line, const char *fn_name);

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//
//
//
//

/******************************************************************************/
/*                               UNIT TESTS API                               */
/******************************************************************************/

/*
 * Wrapper around function name for better clarity as
 * to which function is a test.
 *
 * This is subject to change if library gets rewritten
 * to store test names this macro wraps around during
 * runtime.
 *
 * For now use this to have cleaner code, this provides
 * no functionality.
 */
#ifndef TEST
#define TEST(test_fn) void test_fn(void)
#endif

// Assert SRC is equivalent to DST.
#ifndef ASSERT_EQ
#define ASSERT_EQ(left, right)                                                 \
  _stest_assert_eq((left) == (right), #left, #right, __FILE__, __LINE__,       \
                   __func__)
#endif

// Assert SRC is not equivalent to DST.
#ifndef ASSERT_NEQ
#define ASSERT_NEQ(left, right)                                                \
  _stest_assert_eq((left) != (right), #left, #right, __FILE__, __LINE__,       \
                   __func__)
#endif

// Assert EXP is true.
#ifndef ASSERT
#define ASSERT(exp) _stest_assert((exp), #exp, __FILE__, __LINE__, __func__)
#endif

// Assert SRC string is equivalent to DST string.
#ifndef ASSERT_STR_EQ
#define ASSERT_STR_EQ(left, right)                                             \
  _stest_assert_eq(strcmp(left, right) == 0, #left, #right, __FILE__,          \
                   __LINE__, __func__)
#endif

// Assert SRC string is not equivalent to DST string.
#ifndef ASSERT_STR_NEQ
#define ASSERT_STR_NEQ(left, right)                                            \
  _stest_assert_eq(strcmp(left, right) != 0, #left, #right, __FILE__,          \
                   __LINE__, __func__)
#endif

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//
//
//
//

/******************************************************************************/
/*                              RUNNER/SUITE API                              */
/******************************************************************************/

/*
 * Opaque type representing runner that runs suites contained within it.
 * Suite being the set of tests that are logically related hence being
 * grouped together in a 'suite'.
 */
typedef struct _srunner srunner;

/*
 * Opaque type representing individual suite.
 * Suite is considered to be a set/group of tests.
 * If a group of tests are closley related to each other or are testing
 * the same structure or individual element/function, it is generally
 * a good idea to combine them together into a 'suite'.
 */
typedef struct _ssuite ssuite;

/*
 * Runner constructor.
 * Returns new instance of 'srunner', if malloc returns NULL (out of memory)
 * it returns NULL.
 */
srunner *srunner_new(void);

/*
 * Adds new SUITE into RUNNER.
 *
 * If adding the suite fails (out of memory), program prints err to stderr
 * and exits with EXIT_FAILURE.
 */
void srunner_add_suite(srunner *runner, ssuite *suite);

/*
 * Adds LEN of SUITES into RUNNER.
 *
 * If adding the SUITES fails (out of memory), program prints err to stderr
 * and exits with EXIT_FAILURE.
 */
void srunner_add_suites(srunner *runner, ssuite **suites, size_t len);

/*
 * Runs the RUNNER, running all the suites in the runner.
 * Each suite in turns runs all of its tests.
 */
void srunner_run(srunner *runner);

/*
 * Frees the memory allocated by the RUNNER and the contained SUITES and TESTS.
 * Performs complete cleanup.
 */
void srunner_free(srunner *runner);

/*
 * Creates a new instance of 'ssuite'.
 *
 * Suite is a collection of tests semantically connected in some way
 * (or not if you are doing it wrong).
 *
 * You can provide any NAME you want for the suite (just don't provide NULL).
 * If process runs out of memory or name provided is NULL, then it returns NULL.
 *
 * Note:
 * Currently there is no benefit of naming the suite,
 * but fetching the suite from the runner might become
 * usefull later if/when the API expands and especially
 * when the thread safety (if) gets added in.
 */
ssuite *ssuite_new(const char *name);

/*
 * Use the ssuite_add_test macro instead (below).
 */
extern void _ssuite_add_test(ssuite *suite, const char *name, void (*fn)(void));

/*
 * Inserts FN into SSUITE.
 * FN is a function pointer representing the test that you want to run.
 * Use this macro always isntead of the '_ssuite_add_test'.
 *
 * Note:
 * If for example function pointer is valid but the function itself does not
 * contain any ASSERT macros of this library, the test will pass as success even
 * if the function is not a proper test function. Unsoundness here is that test
 * might get printed as it was ran and was successfull even if the function that
 * is running is not an actual test function (it is not testing anything).
 * So please be carefull and provide the actual tests!
 */
#ifndef ssuite_add_test
#define ssuite_add_test(ssuite, fn) _ssuite_add_test(ssuite, #fn, fn)
#endif

#endif
