#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>
#include <stdio.h>

void createStrStore();
void destroyStrStore();

size_t addStr(const char* str);
void deleteStr(size_t index);
const char* getStr(size_t index);
size_t getStrLen(size_t index);
int compareStr(size_t index, const char* str);
size_t arithAddStr(size_t left, size_t right);

void saveStrStore(FILE* fp);
void loadStrStore(FILE* fp);

void dumpStrStore();

#endif