#include "Solver.h"
#include "GenomeQueens.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

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
		MyRhoCommaLambdaCombine = 6,
		NQueensGenetic = 7
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
	" Queens",
};

struct Print
{
	enum Enum
	{
		Nothing,
		Fitness
	};
};

const int queenCount = 8;

void RunSolver(Solver& solver, SolverType::Enum solverType, unsigned int my, unsigned int lambda, unsigned int rho, unsigned int maxGenerations, Print::Enum print);
void DisplayQueens(GenomeQueens* solution);

int main()
{
	unsigned int my = 100u;
	unsigned int lambda = 200u;
	unsigned int rho = 2u;

	unsigned int maxGenerations = 100u;

	GenomeXYAB* baseGenome = new GenomeXYAB(0, 0, 0, 0);
	Solver solver(my, lambda, rho, baseGenome);

	GenomeQueens* baseQueenGenome = new GenomeQueens(queenCount);
	Solver queenSolver(my, lambda, rho, baseQueenGenome);

	RunSolver(solver, SolverType::OnePlusOne, 1u, 1u, 1u, maxGenerations, Print::Fitness);
	RunSolver(solver, SolverType::MyPlusLambda, my, lambda, rho, maxGenerations, Print::Fitness);
	RunSolver(solver, SolverType::MyCommaLambda, my, lambda, rho, maxGenerations, Print::Fitness);
	RunSolver(solver, SolverType::MyRhoPlusLambdaBlend, my, lambda, rho, maxGenerations, Print::Fitness);
	RunSolver(solver, SolverType::MyRhoPlusLambdaCombine, my, lambda, rho, maxGenerations, Print::Fitness);
	RunSolver(solver, SolverType::MyRhoCommaLambdaBlend, my, lambda, rho, maxGenerations, Print::Fitness);
	RunSolver(solver, SolverType::MyRhoCommaLambdaCombine, my, lambda, rho, maxGenerations, Print::Fitness);
	//RunSolver(queenSolver, SolverType::NQueensGenetic, my, lambda, rho, maxGenerations, Print::Fitness);

	//if(queenSolver.HasSolved())
	//{
	//	DisplayQueens(queenSolver.GetSolution<GenomeQueens>());
	//}

	delete baseGenome;
	delete baseQueenGenome;

	return 0;
}

void RunSolver(Solver& solver, SolverType::Enum solverType, unsigned int my, unsigned int lambda, unsigned int rho, unsigned int maxGenerations, Print::Enum print)
{
	std::ofstream logfile;
	logfile.open("log.csv", std::fstream::out | std::fstream::app);

	solver.ResetPopulation(my);

	if (solverType == SolverType::NQueensGenetic)
	{
		printf("%d", queenCount);
		logfile << std::endl;
	}

	printf(SolverType::EnumToString(solverType));
	printf(":\n--------------------------------\n");
	logfile << SolverType::EnumToString(solverType) << ":" << std::endl;

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
		case SolverType::NQueensGenetic:
			solver.EvolveGenetic();
			break;
		}

		if (solver.HasSolved())
		{
			printf("%d. Generation: ", i);
			solver.PrintSolution();

			logfile << 0;

			break;
		}

		if (print == Print::Fitness)
		{
			printf("%d. Generation Fitness: ", i + 1);
			solver.PrintFitness();

			logfile << solver.GetFitness() << ",";
		}

		if (i == maxGenerations - 1)
			printf("No solution found in %d generations.\n", maxGenerations);
	}
	printf("\n");
	logfile << std::endl;

	logfile.close();
}

void DisplayQueens(GenomeQueens* solution)
{
	int windowSize = 800;
	sf::VideoMode vm(windowSize, windowSize);
	sf::RenderWindow window(vm, "Queens Problem - Alex Müller");

	sf::View gameView(sf::FloatRect(0, 0, windowSize, windowSize));
	window.setView(gameView);

	float rectSize = windowSize / static_cast<float>(queenCount);
	sf::RectangleShape square(sf::Vector2f(rectSize, rectSize));

	sf::CircleShape queen(rectSize * 0.5f);
	queen.setFillColor(sf::Color(0xFF6A00FF));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		for(int y = 0; y < queenCount; ++y)
		{
			for (int x = 0; x < queenCount; ++x)
			{
				if ((x + y) % 2 == 0)
					square.setFillColor(sf::Color::White);
				else
					square.setFillColor(sf::Color::Black);

				square.setPosition(rectSize * x, rectSize * y);

				window.draw(square);
			}
		}
		
		for (size_t q = 0; q < queenCount; ++q)
		{
			queen.setPosition(rectSize * q, rectSize * solution->values[q]);
			window.draw(queen);
		}

		window.display();
	}
}