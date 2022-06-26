#include "HIF.hpp"

namespace HIF {

// Recursively skeletonization.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveSkel(int whatlevel)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveSkel"))

	if (level_ == whatlevel)
	{
		if (Button())
		{
			Skel();
		}
		else
		{
			NoSkel();
		}
	}
	else
	{
		if (endflag_ == 0)
		{
			for (int iter = 0; iter < 2; iter++)
			{
				children_[iter]->RecursiveSkel(whatlevel);
			}
		}
	}

}

// Skeletonization.
template<typename Scalar>
void HIFGraph<Scalar>::Skel()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Skel"))

	QRCtrl<Base<Scalar>> ctrl;
	ctrl.boundRank = false;
	ctrl.maxRank = 1000;
	ctrl.adaptive = true;
	ctrl.tol = Tol();
	ctrl.smallestFirst = false;

	nbinfo_.resize(nbnode_.size());

	for (int k = 0; k < nbnode_.size(); k++)
	{
		HIFGraph* nodek = nbnode_[k];
		if (nodek->level_ != level_)
		{
			nbinfo_[k].skip = 1;
			continue;
		}
		if (nodek->seqnum_ < seqnum_) 
		{
			nbinfo_[k].skip = 1;
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

		int korder = -1;
		vector<int> veckorder;
		FindAllIndex(nodek->nbnodeseqnum_, seqnum_, veckorder);
		if (veckorder.size() == 1)
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
		vector<int> sep2 = nodek->singlesep_[korder];
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
		nodeksep2C.insert(nodeksep2C.end(), (nodek->complexsep_).begin(), (nodek->complexsep_).end());
		sort(nodeksep2C.begin(), nodeksep2C.end());
		vector<int> mysep2C;
		Diff_Sort(nb_, sep2, mysep2C);

		// The following data are indices.
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
		if ((myindex_sep1C.size() + myindex_mysep2C.size() == 0) || 
			(myindex_sep1.size() == 0) ||
			(nodekindex_sep2C.size() + nodekindex_nodeksep1C.size() == 0) ||
			(nodekindex_sep2.size() == 0))
		{
			nbinfo_[k].skip = 1;
			continue;
		}
		MatrixS skelmtx1;
		MatrixS skelmtx2;
		El::Zeros(skelmtx1,
			myindex_sep1C.size() + myindex_mysep2C.size(), myindex_sep1.size());
		El::Zeros(skelmtx2,
			nodekindex_sep2C.size() + nodekindex_nodeksep1C.size(), nodekindex_sep2.size());
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
		viewmtx = (nodek->ASS_)(nodekindex_sep2C, nodekindex_sep2);
		View(viewmtx, skelmtx2,
			nodekindex_sep2C.size(), 0,
			nodekindex_nodeksep1C.size(), nodekindex_sep2.size());
		viewmtx = (nodek->ANS_)(nodekindex_nodeksep1C, nodekindex_sep2);

		TIMER_HIF(TimerStart(TIMER_EL))
		TIMER_HIF(TimerStart(TIMER_EL_ID))
		vector<int> p11;
		vector<int> p12;
		MatrixS& T1 = nbinfo_[k].Th1c1;
		IDSolve(skelmtx1, T1, p11, p12, ctrl); // skelmtx1(:, p12) = skelmtx1(:, p11) * T1.
		TIMER_HIF(TimerStop(TIMER_EL_ID))
		TIMER_HIF(TimerStop(TIMER_EL))

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
			/*re_.push_back(sep1[p12[i]]);
			(nodek->nbre_).push_back(sep1[p12[i]]);*/
			AddElement_Sort(re_, sep1[p12[i]]);
			AddElement_Sort(nodek->nbre_, sep1[p12[i]]);
		}

		TIMER_HIF(TimerStart(TIMER_EL))
		TIMER_HIF(TimerStart(TIMER_EL_ID))
		vector<int> p21;
		vector<int> p22;
		MatrixS& T2 = nbinfo_[k].Th2c2;
		IDSolve(skelmtx2, T2, p21, p22, ctrl); // skelmtx2(:, p22) = skelmtx2(:, p21) * T2.		
		TIMER_HIF(TimerStop(TIMER_EL_ID))
		TIMER_HIF(TimerStop(TIMER_EL))

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
			myindex_p21[i] = myindex_sep2[p21[i]];
			nodekindex_p21[i] = nodekindex_sep2[p21[i]];
		}	
		for (int i = 0; i < p22.size(); i++)
		{
			myindex_p22[i] = myindex_sep2[p22[i]];
			nodekindex_p22[i] = nodekindex_sep2[p22[i]];
			/*nbre_.push_back(sep2[p22[i]]);
			(nodek->re_).push_back(sep2[p22[i]]);*/
			AddElement_Sort(nbre_, sep2[p22[i]]);
			AddElement_Sort(nodek->re_, sep2[p22[i]]);
		}

		nbinfo_[k].skip = 0;

		MatrixS copymtx; // Copy of updated matrix. 
		MatrixS copymtxT; // copymtx^{T}.
		MatrixS tmpmtx;
		
		TIMER_HIF(TimerStart(TIMER_EL))
		
		// Step 1.
		MatrixS Ac1h1T1(myindex_p12.size(),T1.Width());
		MatrixS Ah1h1T1(myindex_p11.size(), T1.Width());
		MatrixS Ac2h2T2(nodekindex_p22.size(), T2.Width());
		MatrixS Ah2h2T2(nodekindex_p21.size(), T2.Width());
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), ASS_(myindex_p11, myindex_p12), T1,
			Scalar(0), Ac1h1T1);
		Gemm(NORMAL, NORMAL,
			Scalar(1), ASS_(myindex_p11, myindex_p11), T1,
			Scalar(0), Ah1h1T1);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22), T2,
			Scalar(0), Ac2h2T2);
		Gemm(NORMAL, NORMAL,
			Scalar(1), (nodek->ASS_)(nodekindex_p21, nodekindex_p21), T2,
			Scalar(0), Ah2h2T2);
		// Ac1c1 = Ac1c1 - Ah1c1^{T} * Th1c1 - Th1c1^{T} * Ah1c1 + Th1c1^{T} * Ah1h1 * Th1c1.
		copymtx = ASS_(myindex_p12, myindex_p12);
		copymtx -= Ac1h1T1;
		Transpose(Ac1h1T1, tmpmtx);
		copymtx -= tmpmtx;
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), T1, Ah1h1T1,
			Scalar(1), copymtx);
		SubMatrixUpdate(ASS_, myindex_p12, myindex_p12, copymtx);
		// Ah1c1 = Ah1c1 - Ah1h1 * Th1c1.
		copymtx = ASS_(myindex_p11, myindex_p12);
		copymtx -= Ah1h1T1;
		SubMatrixUpdate(ASS_, myindex_p11, myindex_p12, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(ASS_, myindex_p12, myindex_p11, copymtxT);
		// Ac2c1 = Ac2c1 - Ac2h1 * Th1c1 - Th2c2^{T} * Ah2c1 + Th2c2^{T} * Ah2h1 * Th1c1.
		copymtx = ANS_(myindex_p22, myindex_p12);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p11), T1,
			Scalar(1), copymtx);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), T2, ANS_(myindex_p21, myindex_p12),
			Scalar(1), copymtx);
		tmpmtx.Resize(myindex_p21.size(), T1.Width());
		Gemm(NORMAL, NORMAL,
			Scalar(1), ANS_(myindex_p21, myindex_p11), T1,
			Scalar(0), tmpmtx);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), T2, tmpmtx,
			Scalar(1), copymtx);
		SubMatrixUpdate(ANS_, myindex_p22, myindex_p12, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ANS_, nodekindex_p12, nodekindex_p22, copymtxT);
		// Ah2c1 = Ah2c1 - Ah2h1 * Th1c1.
		copymtx = ANS_(myindex_p21, myindex_p12);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p11), T1,
			Scalar(1), copymtx);
		SubMatrixUpdate(ANS_, myindex_p21, myindex_p12, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ANS_, nodekindex_p12, nodekindex_p21, copymtxT);
		// Ac2h1 = Ac2h1 - Th2c2^{T} * Ah2h1.
		copymtx = ANS_(myindex_p22, myindex_p11);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), T2, ANS_(myindex_p21, myindex_p11),
			Scalar(1), copymtx);
		SubMatrixUpdate(ANS_, myindex_p22, myindex_p11, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ANS_, nodekindex_p11, nodekindex_p22, copymtxT);
		// Ac2c2 = Ac2c2 - Ah2c2^{T} * Th2c2 - Th2c2^{T} * Ah2c2 + Th2c2^{T} * Ah2h2 * Th2c2.
		copymtx = (nodek->ASS_)(nodekindex_p22, nodekindex_p22);
		copymtx -= Ac2h2T2;
		Transpose(Ac2h2T2, tmpmtx);
		copymtx -= tmpmtx;
		Gemm(TRANSPOSE, NORMAL,
			Scalar(1), T2, Ah2h2T2,
			Scalar(1), copymtx);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p22, nodekindex_p22, copymtx);
		// Ah2c2 = Ah2c2 - Ah2h2 * Th2c2.
		copymtx = (nodek->ASS_)(nodekindex_p21, nodekindex_p22);
		copymtx -= Ah2h2T2;
		SubMatrixUpdate(nodek->ASS_, nodekindex_p21, nodekindex_p22, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p22, nodekindex_p21, copymtxT);
		
		// Step 2.
		// Ac1c1 = Lc1 * Dc1 * Lc1^{T}.
		copymtx = ASS_(myindex_p12, myindex_p12);
		TIMER_HIF(TimerStart(TIMER_EL_LDLSOLVE))
		LDLSolve(copymtx, nbinfo_[k].Ac1c1inv);
		TIMER_HIF(TimerStop(TIMER_EL_LDLSOLVE))
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
		copymtx = ASS_(myindex_p11, myindex_p11);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ASS_(myindex_p11, myindex_p12), nbinfo_[k].Ac1c1invAc1h1,
			Scalar(1), copymtx);
		SubMatrixUpdate(ASS_, myindex_p11, myindex_p11, copymtx);
		// Ac2h1 = Ac2h1 - Ac2c1 * Ac1c1^{-1} * Ah1c1^{T}.
		copymtx = ANS_(myindex_p22, myindex_p11);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p12), nbinfo_[k].Ac1c1invAc1h1,
			Scalar(1), copymtx);
		SubMatrixUpdate(ANS_, myindex_p22, myindex_p11, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ANS_, nodekindex_p11, nodekindex_p22, copymtxT);
		// Ah2h1 = Ah2h1 - Ah2c1 * Ac1c1^{-1} * Ah1c1^{T}.
		copymtx = ANS_(myindex_p21, myindex_p11);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1h1,
			Scalar(1), copymtx);
		SubMatrixUpdate(ANS_, myindex_p21, myindex_p11, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ANS_, nodekindex_p11, nodekindex_p21, copymtxT);
		// Ac2c2 = Ac2c2 - Ac2c1 * Ac1c1^{-1} * Ac2c1^{T}.
		copymtx = (nodek->ASS_)(nodekindex_p22, nodekindex_p22);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p12), nbinfo_[k].Ac1c1invAc1c2,
			Scalar(1), copymtx);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p22, nodekindex_p22, copymtx);
		// Ah2c2 = Ah2c2 - Ah2c1 * Ac1c1^{-1} * Ac2c1^{T}.
		copymtx = (nodek->ASS_)(nodekindex_p21, nodekindex_p22);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1c2,
			Scalar(1), copymtx);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p21, nodekindex_p22, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p22, nodekindex_p21, copymtxT);
		// Ah2h2 = Ah2h2 - Ah2c1 * Ac1c1^{-1} * Ah2c1^{T}.
		copymtx = (nodek->ASS_)(nodekindex_p21, nodekindex_p21);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), ANS_(myindex_p21, myindex_p12), nbinfo_[k].Ac1c1invAc1h2,
			Scalar(1), copymtx);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p21, nodekindex_p21, copymtx);
		// Ah1c1 = Ac2c1 = Ah2c1 = 0.

		// Step 3.
		// Ac2c2 = Lc2 * Dc2 * Lc2^{T}.
		copymtx = (nodek->ASS_)(nodekindex_p22, nodekindex_p22);
		TIMER_HIF(TimerStart(TIMER_EL_LDLSOLVE))
		LDLSolve(copymtx, nbinfo_[k].Ac2c2inv);
		TIMER_HIF(TimerStop(TIMER_EL_LDLSOLVE))
		// Ac2c2invAc2h1 = Ac2c2^{-1} * Ac2h1.
		nbinfo_[k].Ac2c2invAc2h1 = ANS_(myindex_p22, myindex_p11);
		MultiplySolve(nbinfo_[k].Ac2c2inv, nbinfo_[k].Ac2c2invAc2h1);
		// Ac2c2invAc2h2 = Ac2c2^{-1} * Ah2c2^{T}.
		Transpose((nodek->ASS_)(nodekindex_p21, nodekindex_p22), nbinfo_[k].Ac2c2invAc2h2);
		MultiplySolve(nbinfo_[k].Ac2c2inv, nbinfo_[k].Ac2c2invAc2h2);
		// Ah1h1 = Ah1h1 - Ac2h1^{T} * Ac2c2^{-1} * Ac2h1.
		copymtx = ASS_(myindex_p11, myindex_p11);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), ANS_(myindex_p22, myindex_p11), nbinfo_[k].Ac2c2invAc2h1,
			Scalar(1), copymtx);		
		SubMatrixUpdate(ASS_, myindex_p11, myindex_p11, copymtx);
		// Ah2h1 = Ah2h1 - Ah2c2 * Ac2c2^{-1} * Ac2h1.
		copymtx = ANS_(myindex_p21, myindex_p11);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22), nbinfo_[k].Ac2c2invAc2h1,
			Scalar(1), copymtx);
		SubMatrixUpdate(ANS_, myindex_p21, myindex_p11, copymtx);
		Transpose(copymtx, copymtxT);
		SubMatrixUpdate(nodek->ANS_, nodekindex_p11, nodekindex_p21, copymtxT);
		// Ah2h2 = Ah2h2 - Ah2c2 * Ac2c2^{-1} * Ah2c2^{T}.
		copymtx = (nodek->ASS_)(nodekindex_p21, nodekindex_p21);
		Gemm(NORMAL, NORMAL,
			Scalar(-1), (nodek->ASS_)(nodekindex_p21, nodekindex_p22), nbinfo_[k].Ac2c2invAc2h2,
			Scalar(1), copymtx);
		SubMatrixUpdate(nodek->ASS_, nodekindex_p21, nodekindex_p21, copymtx);
		// Ah2c2 = Ac2h1 = 0.

		TIMER_HIF(TimerStop(TIMER_EL))
	}

	// sort(re_.begin(), re_.end());
	Diff_Sort(sep_, re_, sk_);
	// sort(nbre_.begin(), nbre_.end());
	Diff_Sort(nb_, nbre_, nbsk_);
}

// No skeletonization.
template <typename Scalar>
void HIFGraph<Scalar>::NoSkel()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:NoSkel"))

	sk_.assign(sep_.begin(), sep_.end());
	nbsk_.assign(nb_.begin(), nb_.end());
}

} // namespace HIF.
