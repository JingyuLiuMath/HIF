#ifndef LA_MATRIX_IMPL_HPP
#define LA_MATRIX_IMPL_HPP

namespace LA {

template<typename Ring>
Matrix<Ring>::Matrix() { }

template<typename Ring>
Matrix<Ring>::Matrix( Int height, Int width )
: height_(height), width_(width), leadingDimension_(max(height,1))
{
    memory_.resize( leadingDimension_ * width, 0 );
    data_ = memory_.data();
}

template<typename Ring>
Matrix<Ring>::Matrix( Int height, Int width, Int leadingDimension )
: height_(height), width_(width), leadingDimension_(leadingDimension)
{
    memory_.resize( leadingDimension_ * width, 0 );
    data_ = memory_.data();
}

template<typename Ring>
Matrix<Ring>::Matrix
( Int height, Int width, Ring* buffer, Int leadingDimension )
: viewType_(VIEW),
  height_(height), width_(width), leadingDimension_(leadingDimension), 
  data_(buffer)
{
}

template<typename Ring>
Matrix<Ring>::Matrix( const Matrix<Ring>& A )
{
    if( &A != this )
        *this = A;
    else
        LogicError("You just tried to construct a Matrix with itself!");
}

template<typename Ring>
Matrix<Ring>::Matrix( Matrix<Ring>&& A ) LA_NO_EXCEPT
: viewType_(A.viewType_),
  height_(A.height_), width_(A.width_), leadingDimension_(A.leadingDimension_),
  memory_(std::move(A.memory_)), data_(nullptr)
{ std::swap( data_, A.data_ ); }

template<typename Ring>
Matrix<Ring>::~Matrix() { }

template<typename Ring>
void Matrix<Ring>::Empty( bool freeMemory )
{
    Empty_( freeMemory );
}

template<typename Ring>
void Matrix<Ring>::Resize( Int height, Int width )
{
    LA_DEBUG_ONLY(
        AssertValidDimensions( height, width );
        if( Viewing() && ( height > height_ || width > width_ ) )
            LogicError("Cannot increase the size of this matrix");
    )
    Resize_( height, width );
}

template<typename Ring>
void Matrix<Ring>::Resize( Int height, Int width, Int leadingDimension )
{
    LA_DEBUG_ONLY(
      AssertValidDimensions( height, width, leadingDimension );
      if( Viewing() && (height > height_ || width > width_ || 
          leadingDimension != leadingDimension_) )
          LogicError("Cannot increase the size of this matrix");
    )
    Resize_( height, width, leadingDimension );
}

template<typename Ring>
Matrix<Ring> Matrix<Ring>::operator()( Range<Int> I, Range<Int> J )
{
    return View( *this, I, J );
}

template<typename Ring>
Matrix<Ring> Matrix<Ring>::operator()
( Range<Int> I, const vector<Int>& J ) const
{
    Matrix<Ring> ASub; 
    GetSubmatrix( *this, I, J, ASub );
    return ASub;
}

template<typename Ring>
Matrix<Ring> Matrix<Ring>::operator()
( const vector<Int>& I, Range<Int> J ) const
{
    Matrix<Ring> ASub;
    GetSubmatrix( *this, I, J, ASub );
    return ASub;
}

template<typename Ring>
Matrix<Ring> Matrix<Ring>::operator()
( const vector<Int>& I, const vector<Int>& J ) const
{
    Matrix<Ring> ASub;
    GetSubmatrix( *this, I, J, ASub );
    return ASub;
}

template<typename Ring>
const Matrix<Ring>& Matrix<Ring>::operator=( const Matrix<Ring>& A )
{
    Copy( A, *this );
    return *this;
}

template<typename Ring>
Matrix<Ring>& Matrix<Ring>::operator=( Matrix<Ring>&& A )
{
    if( Viewing() || A.Viewing() )
    {
        operator=( (const Matrix<Ring>&)A );
    }
    else
    {
        memory_.ShallowSwap( A.memory_ );
        std::swap( data_, A.data_ );
        viewType_ = A.viewType_;
        height_ = A.height_;
        width_ = A.width_;
        leadingDimension_ = A.leadingDimension_;
    }
    return *this;
}

template<typename Ring>
const Matrix<Ring>& Matrix<Ring>::operator*=( const Ring& alpha )
{
    Scale( alpha, *this );
    return *this;
}

template<typename Ring>
const Matrix<Ring>& Matrix<Ring>::operator+=( const Matrix<Ring>& A )
{
    Axpy( Ring(1), A, *this );
    return *this;
}

template<typename Ring>
const Matrix<Ring>& Matrix<Ring>::operator-=( const Matrix<Ring>& A )
{
    Axpy( Ring(-1), A, *this );
    return *this;
}

template<typename Ring>
Int Matrix<Ring>::Height() const LA_NO_EXCEPT { return height_; }

template<typename Ring>
Int Matrix<Ring>::Width() const LA_NO_EXCEPT { return width_; }

template<typename Ring>
Int Matrix<Ring>::LDim() const LA_NO_EXCEPT { return leadingDimension_; }

template<typename Ring>
Int Matrix<Ring>::MemorySize() const LA_NO_EXCEPT { return memory_.size(); }

template<typename Ring>
Ring* Matrix<Ring>::Buffer() LA_NO_EXCEPT
{
    return data_;
}

template<typename Ring>
Ring* Matrix<Ring>::Buffer( Int i, Int j ) LA_NO_EXCEPT
{
    if( data_ == nullptr )
        return nullptr;
    if( i == END ) i = height_ - 1;
    if( j == END ) j = width_ - 1;
    return &data_[i+j*leadingDimension_];
}

template<typename Ring>
bool Matrix<Ring>::Viewing() const LA_NO_EXCEPT
{ return viewType_ == VIEW; }

template<typename Ring>
void Matrix<Ring>::SetViewType( LA::ViewType viewType ) LA_NO_EXCEPT
{ viewType_ = viewType; }

template<typename Ring>
LA::ViewType Matrix<Ring>::ViewType() const LA_NO_EXCEPT
{ return viewType_; }

template<typename Ring>
Ring Matrix<Ring>::Get( Int i, Int j ) const LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i, j ))
    if( i == END ) i = height_ - 1;
    if( j == END ) j = width_ - 1;
    return CRef( i, j );
}

template<typename Ring>
void Matrix<Ring>::Set( Int i, Int j, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i, j ))
    if( i == END ) i = height_ - 1;
    if( j == END ) j = width_ - 1;
    Ref( i, j ) = alpha;
}

template<typename Ring>
void Matrix<Ring>::Update( Int i, Int j, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i, j ))
    if( i == END ) i = height_ - 1;
    if( j == END ) j = width_ - 1;
    Ref( i, j ) += alpha;
}

template<typename Ring>
void Matrix<Ring>::ShallowSwap( Matrix<Ring>& A )
{
    memory_.swap( A.memory_ );
    std::swap( data_, A.data_ );
    std::swap( viewType_, A.viewType_ );
    std::swap( height_, A.height_ );
    std::swap( width_, A.width_ );
    std::swap( leadingDimension_, A.leadingDimension_ );
}

template<typename Ring>
void Matrix<Ring>::Empty_( bool freeMemory )
{
    if( freeMemory )
        vector<Ring>().swap(memory_); 
    height_ = 0;
    width_ = 0;
    leadingDimension_ = 1;
    data_ = nullptr;
}

template<typename Ring>
const Ring& Matrix<Ring>::CRef( Int i, Int j ) const LA_NO_EXCEPT
{ 
    return data_[i+j*leadingDimension_]; 
}

template<typename Ring>
const Ring& Matrix<Ring>::operator()( Int i, Int j ) const LA_NO_EXCEPT
{
    return data_[i+j*leadingDimension_];
}

template<typename Ring>
Ring& Matrix<Ring>::Ref( Int i, Int j ) LA_NO_EXCEPT
{
    return data_[i+j*leadingDimension_];
}

template<typename Ring>
Ring& Matrix<Ring>::operator()( Int i, Int j ) LA_NO_EXCEPT
{
    return data_[i+j*leadingDimension_];
}

template<typename Ring>
void Matrix<Ring>::AssertValidDimensions( Int height, Int width ) const
{
    if( height < 0 || width < 0 )
        LogicError("Height and width must be non-negative");
}

template<typename Ring>
void Matrix<Ring>::AssertValidDimensions
( Int height, Int width, Int leadingDimension ) const
{
    AssertValidDimensions( height, width );
    if( leadingDimension < height )
        LogicError("Leading dimension must be no less than height");
}

template<typename Ring>
void Matrix<Ring>::AssertValidEntry( Int i, Int j ) const
{
    if( i == END ) i = height_ - 1;
    if( j == END ) j = width_ - 1;
    if( i < 0 || j < 0 || i >= Height() || j >= Width() )
        LogicError
        ("Out of bounds: (",i,",",j,") of ",Height()," x ",Width()," Matrix");
}

template<typename Ring>
void Matrix<Ring>::Resize_( Int height, Int width )
{
    const bool reallocate = height > leadingDimension_ || width > width_;
    height_ = height;
    width_ = width;
    if( reallocate )
    {
        leadingDimension_ = max( height, 1 );
        memory_.resize( leadingDimension_ * width );
        data_ = memory_.Buffer();
    }
}

template<typename Ring>
void Matrix<Ring>::Resize_( Int height, Int width, Int leadingDimension )
{
    const bool reallocate =
      height > leadingDimension_ || width > width_ ||
      leadingDimension != leadingDimension_;
    height_ = height;
    width_ = width;
    if( reallocate )
    {
        leadingDimension_ = leadingDimension;
        memory_.Require(leadingDimension*width);
        data_ = memory_.data();
    }
}

} // namespace LA

#endif // ifndef LA_MATRIX_IMPL_HPP
