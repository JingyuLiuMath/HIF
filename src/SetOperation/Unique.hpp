#include "HIF.hpp"

namespace HIF {

// vec = unique(vec) where vec is a sorted vec.
template <typename Scalar>
void Unique_Sort(vector<Scalar>& vec)
{
	DEBUG_HIF(CallStackEntry cse("Unique_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	/*std::vector<Scalar>::iterator it;
	it = std::unique(vec.begin(), vec.end());
	vec.resize(std::distance(vec.begin(), it));*/
	TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.
