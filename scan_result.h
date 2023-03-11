#pragma once
#include <cstdint>
#include <list>
#include <map>

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

    void dump(const CharT* bufferStart, const CharT* bufferEnd) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
        std::cout << _name << " Score: " << _score << std::endl;
        for (auto it : _matches) {
            auto matchStart = it.first;
            auto matchEnd = it.second;
            matchStart -= 3;
            matchEnd += 3;

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
       // std::wstring tmp((const wchar_t*)matchStart, (const wchar_t*)matchEnd);
       // std::wcout << L"Found word [" << tmp << L"] ";

        if (caseSensitive) {
            if (0 != memcmp(matchStart, resultString, matchEnd - matchStart)) {
                return 0;
            }
        }

        if (wholeWord) {
            // check if start of buffer
            if (matchStart > pStart) {
                if (!isspace(*(matchStart - 1))) {
                    // preceding char not a space
                    return 0;
                }
            }
            // check if end of buffer
            if (matchEnd < pEnd - 1) {
                if (!isspace(*matchEnd)) {
                    // successive char not a space
                    return 0;
                }
            }
        }
        

        if (pStart && matchEnd) {
            std::wcout << " column :" << std::distance(pStart, matchStart) << std::endl;
            std::wcout << "Length " << std::distance(matchStart, matchEnd) << std::endl;
        }

        if (distinct) {
            std::u32string match;
            auto pd = dictionaryName;
            while (*pd) {
                match.append(1, (char32_t)*pd);
            }
            
            auto pm = matchStart;
            if (!caseSensitive) {
                while (pm < matchEnd)
                    match.append(1, tolower(*pm));
            } else {
                while (pm < matchEnd)
                    match.append(1, *pm);
            }
          
        }
        
        if (_matchCount < _maxMatches) {
            ++_matchCount;
            addMatch(dictionaryName, score, matchStart, matchEnd);
        }
        
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

    void dump(const CharT* bufferStart, const CharT* bufferEnd) {
        for (auto it : _dictionaryMatches) {
            it.second.dump(bufferStart, bufferEnd);
        }
    }
protected:
	uint32_t _maxMatches = 1000;
	uint8_t _matchSuffix = 10;
    uint32_t _matchCount = 0;
    std::map < std::string, DictionaryMatches > _dictionaryMatches;
};