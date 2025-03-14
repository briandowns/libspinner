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

#ifndef _SPINNER_H
#define _SPINNER_H

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define ERR_CREATING_THREAD 1

/**
 * maximun number of characters in an array of indicators.
 */
#define MAX_CHARS 1024

/**
 * toggle the cursor on and off.
 */
#define CURSOR_STATE(x)        \
    switch (x) {               \
        case 0:                \
            printf("\e[?25l"); \
            break;             \
        case 1:                \
            printf("\e[?25h"); \
            break;             \
    }                          \
    fflush(s->output_dst);

    /**
 * char_sets is the collection of spinners.
 */
static char *char_sets[][MAX_CHARS] = {
    { "←", "↖", "↑", "↗", "→", "↘", "↓", "↙" },
    { "▁", "▃", "▄", "▅", "▆", "▇", "█", "▇", "▆", "▅", "▄", "▃", "▁" },
    { "▖", "▘", "▝", "▗" },
    { "┤", "┘", "┴", "└", "├", "┌", "┬", "┐" },
    { "◢", "◣", "◤", "◥" },
    { "◰", "◳", "◲", "◱" },
    { "◴", "◷", "◶", "◵" },
    { "◐", "◓", "◑", "◒" },
    { ".", "o", "O", "@", "*" },
    { "|", "/", "-", "\\" },
    { "◡◡", "⊙⊙", "◠◠" },
    { "⣾", "⣽", "⣻", "⢿", "⡿", "⣟", "⣯", "⣷" },
    { ">))'>", " >))'>", "  >))'>", "   >))'>", "    >))'>", "   <'((<", "  <'((<", " <'((<" },
    { "⠁", "⠂", "⠄", "⡀", "⢀", "⠠", "⠐", "⠈" },
    { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" },
    { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
      "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z" },
    { "▉", "▊", "▋", "▌", "▍", "▎", "▏", "▎", "▍", "▌", "▋", "▊", "▉" },
    { "■", "□", "▪", "▫" },
    { "←", "↑", "→", "↓" },
    { "╫", "╪" },
    { "⇐", "⇖", "⇑", "⇗", "⇒", "⇘", "⇓", "⇙" },
    { "⠁", "⠁", "⠉", "⠙", "⠚", "⠒", "⠂", "⠂", "⠒", "⠲", "⠴", "⠤", "⠄", "⠄", "⠤",
      "⠠", "⠠", "⠤", "⠦", "⠖", "⠒", "⠐", "⠐", "⠒", "⠓", "⠋", "⠉", "⠈", "⠈" },
    { "⠈", "⠉", "⠋", "⠓", "⠒", "⠐", "⠐", "⠒", "⠖", "⠦", "⠤", "⠠",
      "⠠", "⠤", "⠦", "⠖", "⠒", "⠐", "⠐", "⠒", "⠓", "⠋", "⠉", "⠈" },
    { "⠁", "⠉", "⠙", "⠚", "⠒", "⠂", "⠂", "⠒", "⠲", "⠴", "⠤", "⠄",
      "⠄", "⠤", "⠴", "⠲", "⠒", "⠂", "⠂", "⠒", "⠚", "⠙", "⠉", "⠁" },
    { "⠋", "⠙", "⠚", "⠒", "⠂", "⠂", "⠒", "⠲", "⠴", "⠦", "⠖", "⠒", "⠐", "⠐", "⠒", "⠓", "⠋" },
    { "ｦ", "ｧ", "ｨ", "ｩ", "ｪ", "ｫ", "ｬ", "ｭ", "ｮ", "ｯ", "ｱ", "ｲ", "ｳ", "ｴ", "ｵ", "ｶ", "ｷ", "ｸ", "ｹ",
      "ｺ", "ｻ", "ｼ", "ｽ", "ｾ", "ｿ", "ﾀ", "ﾁ", "ﾂ", "ﾃ", "ﾄ", "ﾅ", "ﾆ", "ﾇ", "ﾈ", "ﾉ", "ﾊ", "ﾋ", "ﾌ",
      "ﾍ", "ﾎ", "ﾏ", "ﾐ", "ﾑ", "ﾒ", "ﾓ", "ﾔ", "ﾕ", "ﾖ", "ﾗ", "ﾘ", "ﾙ", "ﾚ", "ﾛ", "ﾜ", "ﾝ" },
    { ".", "..", "..." },
    { "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", "▉", "▊", "▋", "▌", "▍", "▎", "▏",
      "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█", "▇", "▆", "▅", "▄", "▃", "▂", "▁" },
    { ".", "o", "O", "°", "O", "o", "." },
    { "+", "x" },
    { "v", "<", "^", ">" },
    { ">>--->",
      " >>--->",
      "  >>--->",
      "   >>--->",
      "    >>--->",
      "    <---<<",
      "   <---<<",
      "  <---<<",
      " <---<<",
      "<---<<" },
    { "|",
      "||",
      "|||",
      "||||",
      "|||||",
      "|||||||",
      "||||||||",
      "|||||||",
      "||||||",
      "|||||",
      "||||",
      "|||",
      "||",
      "|" },
    { "[          ]",
      "[=         ]",
      "[==        ]",
      "[===       ]",
      "[====      ]",
      "[=====     ]",
      "[======    ]",
      "[=======   ]",
      "[========  ]",
      "[========= ]",
      "[==========]" },
    { "(*---------)",
      "(-*--------)",
      "(--*-------)",
      "(---*------)",
      "(----*-----)",
      "(-----*----)",
      "(------*---)",
      "(-------*--)",
      "(--------*-)",
      "(---------*)" },
    { "█▒▒▒▒▒▒▒▒▒", "███▒▒▒▒▒▒▒", "█████▒▒▒▒▒", "███████▒▒▒", "██████████" },
    { "[                    ]",
      "[=>                  ]",
      "[===>                ]",
      "[=====>              ]",
      "[======>             ]",
      "[========>           ]",
      "[==========>         ]",
      "[============>       ]",
      "[==============>     ]",
      "[================>   ]",
      "[==================> ]",
      "[===================>]" },
    { "🌍", "🌎", "🌏" },
    { "◜", "◝", "◞", "◟" },
    { "⬒", "⬔", "⬓", "⬕" },
    { "⬖", "⬘", "⬗", "⬙" },
    { "[>>>          >]",
      "[]>>>>        []",
      "[]  >>>>      []",
      "[]    >>>>    []",
      "[]      >>>>  []",
      "[]        >>>>[]",
      "[>>          >>]" },
    { "♠", "♣", "♥", "♦" },
    { "➞", "➟", "➠", "➡", "➠", "➟" },
    { "  |  ", " \\   ", "_    ", " \\   ", "  |  ", "   / ", "    _", "   / " },
    { "  . . . .", ".   . . .", ". .   . .", ". . .   .", ". . . .  ", ". . . . ." },
    { " |     ", "  /    ", "   _   ", "    \\  ", "     | ", "    \\  ", "   _   ", "  /    " },
    { "⎺", "⎻", "⎼", "⎽", "⎼", "⎻" },
    { "▹▹▹▹▹", "▸▹▹▹▹", "▹▸▹▹▹", "▹▹▸▹▹", "▹▹▹▸▹", "▹▹▹▹▸" },
    { "[    ]", "[   =]", "[  ==]", "[ ===]", "[====]", "[=== ]", "[==  ]", "[=   ]" },
    { "( ●    )", "(  ●   )", "(   ●  )", "(    ● )", "(     ●)", "(    ● )", "(   ●  )", "(  ●   )", "( ●    )" },
    { "✶", "✸", "✹", "✺", "✹", "✷" },
    { "▐|\\____________▌", "▐_|\\___________▌", "▐__|\\__________▌", "▐___|\\_________▌", "▐____|\\________▌",
      "▐_____|\\_______▌", "▐______|\\______▌", "▐_______|\\_____▌", "▐________|\\____▌", "▐_________|\\___▌",
      "▐__________|\\__▌", "▐___________|\\_▌", "▐____________|\\▌", "▐____________/|▌",  "▐___________/|_▌",
      "▐__________/|__▌",  "▐_________/|___▌",  "▐________/|____▌",  "▐_______/|_____▌",  "▐______/|______▌",
      "▐_____/|_______▌",  "▐____/|________▌",  "▐___/|_________▌",  "▐__/|__________▌",  "▐_/|___________▌",
      "▐/|____________▌" },
    { "▐⠂       ▌", "▐⠈       ▌", "▐ ⠂      ▌", "▐ ⠠      ▌", "▐  ⡀     ▌", "▐  ⠠     ▌", "▐   ⠂    ▌", "▐   ⠈    ▌",
      "▐    ⠂   ▌", "▐    ⠠   ▌", "▐     ⡀  ▌", "▐     ⠠  ▌", "▐      ⠂ ▌", "▐      ⠈ ▌", "▐       ⠂▌", "▐       ⠠▌",
      "▐       ⡀▌", "▐      ⠠ ▌", "▐      ⠂ ▌", "▐     ⠈  ▌", "▐     ⠂  ▌", "▐    ⠠   ▌", "▐    ⡀   ▌", "▐   ⠠    ▌",
      "▐   ⠂    ▌", "▐  ⠈     ▌", "▐  ⠂     ▌", "▐ ⠠      ▌", "▐ ⡀      ▌", "▐⠠       ▌" },
    { "?", "¿" },
    { "⢹", "⢺", "⢼", "⣸", "⣇", "⡧", "⡗", "⡏" },
    { "⢄", "⢂", "⢁", "⡁", "⡈", "⡐", "⡠" },
    { ".  ", ".. ", "...", " ..", "  .", "   " },
    { ".", "o", "O", "°", "O", "o", "." },
    { "▓", "▒", "░" },
    { "▌", "▀", "▐", "▄" },
    { "⊶", "⊷" },
    { "▪", "▫" },
    { "□", "■" },
    { "▮", "▯" },
    { "-", "=", "≡" },
    { "d", "q", "p", "b" },
    { "∙∙∙", "●∙∙", "∙●∙", "∙∙●", "∙∙∙" },
    { "🌑 ", "🌒 ", "🌓 ", "🌔 ", "🌕 ", "🌖 ", "🌗 ", "🌘 " },
    { "☗", "☖" },
    { "⧇", "⧆" },
    { "◉", "◎" },
    { "㊂", "㊀", "㊁" },
    { "⦾", "⦿" },
    { "ဝ", "၀" },
    { "▌", "▀", "▐", "▄" },
    {"⠈⠁", "⠈⠑", "⠈⠱", "⠈⡱", "⢀⡱", "⢄⡱", "⢄⡱", "⢆⡱", "⢎⡱", "⢎⡰", "⢎⡠", "⢎⡀", "⢎⠁", "⠎⠁", "⠊⠁"},
	  {"________", "-_______", "_-______", "__-_____", "___-____", "____-___", "_____-__", "______-_", "_______-", "________", "_______-", "______-_", "_____-__", "____-___", "___-____", "__-_____", "_-______", "-_______", "________"},
	  {"|_______", "_/______", "__-_____", "___\\____", "____|___", "_____/__", "______-_", "_______\\", "_______|", "______\\_", "_____-__", "____/___", "___|____", "__\\_____", "_-______"},
	  {"□", "◱", "◧", "▣", "■"},
	  {"□", "◱", "▨", "▩", "■"},
	  {"░", "▒", "▓", "█"},
	  {"░", "█"},
	  {"⚪", "⚫"},
	  {"◯", "⬤"},
	  {"▱", "▰"},
	  {"➊", "➋", "➌", "➍", "➎", "➏", "➐", "➑", "➒", "➓"},
	  {"½", "⅓", "⅔", "¼", "¾", "⅛", "⅜", "⅝", "⅞"},
	  {"↞", "↟", "↠", "↡"}
};

/**
 * spinner_t maintains the state of the spinnerand allows for control. The
 * library methods should be used to update values on this data to avoid
 * potential races. This memory needs to be freed by the caller.
 */
typedef struct
{
    uint8_t char_set_id;
    uint64_t delay;
    pthread_mutex_t mu;
    FILE *output_dst;
    char *prefix;
    char *suffix;
    char *final_msg;
    char *last_output;
    bool active;
    bool reversed;
} spinner_t;

/**
 * spinner_new creates a new pointer to a spinner_t struct and sets sane
 * defaults for immediate use.
 */
static inline spinner_t*
spinner_new(const int id)
{
    spinner_t *s = (spinner_t*)calloc(1, sizeof(spinner_t));
    s->char_set_id = id;
    s->output_dst = stdout;
    s->prefix = "";
    s->suffix = "";
    s->final_msg = "";
    s->active = false;

    return s;
}

/**
 * spinner_state safely checks the state of the
 * spinner by aquiring a lock and returning the
 * current state.
 */
static inline uint8_t
spinner_state(spinner_t *s)
{
    uint8_t state;

    pthread_mutex_lock(&s->mu);
    state = s->active;
    pthread_mutex_unlock(&s->mu);

    return state;
}
/**
 * spin is run in a pthread and is responsible for
 * iterating across the selected character set and
 * printing the character to screen.
 */
static inline void*
spin(void *arg)
{
    spinner_t *s = (spinner_t*)arg;
    if (s->reversed == true) {}

    for (int i = 0;; i++) {
        // check if we're reached an index with no string. If
        // we have, reset the counter and start again.
        if (!char_sets[s->char_set_id][i]) {
            i = -1;
            continue;
        }
    
        char output[MAX_CHARS * 4];
        sprintf(output, "\r%s%s%s",
            s->prefix, char_sets[s->char_set_id][i], s->suffix);
    
        fprintf(s->output_dst, "%s", output);
        fflush(s->output_dst);
        fprintf(s->output_dst, "\33[2K\r");

        if (s->output_dst != stdout) {
            fclose(s->output_dst);
        }
    
        usleep(s->delay);
    }

    pthread_exit(0);

    return NULL;
}

/**
 * spinner_free frees the used memory of the spinner_t pointer.
 */
static inline void
spinner_free(spinner_t *s)
{
    if (s != NULL) {
        if (s->prefix != NULL && s->prefix[0] != '\0') {
            free(s->prefix);
        }
        if (s->suffix != NULL && s->prefix[0] != '\0') {
            free(s->suffix);
        }
        if (s->final_msg != NULL && s->prefix[0] != '\0') {
            free(s->final_msg);
        }
        if (s->last_output != NULL && s->prefix[0] != '\0') {
            free(s->last_output);
        }
        free(s);
    }
}

/**
 * spinner_start starts the spinner.
 */
static inline const uint8_t
spinner_start(spinner_t *s)
{
    if (s->active) {
        return 0;
    }

    pthread_mutex_lock(&s->mu);
    CURSOR_STATE(0);
    pthread_t spin_thread;
    pthread_mutex_unlock(&s->mu);

    if (pthread_create(&spin_thread, NULL, spin, s)) {
        return ERR_CREATING_THREAD;
    }

    s->active = true;

    return 0;
}

/**
 * spinner_stop stops the spinner.
 */
static inline void
spinner_stop(spinner_t *s)
{
    pthread_mutex_lock(&s->mu);
    s->active = false;
    pthread_mutex_unlock(&s->mu);

    if (s->final_msg[0] != '\0') {
        fprintf(s->output_dst, "%s", s->final_msg);
        fflush(s->output_dst);

        if (s->output_dst != stdout) {
            fclose(s->output_dst);
        }
    }

    CURSOR_STATE(1);
}

/**
 * spinner_restart will restart the spinner.
 */
static inline void
spinner_restart(spinner_t *s)
{
    spinner_stop(s);
    spinner_start(s);
}

/**
 * spinner_char_set_update updates the character set with the new given one.
 */
static inline void
spinner_char_set_update(spinner_t *s, const int id)
{
    pthread_mutex_lock(&s->mu);
    s->char_set_id = id;
    pthread_mutex_unlock(&s->mu);
}

/**
 * spinner_update_speed updates the speed at which the spinner is spinning.
 */
static inline void
spinner_update_speed(spinner_t *s, const uint64_t delay)
{
    pthread_mutex_lock(&s->mu);
    s->delay = delay;
    pthread_mutex_unlock(&s->mu);
}

/**
 * spinner_set_output_dest sets the file descriptor to write spinner output to.
 */
static inline void
spinner_set_output_dest(spinner_t *s, FILE *fd)
{
    pthread_mutex_lock(&s->mu);
    s->output_dst = fd;
    pthread_mutex_unlock(&s->mu);
}

/**
 * spinner_reverse reverses the direction of the spinner.
 */
static inline void
spinner_reverse(spinner_t *s)
{
    pthread_mutex_lock(&s->mu);

    if (!s->reversed) {
        s->reversed = true;
    }

    size_t n = sizeof(char_sets[s->char_set_id]) / 
        sizeof(char_sets[s->char_set_id][0]) - 1;
    int j = 0;
    
    while (n > j) {
        if (char_sets[s->char_set_id][n] == NULL) {
            n--;
            j++;
            continue;
        }

        char* temp = char_sets[s->char_set_id][n];
        char_sets[s->char_set_id][n] = char_sets[s->char_set_id][j];
        char_sets[s->char_set_id][j] = temp;
        n--;
        j++;
    }

    pthread_mutex_unlock(&s->mu);
    spinner_restart(s);
}

#endif /** end _SPINNER_H */
#ifdef __cplusplus
}
#endif
