#include "HIF.hpp"

namespace HIF {

// Apply on the root.
template <typename Scalar>
void HIFGraph<Scalar>::RootApply()
{
    // xI = LI^{-1} * xI.
    Trmm(ELLR::LEFT, LOWER, NORMAL, UNIT, Scalar(1), AIIinv_, xI_);
    // xI = DI^{-1} * xI.
    auto D = GetDiagonal(AIIinv_);
    DiagonalScale(ELLR::LEFT, NORMAL, D, xI_);
    // xI = LI^{-T} * xI.
    Trmm(ELLR::LEFT, LOWER, TRANSPOSE, UNIT, Scalar(1), AIIinv_, xI_);
}

} // namespace HIF.