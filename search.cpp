const CharT* search(const CharT * pStart, const CharT * pbuff, MatchFunction match, void* data) 
{
  const CharT * p = pbuff;
  const CharT * pRet = NULL;
  const size_t maxWordLen = 4;
  while(*p)
  {
  const CharT * matchStart = p;
  switch(tolower(*p)){
  case 'c':
    switch(tolower(*(++p))){
    case 'a':
      switch(tolower(*(++p))){
      case 't':
      // found a word:caT Store pointer to last character of where it was found.
      pRet=p;
      match(pStart, "caT", matchStart, p, "dictionary1", -1, false, true, true, data); 
        switch(tolower(*(++p))){
        case 's':
        // found a word:caTs Store pointer to last character of where it was found.
        pRet=p;
        match(pStart, "caTs", matchStart, p, "dictionary1", 0, false, true, true, data); 
        // end. No more words.
        return p;
        break;
        default:
        if (pRet) {
        // pRet is pointing past the word it found.
        return pRet;
        }
        // not found!
        break;
        }
      break;
      default:
      if (pRet) {
      // pRet is pointing past the word it found.
      return pRet;
      }
      // not found!
      break;
      }
    break;
    default:
    if (pRet) {
    // pRet is pointing past the word it found.
    return pRet;
    }
    // not found!
    break;
    }
  break;
  default:
  if (pRet) {
  // pRet is pointing past the word it found.
  return pRet;
  }
  // not found!
  break;
  }
  ++pbuff;
  p=pbuff;
  }
  return p;
}

