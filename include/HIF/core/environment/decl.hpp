#pragma once
#ifndef HIF_CORE_ENVIRONMENT_DECL_HPP
#define HIF_CORE_ENVIRONMENT_DECL_HPP

#ifdef RELEASE
#define DEBUG_HIF(cmd)
#else
#define DEBUG_HIF(cmd) cmd;
#endif

#ifdef NDEBUG
#define ASSERT_HIF(ex,msg...) ;
#else
#define \
    ASSERT_HIF(ex,msg...) \
                (void)((ex)||(Assert(#ex,__func__,__FILE__,__LINE__,msg),0));
#endif

#ifdef TIMER
#define TIMER_HIF(cmd) cmd;
#else
#define TIMER_HIF(cmd)
#endif

#ifdef HIF_INFO
#define INFO_HIF(cmd) cmd;
#else
#define INFO_HIF(cmd)
#endif

namespace HIF {

// Pull in a number of useful enums from Elemental
using namespace El::DistNS;
using namespace El::DistWrapNS;
using namespace El::LeftOrRightNS;
using ELLR = El::LeftOrRightNS::LeftOrRight;
using namespace El::OrientationNS;
using namespace El::UnitOrNonUnitNS;
using namespace El::UpperOrLowerNS;

template<typename Scalar>
using complex = El::Complex<Scalar>;

// El Matrix and Grid
using El::ElementalMatrix;
using El::Matrix;
using El::SparseMatrix;
using El::Permutation;
using El::Grid;
using El::ValueInt;
using El::Entry;
using El::QRCtrl;

// El functions
using El::Base;
using El::IR;
using El::SwapClear;
using El::FastResize;
using El::Scan;
using El::Abs;
using El::Max;
using El::Min;
using El::Input;
using El::View;
using El::SampleUniform;
using El::SampleNormal;

using El::Axpy;
using El::Gemv;
using El::Trmm;
using El::Gemm;
using El::TriangularInverse;
using El::Transpose;
using El::MakeTrapezoidal;
using El::ID;
using El::LDL;
using El::GetDiagonal;
using El::ldl::MultiplyAfter;
using El::DiagonalScale;

template<typename Scalar>
using Vector = Matrix<Scalar>;

// Pull in some of Elemental's imported libraries
namespace blas = El::blas;
namespace lapack = El::lapack;
// namespace mpi = El::mpi;

// Pull in some of standard libraries imported libraries
using std::vector;
using std::string;
using std::setw;
using std::setprecision;
using std::scientific;
using std::endl;

inline void BuildStream( std::ostringstream& os ) { }

template<typename T, typename... Args>
inline void BuildStream( std::ostringstream& os, T item, Args... args )
{
    os << item;
    HIF::BuildStream( os, args... );
}

void OpenLog( const char* filename );
void LogAppend( bool logAppend );
std::ostream & LogOS();
void CloseLog();
template<typename... Args>
void Log( Args... args )
{
    std::ostringstream str;
    HIF::BuildStream( str, args... );
    LogOS() << str.str() << std::endl;
}

DEBUG_HIF(
    void PushCallStack( string s );
    void PopCallStack();
    void DumpCallStack();

    class CallStackEntry
    {
    public:
        CallStackEntry( string s )
        {
            if( !std::uncaught_exception() )
                PushCallStack(s);
        }
        ~CallStackEntry()
        {
            if( !std::uncaught_exception() )
                PopCallStack();
        }
    };
)

enum TimerType
{
    TIMER_FACTOR=0,
    TIMER_SPARSEELIM,
    TIMER_SKEL,
    TIMER_MERGE,
    TIMER_ROOTFACTOR,
    TIMER_EL,
    TIMER_EL_LDLSOLVE,
    TIMER_EL_ID,
    TIMER_INIT_BUILD,
    TIMER_INIT_FILL,
    TIMER_SETOP,
    TIMER_FILLVEC,
    TIMER_APPLY,
    TIMER_DEBUG,
    TIMER_GETSUBMATRIX,
    TIMER_METIS,
    NUM_OF_TIMER_TYPE
};

TIMER_HIF(
    void InitTimer();
    void TimerStart( TimerType typ );
    double TimerStop( TimerType typ );
    double TimerPartial( TimerType typ );
    double TimerTotal( TimerType typ );
)

template<typename... Args>
void Assert(const char* ex, const char* func, const char* file, int line,
            Args... args)
{
    std::ostringstream msg;
    HIF::BuildStream( msg, args... );

    LogOS() << "Assertion failed: " << msg.str() << "\nExpression "
            << ex << ", " << "function " << func
            << ", file " << file << ", line " << line << "\n";

    std::cerr << "Assertion failed: " << msg.str() << "\nExpression "
              << ex << ", " << "function " << func
              << ", file " << file << ", line " << line << "\n";
    DEBUG_HIF(DumpCallStack())
    std::cerr.flush();
    throw std::logic_error(msg.str());
}

template<typename... Args>
void MasterCout( Args... args )
{
    std::ostringstream str;
    HIF::BuildStream( str, args... );
    std::cout << str.str() << std::endl;
}

template<typename... Args>
void MasterLog( Args... args )
{
    std::ostringstream str;
    HIF::BuildStream( str, args... );
    LogOS() << str.str() << std::endl;
}

void SetTol(double tol);
double Tol();

void SetButton(bool button);
bool Button();

} // namespace HIF.

#endif // ifndef HIF_CORE_ENVIRONMENT_DECL_HPP
