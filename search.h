#pragma once
typedef char CharT;
typedef int (CallbackFunction)(const CharT * pStart, const CharT * pbuff, const CharT * resultString, const CharT * position, const char * dictionaryName, int16_t score, bool distinct, void * data);
const CharT * search(const CharT * pStart, const CharT * pbuff, CallbackFunction match, void * data = NULL);
