/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2024 Brian J. Downs
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

#ifndef _SPINNER_H
#define _SPINNER_H

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define ERR_CREATING_THREAD 1

/*
 * spinner_t maintains the state of the spinner
 * and allows for control. The library methods 
 * should be used to update values on this data
 * to avoid potential races. This memory needs
 * to be freed by the caller.
 */
typedef struct
{
    uint8_t char_set_id;
    uint64_t delay;
    pthread_mutex_t mu;
    FILE* output_dst;
    char* prefix;
    char* suffix;
    char* final_msg;
    char* last_output;
    uint8_t active;
    uint8_t reversed;
} spinner_t;

/*
 * spinner_new creates a new pointer to a spinner_t
 * struct and sets sane defaults for immediate use.
 */
spinner_t*
spinner_new(const int id);

/*
 * spinner_free frees the used memory of the
 * spinner_t pointer.
 */
void
spinner_free(spinner_t *s);

/*
 * spinner_state safely checks the state of the
 * spinner by aquiring a lock and returning the
 * current state.
 */
static uint8_t
spinner_state(spinner_t *s);

/*
 * spin is run in a pthread and is responsible for
 * iterating across the selected character set and
 * printing the character to screen.
 */
static void*
spin(void *arg);

/*
 * spinner_start starts the spinner.
 */
const uint8_t
spinner_start(spinner_t *s);

/*
 * spinner_stop stops the spinner.
 */
void
spinner_stop(spinner_t *s);

/*
 * spinner_restart will restart the spinner.
 */
void
spinner_restart(spinner_t *s);

/*
 * spinner_char_set_update updates the character
 * set with the new given one.
 */
void
spinner_char_set_update(spinner_t *s, const int id);

/*
 * spinner_update_speed updates the speed at which
 * the spinner is spinning.
 */
void
spinner_update_speed(spinner_t *s, const uint64_t delay);

/*
 * spinner_set_output_dest sets the file descriptor to
 * write spinner output to.
 */
void
spinner_set_output_dest(spinner_t *s, FILE *fd);

/*
 * spinner_reverse reverses the direction of the spinner.
 */
void
spinner_reverse(spinner_t *s);

#endif /** end _SPINNER_H */
