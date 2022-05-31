#include "HIF.hpp"

// input file of A
// n
// i j aij
// ...

// input file of b (if exists)
// bi
// ...

namespace HIF {

int main(int argc, char* argv[])
{
	El::Initialize(argc, argv);
	typedef double Scalar;

	const string inputfileA = Input("--input_b", "input filename of A");
	const string inputfileb = Input("--input_A", "input filename of b");
	const int cutoff = Input("--cutoff", "cutoff", 64);
	const double tol = Input("--tol", "tolerance", 1e-3);
	
	string fileA = "./" + inputfileA;
	ifstream finA;
	finA.open(fileA, ios::in);
	if (!finA)
	{
		std::cerr << "cannot open the file";
	}
	int n;
	finA >> n;
	SparseMatrix A(n, n);
	int i, j, value;
	while (finA >> i >> j >> value)
	{
		A.Set(i, j, value);
	}

	string fileb = "./" + inputfileb;
	ifstream finb;
	finb.open(fileb, ios::in);
	if (!finb)
	{
		std::cerr << "cannot open the file";
	}
	Matrix<Scalar> b(n, 1);
	int k = 0;
	while (finb >> value)
	{
		b.Set(k, 0, value);
		k++;
	}

	HIFGraph<Scalar> HIF(A, cutoff);

	HIF.Factorization(tol);

	HIF.Apply(b);
}

} // namespace HIF.