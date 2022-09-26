/*
 * lone.c
 * Utilities for the rest of lone
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

/* from main.c */
extern char *arg0;

void /* General error printing routine */
lerror(const char *format, ...)
{
        va_list args;

        va_start(args, format);

        fprintf(stderr, "%s: error: ", arg0);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");

        va_end(args);
}

void /* Fatal error */
lfatal(const char *format, ...)
{
        va_list args;

        va_start(args, format);

        /* FATAL in red */
        fprintf(stderr, "%s: \x1b[31mFATAL:\x1b[0m ", arg0);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");

        va_end(args);

        exit(1);
}

void * /* Easy allocation with null check */
lalloc(size_t s)
{
        void *p;

        p = calloc(1, s);

        if (p == NULL)
                lfatal("Failure to allocate memory: %s", strerror(errno));

        return p;
}