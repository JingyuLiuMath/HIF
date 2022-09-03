#include "HIF.hpp"

namespace HIF {

// Set neighbor node.
template<typename Scalar>
void HIFGraph<Scalar>::SetNeighborNode()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:SetNeighborNode"))

	// We stand on the parent level to set its children's nbnode.
	if (endflag_ == 1)
	{
		return;
	}

	for (int iter = 0; iter < 2; iter++)
	{
		HIFGraph& childnode = childrennode_.Child(iter);
		childnode.nbnode_.push_back(childrennode_.PtChild(1 - iter));
		childnode.nbnodeseqnum_.push_back(childrennode_.Child(1 - iter).seqnum_);
		childnode.nbnodelevel_.push_back(childrennode_.Child(1 - iter).level_);
		for (int i = 0; i < nbnode_.size(); i++)
		{
			HIFGraph& nbnodei = *nbnode_[i];
			if (nbnodei.endflag_ == 1)
			{
				// Now nbnodei doesn't have a child, we should treat it as a nbnode.
				if (Intersect_Sort(childnode.nb_, nbnodei.sep_))
				{
					// We have to avoid add one's ancestor as its nbnode.
					int dlevel = childnode.level_ - nbnodei.level_;
					int myseqnum = childnode.seqnum_;
					for (int k = 0; k < dlevel; k++)
					{
						myseqnum = myseqnum / 2;
					}
					if (myseqnum == nbnodei.seqnum_)
					{
						continue;
					}
					childnode.nbnode_.push_back(nbnode_[i]);
					childnode.nbnodeseqnum_.push_back(nbnodei.seqnum_);
					childnode.nbnodelevel_.push_back(nbnodei.level_);
					/*nbnodei.nbnode_.push_back(childrennode_.PtChild(iter));
					nbnodei.nbnodeseqnum_.push_back(childnode.seqnum_);
					nbnodei.nbnodelevel_.push_back(childnode.level_);*/
				}
			}
			else
			{
				for (int it = 0; it < 2; it++)
				{
					HIFGraph& childnbnodei = nbnodei.childrennode_.Child(it);
					if (Intersect_Sort(childnode.nb_, childnbnodei.sep_))
					{
						childnode.nbnode_.push_back(nbnodei.childrennode_.PtChild(it));
						childnode.nbnodeseqnum_.push_back(childnbnodei.seqnum_);
						childnode.nbnodelevel_.push_back(childnbnodei.level_);
					}
				}
			}
		}
	}

	// Recursively set neighbor node.
	for (int iter = 0; iter < 2; iter++)
	{
		childrennode_.Child(iter).SetNeighborNode();
	}
}

} // namespace HIF.
