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
#include <limits.h>

static bool newline = false;/* print a new line after each step */
static bool loop = false;   /* wether to loop text or not */
static float delay = 0.1;   /* scroll speed, in seconds */
static int number = 20;     /* number of chars to be shown at the same time */

/* scroll <input> to stdout */
void skroll (const char *input)
{
    int offset, buflen = 0;
    char *tmp, *buf = NULL;

    /* 
     * allocate a new buffer to hold our input PLUS number spaces at the
     * start/end of the string
     */ 
    buflen = strnlen(input, LINE_MAX) + (number * 2) + 1;
    if ((buf = calloc(buflen, sizeof(char))) == NULL)
    {
        return;
    }

    /* initialize memory with "spaces" and null-terminate the buffer */
    memset(buf, ' ', buflen - 1);
    buf[buflen - 1] = 0;

    /* copy input at place `number` */
    memcpy(buf + number, input, strlen(input));

    /* remove \n from input string to sanitize output */
    if ((tmp = strchr(buf, '\n')) != NULL)
    {
        tmp[0] = ' '; 
    }

    /* main loop. will loop forever if run with -l */
    do
    {
        /*
         * each step of the loop will print the buffer, one byte further after each step.
         * using a carriage return, it makes the text scroll out. Magic..
         * leading/ending spaces are here to make sure that the text goes from
         * far right, and goes all the way to far left
         */
        for (offset = 0; buf [offset + number] != 0; offset++)
        {

            /* print out `number` characters from the buffer ! */
            putc('\r', stdout);
            write(1, buf + offset, number);

            /* if we want a new line, let's do it here */
            if (newline) putc('\n', stdout);

            /* flush stdout, and wait for the next step */
            fflush(stdout);
            usleep(delay*1000000);
        }
    /* magnolia ? FOWEVA ! */
    } while(loop);

    return; /* void */
}

/* returns a char that contains the input bufferized */
const char *bufferize (FILE *stream)
{
    char *buf = NULL;

    /* allocate space to store the input */
    if (!(buf = calloc (LINE_MAX + 1, sizeof(char)))) { return NULL; }
    /* buf[LINE_MAX] = 0; */
    memset(buf, 0, LINE_MAX + 1);

    /* OMG, NO MORE SPACE LEFT ON DEVICE (or no more input, in fact) */
    if (feof(stream) || !fgets(buf, LINE_MAX, stream))
    {
        free (buf);
        return NULL;
    }

    return buf;
}

int main (int argc, char **argv)
{
    char ch;
    const char *buf = NULL;

    while ((ch = getopt(argc, argv, "hd:ln:r")) != -1)
    {
        switch (ch)
        {
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
    while((buf = bufferize(stdin)) != NULL)
    {
        skroll(buf);
    }

    /* End with a new line, no matter what */
    putc('\n', stdout);

    return 0;
}
