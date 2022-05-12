#ifndef LA_MATRIX_DECL_HPP
#define LA_MATRIX_DECL_HPP

namespace LA {

template<typename Ring>
class Matrix
{
public:    
    Matrix();
    Matrix( Int height, Int width );
    Matrix( Int height, Int width, Int leadingDimension );
    Matrix( Int height, Int width, Ring* buffer, Int leadingDimension );
    Matrix( const Matrix<Ring>& A );
    Matrix( Matrix<Ring>&& A ) LA_NO_EXCEPT;

    ~Matrix();

    void Empty( bool freeMemory=true );
    void Resize( Int height, Int width );
    void Resize( Int height, Int width, Int leadingDimension );

    Matrix<Ring> operator()( Range<Int> I, Range<Int> J );
    Matrix<Ring>
    operator()( Range<Int> I, const std::vector<Int>& J ) const;
    Matrix<Ring>
    operator()( const std::vector<Int>& I, Range<Int> J ) const;
    Matrix<Ring>
    operator()( const std::vector<Int>& I, const std::vector<Int>& J ) const;

    const Matrix<Ring>& operator=( const Matrix<Ring>& A );
    Matrix<Ring>& operator=( Matrix<Ring>&& A );
    const Matrix<Ring>& operator*=( const Ring& alpha );
    const Matrix<Ring>& operator+=( const Matrix<Ring>& A );
    const Matrix<Ring>& operator-=( const Matrix<Ring>& A );

    Int Height() const LA_NO_EXCEPT;
    Int Width() const LA_NO_EXCEPT;
    Int LDim() const LA_NO_EXCEPT;
    Int MemorySize() const LA_NO_EXCEPT;

    Ring* Buffer() LA_NO_EXCEPT;
    Ring* Buffer( Int i, Int j ) LA_NO_EXCEPT;

    bool Viewing()   const LA_NO_EXCEPT;

    void SetViewType( LA::ViewType viewType ) LA_NO_EXCEPT;
    LA::ViewType ViewType() const LA_NO_EXCEPT;

    Ring Get( Int i, Int j=0 ) const LA_NO_EXCEPT;

    void Set( Int i, Int j, const Ring& alpha ) LA_NO_EXCEPT;

    void Update( Int i, Int j, const Ring& alpha ) LA_NO_EXCEPT;

    inline const Ring& CRef( Int i, Int j=0 ) const LA_NO_EXCEPT;
    inline const Ring& operator()( Int i, Int j=0 ) const LA_NO_EXCEPT;

    inline Ring& Ref( Int i, Int j=0 ) LA_NO_EXCEPT;
    inline Ring& operator()( Int i, Int j=0 ) LA_NO_EXCEPT;

private:
    LA::ViewType viewType_=OWNER;
    Int height_=0, width_=0, leadingDimension_=1;

    std::vector<Ring> memory_;
    Ring* data_=nullptr;

    void ShallowSwap( Matrix<Ring>& A );

    void Empty_( bool freeMemory=true );
    void Resize_( Int height, Int width );
    void Resize_( Int height, Int width, Int leadingDimension );

    void AssertValidDimensions( Int height, Int width ) const;
    void AssertValidDimensions
    ( Int height, Int width, Int leadingDimension ) const;
    void AssertValidEntry( Int i, Int j ) const;
   
    // Friend class 
    template<typename S> friend class Matrix;
};

} // namespace LA

#endif // ifndef LA_MATRIX_DECL_HPP
