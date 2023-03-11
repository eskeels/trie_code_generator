#pragma once
#include <cctype>
typedef char32_t CharT;
#include "scan_result.h"
const CharT * search(const CharT * pStart, const CharT * pEnd, const CharT * pbuff, ScanResult& result);
