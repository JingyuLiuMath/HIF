#ifndef LA_TENSOR_IMPL_HPP
#define LA_TENSOR_IMPL_HPP

namespace LA {

template<typename Ring>
Tensor<Ring>::Tensor() { }

template<typename Ring>
Tensor<Ring>::Tensor( Int D1 )
{
    dim_ = 1;
    size_.resize(dim_);
    size_[0] = D1;
    GenSizes_();
    memory_.resize( totsize_, 0 );
    data_ = memory_.data();
}

template<typename Ring>
Tensor<Ring>::Tensor( Int D1, Ring* buffer )
{
    dim_ = 1;
    size_.resize(dim_);
    size_[0] = D1;
    GenSizes_();
    data_ = buffer;
    viewType_ = VIEW;
}

template<typename Ring>
Tensor<Ring>::Tensor( Int D1, Int D2 )
{
    dim_ = 2;
    size_.resize(dim_);
    size_[0] = D1;
    size_[1] = D2;
    GenSizes_();
    memory_.resize( totsize_, 0 );
    data_ = memory_.data();
}

template<typename Ring>
Tensor<Ring>::Tensor( Int D1, Int D2, Ring* buffer )
{
    dim_ = 2;
    size_.resize(dim_);
    size_[0] = D1;
    size_[1] = D2;
    GenSizes_();
    data_ = buffer;
    viewType_ = VIEW;
}

template<typename Ring>
Tensor<Ring>::Tensor( Int D1, Int D2, Int D3 )
{
    dim_ = 3;
    size_.resize(dim_);
    size_[0] = D1;
    size_[1] = D2;
    size_[2] = D3;
    GenSizes_();
    memory_.resize( totsize_, 0 );
    data_ = memory_.data();
}

template<typename Ring>
Tensor<Ring>::Tensor( Int D1, Int D2, Int D3, Ring* buffer )
{
    dim_ = 3;
    size_.resize(dim_);
    size_[0] = D1;
    size_[1] = D2;
    size_[2] = D3;
    GenSizes_();
    data_ = buffer;
    viewType_ = VIEW;
}

template<typename Ring>
Tensor<Ring>::Tensor( const std::vector<Int>& Ds )
{
    dim_ = Ds.size();
    size_.resize(dim_);
    for(int i=0; i<dim_; i++)
        size_[i] = Ds[i];
    GenSizes_();
    memory_.resize( totsize_, 0 );
    data_ = memory_.data();
}

template<typename Ring>
Tensor<Ring>::Tensor( const std::vector<Int>& Ds, Ring* buffer )
{
    dim_ = Ds.size();
    size_.resize(dim_);
    for(int i=0; i<dim_; i++)
        size_[i] = Ds[i];
    GenSizes_();
    data_ = buffer;
    viewType_ = VIEW;
}

template<typename Ring>
Tensor<Ring>::Tensor( const Tensor<Ring>& A )
{
    if( &A != this )
        *this = A;
    else
        LogicError("You just tried to construct a Tensor with itself!");
}

template<typename Ring>
Tensor<Ring>::Tensor( Tensor<Ring>&& A ) LA_NO_EXCEPT
: viewType_(A.viewType_), dim_(A.dim_),
  memory_(std::move(A.memory_)), data_(nullptr)
{
    size_.swap(A.size_);
    GenSizes_();
    std::swap( data_, A.data_ );
}

template<typename Ring>
Tensor<Ring>::~Tensor() { }

template<typename Ring>
void Tensor<Ring>::Empty( bool freeMemory )
{
    Empty_( freeMemory );
}

template<typename Ring>
void Tensor<Ring>::Resize( Int D1, Int D2, Int D3 )
{
    LA_DEBUG_ONLY(
        AssertValidDimensions( D1, D2, D3 );
        if( Viewing() )
            LogicError("Cannot change the size of this matrix");
    )
    Resize_( D1, D2, D3 );
}

template<typename Ring>
void Tensor<Ring>::Resize( const std::vector<Int>& Ds )
{
    LA_DEBUG_ONLY(
        AssertValidDimensions( Ds );
        if( Viewing() )
            LogicError("Cannot change the size of this matrix");
    )
    Resize_( Ds );
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()( Range<Int> I1 )
{
    return View( *this, I1 );
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()( Range<Int> I1, Range<Int> I2 )
{
    return View( *this, I1, I2 );
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
(Range<Int> I1, Range<Int> I2, Range<Int> I3 )
{
    return View( *this, I1, I2, I3 );
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()( const std::vector<Range<Int>>& Is )
{
    return View( *this, Is );
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<Int>& I1 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( Range<Int> I1, const std::vector<Int>& I2 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<Int>& I1, Range<Int> I2 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<Int>& I1, const std::vector<Int>& I2 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( Range<Int> I1, Range<Int> I2, const std::vector<Int>& I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( Range<Int> I1, const std::vector<Int>& I2, Range<Int> I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<Int>& I1, Range<Int> I2, Range<Int> I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( Range<Int> I1, const std::vector<Int>& I2, const std::vector<Int>& I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<Int>& I1, Range<Int> I2, const std::vector<Int>& I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( Range<Int> I1, const std::vector<Int>& I2, const std::vector<Int>& I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<Int>& I1, const std::vector<Int>& I2,
  const std::vector<Int>& I3 ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, I1, I2, I3, ASub );
    return ASub;
}

template<typename Ring>
Tensor<Ring> Tensor<Ring>::operator()
( const std::vector<std::vector<Int>>& Is ) const
{
    Tensor<Ring> ASub; 
    GetSubtensor( *this, Is, ASub );
    return ASub;
}

template<typename Ring>
const Tensor<Ring>& Tensor<Ring>::operator=( const Tensor<Ring>& A )
{
    Copy( A, *this );
    return *this;
}

template<typename Ring>
Tensor<Ring>& Tensor<Ring>::operator=( Tensor<Ring>&& A )
{
    if( Viewing() || A.Viewing() )
    {
        operator=( (const Tensor<Ring>&)A );
    }
    else
    {
        memory_.swap( A.memory_ );
        std::swap( data_, A.data_ );
        viewType_ = A.viewType_;
        dim_ = A.dim_;
        size_ = A.size_;
        totsize_ = A.totsize_;
        prodsize_ = A.prodsize_;
    }
    return *this;
}

template<typename Ring>
const Tensor<Ring>& Tensor<Ring>::operator*=( const Ring& alpha )
{
    Scale( alpha, *this );
    return *this;
}

template<typename Ring>
const Tensor<Ring>& Tensor<Ring>::operator+=( const Tensor<Ring>& A )
{
    Axpy( Ring(1), A, *this );
    return *this;
}

template<typename Ring>
const Tensor<Ring>& Tensor<Ring>::operator-=( const Tensor<Ring>& A )
{
    Axpy( Ring(-1), A, *this );
    return *this;
}

template<typename Ring>
Int Tensor<Ring>::Height() const LA_NO_EXCEPT { return size_[0]; }

template<typename Ring>
Int Tensor<Ring>::Width() const LA_NO_EXCEPT { return size_[1]; }

template<typename Ring>
Int Tensor<Ring>::Depth() const LA_NO_EXCEPT { return size_[2]; }

template<typename Ring>
Int Tensor<Ring>::Size(Int dim) const LA_NO_EXCEPT { return size_[dim]; }

template<typename Ring>
Int Tensor<Ring>::MemorySize() const LA_NO_EXCEPT { return memory_.size(); }

template<typename Ring>
Ring* Tensor<Ring>::Buffer() LA_NO_EXCEPT
{
    return data_;
}

template<typename Ring>
Ring* Tensor<Ring>::Buffer( Int i1, Int i2, Int i3 ) LA_NO_EXCEPT
{
    if( data_ == nullptr )
        return nullptr;
    return &data_[Index(i1, i2, i3)];
}

template<typename Ring>
Ring* Tensor<Ring>::Buffer( std::vector<Int>& Is ) LA_NO_EXCEPT
{
    if( data_ == nullptr )
        return nullptr;
    return &data_[Index(Is)];
}

template<typename Ring>
bool Tensor<Ring>::Viewing() const LA_NO_EXCEPT
{ return viewType_ == VIEW; }

template<typename Ring>
void Tensor<Ring>::SetViewType( LA::ViewType viewType ) LA_NO_EXCEPT
{ viewType_ = viewType; }

template<typename Ring>
LA::ViewType Tensor<Ring>::ViewType() const LA_NO_EXCEPT
{ return viewType_; }

template<typename Ring>
Int Tensor<Ring>::Index( Int i1, Int i2, Int i3 ) const LA_NO_EXCEPT
{
    if( i1 == END ) i1 = size_[0] - 1;
    if( i2 == END ) i2 = size_[1] - 1;
    if( i3 == END ) i3 = size_[2] - 1;
    if( dim_ == 1 )
        return i1*prodsize_[0];
    else if( dim_ == 2 )
        return i1*prodsize_[0] + i2*prodsize_[1];
    else
        return i1*prodsize_[0] + i2*prodsize_[1] + i3*prodsize_[2];
}

template<typename Ring>
Int Tensor<Ring>::Index( std::vector<Int>& is ) const LA_NO_EXCEPT
{
    Int idx = 0;
    for(int i=0; i<dim_; i++)
        if( Is[i] == END )
            idx += (size_[i]-1)*prodsize_[i];
        else
            idx += Is[i]*prodsize_[i];
    return idx;
}

template<typename Ring>
Ring Tensor<Ring>::Get( Int i1, Int i2, Int i3 ) const LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1, i2, i3 ))
    return CRef( i1, i2, i3 );
}

template<typename Ring>
Ring Tensor<Ring>::Get( std::vector<Int>& is ) const LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( is ))
    return CRef( is );
}

template<typename Ring>
void Tensor<Ring>::Set( Int i1, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1 ))
    Ref( i1 ) = alpha;
}

template<typename Ring>
void Tensor<Ring>::Set( Int i1, Int i2, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1, i2 ))
    Ref( i1, i2 ) = alpha;
}

template<typename Ring>
void Tensor<Ring>::Set( Int i1, Int i2, Int i3, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1, i2, i3 ))
    Ref( i1, i2, i3 ) = alpha;
}

template<typename Ring>
void Tensor<Ring>::Set( std::vector<Int>& is, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( is ))
    Ref( is ) = alpha;
}

template<typename Ring>
void Tensor<Ring>::Update( Int i1, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1 ))
    Ref( i1 ) += alpha;
}

template<typename Ring>
void Tensor<Ring>::Update( Int i1, Int i2, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1, i2 ))
    Ref( i1, i2 ) += alpha;
}

template<typename Ring>
void Tensor<Ring>::Update
( Int i1, Int i2, Int i3, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( i1, i2, i3 ))
    Ref( i1, i2, i3 ) += alpha;
}

template<typename Ring>
void Tensor<Ring>::Update
( std::vector<Int>& is, const Ring& alpha ) LA_NO_EXCEPT
{
    LA_DEBUG_ONLY(AssertValidEntry( is ))
    Ref( is ) += alpha;
}

template<typename Ring>
void Tensor<Ring>::ShallowSwap( Tensor<Ring>& A )
{
    memory_.swap( A.memory_ );
    std::swap( data_, A.data_ );
    std::swap( viewType_, A.viewType_ );
    std::swap( dim_, A.dim_ );
    std::swap( size_, A.size_ );
    std::swap( totsize_, A.totsize_ );
    std::swap( prodsize_, A.prodsize_ );
}

template<typename Ring>
void Tensor<Ring>::Empty_( bool freeMemory )
{
    if( freeMemory )
        vector<Ring>().swap(memory_); 
    dim_ = 0;
    vector<Int>().swap(size_); 
    totsize_ = 0;
    vector<Int>().swap(prodsize_); 
    data_ = nullptr;
}

template<typename Ring>
const Ring& Tensor<Ring>::CRef( Int i1, Int i2, Int i3 ) const LA_NO_EXCEPT
{ 
    return data_[Index(i1, i2, i3)]; 
}

template<typename Ring>
const Ring& Tensor<Ring>::CRef( std::vector<Int>& is ) const LA_NO_EXCEPT
{ 
    return data_[Index(is)]; 
}

template<typename Ring>
const Ring& Tensor<Ring>::operator()( Int i1, Int i2, Int i3 ) const LA_NO_EXCEPT
{
    return data_[Index(i1, i2, i3)];
}

template<typename Ring>
const Ring& Tensor<Ring>::operator()( std::vector<Int>& is ) const LA_NO_EXCEPT
{
    return data_[Index(is)];
}

template<typename Ring>
Ring& Tensor<Ring>::Ref( Int i1, Int i2, Int i3 ) LA_NO_EXCEPT
{
    return data_[Index(i1, i2, i3)];
}

template<typename Ring>
Ring& Tensor<Ring>::Ref( std::vector<Int>& is ) LA_NO_EXCEPT
{
    return data_[Index(is)];
}

template<typename Ring>
Ring& Tensor<Ring>::operator()( Int i1, Int i2, Int i3 ) LA_NO_EXCEPT
{
    return data_[Index(i1, i2, i3)];
}

template<typename Ring>
Ring& Tensor<Ring>::operator()( std::vector<Int>& is ) LA_NO_EXCEPT
{
    return data_[Index(is)];
}

template<typename Ring>
void Tensor<Ring>::AssertValidDimensions( Int i1, Int i2, Int i3 ) const
{
    if( i1 < 0 || i2 < 0 || i3 < 0 )
        LogicError("Height and width must be non-negative");
}

template<typename Ring>
void Tensor<Ring>::AssertValidDimensions( std::vector<Int>& is ) const
{
    for(int i=0; i<dim_; i++)
        if( is[i] < 0 )
            LogicError("Size must be non-negative");
}

template<typename Ring>
void Tensor<Ring>::AssertValidEntry( Int i1, Int i2, Int i3 ) const
{
    if( i1 == END ) i1 = size_[0] - 1;
    if( i2 == END ) i2 = size_[1] - 1;
    if( i3 == END ) i3 = size_[2] - 1;
    if( i1 < 0 || i1 >= size_[0] )
        LogicError
        ("Out of bounds: ",i1," of ",size_[0]," Tensor");
    if( i2 < 0 || i2 >= size_[1] )
        LogicError
        ("Out of bounds: ",i2," of ",size_[1]," Tensor");
    if( i3 < 0 || i3 >= size_[2] )
        LogicError
        ("Out of bounds: ",i3," of ",size_[2]," Tensor");
}

template<typename Ring>
void Tensor<Ring>::AssertValidEntry( std::vector<Int>& is ) const
{
    for(int i=0; i<dim_; i++)
    {
        if( is[i] == END ) continue;
        if( is[i] < 0 || is[i] >= size_[i] )
            LogicError
            ("Out of bounds: ",is[i]," of ",size_[i]," Tensor");
    }
}

template<typename Ring>
void Tensor<Ring>::GenSizes_()
{
    prodsize_.resize(dim_);
    totsize_ = 1;
    for(int i=dim_-1; i>=0; i--)
    {
        prodsize_[i] = totsize_;
        totsize_ *= size_[i];
    }
}

// Left Here
template<typename Ring>
void Tensor<Ring>::Resize_( Int I1 )
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
