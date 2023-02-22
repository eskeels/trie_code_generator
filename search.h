#pragma once
typedef char CharT;
typedef int (MatchFunction)(const CharT * pStart, const CharT * resultString, const CharT * matchStart, const CharT * matchEnd, const char * dictionaryName, int16_t score, bool distinct, bool caseSensitive, void * data);
const CharT * search(const CharT * pStart, const CharT * pbuff, MatchFunction match, void * data = NULL);
