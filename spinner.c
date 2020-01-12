/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Brian J. Downs
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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "spinner.h"

/*
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
    fflush(stdout);

char* char_sets[][MAX_CHARS] = {
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
    { "▌", "▀", "▐▄" }
};

spinner_t*
spinner_new(const int id)
{
    spinner_t* s = malloc(sizeof(spinner_t));
    s->char_set_id = id;
    s->output_dst = stdout;
    s->prefix = "";
    s->suffix = "";
    s->final_msg = "";
    s->active = 0;
    return s;
}

void
spinner_free(spinner_t *s)
{
    if (s) {
        free(s);
    }
    return;
}

/*
 * spinner_state safely checks the state of the
 * spinner by aquiring a lock and returning the
 * current state.
 */
static int
spinner_state(spinner_t *s)
{
    int state;
    pthread_mutex_lock(&s->mu);
    state = s->active;
    pthread_mutex_unlock(&s->mu);
    return state;
}

/*
 * spin is run in a pthread and is responsible for
 * iterating across the selected character set and
 * printing the character to screen.
 */
static void*
spin(void *arg)
{
    spinner_t *s = (spinner_t*)arg;
    if (s->reversed == 1) {
    }
    for (int i = 0;; i++) {
        // check if we're reached an index with no string. If
        // we have, reset the counter and start again.
        if (!char_sets[s->char_set_id][i]) {
            i = -1;
            continue;
        }
        char output[MAX_CHARS * 4];
        sprintf(output, "\r%s%s%s", s->prefix, char_sets[s->char_set_id][i], s->suffix);
        s->last_output = output;
        fprintf(s->output_dst, "%s", output);
        fflush(stdout);
        printf("%c[2K", 27);
        usleep(s->delay);
    }
    return NULL;
}

void
spinner_start(spinner_t *s)
{
    if (s->active > 0) {
        return;
    }
    pthread_mutex_lock(&s->mu);
    CURSOR_STATE(0);
    pthread_t spin_thread;
    pthread_mutex_unlock(&s->mu);
    if (pthread_create(&spin_thread, NULL, spin, s)) {
        fprintf(stderr, "error creating thread\n");
        return;
    }
    s->active = 1;
}

void
spinner_stop(spinner_t *s)
{
    pthread_mutex_lock(&s->mu);
    s->active = 0;
    pthread_mutex_unlock(&s->mu);
    if (strlen(s->final_msg) > 0) {
        printf("%s", s->final_msg);
    }
    CURSOR_STATE(1);
}

void
spinner_restart(spinner_t *s)
{
    spinner_stop(s);
    spinner_start(s);
}

void
spinner_char_set_update(spinner_t *s, const int id)
{
    pthread_mutex_lock(&s->mu);
    s->char_set_id = id;
    pthread_mutex_unlock(&s->mu);
}

void
spinner_update_speed(spinner_t *s, const uint64_t delay)
{
    pthread_mutex_lock(&s->mu);
    s->delay = delay;
    pthread_mutex_unlock(&s->mu);
}

void
spinner_set_output_dest(spinner_t *s, FILE *fd)
{
    pthread_mutex_lock(&s->mu);
    s->output_dst = fd;
    pthread_mutex_unlock(&s->mu);
}

void
spinner_reverse(spinner_t *s)
{
    pthread_mutex_lock(&s->mu);
    if (s->reversed == 0) {
        s->reversed = 1;
    }
    size_t n = sizeof(char_sets[s->char_set_id]) / sizeof(char_sets[s->char_set_id][0]) - 1;
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
