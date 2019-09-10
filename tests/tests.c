#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "../spinner.h"
#include "unity/unity.h"

/**
 * test_spinner_new verifies that the spinner_new
 * function returns a valid pointer.
 */
void
test_spinner_new(void)
{
    spinner_t* s = spinner_new(0);
    TEST_ASSERT_NOT_NULL(s);
    spinner_free(s);
}

/*
 * test_spinner_start verifies that the spinner
 * starts when start is called.
 */
void
test_spinner_start(void)
{
    spinner_t* s = spinner_new(0);
    spinner_start(s);
    int current_state = s->active;
    TEST_ASSERT_EQUAL_INT(s->active, current_state);
    spinner_free(s);
}

/*
 * test_spinner_stop verifies that the spinner
 * stops when stop is called.
 */
void
test_spinner_stop(void)
{
    spinner_t* s = spinner_new(0);
    spinner_start(s);
    spinner_stop(s);
    int current_state = s->active;
    TEST_ASSERT_EQUAL_INT(s->active, current_state);
    spinner_free(s);
}

/*
 * test_spinner_char_set_update verifies that a
 * new character set can be provided safely.
 */
void
test_spinner_char_set_update(void)
{
    spinner_t* s = spinner_new(0);
    spinner_char_set_update(s, 1);
    TEST_ASSERT_EQUAL_UINT8(s->char_set_id, 1);
    spinner_free(s);
}

/*
 * test_spinner_update_speed verifies that the
 * speed of the spinner can be updates safely.
 */
void
test_spinner_update_speed(void)
{
    spinner_t* s = spinner_new(0);
    s->delay = 100000;
    uint64_t expected = 200000;
    spinner_update_speed(s, expected);
    TEST_ASSERT_EQUAL_UINT8(s->delay, expected);
    spinner_free(s);
}

int
main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_spinner_new);
    RUN_TEST(test_spinner_start);
    RUN_TEST(test_spinner_stop);
    RUN_TEST(test_spinner_char_set_update);
    RUN_TEST(test_spinner_update_speed);

    return UNITY_END();
}
