#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define MSG(f, ...) do {  \
        printf("MSG: %s: %s: %d: ", __FILE__, __func__, __LINE__); \
        printf(f, ## __VA_ARGS__); \
        printf("\n"); } while(0)
#define ENTER       do { \
        printf("ENTER:  %s: %s: %d\n", __FILE__, __func__, __LINE__); } while(0)
#define RET printf("RETURN: %s: %s: %d\n", __FILE__, __func__, __LINE__); return

#endif