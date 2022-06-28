#include "HIF.hpp"

namespace HIF {

// Show partition results.
template <typename Scalar>
void HIFGraph<Scalar>::ShowPartition()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:ShowPartition"))

	if (endflag_ == 1)
	{
		std::cout << " curren level " << level_ << std::endl;;
		std::cout << " curren seqnum " << seqnum_ << std::endl;
		std::cout << " vtx " << vtx_.size() << std::endl;
		std::cout << "[" << std::endl;
		for (int i = 0; i < vtx_.size(); i++)
		{
			std::cout << vtx_[i] << ";" << std::endl;
		}
		std::cout << "]" << std::endl;
		std::cout << " sep " << sep_.size() << std::endl;
		std::cout << "[" << std::endl;
		for (int i = 0; i < sep_.size(); i++)
		{
			std::cout << sep_[i] << ";" << std::endl;
		}
		std::cout << "]" << std::endl;
		std::cout << " nb " << nb_.size() << std::endl;
		std::cout << "[" << std::endl;
		for (int i = 0; i < nb_.size(); i++)
		{
			std::cout << nb_[i] << ";" << std::endl;
		}
		std::cout << "]" << std::endl;
	}
	else 
	{
		for (int iter = 0; iter < 2; iter++)
		{
			children_[iter]->ShowPartition();
		}
	}
}

} // namespace HIF.
