const CharT* search(const CharT * pStart, const CharT * pbuff, CallbackFunction match, void* data) 
{
  const CharT * p = pbuff;
  const CharT * pRet = NULL;
  const size_t maxWordLen = 4;
  while(*p)
  {
  switch(*p){
  case 'c':
    switch(*(++p)){
    case 'a':
      switch(*(++p)){
      case 't':
      // found a word:cat Store pointer to last character of where it was found.
      pRet=p;
      match(pStart, pbuff, "cat", p, "dictionary1", -1, false, data); 
        switch(*(++p)){
        case 's':
        // found a word:cats Store pointer to last character of where it was found.
        pRet=p;
        match(pStart, pbuff, "cats", p, "dictionary1", 0, false, data); 
        // end. No more words.
        return p;
        break;
        case 'z':
        // found a word:catz Store pointer to last character of where it was found.
        pRet=p;
        match(pStart, pbuff, "catz", p, "dictionary1", 1, false, data); 
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

