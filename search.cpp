const CharT* search(const CharT * pStart, const CharT * pEnd, const CharT * pbuff, ScanResult& result) 
{
  const CharT * p = pbuff;
  const CharT * pRet = NULL;
  const size_t maxWordLen = 4;
  while(p < pEnd)
  {
  const CharT * matchStart = p;
  switch(fastTolower(*p)){
  case 0x62:
    switch(fastTolower(*(++p))){
    case 0x75:
      switch(fastTolower(*(++p))){
      case 0x74:
      // found a word:but Store pointer to last character of where it was found.
      pRet=p;
      result.match(pStart, pEnd, U"but", matchStart, p+1, "animals", 1, false, false, true); 
      // end. No more words.
      return p;
      break;
      default:
      if (pRet) {
        // pRet is pointing past the word it found.
        return pRet;
      }
      // no match found!
      break;
      }
    break;
    default:
    if (pRet) {
      // pRet is pointing past the word it found.
      return pRet;
    }
    // no match found!
    break;
    }
  break;
  case 0x63:
    switch(fastTolower(*(++p))){
    case 0x61:
      switch(fastTolower(*(++p))){
      case 0x74:
      // found a word:cat Store pointer to last character of where it was found.
      pRet=p;
      result.match(pStart, pEnd, U"cat", matchStart, p+1, "animals", 5, false, true, true); 
        switch(fastTolower(*(++p))){
        case 0x73:
        // found a word:cats Store pointer to last character of where it was found.
        pRet=p;
        result.match(pStart, pEnd, U"cats", matchStart, p+1, "animals", 5, false, true, true); 
        // end. No more words.
        return p;
        break;
        default:
        if (pRet) {
          // pRet is pointing past the word it found.
          return pRet;
        }
        // no match found!
        break;
        }
      break;
      default:
      if (pRet) {
        // pRet is pointing past the word it found.
        return pRet;
      }
      // no match found!
      break;
      }
    break;
    default:
    if (pRet) {
      // pRet is pointing past the word it found.
      return pRet;
    }
    // no match found!
    break;
    }
  break;
  default:
  if (pRet) {
    // pRet is pointing past the word it found.
    return pRet;
  }
  // no match found!
  break;
  }
  ++pbuff;
  p=pbuff;
  }
  return p;
}

