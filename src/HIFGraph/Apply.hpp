#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::Apply(MatrixS& b)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Apply"))

	INFO_HIF
	(
		Log(
			"==================================================",
			"==================================================",
			"=================================================="
		);
		Log(
			"        FillVecT        ApplyTime"
		);
	)

	vector<int> xcol;
	RangeVec(0, b.Width(), xcol);
	
	// Fill vector in the tree.
	TIMER_HIF(TimerStart(TIMER_FILLVEC))
	FillVecTree(b, xcol);
	TIMER_HIF(TimerStop(TIMER_FILLVEC))

	TIMER_HIF(TimerStart(TIMER_APPLY))

	// ApplyUp.
	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		RecursiveApplySparseElimUp(tmplevel);
		RecursiveApplySkelUp(tmplevel, xcol);
		RecursiveApplyMerge(tmplevel - 1, xcol);
	}
	
	// Root apply.
	RootApply();

	// ApplyDown.
	for (int tmplevel = 1; tmplevel <= numlevels_; tmplevel++)
	{
		RecursiveApplySplit(tmplevel - 1, xcol);
		RecursiveApplySkelDown(tmplevel, xcol);
		RecursiveApplySparseElimDown(tmplevel);
	}
	
	// GetSolution.
	GetSolution(b, xcol);

	TIMER_HIF(TimerStop(TIMER_APPLY))

	INFO_HIF
	(
		Log(
			setw(16), scientific, setprecision(2), TimerTotal(TIMER_FILLVEC),
			setw(17), scientific, setprecision(2), TimerTotal(TIMER_APPLY)
		);
		Log(
			"==================================================",
			"==================================================",
			"=================================================="
		);
	)
}

} // namespace HIF.
