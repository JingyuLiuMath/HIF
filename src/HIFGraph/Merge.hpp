#include "HIF.hpp"

namespace HIF {

// Recusively send matrices' information from children to parent.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveMerge(int whatlevel)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveMerge"))

	if (level_ == whatlevel)
	{
		Merge();
	}
	else
	{
		if (endflag_ == 0)
		{
			for (int iter = 0; iter < 2; iter++)
			{
				childrennode_.Child(iter).RecursiveMerge(whatlevel);
			}
		}
	}
}

// Send matrices' information from children to parent.
template <typename Scalar>
void HIFGraph<Scalar>::Merge()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Merge"))

	// We stand on the parent level.
	if (endflag_ == 1)
	{
		return;
	}

	childreninfo_.resize(2);

	// First we tell the parent what its intr, sep, nb is after we eliminate the children's intr and re.

	// intr: children's sk - sep.
	// sep : sep \cup children's sk.
	// nb: nb \cup children's nbsk.
	/*vector<int> tmpnbsk;*/
	for (int iter = 0; iter < 2; iter++)
	{
		for (int i = 0; i < childrennode_.Child(iter).sk_.size(); i++)
		{
			intr_.push_back(childrennode_.Child(iter).sk_[i]);
		}
		/*for (int i = 0; i < childrennode_.Child(iter).nbsk_.size(); i++)
		{
			tmpnbsk.push_back(childrennode_.Child(iter).nbsk_[i]);
		}*/
	}
	std::sort(intr_.begin(), intr_.end());
	/*std::sort(tmpnbsk.begin(), tmpnbsk.end());
	Unique_Sort(tmpnbsk);*/
	/*vector<int> tmpsep;
	Intersect_Sort(sep_, intr_, tmpsep);
	sep_.assign(tmpsep.begin(), tmpsep.end());*/
	vector<int> tmpintr;
	Diff_Sort(intr_, sep_, tmpintr);
	intr_.assign(tmpintr.begin(), tmpintr.end());
	/*vector<int> tmpnb;
	Intersect_Sort(nb_, tmpnbsk, tmpnb);
	nb_.assign(tmpnb.begin(), tmpnb.end());*/

	// Next we assign the corresponding matrices blockly.

	// AII
	// An intr of the parent only belongs to the sep of one of its children.
	// If two intrs belong to the same child, we assign AII from the child's ASS. 
	// Otherwise, we assign AII from one child's ANS or 0.
	El::Zeros(AII_, intr_.size(), intr_.size());

	vector<int> intr1;
	vector<int>& myindex_intr1 = childreninfo_[0].myindex_intr;
	vector<int>& cindex_intr1 = childreninfo_[0].cindex_intr;
	Intersect_Sort(intr_, childrennode_.Child(0).vtx_, intr1, myindex_intr1, 1);
	FindAllIndex_Sort(intr1, childrennode_.Child(0).sep_, cindex_intr1);
	
	vector<int> intr2;
	vector<int>& myindex_intr2 = childreninfo_[1].myindex_intr;
	vector<int>& cindex_intr2 = childreninfo_[1].cindex_intr;
	Intersect_Sort(intr_, childrennode_.Child(1).vtx_, intr2, myindex_intr2, 1);
	FindAllIndex_Sort(intr2, childrennode_.Child(1).sep_, cindex_intr2);
	
	vector<int> myindex_intr21;
	vector<int> cindex_intr21;
	Intersect_Sort(intr_, childrennode_.Child(0).nb_, myindex_intr21, cindex_intr21);
	
	SubMatrixUpdate(AII_, myindex_intr1, myindex_intr1, childrennode_.Child(0).ASS_(cindex_intr1, cindex_intr1));

	SubMatrixUpdate(AII_, myindex_intr2, myindex_intr2, childrennode_.Child(1).ASS_(cindex_intr2, cindex_intr2));

	SubMatrixUpdate(AII_, myindex_intr21, myindex_intr1, childrennode_.Child(0).ANS_(cindex_intr21, cindex_intr1));

	SubMatrixTransposeUpdate(AII_, myindex_intr1, myindex_intr2, AII_(myindex_intr2, myindex_intr1));

	// ASI
	// A sep of the parent only belongs to the sep of one of its children.
	// If an intr and a sep belongs to the same child, we assign ASI from the child's ASS.
	// Otherwise, we assign ASI from one child's ANS or 0.
	El::Zeros(ASI_, sep_.size(), intr_.size());
	
	vector<int> myindex_sep1x;
	vector<int> cindex_sep1x;
	Intersect_Sort(sep_, childrennode_.Child(0).sep_, myindex_sep1x, cindex_sep1x);

	vector<int> myindex_sep1y;
	vector<int> cindex_sep1y;
	Intersect_Sort(sep_, childrennode_.Child(0).nb_, myindex_sep1y, cindex_sep1y);

	vector<int> myindex_sep2x;
	vector<int> cindex_sep2x;
	Intersect_Sort(sep_, childrennode_.Child(1).sep_, myindex_sep2x, cindex_sep2x);

	vector<int> myindex_sep2y;
	vector<int> cindex_sep2y;
	Intersect_Sort(sep_, childrennode_.Child(1).nb_, myindex_sep2y, cindex_sep2y);

	SubMatrixUpdate(ASI_, myindex_sep1x, myindex_intr1, childrennode_.Child(0).ASS_(cindex_sep1x, cindex_intr1));

	SubMatrixUpdate(ASI_, myindex_sep2x, myindex_intr2, childrennode_.Child(1).ASS_(cindex_sep2x, cindex_intr2));

	SubMatrixUpdate(ASI_, myindex_sep1y, myindex_intr1, childrennode_.Child(0).ANS_(cindex_sep1y, cindex_intr1));

	SubMatrixUpdate(ASI_, myindex_sep2y, myindex_intr2, childrennode_.Child(1).ANS_(cindex_sep2y, cindex_intr2));

	// ASS
	// If two seps belongs to the same child, we assign ASS from the child's ASS. 
	// Otherwise, we assign ASS from one child's ANS or 0.
	El::Zeros(ASS_, sep_.size(), sep_.size());
	
	vector<int> sep1;
	vector<int>& myindex_sep1 = childreninfo_[0].myindex_sep;
	vector<int>& cindex_sep1 = childreninfo_[0].cindex_sep;
	Intersect_Sort(sep_, childrennode_.Child(0).vtx_, sep1, myindex_sep1, 1);
	FindAllIndex_Sort(sep1, childrennode_.Child(0).sep_, cindex_sep1);

	vector<int> sep2;
	vector<int>& myindex_sep2 = childreninfo_[1].myindex_sep;
	vector<int>& cindex_sep2 = childreninfo_[1].cindex_sep;
	Intersect_Sort(sep_, childrennode_.Child(1).vtx_, sep2, myindex_sep2, 1);
	FindAllIndex_Sort(sep2, childrennode_.Child(1).sep_, cindex_sep2);

	vector<int> myindex_sep21;
	vector<int> cindex_sep21;
	Intersect_Sort(sep_, childrennode_.Child(0).nb_, myindex_sep21, cindex_sep21);

	SubMatrixUpdate(ASS_, myindex_sep1, myindex_sep1, childrennode_.Child(0).ASS_(cindex_sep1, cindex_sep1));

	SubMatrixUpdate(ASS_, myindex_sep2, myindex_sep2, childrennode_.Child(1).ASS_(cindex_sep2, cindex_sep2));

	SubMatrixUpdate(ASS_, myindex_sep21, myindex_sep1, childrennode_.Child(0).ANS_(cindex_sep21, cindex_sep1));

	SubMatrixTransposeUpdate(ASS_, myindex_sep1, myindex_sep2, ASS_(myindex_sep2, myindex_sep1));

	// ANS
	// If a nb and a sep in the same child, we assign ANS from the child's ANS.
	// Otherwise, ANS= 0.
	El::Zeros(ANS_, nb_.size(), sep_.size());
	
	vector<int> myindex_nb1x;
	vector<int> cindex_nb1x;
	Intersect_Sort(nb_, childrennode_.Child(0).nb_, myindex_nb1x, cindex_nb1x);

	vector<int> myindex_nb2x;
	vector<int> cindex_nb2x;
	Intersect_Sort(nb_, childrennode_.Child(1).nb_, myindex_nb2x, cindex_nb2x);

	SubMatrixUpdate(ANS_, myindex_nb1x, myindex_sep1, childrennode_.Child(0).ANS_(cindex_nb1x, cindex_sep1));

	SubMatrixUpdate(ANS_, myindex_nb2x, myindex_sep2, childrennode_.Child(1).ANS_(cindex_nb2x, cindex_sep2));

	// Clear children's unnecessary information.
	for (int iter = 0; iter < 2; iter++)
	{
		childrennode_.Child(iter).FactorClear();
	}
}

// Clear unnecessary information.
template <typename Scalar>
void HIFGraph<Scalar>::FactorClear()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:FactorClear"))

	ASS_.Empty();
	ANS_.Empty();
}

} // namespace HIF.
