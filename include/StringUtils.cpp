#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <sys/types.h>

using namespace std;

#include "StringUtils.h"

namespace StringUtilsNS {
const string MML_FIELD_SEP = "\x1C";
void maskXMLTag(string &xml, string tag, CC_MASK_TYPE maskType);
void maskMMLData(string &mml, string mmlField, CC_MASK_TYPE maskType);
void insertTagWithData(string tag, string beforeTag, string data, string &xml);
void insertTagWithData(string tag, string beforeTag, int data, string &xml);
vector<string> split(const string &s, char delim);
void stripLeadingNotOf(string &s, const string &notOf);
string stripLeadingNotOf(const string &s, const string &notOf);
bool stringToBool(const string &s);
} // namespace StringUtilsNS

using namespace StringUtilsNS;

//---------------------------------------------------------------------------
void StringUtils::maskString(string &destString, CC_MASK_TYPE maskType) {
  unsigned int dataSize = destString.size();

  int clearSize;

  switch (maskType) {
  case CC_MASK_LEAVE_LAST_FOUR:
    clearSize = dataSize >= 4 ? 4 : dataSize;
    break;

  case CC_MASK_ALL:
  default:
    clearSize = 0;
    break;
  }

  // Mask out data
  unsigned int maskedSize = dataSize - clearSize;

  if (maskedSize) {
    for (unsigned int i = 0; i < maskedSize; i++) {
      destString[i] = '*';
    }
  }
}

//---------------------------------------------------------------------------
void StringUtils::maskXMLTag(string &xml, string tag, CC_MASK_TYPE maskType) {
  // Look for the start tag
  string startTag = "<" + tag + ">";
  size_t startPos = xml.find(startTag);

  if (startPos == string::npos) {
    return;
  }

  size_t startTagSize = startTag.size();

  // Look for the endtag
  string endTag = startTag;
  endTag.insert(1, "/");
  size_t endPos = xml.find(endTag, startPos + startTagSize);

  if (endPos == string::npos) {
    return;
  }

  // Get the data
  size_t startOfData = startPos + startTagSize;
  size_t dataSize = endPos - startPos - startTagSize;

  if (dataSize == 0) {
    return;
  }

  string data =
      xml.substr(startPos + startTagSize, endPos - startPos - startTagSize);

  // Mask
  if (maskType == CC_MASK_ALL) {
    ostringstream s;
    s << setfill('*') << std::setw(dataSize) << "";
    xml.replace(startOfData, dataSize, s.str());

  } else if (maskType == CC_MASK_LEAVE_LAST_FOUR) {
    if (data.size() > 4) {
      ostringstream s;
      string lastFour = data.substr(dataSize - 4, 4);
      s << setfill('*') << std::setw(dataSize - 4) << lastFour;
      xml.replace(startOfData, dataSize, s.str());
    }
  }
}

//---------------------------------------------------------------------------
void StringUtils::maskMMLData(string &mml, string mmlField,
                              CC_MASK_TYPE maskType) {
  // Look for the starting field and field sep.
  string startField = MML_FIELD_SEP + mmlField;
  size_t startPos = mml.find(startField);

  if (startPos == string::npos) {
    return;
  }

  size_t startFieldSize = startField.size();

  // Look for the end field sep
  size_t endPos = mml.find(MML_FIELD_SEP, startPos + startFieldSize);

  if (endPos == string::npos) {
    return;
  }

  // Get the data
  size_t startOfData = startPos + startFieldSize;
  size_t dataSize = endPos - startPos - startFieldSize;

  if (dataSize == 0) {
    return;
  }

  string data =
      mml.substr(startPos + startFieldSize, endPos - startPos - startFieldSize);

  // Mask
  if (maskType == CC_MASK_ALL) {
    ostringstream s;
    s << setfill('*') << std::setw(dataSize) << "";
    mml.replace(startOfData, dataSize, s.str());

  } else if (maskType == CC_MASK_LEAVE_LAST_FOUR) {
    ostringstream s;
    string lastFour = data.substr(dataSize - 4, 4);
    s << setfill('*') << std::setw(dataSize - 4) << lastFour;
    mml.replace(startOfData, dataSize, s.str());
  }
}

//---------------------------------------------------------------------------
void StringUtils::insertTagWithData(string tag, string beforeTag, string data,
                                    string &xml) {
  ostringstream tagAndData;
  tagAndData << "<" << tag << ">";
  tagAndData << data;
  tagAndData << "</" << tag << ">\n";

  size_t foundPos = xml.find(beforeTag);

  if (foundPos != string::npos) {
    xml.insert(foundPos, tagAndData.str());
  }
}

//---------------------------------------------------------------------------
void StringUtils::insertTagWithData(string tag, string beforeTag, int data,
                                    string &xml) {
  ostringstream intString;
  intString << data;
  insertTagWithData(tag, beforeTag, intString.str(), xml);
}

//---------------------------------------------------------------------------
vector<string> StringUtils::split(const string &s, char delim) {
  vector<string> items;
  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    items.push_back(item);
  }

  return items;
}

//---------------------------------------------------------------------------
vector<string> StringUtils::split(const string &s, const string &delim) {
  vector<string> items;
  size_t sLen = s.size();
  size_t lastPos = 0;

  // Loop through looking for delim
  size_t pos = s.find(delim);

  while (pos != string::npos) {
    // Add the item
    items.push_back(s.substr(lastPos, pos - lastPos));

    // Set our last found and look again
    lastPos = pos + delim.size();
    pos = s.find(delim, lastPos);

    // See if we have any leftovers
    if (pos == string::npos && lastPos < sLen) {
      items.push_back(s.substr(lastPos, sLen - lastPos));
    }
  }

  // Return found items
  return items;
}

//---------------------------------------------------------------------------
void StringUtils::stripLeadingNotOf(string &s, const string &notOf) {
  if (s.size() == 0) {
    return;
  }

  size_t foundPos = s.find_first_not_of(notOf);

  if (foundPos == string::npos) {
    // See if we have a zero, if so, then we have all zeros
    if (s[0] == '0') {
      s = "";
    }

    return;
  }

  s.erase(0, foundPos);
}

//---------------------------------------------------------------------------
string StringUtils::stripLeadingNotOf(const string &s, const string &notOf) {
  string tempS = s;
  stripLeadingNotOf(tempS, notOf);
  return tempS;
}

// ---------------------------------------------------------------------------
bool StringUtils::stringToBool(const string &s) {
  // Default to false if empty or too large
  size_t size = s.size();

  if (size == 0 || size > 4) {
    return false;
  }

  // See if we have a single char, or a word
  if (size == 1) {
    switch (s.c_str()[0]) {
    case 'y':
    case 'Y':
    case '1':
      return true;

    default:
      return false;
    }

  } else {
    string sCopy = s;
    transform(sCopy.begin(), sCopy.end(), sCopy.begin(), ::tolower);

    if (sCopy.compare("true") == 0) {
      return true;
    }

    if (sCopy.compare("yes") == 0) {
      return true;
    }

    if (sCopy.compare("on") == 0) {
      return true;
    }

    if (sCopy.compare("ok") == 0) {
      return true;
    }

    if (sCopy.compare("sure") == 0) {
      return true;
    }

    if (sCopy.compare("yep") == 0) {
      return true;
    }

    return false;
  }
}

// ---------------------------------------------------------------------------
string StringUtils::formatWithCommas(unsigned int n) {
  ostringstream temp;
  temp.imbue(locale(""));
  temp << n;
  return temp.str();
}

// ---------------------------------------------------------------------------
string StringUtils::formatWithCommas(const string &s) {
  return formatWithCommas(atol(s.c_str()));
}

//---------------------------------------------------------------------------
void StringUtils::trimEnd(string &s) {
  size_t endPos = s.find_last_not_of(" ");

  if (endPos != string::npos) {
    s = s.substr(0, endPos + 1);
  }
}

//---------------------------------------------------------------------------
void StringUtils::trimStart(string &s) {
  // Leading
  size_t startPos = s.find_first_not_of(" ");

  if (startPos != string::npos) {
    s = s.substr(startPos);
  }
}

//---------------------------------------------------------------------------
void StringUtils::trim(string &s) {
  trimEnd(s);
  trimStart(s);
}

//---------------------------------------------------------------------------
string StringUtils::trimLeading(const string &str, char c) {
  size_t pos = str.find_first_not_of(c);
  return pos != string::npos ? str.substr(pos, str.length()) : "";
}

//---------------------------------------------------------------------------
string StringUtils::trimTrailing(const string &str, char c) {
  size_t pos = str.find_last_not_of(c);
  return pos != string::npos ? str.substr(0, pos + 1) : "";
}

//---------------------------------------------------------------------------
string StringUtils::trim(const string &str, char c) {
  return trimTrailing(trimLeading(str, c), c);
}

//---------------------------------------------------------------------------
const static unsigned char AsciiToEbcdicTable[256] = {
    0,   1,   2,   3,   55,  45,  46,  47,  22,  5,   37,  11,  12,  13,  14,
    15,  16,  17,  18,  19,  60,  61,  50,  38,  24,  25,  63,  39,  28,  29,
    30,  31,  64,  79,  127, 123, 91,  108, 80,  125, 77,  93,  92,  78,  107,
    96,  75,  97,  240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 122, 94,
    76,  126, 110, 111, 124, 193, 194, 195, 196, 197, 198, 199, 200, 201, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 226, 227, 228, 229, 230, 231, 232,
    233, 74,  224, 90,  95,  109, 121, 129, 130, 131, 132, 133, 134, 135, 136,
    137, 145, 146, 147, 148, 149, 150, 151, 152, 153, 162, 163, 164, 165, 166,
    167, 168, 169, 192, 106, 208, 161, 7,   32,  33,  34,  35,  36,  21,  6,
    23,  40,  41,  42,  43,  44,  9,   10,  27,  48,  49,  26,  51,  52,  53,
    54,  8,   56,  57,  58,  59,  4,   20,  62,  225, 65,  66,  67,  68,  69,
    70,  71,  72,  73,  81,  82,  83,  84,  85,  86,  87,  88,  89,  98,  99,
    100, 101, 102, 103, 104, 105, 112, 113, 114, 115, 116, 117, 118, 119, 120,
    128, 138, 139, 140, 141, 142, 143, 144, 154, 155, 156, 157, 158, 159, 160,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184,
    185, 186, 187, 188, 189, 190, 191, 202, 203, 204, 205, 206, 207, 218, 219,
    220, 221, 222, 223, 234, 235, 236, 237, 238, 239, 250, 251, 252, 253, 254,
    255};

//---------------------------------------------------------------------------
const static unsigned char EbcdicToAsciiTable[256] = {
    0,   1,   2,   3,   156, 9,   134, 127, 151, 141, 142, 11,  12,  13,  14,
    15,  16,  17,  18,  19,  157, 133, 8,   135, 24,  25,  146, 143, 28,  29,
    30,  31,  128, 129, 130, 131, 132, 10,  23,  27,  136, 137, 138, 139, 140,
    5,   6,   7,   144, 145, 22,  147, 148, 149, 150, 4,   152, 153, 154, 155,
    20,  21,  158, 26,  32,  160, 161, 162, 163, 164, 165, 166, 167, 168, 91,
    46,  60,  40,  43,  33,  38,  169, 170, 171, 172, 173, 174, 175, 176, 177,
    93,  36,  42,  41,  59,  94,  45,  47,  178, 179, 180, 181, 182, 183, 184,
    185, 124, 44,  37,  95,  62,  63,  186, 187, 188, 189, 190, 191, 192, 193,
    194, 96,  58,  35,  64,  39,  61,  34,  195, 97,  98,  99,  100, 101, 102,
    103, 104, 105, 196, 197, 198, 199, 200, 201, 202, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 203, 204, 205, 206, 207, 208, 209, 126, 115, 116, 117,
    118, 119, 120, 121, 122, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 123, 65,  66,
    67,  68,  69,  70,  71,  72,  73,  232, 233, 234, 235, 236, 237, 125, 74,
    75,  76,  77,  78,  79,  80,  81,  82,  238, 239, 240, 241, 242, 243, 92,
    159, 83,  84,  85,  86,  87,  88,  89,  90,  244, 245, 246, 247, 248, 249,
    48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  250, 251, 252, 253, 254,
    255};

//---------------------------------------------------------------------------
string StringUtils::ebcdicToAscii(const string &source) {
  ostringstream oss;
  size_t len = source.size();

  // Perform lookup and assign
  for (unsigned int i = 0; i < len; i++) {
    oss << EbcdicToAsciiTable[(unsigned char)source[i]];
  }

  return oss.str();
}

//---------------------------------------------------------------------------
string StringUtils::asciiToebcdic(const string &source) {
  ostringstream oss;
  size_t len = source.size();

  // Perform lookup and assign
  for (unsigned int i = 0; i < len; i++) {
    oss << AsciiToEbcdicTable[(unsigned char)source[i]];
  }

  return oss.str();
}

//---------------------------------------------------------------------------
string StringUtils::ebcdicToAscii(const char *source, size_t size) {
  string result;
  result.reserve(size);

  // Perform lookup and assign
  for (unsigned int i = 0; i < size; i++) {
    result.push_back(EbcdicToAsciiTable[(unsigned char)source[i]]);
  }

  return result;
}

//---------------------------------------------------------------------------
map<string, string> StringUtils::parseKeyValueParams(const string &params) {
  vector<string> items = StringUtils::split(params, '&');
  map<string, string> parsedParams;
  vector<string>::const_iterator outerIter;

  // Separate key/value pairs
  for (outerIter = items.begin(); outerIter != items.end(); outerIter++) {
    vector<string> kvVector = split(*outerIter, '=');

    if (kvVector.size() == 2) {
      string k = kvVector[0];
      string v = kvVector[1];
      trim(k);
      trim(v);
      parsedParams[k] = v;
    }
  }

  return parsedParams;
}

//---------------------------------------------------------------------------
void StringUtils::replaceStringWithString(string &source,
                                          const string &stringToFind,
                                          const string &replaceString) {
  size_t foundPos = source.find(stringToFind);

  if (foundPos == string::npos) {
    return;
  }

  source.replace(foundPos, stringToFind.size(), replaceString);
}

//---------------------------------------------------------------------------
pair<string, unsigned int> StringUtils::hostAndPort(const string &hostPort) {
  pair<string, unsigned int> hostPortPair;

  vector<string> parts = StringUtils::split(hostPort, ':');

  if (parts.size() != 2) {
    return hostPortPair;
  }

  // Save the host and port
  hostPortPair.first = parts[0];
  hostPortPair.second = atoi(parts[1].c_str());

  return hostPortPair;
}

//---------------------------------------------------------------------------
void StringUtils::toUpper(string &str) {
  transform(str.begin(), str.end(), str.begin(), ::toupper);
}

//---------------------------------------------------------------------------
void StringUtils::toLower(string &str) {
  transform(str.begin(), str.end(), str.begin(), ::tolower);
}

//---------------------------------------------------------------------------
string StringUtils::returnUpper(string str) {
  transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

//---------------------------------------------------------------------------
string StringUtils::returnLower(string str) {
  transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

//---------------------------------------------------------------------------
string StringUtils::safeSubstr(const string &str, size_t pos, size_t count) {
  return pos < str.size() ? str.substr(pos, count) : "";
}

//---------------------------------------------------------------------------
string StringUtils::formatString(const string &buffer, ushort size,
                                 char fillChar, bool leftJust) {
  ostringstream oss;
  leftJust ? oss << left : oss << right;
  oss << setfill(fillChar) << setw(size)
      << (buffer.length() > size ? StringUtils::safeSubstr(buffer, 0, size)
                                 : buffer);
  return oss.str();
}
//---------------------------------------------------------------------------
char StringUtils::getChar(const std::string &str, ushort pos) {
  try {
    return str.at(pos);
  } catch (...) {
    return '\0';
  }
}
