#pragma once
#include <cstdint>
#include <list>
#include <map>
#include <unordered_set>

inline CharT fastTolower(CharT c) {
    return tolower(c);
}

inline bool fastIsSpace(CharT c) {
    return (isspace(c)!=0);
}

class DictionaryMatches {
public:
    DictionaryMatches(const std::string& name, int16_t score, const CharT* start, const CharT* end) {
        _name = name;
        addMatch(score, start, end);
    }

    void addMatch(int16_t score, const CharT* start, const CharT* end) {
        _matches.push_back(std::make_pair(start, end));
        _score += score;
    }

    void dump(const CharT* bufferStart, const CharT* bufferEnd, int snipSize) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
        std::cout << _name << " Score: " << _score << std::endl;
        for (auto it : _matches) {
            auto matchStart = it.first;
            auto matchEnd = it.second;
            matchStart -= snipSize;
            matchEnd += snipSize;

            if (matchStart < bufferStart) {
                matchStart = bufferStart;
            }

            if (matchEnd > bufferEnd) {
                matchEnd = bufferEnd;
            }
            
            std::u32string match(matchStart, matchEnd);
            std::string utf8Match = cvt.to_bytes(match);
            std::cout << "..." << utf8Match << "..." << std::endl;
        }
    }

protected:
    std::string _name;
    std::list<std::pair<const CharT*, const CharT*> > _matches;
    int32_t _score = 0;
};

class ScanResult {
public:
	ScanResult() {
	}

	void setMaxMatches(uint32_t v) {
		_maxMatches = v;
	}

	void setMatchSuffix(uint8_t v) {
		_matchSuffix = v;
	}

    bool caseCheck(bool caseSensitive, 
                   const CharT* resultString,
                   const CharT* matchStart,
                   const CharT* matchEnd) {
        if (caseSensitive) {
            if (0 != memcmp(matchStart, resultString, matchEnd - matchStart)) {
                return false;
            }
        }
        return true;
    }

    bool wholeWordCheck(bool wholeWord,
                        const CharT* pStart,
                        const CharT* pEnd,
                        const CharT* matchStart,
                        const CharT* matchEnd) {
        if (wholeWord) {
            // check if start of buffer
            if (matchStart > pStart) {
                if (!fastIsSpace(*(matchStart - 1))) {
                    // preceding char not a space
                    return false;
                }
            }
            // check if end of buffer
            if (matchEnd < pEnd - 1) {
                if (!fastIsSpace(*matchEnd)) {
                    // successive char not a space
                    return false;
                }
            }
        }
        return true;
    }

    bool distinctCheck(bool distinct,
                       bool caseSensitive,
                       const CharT* matchStart,
                       const CharT* matchEnd,
                       const char* dictionaryName) {
        if (distinct) {
            // NB this distinct key could be generated
            // by the Trie
            std::u32string distinctMatch(1, U'.');
            auto pd = dictionaryName;
            while (*pd)
                distinctMatch.append(1, (char32_t)*pd++);

            distinctMatch.append(1, U'.');

            auto pm = matchStart;
            if (caseSensitive) {
                while (pm < matchEnd)
                    distinctMatch.append(1, *pm++);
            } else {
                while (pm < matchEnd)
                    distinctMatch.append(1, tolower(*pm++));
            }
            auto it = _distinctMatches.find(distinctMatch);
            if (it != _distinctMatches.end()) {
                return false;
            }
            _distinctMatches.insert(distinctMatch);
        }
        return true;
    }

    bool maxMatchCountCheck() {
        if (_matchCount >= _maxMatches) {
            return false;
        }
        
        ++_matchCount;
        return true;
    }

	int match(const CharT* pStart,
			  const CharT* pEnd,
			  const CharT* resultString,
			  const CharT* matchStart,
			  const CharT* matchEnd,
			  const char* dictionaryName,
			  int16_t score,
			  bool distinct,
			  bool caseSensitive,
			  bool wholeWord) {

        if (!caseCheck(caseSensitive, resultString, matchStart, matchEnd)) {
            return 0;
        }

        if (!wholeWordCheck(wholeWord, pStart, pEnd, matchStart, matchEnd)) {
            return 0;
        }

        if (!distinctCheck(distinct, caseSensitive, matchStart, matchEnd,  dictionaryName)) {
            return 0;
        }

        if (!maxMatchCountCheck()) {
            return 0;
        }

        addMatch(dictionaryName, score, matchStart, matchEnd);
               
        return 0;
	}

    void addMatch(const std::string& dictionaryName, int16_t score, const CharT* matchStart, const CharT* matchEnd) {
        auto it = _dictionaryMatches.find(dictionaryName);
        if (it != _dictionaryMatches.end()) {
            it->second.addMatch(score, matchStart, matchEnd);
        } else {
            _dictionaryMatches.insert(std::make_pair(dictionaryName, DictionaryMatches(dictionaryName, score, matchStart, matchEnd)));
        }
    }

    void dump(const CharT* bufferStart, const CharT* bufferEnd, int snipSize) {
        for (auto it : _dictionaryMatches) {
            it.second.dump(bufferStart, bufferEnd, snipSize);
        }
    }
protected:
	uint32_t _maxMatches = 1000;
	uint8_t _matchSuffix = 10;
    uint32_t _matchCount = 0;
    std::map < std::string, DictionaryMatches > _dictionaryMatches;
    std::unordered_set< std::u32string> _distinctMatches;
};