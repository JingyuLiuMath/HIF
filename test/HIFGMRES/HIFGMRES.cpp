#include "HIF.hpp"

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
			std::cerr << "cannot open the file" << std::endl;
		}
		Matrix<double> b(n, 1);
		int k = 0;
		while (finb >> value)
		{
			b.Set(k, 0, value);
			k++;
		}
		finb.close();

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

		// GMRES.
		// 'applyA' should be a function of the form
		// 
		//   void applyA
		//   (Scalar alpha, const Matrix<Scalar>& X, Scalar beta, Matrix<Scalar>& Y)
		//
		// and overwrite Y := alpha A X + beta Y.
		auto applyA =
			[&](double alpha, const Matrix<double>& X,
					double beta, Matrix<double>& Y)
		{
			int nnzA = A.NumEntries();
			const int* sourceA = A.LockedSourceBuffer();
			const int* targetA = A.LockedTargetBuffer();
			const double* valueA = A.LockedValueBuffer();
			Y *= beta;
			for (int col = 0; col < Y.Width(); col++)
			{
				for (int k = 0; k < nnzA; k++)
				{
					Y.Update(sourceA[k], col, alpha * valueA[k] * X.Get(targetA[k], col));
				}
			}
		};

		// 'precond' should have the form
		//
		//   void precond(Matrix<Sclar>& W)
		//
		// and overwrite W with an approximation of inv(A) W.
		auto precond =
			[&](Matrix<double>& W)
		{
			HIF.Apply(W);
		};
		
		// GMRES settings.
		double relTol = 1e-12;
		int restart = 30;
		int maxIts = floor(b.Height() / restart);
		bool progress = true;


		El::Timer gmresTimer("GMRESTimer");
		gmresTimer.Start();
		MasterCout("GMRES starts...");

		int iter = LGMRES(applyA, precond, b, relTol,
			restart, maxIts, progress);
		
		gmresTimer.Stop();
		MasterCout("GMRES ends in ", gmresTimer.Total(), " sec.");
		MasterCout("GMRES ends in ", iter, " steps.");
		MasterCout("Relative Error in  2  Norm: ", scientific, setprecision(2), relTol);

		INFO_HIF
		(
			Log(
				"        InitT        FactT        GmresT        GmresIt"
			);
			Log(
				setw(13), scientific, setprecision(2), initTimer.Total(),
				setw(13), scientific, setprecision(2), factTimer.Total(),
				setw(14), scientific, setprecision(2), gmresTimer.Total(),
				setw(15), scientific, setprecision(2), iter
			);
			Log(
				"==================================================",
				"==================================================",
				"=================================================="
			);
		)

		string filex = "./sol.txt";
		std::ofstream foutx;
		foutx.open(filex, std::ios::out);
		for (int k = 0; k < b.Height(); k++)
		{
			foutx << b.Get(k, 1) << endl;
		}
		foutx.close();
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
