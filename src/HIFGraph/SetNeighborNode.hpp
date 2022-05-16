#include "HIF.hpp"

namespace HIF {

// Set neighbor node.
template<typename Scalar>
void HIFGraph<Scalar>::SetNeighborNode()
{
	// We stand on the parent level to set its children's nbnode.
	if (endflag_ == 1)
	{
		return;
	}

	for (int iter = 0; iter < 2; iter++)
	{
		HIFGraph* childnode = children_[iter];
		childnode->nbnode_.push_pack(children_[1 - iter]);
		childnode->nbnodeseqnum_.push_pack(children_[1 - iter]->seqnum_);
		childnode->nbnodelevel_.push_pack(children_[1 - iter]->level_);
		if (!nbnode_.empty())
		{
			for (int i = 0; i < nbnode_.size(); i++)
			{
				HIFGraph* nbnodei = nbnode_[i];
				if (nbnodei->children_.empty())
				{
					// Now nbnodei doesn't have a child, we should treat it as a nbnode.
					if (Intersect_Sort(childnode->nb_, nbnodei->vtx_))
					{
						// We have to avoid add one's ancestor as its nbnode.
						int dlevel = childnode->level_ - nbnodei->level_;
						int myseqnum = childnode->seqnum_;
						for (int k = 0; k < dlevel; k++)
						{
							myseqnum = myseqnum / 2;
						}
						if (myseqnum == nbnodei->seqnum_)
						{
							break;
						}
						childnode->nbnode_.push_back(nbnodei);
						childnode->nbnodeseqnum_.push_pack(nbnodei->seqnum_);
						childnode->nbnodelevel_.push_pack(nbnodei->level_);
						nbnodei->nbnode_.push_back(childnode);
						nbnodei->nbnodeseqnum_.push_back(childnode->seqnum_);
						nbnodei->nbnodelevel_.push_back(childnode->level_);
					}
				}
			}
		}
		else
		{
			for (int it = 0; it < 2; it++)
			{
				HIFGraph* childnbnodei = nbnodei->children_[it];
				if (Intersect_Sort(childnode->nb_, childnbnodei->vtx_))
				{
					childnode->nbnode_.push_back(childnbnodei);
					childnode->nbnodeseqnum_.push_back(childnbnodei->seqnum_);
					childnode->nbnodelevel_.push_back(childnbnodei->level_);
				}
			}
		}
	}

	// Recursively set neighbor node.
	for (int iter = 0; iter < 2; iter++)
	{
		children_[iter]->SetNeighborNode();
	}
}

}

} // namespace HIF.