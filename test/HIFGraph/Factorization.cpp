#include "HIF.hpp"

// input file of A
// n
// i j aij
// ...

// input file of b (if exists)
// bi
// ...

using namespace HIF;

int main(int argc, char* argv[])
{
	El::Initialize(argc, argv);
	typedef double Scalar;

	try
	{
		const string inputfileA = Input("--input_A", "input filename of A", "./A.txt");
		const string inputfileb = Input("--input_b", "input filename of b", "./b.txt");
		const int cutoff = Input("--cutoff", "cutoff", 64);
		const double tol = Input("--tol", "tolerance", 1e-3);

		El::ProcessInput();
		El::PrintInputReport();

		string fileA = inputfileA;
		std::ifstream finA;
		finA.open(fileA, std::ios::in);
		if (!finA)
		{
			std::cerr << "cannot open the file";
		}
		int n;
		finA >> n;
		SparseMatrix<Scalar> A(n, n);
		int i, j;
		double value;
		while (finA >> i >> j >> value)
		{
			A.Set(i, j, value);
		}
		finA.close();

		string fileb = inputfileb;
		std::ifstream finb;
		finb.open(fileb, std::ios::in);
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
		finb.close();

		// HIFGraph<Scalar> HIF(A, cutoff);

		/*HIF.Factorization(tol);

		HIF.Apply(b);

		string filex = "./sol.txt";
		std::ofstream foutx;
		foutx.open(filex, std::ios::out);
		for (int k = 0; k < b.Height(); k++)
		{
			foutx << b.Get(k, 1) << endl;
		}
		foutx.close();*/
	}
	catch (std::exception& e)
	{

	}
	El::Finalize();
	return 0;
}