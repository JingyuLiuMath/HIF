#ifndef LA_TYPES_HPP
#define LA_TYPES_HPP

namespace LA {

typedef int Int;

template<typename T>
struct Range
{
    T beg, end;
    Range() : beg(0), end(0) { }
    Range( T idx ) : beg(idx), end(idx+1) { }
    Range( T b, T e ) : beg(b), end(e) { }

    Range<T> operator+( T shift ) const
    { return Range<T>(beg+shift,end+shift); }

    Range<T> operator-( T shift ) const
    { return Range<T>(beg-shift,end-shift); }

    Range<T> operator*( T scale ) const
    { return Range<T>(beg*scale,end*scale); }
};

static const Int END = -100;

typedef Range<Int> IR;

static const IR ALL(0,END);

template<typename T>
inline bool operator==( const Range<T>& a, const Range<T>& b )
{ return a.beg == b.beg && a.end == b.end; }

namespace ViewTypeNS {
enum ViewType
{
    OWNER = 0x0,
    VIEW = 0x1,
};
static inline bool IsViewing( ViewType v ) EL_NO_EXCEPT
{ return ( v & VIEW ) != 0; }
}
using namespace ViewTypeNS;

enum Orientation
{
    NORMAL,
    TRANSPOSE,
    ADJOINT
};
char OrientationToChar( Orientation orientation );
Orientation CharToOrientation( char c );
}
using namespace OrientationNS;

} // namespace LA

#endif // ifndef LA_TYPES_HPP
