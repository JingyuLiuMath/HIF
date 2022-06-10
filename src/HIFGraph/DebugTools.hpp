#include "HIF.hpp"

namespace HIF {

// Show partition results.
template <typename Scalar>
void HIFGraph<Scalar>::ShowPartition()
{
	if (endflag_ == 1)
	{
		std::cout << " curren level " << level_ << std::endl;;
		std::cout << " curren seqnum " << seqnum_ << std::endl;
		std::cout << " vtx " << vtx_.size() << std::endl;
		for (int i = 0; i < vtx_.size(); i++)
		{
			std::cout << vtx_[i] << std::endl;
		}
		/*std::cout << " sep " << sep_.size()  << std::endl;
		for (int i = 0; i < sep_.size(); i++)
		{
			std::cout << sep_[i] << std::endl;
		}
		std::cout << " nb " << nb_.size() << std::endl;
		for (int i = 0; i < nb_.size(); i++)
		{
			std::cout << nb_[i] << std::endl;
		}*/
	}
	else 
	{
		for (int iter = 0; iter < 2; iter++)
		{
			children_[iter]->ShowPartition();
		}
	}
}

// Show matrix.
template <typename Scalar>
void HIFGraph<Scalar>::ShowMatrix(const MatrixS& A)
{
	std::cout << "Height " << A.Height() << std::endl;
	std::cout << "Width " << A.Width() << std::endl;
	std::cout << " [ " << std::endl;
	for (int i = 0; i < A.Height(); i++)
	{
		for (int j = 0; j < A.Width(); j++)
		{
			std::cout << A.Get(i, j) << ",";
		}
		std::cout << ";" << std::endl;
	}
	std::cout << " ] " << std::endl;
}

// Show vector.
template <typename Scalar>
void HIFGraph<Scalar>::ShowVector(const vector<int>& v)
{
	std::cout << " Size " << v.size() << std::endl;
	std::cout << " [ " << std::endl;
	for (int i = 0; i < v.size(); i++)
	{
		std::cout << v[i] << ";" << std::endl;
	}
	std::cout << " ] " << std::endl;
}

} // namespace HIF.