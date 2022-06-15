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
		Log(" Level  Blocks     tRank     aRank",
		"    totMem   levTime    ElTime    SpElim",
		"    SpComm      Skel    SkComm     Merge    RootMg",
		"    RootFt   ElSolve      ElID");
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

		/*INFO_HIF
		(
			TIMER_HIF(TimerStart(TIMER_MEM_COLL))
			int SumRank = RankCollection(endLevel);
			double totMem = MemCollection(endLevel);
			TIMER_HIF(TimerStop(TIMER_MEM_COLL))
		)*/

		// Merge.
		TIMER_HIF(TimerStart(TIMER_MERGE))
		RecursiveMerge(tmplevel - 1);
		TIMER_HIF(TimerStop(TIMER_MERGE))

		TIMER_HIF(TimerStop(TIMER_FACTOR))

		/*INFO_HIF
		(
			Log(setw(6), endLevel,
				setw(8), std::pow(8, endLevel),
				setw(10), SumRank,
				setw(10), double(SumRank) / std::pow(8, endLevel),
				setw(10), scientific, setprecision(2), totMem,
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_FACTOR)
				- TimerPartial(TIMER_MEM_COLL),
				setw(10), scientific, setprecision(2),
				TimerTotal(TIMER_EL),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_SPARSE_ELIM),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_SPARSE_ELIM_COMM),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_SKEL),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_SKEL_COMM),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_MERGE_COMM),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_ROOT_MERGE),
				setw(10), scientific, setprecision(2),
				TimerPartial(TIMER_ROOT_FACTOR),
				setw(10), scientific, setprecision(2),
				TimerTotal(TIMER_EL_SOLVE),
				setw(10), scientific, setprecision(2),
				TimerTotal(TIMER_EL_ID));
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

	/*INFO_HIF
	(
		double totMem = MemCollection();
		Log(setw(6), 0,
			setw(8), 1,
			setw(10), AAllinv_.Height(),
			setw(10), AAllinv_.Width(),
			setw(10), scientific, setprecision(2), totMem,
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_FACTOR)
			- TimerTotal(TIMER_MEM_COLL),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_EL),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_SPARSE_ELIM),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_SPARSE_ELIM_COMM),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_SKEL),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_SKEL_COMM),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_MERGE_COMM),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_ROOT_MERGE),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_ROOT_FACTOR),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_EL_SOLVE),
			setw(10), scientific, setprecision(2),
			TimerTotal(TIMER_EL_ID));
		Log("=================================================",
			"=========================");
		//ClearMemCheck();
	)*/
}

} // namespace HIF.
