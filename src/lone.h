/*
 * lone.h
 * Definitions of utilities for lone
 */
#ifndef _LONE_H_
#define _LONE_H_

void lerror(const char *format, ...);

void lfatal(const char *format, ...);

void * lalloc(size_t s);

#endif