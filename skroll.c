/*
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define BUFFER_SIZE 512

static bool loop = false;   /* wether to loop text or not */
static float delay = 1;     /* scroll speed, in usec */
static int number = 10;     /* number of chars to be shown at the same time */

void zero_fill (char **str, size_t num)
{
    int c;
    size_t s;

    s = strnlen((*str),num);

    for (c=0; c<num; ++c) {
        if ( (*str)[c] == '\n' || c >= s ) {
           (*str)[c] = ' ';
        }
    }

    (*str)[num] = 0;

    return; /* void */
}

void skroll (const char *input)
{
    int offset = 0;
    char *buf = NULL;

    if ( !(buf = calloc (number, sizeof(char))) ) return;
    buf[number] = 0;

    do {
        for (offset=0; input[offset]; ++offset) {
            strncpy(buf, input + offset, number-1);
            zero_fill(&buf, number);
            printf("\r%s", buf);

            fflush(stdout);
            usleep(delay*1000000);
        }
    } while(loop);

    putc('\n', stdout);

    return; /* void */
}

const char *bufferize (FILE *stream)
{
    char *buf = NULL;

    if ( !(buf = calloc (BUFFER_SIZE, sizeof(char))) ) return NULL;

    buf[BUFFER_SIZE] = 0;

    if ( feof(stream) || !fgets(buf, BUFFER_SIZE, stream) ) {
        free (buf);
        return NULL;
    }

    return buf;
}

int main (int argc, char **argv)
{
    char ch;
    const char *buf = NULL;

    while ( (ch = getopt(argc, argv, "hd:ln:")) != -1 ) {
        switch (ch) {
            case 'h':
                printf("usage: %s [-hl] [-d delay] [-n number]", argv[0]);
                break;
            case 'd': delay = strtof(optarg, NULL); break;
            case 'n': number = strtoul(optarg, NULL, 10); break;
            case 'l': loop = true; break;
        }
    }

    while( (buf = bufferize(stdin)) != NULL ) {
        skroll(buf);
    }

    return 0;
}
