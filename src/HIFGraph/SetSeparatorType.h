#ifndef HIF_SRC_HIFGRAPH_SETSEPARATORTYPE_H
#define HIF_SRC_HIFGRAPH_SETSEPARATORTYPE_H

#include "HIF.h"

namespace HIF {
// Recusively set separator vertices' type.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveSetSeparatorType(int whatlevel) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveSetSeparatorType"))

  if (level_ == whatlevel) {
    SetSeparatorType();
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveSetSeparatorType(whatlevel);
      }
    }
  }
}

// Set separator vertices' type.
template <typename Scalar> void HIFGraph<Scalar>::SetSeparatorType() {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:SetSeparatorType"))

  singlesep_.resize(nbnode_.size());

  vector<int> ordersep(sep_.size(), 0);

  for (int k = 0; k < nbnode_.size(); k++) {
    HIFGraph &nodek = *nbnode_[k];
    vector<int> tmpindex;
    Intersect_Sort(sep_, nodek.nb_, tmpindex, 1);
    singlesep_[k].assign(tmpindex.begin(), tmpindex.end());
    for (int i = 0; i < tmpindex.size(); i++) {
      ordersep[tmpindex[i]] += 1;
    }
  }

  vector<int> tmpvec = sep_;
  for (int k = 0; k < nbnode_.size(); k++) {
    vector<int> index_find_ordersep_1;
    FindAllIndex(ordersep, 1, index_find_ordersep_1);
    vector<int> index_sep;
    Intersect_Sort(index_find_ordersep_1, singlesep_[k], index_sep);
    singlesep_[k].resize(index_sep.size());
    for (int i = 0; i < index_sep.size(); i++) {
      singlesep_[k][i] = sep_[index_sep[i]];
    }
    complexsep_.clear();
    Diff_Sort(tmpvec, singlesep_[k], complexsep_);
    tmpvec = complexsep_;
  }
}
} // namespace HIF.

#endif // ifndef HIF_SRC_HIFGRAPH_SETSEPARATORTYPE_H
