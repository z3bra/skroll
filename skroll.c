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

static bool newline = false;/* print a new line after each step */
static bool loop = false;   /* wether to loop text or not */
static float delay = 1;     /* scroll speed, in usec */
static int number = 10;     /* number of chars to be shown at the same time */

/* fills a string with spaces if it has less than <num> chars */
void zero_fill (char **str, size_t num)
{
    int c;
    size_t s;

    s = strnlen((*str),num);

    /* check every char of the buffer */
    for (c=0; c<num; ++c) {
        /* if the char is a new line, or if the string is to small */
        if ( (*str)[c] == '\n' || c >= s ) {
            /* replace current char with a space */
            (*str)[c] = ' ';
        }
    }

    /* double-check your pointers */
    (*str)[num] = 0;

    return; /* void */
}

/* scroll <input> to stdout */
void skroll (const char *input)
{
    int offset = 0;
    char *buf = NULL;

    if ( !(buf = calloc (number, sizeof(char))) ) return;
    buf[number] = 0;

    do {
        for (offset=0; input[offset]; ++offset) {
            /* copy the number of char we want to the buffer */
            strncpy(buf, input + offset, number-1);

            /* fill missing chars with spaces */
            zero_fill(&buf, number);

            /* print out the buffer ! */
            printf("\r%s", buf);

            /* if we want a new line, let's do it here */
            if (newline) putc('\n', stdout);

            /* flush stdout, and wait for the next step */
            fflush(stdout);
            usleep(delay*1000000);
        }
    /* magnolia ? FOWEVA ! */
    } while(loop);

    /* And with a new line, no matter what */
    putc('\n', stdout);

    return; /* void */
}

/* returns a char that contains the input bufferized */
const char *bufferize (FILE *stream)
{
    char *buf = NULL;

    /* allocate space to store the input */
    if ( !(buf = calloc (BUFFER_SIZE, sizeof(char))) ) return NULL;
    buf[BUFFER_SIZE] = 0;

    /* OMG, NO MORE SPACE LEFT ON DEVICE (or no more input, in fact) */
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

    while ( (ch = getopt(argc, argv, "hd:ln:r")) != -1 ) {
        switch (ch) {
            case 'h':
                printf("usage: %s [-hlr] [-d delay] [-n number]\n", argv[0]);
                exit(0);
                break;
            case 'd': delay = strtof(optarg, NULL); break;
            case 'n': number = strtoul(optarg, NULL, 10); break;
            case 'l': loop = true; break;
            case 'r': newline = true; break;
        }
    }

    /* SCROLL ALL THE TEXT! */
    while( (buf = bufferize(stdin)) != NULL ) {
        skroll(buf);
    }

    return 0;
}
