#include "HIF.hpp"

namespace HIF {

// Factorization
template <typename Scalar>
void HIFGraph<Scalar>::Factorization()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Factorization"))

	double setoptimecutoff = TimerTotal(TIMER_SETOP);

	// ElTime, ElLDL, ElID, SetOperation are accumulative.
	// Others' time is only about their level except on level 0.
	INFO_HIF
	(
		Log(
			"==================================================",
			"==================================================",
			"=================================================="
		);
		Log(
			"        Level        LevelTime        ElTime",        
			"        SparseElim        Skel        Merge",
			"        ElLDL        ElID        SetOperation"
		);
		Log(
			"--------------------------------------------------",
			"--------------------------------------------------",
			"--------------------------------------------------"
		);
	)

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
		RecursiveSetSeparatorType(tmplevel - 1);
		TIMER_HIF(TimerStop(TIMER_MERGE))

		TIMER_HIF(TimerStop(TIMER_FACTOR))

		INFO_HIF
		(
			Log(
				setw(13), tmplevel,
				setw(17), scientific, setprecision(2),TimerPartial(TIMER_FACTOR),
				setw(14), scientific, setprecision(2),TimerTotal(TIMER_EL),
				setw(18), scientific, setprecision(2), TimerPartial(TIMER_SPARSEELIM),
				setw(12), scientific, setprecision(2), TimerPartial(TIMER_SKEL),
				setw(13), scientific, setprecision(2), TimerPartial(TIMER_MERGE),
				setw(13), scientific, setprecision(2), TimerTotal(TIMER_EL_LDLSOLVE),
				setw(12), scientific, setprecision(2), TimerTotal(TIMER_EL_ID),
				setw(20), scientific, setprecision(2), TimerTotal(TIMER_SETOP)-setoptimecutoff
				);
			Log(
				"--------------------------------------------------",
				"--------------------------------------------------",
				"--------------------------------------------------"
			);
		)
	}

	TIMER_HIF(TimerStart(TIMER_FACTOR))

	// Root factorization.
	TIMER_HIF(TimerStart(TIMER_ROOTFACTOR))
	RootFactorization();
	TIMER_HIF(TimerStop(TIMER_ROOTFACTOR))

	TIMER_HIF(TimerStop(TIMER_FACTOR))

	INFO_HIF
	(
		Log(
			setw(13), 0,
			setw(17), scientific, setprecision(2), TimerTotal(TIMER_FACTOR),
			setw(14), scientific, setprecision(2), TimerTotal(TIMER_EL),
			setw(18), scientific, setprecision(2), TimerTotal(TIMER_SPARSEELIM),
			setw(12), scientific, setprecision(2), TimerTotal(TIMER_SKEL),
			setw(13), scientific, setprecision(2), TimerTotal(TIMER_MERGE),
			setw(13), scientific, setprecision(2), TimerTotal(TIMER_EL_LDLSOLVE),
			setw(12), scientific, setprecision(2), TimerTotal(TIMER_EL_ID),
			setw(20), scientific, setprecision(2), TimerTotal(TIMER_SETOP)- setoptimecutoff
		);
		Log(
			"==================================================",
			"==================================================",
			"=================================================="
		);
	)
}

} // namespace HIF.
