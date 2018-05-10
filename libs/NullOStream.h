// http://pkuwwt.github.io/programming/2014-10-18-cpp-null-ostream/
#pragma once

#include <iostream>

class NulStreambuf : public std::streambuf
{
  char                dummyBuffer[ 64 ];
protected:
  virtual int         overflow( int c )
  {
    setp( dummyBuffer, dummyBuffer + sizeof( dummyBuffer ) );
    return (c == traits_type::eof()) ? '\0' : c;
  }
};
class NulOStream : private NulStreambuf, public std::ostream
{
public:
  NulOStream() : std::ostream( this ) {}
  NulStreambuf* rdbuf() { return this; }
};
