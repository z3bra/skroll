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
static float delay = 0.1;   /* scroll speed, in seconds */
static int number = 20;     /* number of chars to be shown at the same time */

/* scroll <input> to stdout */
void skroll (const char *input)
{
    int offset, buflen = 0;
    char *buf = NULL;

    buflen = strnlen( input, BUFFER_SIZE ) + ( number * 2 ) + 1;

    buf = calloc( buflen, sizeof( char ) );

    if ( buf == NULL )
    {
        return;
    }

    /* initialize memory with "spaces" and null-terminate the buffer */
    memset( buf, 0x20, buflen - 1 );
    buf[ buflen - 1 ] = 0;

    /* copy input at place `number` */
    memcpy( buf + number, input, strlen( input ) );

    /* remove \n from input string to sanitize output */
    if ( buf[ number + strlen( input ) - 1 ] == '\n' )
    {
        buf[ number + strlen( input ) - 1 ] = 0x20; 
    }

    /* main loop. will loop forever if run with -l */
    do {

        offset = 0;

        /* loop executed on each step, after <delay> seconds */
        while ( buf [ offset + number ] != 0 ) {

            /* print out `number` characters from the buffer ! */
            putc( '\r', stdout );
            write( 1, buf + offset, number );

            /* if we want a new line, let's do it here */
            if (newline) putc('\n', stdout);

            /* flush stdout, and wait for the next step */
            fflush(stdout);
            usleep(delay*1000000);
            offset++;
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
    if ( !(buf = calloc (BUFFER_SIZE + 1, sizeof(char))) ) return NULL;
    /* buf[BUFFER_SIZE] = 0; */
    memset( buf, 0, BUFFER_SIZE + 1 );

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

    /* End with a new line, no matter what */
    putc('\n', stdout);

    return 0;
}
