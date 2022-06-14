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

	//try
	//{
		const string inputfileA = Input("--input_A", "input filename of A", "./A.txt");
		const string inputfileb = Input("--input_b", "input filename of b", "./b.txt");
		const int cutoff = Input("--cutoff", "cutoff", 64);
		// const int HIFbutton = Input("--HIFbutton", "1 for HIF, 0 for MF", 1);
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
		
		/*Matrix<double> B(3, 3);
		for (int i = 0; i < B.Height(); i++)
		{
			for (int j = 0; j < B.Width(); j++)
			{
				B.Set(i, j, i + j);
			}
		}
		vector<int> row;
		vector<int> col;
		row.push_back(0);
		row.push_back(2);
		col.resize(0);
		Matrix<double> tmp = B(row, col);
		std::cout << " [ " << std::endl;
		for (int i = 0; i < tmp.Height(); i++)
		{
			for (int j = 0; j < tmp.Width(); j++)
			{
				std::cout << tmp.Get(i, j) << "," << std::endl;
			}
			std::cout << ";" << std::endl;
		}
		std::cout << " ] " << std::endl;
		Matrix<double> tmpT;
		Transpose(tmp, tmpT);
		std::cout << " [ " << std::endl;
		for (int i = 0; i < tmpT.Height(); i++)
		{
			for (int j = 0; j < tmpT.Width(); j++)
			{
				std::cout << tmpT.Get(i, j) << "," << std::endl;
			}
			std::cout << ";" << std::endl;
		}
		std::cout << " ] " << std::endl;*/

		HIFGraph<double> HIF(A, cutoff);

		// HIF.Factorization(HIFbutton, tol);
		HIF.Factorization(tol);
		
		HIF.Apply(b);

		std::cout << " b " << std::endl;
		std::cout << " [ " << std::endl;
		for (int k = 0; k < n; k++)
		{
			std::cout << b.Get(k, 0) << ";" << std::endl;
		}
		std::cout << " ] " << std::endl;

		/*string filex = "./sol.txt";
		std::ofstream foutx;
		foutx.open(filex, std::ios::out);
		for (int k = 0; k < b.Height(); k++)
		{
			foutx << b.Get(k, 1) << endl;
		}
		foutx.close();*/
	//}
	//catch (std::exception& e)
	//{

	//}
	El::Finalize();
	return 0;
}