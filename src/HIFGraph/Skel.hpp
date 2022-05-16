#include "HIF.hpp"

namespace HIF {

// Recursively skeletonization.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveSkel(int whatlevel, double tol)
{
	if (level_ == whatlevel)
	{
		Skel(tol);
		// NoSkel();
	}
	else
	{
		if (endflag_ == 0)
		{
			for (int iter = 0; iter < 2; iter++)
			{
				children_[iter].RecursiveSkel(whatlevel, tol);
			}
		}
	}

}

// Skeletonization.
template<typename Scalar>
void HIFGraph<Scalar>::Skel(double tol)
{
	QRCtrl<Base<Scalar>> ctrl;
	ctrl.boundRank = false;
	ctrl.maxRank = 1000;
	ctrl.adaptive = true;
	ctrl.tol = tol;
	ctrl.smallestFirst = false;

	nbinfo_.resize(nbnode_.size());

	for (int k = 0; k < nbnode_.size(); k++)
	{
		HIFGraph* nodek = nbnode_[k];
		if (nodek->level_ != level_)
		{
			nbinfo_[k].empty = 1;
			continue;
		}
		if (nodek->seqnum_ < seqnum_) {
			nbinfo_[k].empty = 1;
			continue;
		}
		
		// The following data are vertices.
		vector<int> sep1(singlesep_[k]);
		vector<int> mysep1C;
		for (int nok = 0; nok < nbnode_.size(); nok++)
		{
			if (nok == k)
			{
				continue;
			}
			else
			{
				mysep1C.insert(mysep1C.end(), singlesep_[nok].begin(), singlesep_[nok].end());
			}
		}
		mysep1C.insert(mysep1C.end(), complexsep_.begin(), complexsep_.end());
		sort(mysep1C.begin(), mysep1C.end());
		vector<int> nodeksep1C;
		Diff_Sort(nodek->nb_, sep1, nodeksep1C);

		int korder = 0;
		vector<int> veckorder;
		FindAllIndex(nodek->nbnodeseqnum_, seqnum_, veckorder);
		if £¨veckorder.size() == 1£©
		{
			korder = veckorder[0];
		}
		else
		{
			vector<int> vecklevel;
			FindAllIndex(nodek->nbnodelevel_, level_, vecklevel);
			vector<int> vectmp;
			Intersect_Sort(veckorder, vecklevel, vectmp);
			korder = vectmp[0];
		}
		vector<int> sep2(nodek->singlesep_[korder]);
		vector<int> nodeksep2C;
		for (int nok = 0; nok < (nodek->nbnode_).size(); nok++)
		{
			if (nok == korder)
			{
				continue;
			}
			else
			{
				nodeksep2C.insert(nodeksep2C.end(), (nodek->singlesep_[nok]).begin(), (nodek->singlesep_[nok]).end());
			}
		}
		nodeksep2C.insert(mysep1C.end(), (nodek->complexsep_).begin(), (nodek->complexsep_).end());
		sort(nodeksep2C.begin(), nodeksep2C.end());
		vector<int> mysep2C;
		Diff_Sort(nb_, sep2, mysep2C);

		// % The following data are indices.
		vector<int> myindex_sep1;
		FindAllIndex_Sort(sep1, sep_, myindex_sep1);
		vector<int> nodekindex_sep1;
		FindAllIndex_Sort(sep1, nodek->nb_, nodekindex_sep1);
		vector<int> myindex_sep1C;
		FindAllIndex_Sort(mysep1C, sep_, myindex_sep1C);
		vector<int> myindex_mysep2C;
		FindAllIndex_Sort(mysep2C, nb_, myindex_mysep2C);
		
		vector<int> nodekindex_sep2;
		FindAllIndex_Sort(sep2, nodek->sep_, nodekindex_sep2);
		vector<int> myindex_sep2;
		FindAllIndex_Sort(sep2, nb_, myindex_sep2);
		vector<int> nodekindex_sep2C;
		FindAllIndex_Sort(nodeksep2C, nodek->sep_, nodekindex_sep2C);
		vector<int> nodekindex_nodeksep1C;
		FindAllIndex_Sort(nodeksep1C, nodek->nb_, nodekindex_nodeksep1C);

		// ID decomposition.
		// In the following process, the first "1" or "2" denotes my or nodek, 
		// the second "1" or "2" denotes sk or re.

		/*skelmtx1 = [obj.ASS(myindex_sep1C, myindex_sep1);
			obj.ANS(myindex_mysep2C, myindex_sep1)];
		skelmtx2 = [nodek.ASS(nodekindex_sep2C, nodekindex_sep2);
			nodek.ANS(nodekindex_nodeksep1C, nodekindex_sep2)];*/
		MatrixS skelmtx1;
		MatrixS skelmtx2;
		if ((myindex_sep1C.size() + myindex_mysep2C.size() == 0) || 
			(myindex_sep1.size() + myindex_sep1.size() == 0) ||
			(nodekindex_sep2C.size() + nodekindex_nodeksep1C.size() == 0) ||
			(nodekindex_sep2.size() + nodekindex_sep2.size() == 0))
		{
			nbinfo_[k].empty = 1;
			continue;
		}
		El:zeros(skelmtx1, 
			myindex_sep1C.size() + myindex_mysep2C.size(),
			myindex_sep1.size() + myindex_sep1.size());
		El:zeros(skelmtx2,
			nodekindex_sep2C.size() + nodekindex_nodeksep1C.size(),
			nodekindex_sep2.size() + nodekindex_sep2.size());
		MatrixS viewmtx;
		View(viewmtx, skelmtx1, 
			0, 0, 
			myindex_sep1C.size(), myindex_sep1.size());
		viewmtx = ASS_(myindex_sep1C, myindex_sep1);
		View(viewmtx, skelmtx1, 
			myindex_sep1C.size(), 0, 
			myindex_mysep2C.size(), myindex_sep1.size());
		viewmtx = ANS_(myindex_mysep2C, myindex_sep1);
		View(viewmtx, skelmtx2,
			0, 0,
			nodekindex_sep2C.size(), nodekindex_sep2.size());
		viewmtx = nodek->ASS_(nodekindex_sep2C, nodekindex_sep2);
		View(viewmtx, skelmtx2,
			nodekindex_sep2C.size(), 0,
			nodekindex_nodeksep1C.size(), nodekindex_sep2.size());
		viewmtx = nodek->ANS_(nodekindex_nodeksep1C, nodekindex_sep2);

		vector<int> p11;
		vector<int> p12;
		MatrixS& T1 = nbinfo_[k].Tc1h1;
		IDSolve(skelmtx1, T1, p11, p12, ctrl); // skelmtx(:, p12) = skelmtx(:, p11) * T.
		vector<int>& myindex_p11 = nbinfo_[k].myindex_p11;
		vector<int>& myindex_p12 = nbinfo_[k].myindex_p12;
		vector<int>& nodekindex_p11 = nbinfo_[k].nodekindex_p11;
		vector<int>& nodekindex_p12 = nbinfo_[k].nodekindex_p12;
		/*myindex_p11 = myindex_sep1(p11);
		myindex_p12 = myindex_sep1(p12);
		nodekindex_p11 = nodekindex_sep1(p11);
		nodekindex_p12 = nodekindex_sep1(p12);
		obj.re = [obj.re, sep1(p12)];
		nodek.nbre = [nodek.nbre, sep1(p12)];*/
		myindex_p11.resize(p11.size());
		myindex_p12.resize(p12.size());
		nodekindex_p11.resize(p11.size());
		nodekindex_p12.resize(p12.size());
		for (int i = 0; i < p11.size(); i++)
		{
			myindex_p11[i] = myindex_sep1[p11[i]];
			nodekindex_p11[i] = nodekindex_sep1[p11[i]];
		}
		for (int i = 0; i < p12.size(); i++)
		{
			myindex_p12[i] = myindex_sep1[p12[i]];
			nodekindex_p12[i] = nodekindex_sep1[p12[i]];
			re_.push_back(sep1[p12[i]]);
			(nodek->nbre_).push_back(sep1[p12[i]]);
		}

		vector<int> p21;
		vector<int> p22;
		MatrixS& T2 = nbinfo_[k].Tc2h2;
		IDSolve(skelmtx1, T2, p21, p22, ctrl); // skelmtx(:, p22) = skelmtx(:, p21) * T.
		vector<int>& myindex_p21 = nbinfo_[k].myindex_p21;
		vector<int>& myindex_p22 = nbinfo_[k].myindex_p22;
		vector<int>& nodekindex_p21 = nbinfo_[k].nodekindex_p21;
		vector<int>& nodekindex_p22 = nbinfo_[k].nodekindex_p22;
		/*myindex_p21 = myindex_sep2(p21);
		myindex_p22 = myindex_sep2(p22);
		nodekindex_p21 = nodekindex_sep2(p21);
		nodekindex_p22 = nodekindex_sep2(p22);
		nodek.re = [nodek.re, sep2(p22)];
		obj.nbre = [obj.nbre, sep2(p22)];*/
		myindex_p21.resize(p21.size());
		myindex_p22.resize(p22.size());
		nodekindex_p21.resize(p21.size());
		nodekindex_p22.resize(p22.size());
		for (int i = 0; i < p21.size(); i++)
		{
			myindex_p21[i] = myindex_sep2[p11[i]];
			nodekindex_p21[i] = nodekindex_sep2[p11[i]];
		}
		for (int i = 0; i < p22.size(); i++)
		{
			myindex_p22[i] = myindex_sep2[p12[i]];
			nodekindex_p22[i] = nodekindex_sep2[p12[i]];
			nbre_.push_back(sep2[p22[i]]);
			(nodek->re_).push_back(sep2[p22[i]]);
		}

		nbinfo_[k].empty = 0;

		// Step 1.
		// Ac1c1 = Ac1c1 - Ah1c1^{T} * Th1c1 - Th1c1^{T} * Ah1c1 + Th1c1^{T} * Ah1h1 * Th1c1.
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), ASS_(myindex_p11,myindex_p12), T1,
			Scalar(1), ASS_(myindex_p12, myindex_p12));
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), T1, ASS_(myindex_p11, myindex_p12),
			Scalar(1), ASS_(myindex_p12, myindex_p12));
		MatrixS tmpmtx;
		Gemm(NORMAL, NORMAL,
			Scalar(1), ASS_(myindex_p11, myindex_p11), T1,
			Scalar(0), tmpmtx);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), T1, tmpmtx,
			Scalar(1), ASS_(myindex_p12, myindex_p12));
		tmpmtx.Empty();
		// Ah1c1 = Ah1c1 - Ah1h1 * Th1c1.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ASS_(myindex_p11, myindex_p11), T1,
			Scalar(1), ASS_(myindex_p11, myindex_p12));
		Transpose(ASS_(myindex_p11, myindex_p12), ASS_(myindex_p12, myindex_p11));
		// Ac2c1 = Ac2c1 - Ac2h1 * Th1c1 - Th2c2^{T} * Ah2c1 + Th2c2^{T} * Ah2h1 * Th1c1.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p11), T1,
			Scalar(1), ANS_(myindex_p22, myindex_p12));
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), T2, ANS_(myindex_p21, myindex_p12),
			Scalar(1), ANS_(myindex_p22, myindex_p12));
		Gemm(NORMAL, NORMAL,
			Scalar(1), ANS_(myindex_p21, myindex_p11), T1,
			Scalar(0), tmpmtx);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), T2, tmpmtx,
			Scalar(1), ANS_(myindex_p22, myindex_p12));
		tmpmtx.Empty();
		Transpose(ANS_(myindex_p22, myindex_p12), (nodek->ANS_)(nodekindex_p12, nodekindex_p22));
		// Ah2c1 = Ah2c1 - Ah2h1 * Th1c1.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p11), T1,
			Scalar(1), ANS_(myindex_p21, myindex_p12));
		Transpose(ANS_(myindex_p21, myindex_p12), (nodek->ANS_)(nodekindex_p12, nodekindex_p21));
		// Ac2h1 = Ac2h1 - Th2c2^{T} * Ah2h1.
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), T2, ANS_(myindex_p21, myindex_p11),
			Scalar(1), ANS_(myindex_p22, myindex_p11));
		Transpose(ANS_(myindex_p22, myindex_p11), (nodek->ANS_)(nodekindex_p11, nodekindex_p22));
		// Ac2c2 = Ac2c2 - Ah2c2^{T} * Th2c2 - Th2c2^{T} * Ah2c2 + Th2c2^{T} * Ah2h2 * Th2c2.
		// nodek.ASS(nodekindex_p22, nodekindex_p22) =  T2' * Ah2h2T2;
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22), T2,
			Scalar(1), (nodek->ASS_)(nodekindex_p22, nodekindex_p22));
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), T2, (nodek->ASS_)(nodekindex_p21, nodekindex_p22),
			Scalar(1), (nodek->ASS_)(nodekindex_p22, nodekindex_p22));
		Gemm(NORMAL, NORMAL,
			Scalar(1), (nodek->ASS_)(nodekindex_p21, nodekindex_p21),
			Scalar(1), tmpmtx);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), T2, tmpmtx,
			Scalar(1), (nodek->ASS_)(nodekindex_p22, nodekindex_p22));
		tmpmtx.Empty();
		// Ah2c2 = Ah2c2 - Ah2h2 * Th2c2.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), (nodek->ASS_)(nodekindex_p21, nodekindex_p21), T2,
			Scalar(1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22));
		Transpose((nodek->ASS_)(nodekindex_p21, nodekindex_p22), (nodek->ASS_)(nodekindex_p21, nodekindex_p21));

		// Step 2.
		// Ac1c1 = Lc1 * Dc1 * Lc1^{T}.
		LDLSolve(ASS_(myindex_p12,myindex_p12), nbinfo_[k].Ac1c1inv);
		// Ac1c1invAc1h1 = Ac1c1^{-1} * Ah1c1^{T}.
		Transpose(ASS_(myindex_p11, myindex_p12), nbinfo_[k].Ac1c1invAc1h1);
		MultiplySolve(nbinfo_[k].Ac1c1inv, nbinfo_[k].Ac1c1invAc1h1);
		// Ac1c1invAc1c2 = Ac1c1^{-1} * Ac2c1^{T}.
		Transpose(ANS_(myindex_p22, myindex_p12), nbinfo_[k].Ac1c1invAc1c2);
		MultiplySolve(nbinfo_[k].Ac1c1inv, nbinfo_[k].Ac1c1invAc1c2);
		// Ac1c1invAc1h2 = Ac1c1^{-1} * Ah2c1^{T}.
		Transpose(ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1h2);
		MultiplySolve(nbinfo_[k].Ac1c1inv, nbinfo_[k].Ac1c1invAc1h2);
		// Ah1h1 = Ah1h1 - Ah1c1 * Ac1c1^ { -1 } *Ah1c1^ { T }.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ASS_(myindex_p11, myindex_p12), nbinfo_[k].Ac1c1invAc1h1,
			Scalar(1), ASS_(myindex_p11, myindex_p11));
		// Ac2h1 = Ac2h1 - Ac2c1 * Ac1c1^{-1} * Ah1c1^{T}.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p12), nbinfo_[k].Ac1c1invAc1h1,
			Scalar(1), ANS_(myhindex_p22, myindex_p11));
		Transpose(ANS_(myindex_p22, myindex_p11), (nodek->ANS_)(nodekindex_p11, nodekindex_p22));
		// Ah2h1 = Ah2h1 - Ah2c1 * Ac1c1^{-1} * Ah1c1^{T}.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1h1,
			Scalar(1), ANS_(myindex_p21, myindex_p11));
		Transpose(ANS_(myindex_p21, myindex_p11), (nodek->ANS_)(nodekindex_p11, nodekindex_p21));
		// Ac2c2 = Ac2c2 - Ac2c1 * Ac1c1^{-1} * Ac2c1^{T}.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p12), nbinfo_[k].Ac1c1invAc1c2,
			Scalar(1), (nodek->ASS_)(nodekindex_p22, nodekindex_p22));
		// Ah2c2 = Ah2c2 - Ah2c1 * Ac1c1^{-1} * Ac2c1^{T}.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1c2,
			Scalar(1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22));
		Transpose((nodek->ASS_)(nodekindex_p21, nodekindex_p22), (nodek->ASS_)(nodekindex_p22, nodekindex_p21));
		//  Ah2h2 = Ah2h2 - Ah2c1 * Ac1c1^{-1} * Ah2c1^{T}.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1h2,
			Scalar(1), (nodek->ANS_)(nodekindex_p21, nodekindex_p21));
		// Ah1c1 = Ac2c1 = Ah2c1 = 0.

		// Step 3.
		// Ac2c2 = Lc2 * Dc2 * Lc2^{T}.
		LDLSolve((nodek->ASS_)(nodekindex_p22,nodekindex_p22), nbinfo_[k].Ac2c2inv);
		// Ac2c2invAc2h1 = Ac2c2^{-1} * Ac2h1.
		nbinfo_[k].Ac1c1invAc2h1(ANS_(myindex_p22, myindex_p11));
		MultiplySolve(nbinfo_[k].Ac2c2inv, nbinfo_[k].Ac1c1invAc2h1);
		// Ac2c2invAc2h2 = Ac2c2^{-1} * Ah2c2^{T}.
		Transpose((nodek->ASS_)(nodekindex_p21, nodekindex_p22), nbinfo_[k].Ac2c2invAc2h2);
		MultiplySolve(nbinfo_[k].Ac2c2inv, nbinfo_[k].Ac2c2invAc2h2);
		// Ah1h1 = Ah1h1 - Ac2h1^{T} * Ac2c2^{-1} * Ac2h1.
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p11), nbinfo_[k].Ac1c1invAc2h1,
			Scalar(1), ASS_(myindex_p11, myindex_p11));
		// Ah2h1 = Ah2h1 - Ah2c2 * Ac2c2^{-1} * Ac2h1.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22), nbinfo_[k].Ac1c1invAc2h1,
			Scalar(1), ANS_(myindex_p21, myindex_p11));
		Transpose(ANS_(myindex_p21, myindex_p11), (nodek->ANS_)(nodekindex_p11, nodekindex_p21));
		// Ah2h2 = Ah2h2 - Ah2c2 * Ac2c2^{-1} * Ah2c2^{T}.
		Gemm(NORMAL, NORMAL,
			Scalar(-1), (nodek->ASS_)(myindex_p21, myindex_p22), nbinfo_[k].Ac1c1invAc2h2,
			Scalar(1), (nodek->ASS_)(nodekindex_p21, nodekindex_p21));
		// Ah2c2 = Ac2h1 = 0.
	}

	sort(re_.begin(), re_.end());
	Diff_Sort(sep_, re_, sk_);
	sort(nbre_begin(), nbre.end());
}

// No skeletonization.
template <typename Scalar>
void HIFGraph<Scalar>::NoSkel()
{
	sk_.assign(sep_.begin(), sep_.end());
}

} // namespace HIF