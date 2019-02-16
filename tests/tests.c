#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "../builtins.h"
#include "unity/unity.h"

/**
 * test_true_cmd validates that 1 is returned
 * to represent false.
 */
void
test_true_cmd(void)
{
    int got = true_cmd(0, NULL);
    int expected = 1;

    TEST_ASSERT_EQUAL(expected, got);
}

/**
 * test_false_cmd validates that 0 is returned
 * to represent false.
 */
void
test_false_cmd(void)
{
    int got = false_cmd(0, NULL);
    int expected = 0;

    TEST_ASSERT_EQUAL(expected, got);
}

/**
 * test_which_cmd validates that 0 is returned
 * on valid command and 1 on invalid arg count
 */
void
test_which_cmd(void)
{
  int got = which_cmd(0, NULL);
  int expected = 1;

  TEST_ASSERT_EQUAL(expected, got);

  // since which is guaranteed to be in builtins when testing itself
  // use that as the test example looking for a 0 exit code
  char *args[] = {"which", "which"};
  got = which_cmd(2, args);
  expected = 0;

  TEST_ASSERT_EQUAL(expected, got);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_true_cmd);
  RUN_TEST(test_false_cmd);
  RUN_TEST(test_which_cmd);

  return UNITY_END();
}
