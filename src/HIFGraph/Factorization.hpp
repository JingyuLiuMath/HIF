#include "HIF.hpp"

namespace HIF {

// Factorization
template <typename Scalar>
void HIFGraph<Scalar>::Factorization()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Factorization"))

	/*INFO_HIF
	(
		Log("=================================================",
			"=========================");
		Log(" Level    LevelTime    ElTime    SpElim    Skel    Merge    ElSolve    ElID");
		Log("-------------------------------------------------",
			"-------------------------");
	)*/

	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		TIMER_HIF(TimerStart(TIMER_FACTOR))

		// Sparse elimination.
		TIMER_HIF(TimerStart(TIMER_SPARSEELIM))
		RecursiveSparseElim(tmplevel);
		TIMER_HIF(TimerStop(TIMER_SPARSEELIM))

		// Skeletonization.
		TIMER_HIF(TimerStart(TIMER_SKEL))
		RecursiveSkel(tmplevel);
		TIMER_HIF(TimerStop(TIMER_SKEL))

		// Merge.
		TIMER_HIF(TimerStart(TIMER_MERGE))
		RecursiveMerge(tmplevel - 1);
		TIMER_HIF(TimerStop(TIMER_MERGE))

		TIMER_HIF(TimerStop(TIMER_FACTOR))

		/*INFO_HIF
		(
			Log();
			Log("-------------------------------------------------",
				"-------------------------");
		)*/
	}

	TIMER_HIF(TimerStart(TIMER_FACTOR))

	// Root factorization.
	TIMER_HIF(TimerStart(TIMER_ROOTFACTOR))
	RootFactorization();
	TIMER_HIF(TimerStop(TIMER_ROOTFACTOR))

	TIMER_HIF(TimerStop(TIMER_FACTOR))
}

} // namespace HIF.
