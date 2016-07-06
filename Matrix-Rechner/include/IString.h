#ifndef __ISTRING_H
#define __ISTRING_H


#include <iostream>
#include <string>

namespace Codebook {

struct insensitive_char_traits {
	typedef char char_type;
	typedef int int_type;

  static void assign(char& c1, const char& c2) {
    c1 = c2;
  }

  static bool eq(const char& c1, const char& c2) {
    return (tolower(c1) == tolower(c2));
  }

  static bool lt(const char& c1, const char& c2) {
    return (tolower(c1) < tolower(c2));
  }

  static int compare(const char *s1, const char *s2, size_t len) {
    for (; 0 < len; --len, ++s1, ++s2)
      if (!eq(*s1, *s2))
        return (lt(*s1, *s2) ? -1 : +1);
    return (0);
  }

  static size_t length(const char *s) {
    size_t len;
    for (len = 0; !eq(*s, char()); ++s)
      ++len;
    return (len);
  }		

  static char *copy(char *s1, const char *s2, size_t len) {
    char *ptr = s1;
    for (; 0 < len; --len, ++ptr, ++s2)
      assign(*ptr, *s2);
    return (s1);
  }

	static const char *find(const char *s, size_t len, const char& c) {
    for (; 0 < len; --len, ++s)
      if (eq(*s, c))
        return (s);
    return (0);
  }

  static char *move(char *s1, const char *s2, size_t len) {
    char *ptr = s1;
    if (s2 < ptr && ptr < s2 + len)
      for (ptr += len, s2 += len; 0 < len; --len)
        assign(*--ptr, *--s2);
    else
      for (; 0 < len; --len, ++ptr, ++s2)
        assign(*ptr, *s2);
    return (s1);
  }

  static char *assign(char *s, size_t len, char c) {
    char *ptr = s;
    for (; 0 < len; --len, ++ptr)
      assign(*ptr, c);
    return (s);
  }

  static char to_char_type(const int_type& i) {
    return (i);
  }

  static int_type  to_int_type(const char& c) {
    return (c);
  }

  static bool eq_int_type(const int_type& c1,	const int_type& c2) {
    return (c1 == c2);
  }

  static int_type  eof() {
    return ((int_type)EOF);
  }

  static int_type  not_eof(const int_type& i) {
    return (i != eof() ? i : !eof());
  }

};


typedef std::basic_string<char, insensitive_char_traits, std::allocator<char> >
	insensitive_string;




}; // ende namespace Codebook
#endif


