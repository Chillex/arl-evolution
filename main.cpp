#include "Solver.h"
#include "GenomeQueens.h"

struct SolverType
{
	enum Enum
	{
		OnePlusOne = 0,
		MyPlusLambda = 1,
		MyCommaLambda = 2,
		MyRhoPlusLambdaBlend = 3,
		MyRhoPlusLambdaCombine = 4,
		MyRhoCommaLambdaBlend = 5,
		MyRhoCommaLambdaCombine = 6
	};

	static const char* EnumStrings[];

	static const char* EnumToString(int enumValue)
	{
		return EnumStrings[enumValue];
	}
};
const char* SolverType::EnumStrings[] = {
	"1 + 1",
	"My + Lambda",
	"My , Lambda",
	"My / Rho + Lambda (Blending)",
	"My / Rho + Lambda (Combination)",
	"My / Rho , Lambda (Blending)",
	"My / Rho , Lambda (Combination)",
};

struct Print
{
	enum Enum
	{
		Nothing,
		Fitness
	};
};

void RunSolver(SolverType::Enum solverType, unsigned int my, unsigned int lambda, unsigned int rho, unsigned int maxGenerations, Print::Enum print);

int main()
{
	unsigned int my = 100u;
	unsigned int lambda = 200u;
	unsigned int rho = 3u;

	unsigned int maxGenerations = 10000u;
	
	// BEGIN Test Code
	GenomeQueens base(4);
	BaseGenome* genome = base.GetRandomGenome();
	genome->Print();
	printf(" - Fitness: %d\n", genome->GetFitness());
	// END Test Code

	RunSolver(SolverType::OnePlusOne, 1u, 1u, 1u, maxGenerations, Print::Nothing);
	RunSolver(SolverType::MyPlusLambda, my, lambda, rho, maxGenerations, Print::Nothing);
	RunSolver(SolverType::MyCommaLambda, my, lambda, rho, maxGenerations, Print::Nothing);
	RunSolver(SolverType::MyRhoPlusLambdaBlend, my, lambda, rho, maxGenerations, Print::Nothing);
	RunSolver(SolverType::MyRhoPlusLambdaCombine, my, lambda, rho, maxGenerations, Print::Nothing);
	RunSolver(SolverType::MyRhoCommaLambdaBlend, my, lambda, rho, maxGenerations, Print::Nothing);
	RunSolver(SolverType::MyRhoCommaLambdaCombine, my, lambda, rho, maxGenerations, Print::Nothing);

	return 0;
}

void RunSolver(SolverType::Enum solverType, unsigned int my, unsigned int lambda, unsigned int rho, unsigned int maxGenerations, Print::Enum print)
{
	GenomeXYAB* baseGenome = new GenomeXYAB(0, 0, 0, 0);
	Solver solver(my, lambda, rho, baseGenome);

	printf(SolverType::EnumToString(solverType));
	printf(":\n--------------------------------\n");
	for (unsigned int i = 0; i < maxGenerations; ++i)
	{
		switch (solverType)
		{
		case SolverType::OnePlusOne:
			solver.EvolveOnePlusOne();
			break;
		case SolverType::MyPlusLambda:
		default: // my + lambda as default case
			solver.EvolveMyLambda(MyLambdaSelection::Plus);
			break;
		case SolverType::MyCommaLambda:
			solver.EvolveMyLambda(MyLambdaSelection::Comma);
			break;
		case SolverType::MyRhoPlusLambdaBlend:
			solver.EvolveMyRhoLambda(InheritanceMode::Blend, MyLambdaSelection::Plus);
			break;
		case SolverType::MyRhoPlusLambdaCombine:
			solver.EvolveMyRhoLambda(InheritanceMode::Combination, MyLambdaSelection::Plus);
			break;
		case SolverType::MyRhoCommaLambdaBlend:
			solver.EvolveMyRhoLambda(InheritanceMode::Blend, MyLambdaSelection::Comma);
			break;
		case SolverType::MyRhoCommaLambdaCombine:
			solver.EvolveMyRhoLambda(InheritanceMode::Combination, MyLambdaSelection::Comma);
			break;
		}

		if (solver.HasSolved())
		{
			printf("%d. Generation: ", i);
			solver.PrintSolution();
			break;
		}

		if (print == Print::Fitness)
		{
			printf("%d. Generation Fitness: ", i + 1);
			solver.PrintFitness();
		}

		if (i == maxGenerations - 1)
			printf("No solution found in %d generations.\n", maxGenerations);
	}
	printf("\n");

	delete baseGenome;
}