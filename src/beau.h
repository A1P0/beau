/*
 * beau.h
 * Definitions of utilities for the beau compiler
 */
#ifndef _BEAU_H_
#define _BEAU_H_

void berror(const char *format, ...);

void bfatal(const char *format, ...);

void * balloc(size_t s);

#endif