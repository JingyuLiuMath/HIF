#include "HIF.hpp"
#include "cmath"

// input file of A
// n
// i j aij
// ...

using namespace HIF;

inline int Idx1D(int i, int j, int k, int xSize, int ySize, int zSize);

int main(int argc, char* argv[])
{
	El::Initialize(argc, argv);

	try
	{
		const string inputfileA = Input("--input_A", "input filename of A", "./A.txt");
		const int minvtx = Input("--minvtx", "minvtx", 64);
		const bool button = Input("--HIFbutton", "true for HIF, false for MF", true);
		const double tol = Input("--tol", "tolerance", 1e-3);
		const bool logApp = Input("--logApp", "Log File appending", false);
		const int dirSize = Input("--matSize", "Size on each direction", 16);

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

		// Basic settings.
		const double frac_npi_nplus1 = dirSize * M_PI / (dirSize + 1);
		const double normalized_coeff = pow(sqrt(double(2 / n + 1)), 3);

		Matrix<double> x(n, 1);
		/*for (int xit = 0; xit < dirSize; xit++)
		{
			for (int yit = 0; yit < dirSize; yit++)
			{
				for (int zit = 0; zit < dirSize; zit++)
				{
					x.Set(Idx1D(xit, yit, zit, dirSize, dirSize, dirSize), 0,
						normalized_coeff * sin((xit + 1) * frac_npi_nplus1) * 
						sin((yit + 1) * frac_npi_nplus1) * sin((zit + 1) * frac_npi_nplus1));
				}
			}
		}*/
		for (int k = 0; k < n; k++)
		{
			x.Set(k, 0, 1);
		}

		Matrix<double> b(n, 1);
		int nnzA = A.NumEntries();
		const int* offsetA = A.LockedOffsetBuffer();
		const int* targetA = A.LockedTargetBuffer();
		const double* valueA = A.LockedValueBuffer();
		double tmpvalue = 0.0;
		for (int row = 0; row < n; row++)
		{
			tmpvalue = 0.0;
			for (int t = offsetA[row]; t < offsetA[row + 1]; t++)
			{
				tmpvalue += valueA[t] * x.Get(targetA[t], 0);
			}
			b.Set(row, 0, tmpvalue);
		}

		readTimer.Stop();
		MasterCout("Reading input ends in ", readTimer.Total(), " sec.");

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

		// Problem solving.
		El::Timer solTimer("SolveTimer");
		solTimer.Start();
		MasterCout("Problem solving starts...");

		HIF.Apply(b);

		solTimer.Stop();
		MasterCout("Problem solving ends in ", solTimer.Total(), " sec.");

		double relnorm2 = 0.0;
		double xnorm2 = 0.0;
		for (int k = 0; k < n; k++)
		{
			relnorm2 += std::pow(b.Get(k, 0) - x.Get(k, 0), 2);
			xnorm2 += std::pow(x.Get(k, 0), 2);
		}
		relnorm2 = relnorm2 / xnorm2;
		relnorm2 = std::sqrt(relnorm2);


		INFO_HIF
		(
			Log(
				"        InitT        FactT        SolT        RelErr2"
			);
		Log(
			setw(13), scientific, setprecision(2), initTimer.Total(),
			setw(13), scientific, setprecision(2), factTimer.Total(),
			setw(12), scientific, setprecision(2), solTimer.Total(),
			setw(15), scientific, setprecision(2), relnorm2
		);
		Log(
			"==================================================",
			"==================================================",
			"=================================================="
		);
		)
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

inline int Idx1D(int i, int j, int k, int xSize, int ySize, int zSize)
{
	return i + j * xSize + k * xSize * ySize;
}