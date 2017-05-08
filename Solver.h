#ifndef SOLVER_H
#define SOLVER_H
#include "GenomeXYAB.h"
#include <vector>

struct MyLambdaSelection
{
	enum Enum
	{
		Plus,
		Comma
	};
};

struct InheritanceMode
{
	enum Enum
	{
		Blend,
		Combination,
		OnlyMutation
	};
};


class Solver
{
public:
	struct GenomeFitness
	{
		BaseGenome* genome;
		int fitness;

		GenomeFitness() : genome(nullptr), fitness(0)
		{}

		GenomeFitness(BaseGenome* genome, int fitness) : genome(genome), fitness(fitness)
		{}

		bool operator< (const GenomeFitness& other) const
		{
			return (fitness < other.fitness);
		}

		bool operator> (const GenomeFitness& other) const
		{
			return (fitness > other.fitness);
		}
	};

	Solver(unsigned int my, unsigned int lambda, unsigned int rho, BaseGenome* baseGenome);
	~Solver();

	void EvolveOnePlusOne(void);
	void EvolveMyPlusLambda(InheritanceMode::Enum inheritanceMode);
	void EvolveMyCommaLambda(InheritanceMode::Enum inheritanceMode);
	void EvolveMyLambda(MyLambdaSelection::Enum selectionMode);
	void EvolveMyRhoLambda(InheritanceMode::Enum inheritanceMode, MyLambdaSelection::Enum selectionMode);

	bool HasSolved(void) const;
	void ResetPopulation(size_t size);

	void PrintFitness(void) const;
	void PrintSolution(void) const;

private:
	std::vector<GenomeFitness> m_population;
	BaseGenome* m_solution;
	BaseGenome* m_baseGenome;

	unsigned int m_my;
	unsigned int m_lambda;
	unsigned int m_rho;

	int m_mutationStrength;

	bool m_hasSolved;

	BaseGenome* GetNewGenome(InheritanceMode::Enum inheritanceMode, size_t lastParentIndex);
	bool CheckForSolution(void);
};

#endif
