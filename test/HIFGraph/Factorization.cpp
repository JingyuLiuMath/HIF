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
		SparseMatrix<double> A(n, n);
		int i, j;
		double value;
		while (finA >> i >> j >> value)
		{
			A.QueueUpdate(i, j, value);
		}
		A.ProcessQueues();
		finA.close();

		string fileb = inputfileb;
		std::ifstream finb;
		finb.open(fileb, std::ios::in);
		if (!finb)
		{
			std::cerr << "cannot open the file";
		}
		Matrix<double> b(n, 1);
		int k = 0;
		while (finb >> value)
		{
			b.Update(k, 0, value);
			k++;
		}
		finb.close();

		std::cout << A.Height() << std::endl;
		std::cout << A.NumEntries() << std::endl;
		std::cout << A.Get(6, 6) << std::endl;
		vector<int> index;
		for (int i = 1; i < 5; i += 2)
		{
			index.push_back(i);
		}
		std::cout << "asdfhskf" << std::endl;
		vector<int> zero;
		zero.push_back(0);
		Matrix<double> tmpb = b(index, zero);
		std::cout << tmpb.Height() << std::endl;
		std::cout << "asdfg" << std::endl;
		SparseMatrix<double> tmpA = A(index, id);
		std::cout << tmpA.Height() << std::endl;
		std::cout << "uoooy" << std::endl;

		/*HIFGraph<Scalar> HIF(A, cutoff);

		HIF.Factorization(tol);

		HIF.Apply(b);

		for (int k = 0; k < n; k++)
		{
			std::cout << b.Get(k, 0) << std::endl;
		}
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