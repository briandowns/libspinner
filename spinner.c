/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2026 Brian J. Downs
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
#define _DEFAULT_SOURCE

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "spinner.h"

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

spinner_t*
spinner_new(const int id)
{
    spinner_t *s = (spinner_t*)calloc(1, sizeof(spinner_t));
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
}

/**
 * spinner_state safely checks the state of the
 * spinner by aquiring a lock and returning the
 * current state.
 */
static uint8_t
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
static void*
spin(void *arg)
{
  spinner_t *s = (spinner_t*)arg;
  int i = 0;

  // loop only while marked as active
  while (spinner_state(s) > 0) {
    if (!char_sets[s->char_set_id][i]) {
      i = 0;
      continue;
    }

    char output[MAX_CHARS * 4];

    pthread_mutex_lock(&s->mu); // mutex data race protection
    sprintf(output, "\r%s%s%s", s->prefix, char_sets[s->char_set_id][i], s->suffix);
    FILE *dest = s->output_dst;
    uint64_t delay = s->delay;
    pthread_mutex_unlock(&s->mu); // mutex data race protection

    fprintf(dest, "%s", output);
    fflush(dest);
    fprintf(dest, "\33[2K\r");

    usleep(delay);
    i++;
  }

  pthread_exit(0);
  return NULL;
}

const uint8_t
spinner_start(spinner_t *s)
{
  if (spinner_state(s) > 0) {
    return 0;
  }

  pthread_mutex_lock(&s->mu); // mutex data race protection
  CURSOR_STATE(0);
  s->active = 1; // sets spinner as active
  pthread_mutex_unlock(&s->mu); // mutex data race protection

  if (pthread_create(&s->thread, NULL, spin, s)) { // newly spawned thread gets tracked by the struct
    pthread_mutex_lock(&s->mu); // mutex data race protection
    s->active = 0;
    pthread_mutex_unlock(&s->mu); // mutex data race protection
    return ERR_CREATING_THREAD;
  }

  return 0;
}

void
spinner_stop(spinner_t *s)
{
  pthread_mutex_lock(&s->mu); // mutex protection
  if (s->active == 0) { // check if not active
    pthread_mutex_unlock(&s->mu);
    return;
  }
  s->active = 0; // sets the spinner as non active
  pthread_mutex_unlock(&s->mu);

  // wait on the thread to end
  pthread_join(s->thread, NULL);

  if (s->final_msg[0] != '\0') {
    fprintf(s->output_dst, "%s", s->final_msg);
    fflush(s->output_dst);

    if (s->output_dst != stdout) {
      fclose(s->output_dst);
    }
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
