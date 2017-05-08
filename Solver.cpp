#include "Solver.h"
#include "StaticXorShift.h"
#include <algorithm>
#include <functional>

Solver::Solver(unsigned int my, unsigned int lambda, unsigned int rho, BaseGenome* baseGenome)
	: m_solution(nullptr)
	, m_baseGenome(baseGenome)
	, m_my(my)
	, m_lambda(lambda)
	, m_rho(rho)
	, m_mutationStrength(5)
{
	// generate random startign population
	ResetPopulation(m_my);
}

Solver::~Solver()
{
}

void Solver::EvolveOnePlusOne()
{
	// check if we found a solution
	if (CheckForSolution())
		return;

	// mutate genome
	BaseGenome* mutatedGenome = m_population[0].genome->MutateRandom(m_mutationStrength);
	
	// only use the fitter genome
	int mutatedFitness = mutatedGenome->GetFitness();
	if(mutatedFitness > m_population[0].fitness)
	{
		m_population[0].genome = mutatedGenome;
		m_population[0].fitness = mutatedFitness;
	}
}

void Solver::EvolveMyPlusLambda(InheritanceMode::Enum inheritanceMode)
{
	// check if we found a solution
	if (CheckForSolution())
		return;

	// select lambda random parents
	size_t lastParentIndex = m_population.size() - 1;
	for (unsigned int i = 0; i < m_lambda; ++i)
	{
		// inheritance + mutation to get new genome
		BaseGenome* newGenome = GetNewGenome(inheritanceMode, lastParentIndex);
		m_population.push_back({ newGenome, newGenome->GetFitness()});
	}

	// sort, so the fittest genomes are in the front
	sort(m_population.begin(), m_population.end(), std::greater<GenomeFitness>());

	// only the fittest my genomes live (parents + children)
	m_population.resize(m_my);
}

void Solver::EvolveMyCommaLambda(InheritanceMode::Enum inheritanceMode)
{
	// check if we found a solution
	if (CheckForSolution())
		return;

	std::vector<GenomeFitness> children;
	children.reserve(m_lambda);

	// select lambda random parents
	size_t lastParentIndex = m_population.size() - 1;
	for (unsigned int i = 0; i < m_lambda; ++i)
	{
		// inheritance + mutation to get new genome
		BaseGenome* newGenome = GetNewGenome(inheritanceMode, lastParentIndex);
		children.push_back({ newGenome, newGenome->GetFitness() });
	}

	// sort the children, so the fittest genomes are in the front
	sort(children.begin(), children.end(), std::greater<GenomeFitness>());

	// only the fittest genomes live (only children)
	children.resize(m_my);

	// best children are the new polulation
	for (size_t i = 0; i < m_population.size(); ++i)
	{
		delete m_population[i].genome;
	}
	m_population = children;
}

void Solver::EvolveMyLambda(MyLambdaSelection::Enum selectionMode)
{
	switch (selectionMode)
	{
	case MyLambdaSelection::Plus:
		EvolveMyPlusLambda(InheritanceMode::OnlyMutation);
		break;
	case MyLambdaSelection::Comma:
		EvolveMyCommaLambda(InheritanceMode::OnlyMutation);
		break;
	}
}

void Solver::EvolveMyRhoLambda(InheritanceMode::Enum inheritanceMode, MyLambdaSelection::Enum selectionMode)
{
	switch (selectionMode)
	{
	case MyLambdaSelection::Plus:
		EvolveMyPlusLambda(inheritanceMode);
		break;
	case MyLambdaSelection::Comma:
		EvolveMyCommaLambda(inheritanceMode);
		break;
	}
}

bool Solver::HasSolved() const
{
	return m_hasSolved;
}

void Solver::ResetPopulation(size_t size)
{
	m_population.resize(size);

	
	for (unsigned int i = 0; i < size; ++i)
	{
		BaseGenome* randomGenome = m_baseGenome->GetRandomGenome();
		m_population[i] = {randomGenome, randomGenome->GetFitness()};
	}

	sort(m_population.begin(), m_population.end(), std::greater<GenomeFitness>());
}

void Solver::PrintFitness() const
{
	for (size_t i = 0; i < m_population.size(); ++i)
	{
		printf("%d, ", m_population[i].fitness);
	}
	printf("\n");
}

void Solver::PrintSolution() const
{
	if (m_hasSolved)
	{
		printf("The solution is ");
		m_solution->Print();
		printf(".\n");
	}
	else
	{
		printf("No solution found yet.");
	}
}

BaseGenome* Solver::GetNewGenome(InheritanceMode::Enum inheritanceMode, size_t lastParentIndex)
{
	BaseGenome* newGenome;

	std::vector<BaseGenome*> parents;
	parents.reserve(m_rho);

	switch (inheritanceMode)
	{
	case InheritanceMode::Blend:
		for (size_t cnt = 0; cnt < m_rho; ++cnt)
		{
			parents.emplace_back(m_population[StaticXorShift::GetIntInRange(0, lastParentIndex)].genome);
		}

		newGenome = m_baseGenome->GetBlendedGenome(parents, m_mutationStrength);
		break;
	case InheritanceMode::Combination:
		for (size_t cnt = 0; cnt < m_rho; ++cnt)
		{
			parents.emplace_back(m_population[StaticXorShift::GetIntInRange(0, lastParentIndex)].genome);
		}

		newGenome = m_baseGenome->GetCombinedGenome(parents, m_mutationStrength);
		break;
	case InheritanceMode::OnlyMutation:
	default:
		size_t randomParentIndex = StaticXorShift::GetIntInRange(0, lastParentIndex);
		newGenome = m_population[randomParentIndex].genome->MutateRandom(m_mutationStrength);
		break;
	}

	return newGenome;
}

bool Solver::CheckForSolution()
{
	if (m_population[0].fitness == m_population[0].genome->GetOptimalFitness())
	{
		m_hasSolved = true;
		m_solution = m_population[0].genome;
		return true;
	}

	return false;
}
