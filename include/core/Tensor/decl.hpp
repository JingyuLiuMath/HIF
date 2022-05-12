#ifndef LA_TENSOR_DECL_HPP
#define LA_TENSOR_DECL_HPP

namespace LA {

template<typename Ring>
class Tensor
{
public:
    Tensor();
    Tensor( Int D1 );
    Tensor( Int D1, Ring* buffer);
    Tensor( Int D1, Int D2 );
    Tensor( Int D1, Int D2, Ring* buffer);
    Tensor( Int D1, Int D2, Int D3 );
    Tensor( Int D1, Int D2, Int D3, Ring* buffer);
    Tensor( const std::vector<Int>& Ds );
    Tensor( const std::vector<Int>& Ds, Ring* buffer);
    Tensor( const Tensor<Ring>& A );
    Tensor( Tensor<Ring>&& A ) LA_NO_EXCEPT;

    ~Tensor();

    void Empty( bool freeMemory=true );
    void Resize( Int D1, Int D2=0, Int D3=0 );
    void Resize( const std::vector<Int>& Ds );

    Tensor<Ring> operator()( Range<Int> I1 );
    Tensor<Ring> operator()( Range<Int> I1, Range<Int> I2 );
    Tensor<Ring> operator()( Range<Int> I1, Range<Int> I2, Range<Int> I3 );
    Tensor<Ring> operator()( const std::vector<Range<Int>>& Is);

    Tensor<Ring> operator()( const std::vector<Int>& I1 ) const;
    Tensor<Ring> operator()( Range<Int> I1, const std::vector<Int>& I2 ) const;
    Tensor<Ring> operator()( const std::vector<Int>& I1, Range<Int> I2 ) const;
    Tensor<Ring> operator()
    ( const std::vector<Int>& I1, const std::vector<Int>& I2 ) const;
    Tensor<Ring> operator()
    ( Range<Int> I1, Range<Int> I2, const std::vector<Int>& I3 ) const;
    Tensor<Ring> operator()
    ( Range<Int> I1, const std::vector<Int>& I3, Range<Int> I3 ) const;
    Tensor<Ring> operator()
    ( const std::vector<Int>& I1, Range<Int> I2, Range<Int> I3 ) const;
    Tensor<Ring> operator()
    ( Range<Int> I1, const std::vector<Int>& I2,
      const std::vector<Int>& I3 ) const;
    Tensor<Ring> operator()
    ( const std::vector<Int>& I1, Range<Int> I2,
      const std::vector<Int>& I3 ) const;
    Tensor<Ring> operator()
    ( const std::vector<Int>& I1, const std::vector<Int>& I2,
      Range<Int> I3 ) const;
    Tensor<Ring> operator()
    ( const std::vector<Int>& I1, const std::vector<Int>& I2,
      const std::vector<Int>& I3 ) const;
    Tensor<Ring> operator()( const std::vector<std::vector<Int>>& Is) const;

    const Tensor<Ring>& operator=( const Tensor<Ring>& A );
    Tensor<Ring>& operator=( Tensor<Ring>&& A );
    const Tensor<Ring>& operator*=( const Ring& alpha );
    const Tensor<Ring>& operator+=( const Tensor<Ring>& A );
    const Tensor<Ring>& operator-=( const Tensor<Ring>& A );

    Int Height() const LA_NO_EXCEPT;
    Int Width() const LA_NO_EXCEPT;
    Int Depth() const LA_NO_EXCEPT;
    Int Size(Int dim) const LA_NO_EXCEPT;
    Int MemorySize() const LA_NO_EXCEPT;

    Ring* Buffer() LA_NO_EXCEPT;
    Ring* Buffer( Int i1, Int i2=0, Int i3=0 ) LA_NO_EXCEPT;
    Ring* Buffer( const std::vector<Int>& is ) LA_NO_EXCEPT;

    bool Viewing()   const LA_NO_EXCEPT;

    void SetViewType( LA::ViewType viewType ) LA_NO_EXCEPT;
    LA::ViewType ViewType() const LA_NO_EXCEPT;

    Ring Get( Int i1, Int i2=0, Int i3=0 ) const LA_NO_EXCEPT;
    Ring Get( const std::vector<Int>& is ) const LA_NO_EXCEPT;

    void Set( Int i1, const Ring& alpha ) LA_NO_EXCEPT;
    void Set( Int i1, Int i2, const Ring& alpha ) LA_NO_EXCEPT;
    void Set( Int i1, Int i2, Int i3, const Ring& alpha ) LA_NO_EXCEPT;
    void Set( const std::vector<Int>& is, const Ring& alpha ) LA_NO_EXCEPT;

    void Update( Int i1, const Ring& alpha ) LA_NO_EXCEPT;
    void Update( Int i1, Int i2, const Ring& alpha ) LA_NO_EXCEPT;
    void Update( Int i1, Int i2, Int i3, const Ring& alpha ) LA_NO_EXCEPT;
    void Update( const std::vector<Int>& is, const Ring& alpha ) LA_NO_EXCEPT;

    inline const Ring& CRef( Int i1, Int i2=0, Int i3=0 ) const LA_NO_EXCEPT;
    inline const Ring& operator()( Int i1, Int i2=0, Init i3=0 ) const
        LA_NO_EXCEPT;
    inline const Ring& CRef( const std::vector<Int>& is ) const LA_NO_EXCEPT;
    inline const Ring& operator()( const std::vector<Int>& is ) const
        LA_NO_EXCEPT;

    inline Ring& Ref( Int i1, Int i2=0, Int i3=0 ) LA_NO_EXCEPT;
    inline Ring& operator()( Int i1, Int i2=0, Int i3=0 ) LA_NO_EXCEPT;
    inline Ring& Ref( const std::vector<Int>& is ) LA_NO_EXCEPT;
    inline Ring& operator()( const std::vector<Int>& is ) LA_NO_EXCEPT;

private:
    LA::ViewType viewType_=OWNER;
    Int dim_;
    std::vector<Int> size_;
    Int totsize_;
    std::vector<Int> prodsize_;

    std::vector<Ring> memory_;
    Ring* data_=nullptr;

    void ShallowSwap( Tensor<Ring>& A );

    Int Index( Int i1, Int i2=0, Int i3=0 ) const LA_NO_EXCEPT;
    Int Index( const std::vector<Int>& is ) const LA_NO_EXCEPT;

    void GenSizes_();

    void Empty_( bool freeMemory=true );
    void Resize_( Int I1 );
    void Resize_( Int I1, Int I2 );
    void Resize_( Int I1, Int I2, Int I3 );
    void Resize_( const std::vector<Int>& Is );

    void AssertValidDimensions( Int i1, Int i2=0, Int i3=0 ) const;
    void AssertValidDimensions( const std::vector<Int>& is ) const;
    void AssertValidEntry( Int i1, Int i2=0, Int i3=0 ) const;
    void AssertValidEntry( const std::vector<Int>& is ) const;
   
    // Friend class 
    template<typename S> friend class Tensor;
    template<typename S> friend class Matrix;
};

} // namespace LA

#endif // ifndef LA_TENSOR_DECL_HPP
