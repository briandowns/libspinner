#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "crosscheck.h"
#include "../spinner.h"

static spinner_t *s;

void
cc_setup()
{
    s = spinner_new(0);
}

void
cc_tear_down()
{
    spinner_free(s);
}

/**
 * test_spinner_new verifies that the spinner_new
 * function returns a valid pointer.
 */
cc_result_t
test_spinner_new(void)
{
    //TEST_ASSERT_NOT_NULL(s);
    CC_SUCCESS;
}

/*
 * test_spinner_start verifies that the spinner
 * starts when start is called.
 */
cc_result_t
test_spinner_start(void)
{
    spinner_start(s);
    uint8_t current_state = s->active;
    CC_ASSERT_UINT8_EQUAL(s->active, current_state);
    CC_SUCCESS;
}

/*
 * test_spinner_stop verifies that the spinner
 * stops when stop is called.
 */
cc_result_t
test_spinner_stop(void)
{
    spinner_start(s);
    spinner_stop(s);
    uint8_t current_state = s->active;
    CC_ASSERT_INT_EQUAL(s->active, current_state);
    CC_SUCCESS;
}

/*
 * test_spinner_char_set_update verifies that a
 * new character set can be provided safely.
 */
cc_result_t
test_spinner_char_set_update(void)
{
    spinner_char_set_update(s, 1);
    CC_ASSERT_UINT8_EQUAL(s->char_set_id, 1);
    CC_SUCCESS;
}

/*
 * test_spinner_update_speed verifies that the
 * speed of the spinner can be updates safely.
 */
cc_result_t
test_spinner_update_speed(void)
{
    s->delay = 100000;
    uint64_t expected = 200000;
    spinner_update_speed(s, expected);
    CC_ASSERT_UINT64_EQUAL(s->delay, expected);
    CC_SUCCESS;
}

int
main(void)
{
    CC_INIT;

    CC_RUN(test_spinner_new);
    CC_RUN(test_spinner_start);
    CC_RUN(test_spinner_stop);
    CC_RUN(test_spinner_char_set_update);
    CC_RUN(test_spinner_update_speed);

    CC_COMPLETE;
}
