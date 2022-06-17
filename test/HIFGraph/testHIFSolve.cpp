#include "HIF.hpp"
#include "math.h"

// input file of A
// n
// i j aij
// ...

// input file of b
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
		const int minvtx = Input("--minvtx", "minvtx", 64);
		const bool button = Input("--HIFbutton", "true for HIF, false for MF", true);
		const double tol = Input("--tol", "tolerance", 1e-3);
		const bool logApp = Input("--logApp", "Log File appending", false);

		El::ProcessInput();
		LogAppend(logApp);
		SetTol(tol);
		SetButton(button);
		El::PrintInputReport();

		El::SetBlocksize(128);
		El::SetLocalTrrkBlocksize<double>(8);

		DEBUG_HIF(MasterCout("Debug Mode"));

		// Reading input.
		El::Timer readTimer("ReadTimer");
		readTimer.Start();
		MasterCout("Reading input starts...");

		string fileA = inputfileA;
		std::ifstream finA;
		finA.open(fileA, std::ios::in);
		if (!finA)
		{
			std::cerr << "cannot open the file" << std::endl;
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
			std::cerr << "cannot open the file" << std::endl;;
		}
		Matrix<double> b(n, 1);
		int k = 0;
		while (finb >> value)
		{
			b.Update(k, 0, value);
			k++;
		}
		finb.close();

		readTimer.Stop();
		MasterCout("Reading input ends in ", readTimer.Total(), " sec.");
		
		// sourceA is ordered from small to large
		/*int nnzA = A.NumEntries();
		const int* sourceA = A.LockedSourceBuffer();
		const int* targetA = A.LockedTargetBuffer();
		for (int k = 0; k < nnzA; k++)
		{
			std::cout << sourceA[k] << " " << targetA[k] << " " << A.Get(sourceA[k], targetA[k]) << std::endl;
		}*/

		SparseMatrix<double> B(4, 6);
		B.QueueUpdate(0, 0, 10);
		B.QueueUpdate(0, 1, 20);
		B.QueueUpdate(1, 1, 30);
		B.QueueUpdate(1, 3, 40);
		B.QueueUpdate(2, 2, 50);
		B.QueueUpdate(2, 3, 60);
		B.QueueUpdate(2, 4, 70);
		B.QueueUpdate(3, 5, 80);
		B.ProcessQueues();
		int nnzB = B.NumEntries();
		const int* sourceB = B.LockedSourceBuffer();
		const int* targetB = B.LockedTargetBuffer();
		const int* offsetB = B.LockedOffsetBuffer();
		const double* valueB = B.LockedValueBuffer();
		for (int i = 0; i < nnzB; i++)
		{
			std::cout << sourceB[k] << " " << targetB[k] << " " << valueB[k] << std::endl;
		}
		for (int i = 0; i < 5; i++)
		{
			std::cout << offsetB[k] << std::endl;
		}

		/*
		// Initialization.
		El::Timer initTimer("InitTimer");
		initTimer.Start();
		MasterCout("Initialization starts...");

		HIFGraph<double> HIF(A, minvtx);

		initTimer.Stop();
		MasterCout("Initialization ends in ", initTimer.Total(), " sec.");

		// Factorization.
		El::Timer factTimer("FactTimer");
		factTimer.Start();
		MasterCout("Factorization starts...");

		HIF.Factorization();

		factTimer.Stop();
		MasterCout("Factorization ends in ", factTimer.Total(), " sec.");

		// Application.
		El::Timer applyTimer("ApplyTimer");
		applyTimer.Start();
		MasterCout("Application starts...");

		HIF.Apply(b);

		applyTimer.Stop();
		MasterCout("Application ends in ", applyTimer.Total(), " sec.");

		// We already know the solution is 1.
		double relnormInf = 0.0;
		for (int k = 0; k < n; k++)
		{
			relnormInf = std::max(std::abs(b.Get(k, 0) - 1), relnormInf);
		}


		INFO_HIF
		(
			Log(
				"        InitT        FactT        AppT        RelErrInf"
			);
			Log(
				setw(13), scientific, setprecision(2), initTimer.Total(),
				setw(13), scientific, setprecision(2), factTimer.Total(),
				setw(12), scientific, setprecision(2), applyTimer.Total(),
				setw(17), scientific, setprecision(2), relnormInf
				);
			Log(
				"==================================================",
				"==================================================",
				"=================================================="
			);
		)
		*/
	}
	catch (std::exception& e)
	{
		EL_DEBUG_ONLY(El::DumpCallStack())
			DEBUG_HIF(DumpCallStack())
	}
	CloseLog();
	El::Finalize();
	return 0;
}
