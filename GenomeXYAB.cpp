#include "GenomeXYAB.h"

#include "StaticXorShift.h"
#include <cmath>

GenomeXYAB::GenomeXYAB(int x, int y, int a, int b)
	: values({x, y, a, b})
{
}

GenomeXYAB::GenomeXYAB(Values values)
	: values(values)
{
}

GenomeXYAB::GenomeXYAB(const GenomeXYAB& other)
	: values(other.values)
{
}

GenomeXYAB::~GenomeXYAB()
{
}

BaseGenome* GenomeXYAB::MutateRandom(int mutationStrength)
{
	Values mutations = {
		this->values.x + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength),
		this->values.y + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength),
		this->values.a + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength),
		this->values.b + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength)
	};
	return new GenomeXYAB(mutations);
}

BaseGenome* GenomeXYAB::GetRandomGenome()
{
	int range = 25;

	int x = StaticXorShift::GetIntInRange(-range, range);
	int y = StaticXorShift::GetIntInRange(-range, range);
	int a = StaticXorShift::GetIntInRange(-range, range);
	int b = StaticXorShift::GetIntInRange(-range, range);

	return new GenomeXYAB(x, y, a, b);
}

int GenomeXYAB::GetFitness()
{
	int fitness = (this->values.a > this->values.b) ? GetOptimalFitness() : -GetOptimalFitness();

	// if a < b, use the difference in the fitness value, so smaller gaps are better
	if (fitness < 0)
	{
		fitness += this->values.a - this->values.b;
	}

	int leftSide = 3 * pow(this->values.x, 2) + 5 * pow(this->values.y, 3);
	int rightSide = 7 * this->values.a + 3 * pow(this->values.b, 2);
	fitness -= abs(leftSide - rightSide);

	return fitness;
}

int GenomeXYAB::GetOptimalFitness()
{
	return 100000;
}

BaseGenome* GenomeXYAB::GetBlendedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength)
{
	GenomeXYAB* newGenome = new GenomeXYAB(0, 0, 0, 0);

	for (size_t i = 0; i < parents.size(); ++i)
	{
		GenomeXYAB* parent = dynamic_cast<GenomeXYAB*>(parents[i]);
		newGenome->values.x += parent->values.x;
		newGenome->values.y += parent->values.y;
		newGenome->values.a += parent->values.a;
		newGenome->values.b += parent->values.b;
	}

	newGenome->values.x = newGenome->values.x / static_cast<int>(parents.size()) + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);
	newGenome->values.y = newGenome->values.y / static_cast<int>(parents.size()) + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);
	newGenome->values.a = newGenome->values.a / static_cast<int>(parents.size()) + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);
	newGenome->values.b = newGenome->values.b / static_cast<int>(parents.size()) + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);

	return newGenome;
}

BaseGenome* GenomeXYAB::GetCombinedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength)
{
	GenomeXYAB* newGenome = new GenomeXYAB(0, 0, 0, 0);

	newGenome->values.x = dynamic_cast<GenomeXYAB*>(parents[StaticXorShift::GetIntInRange(0, parents.size() - 1)])->values.x + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);
	newGenome->values.y = dynamic_cast<GenomeXYAB*>(parents[StaticXorShift::GetIntInRange(0, parents.size() - 1)])->values.y + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);
	newGenome->values.a = dynamic_cast<GenomeXYAB*>(parents[StaticXorShift::GetIntInRange(0, parents.size() - 1)])->values.a + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);
	newGenome->values.b = dynamic_cast<GenomeXYAB*>(parents[StaticXorShift::GetIntInRange(0, parents.size() - 1)])->values.b + StaticXorShift::GetIntInRange(-mutationStrength, mutationStrength);

	return newGenome;
}

std::vector<BaseGenome*> GenomeXYAB::GetOnePointCrossoverGenome(const std::vector<BaseGenome*>& parents, float mutationChance)
{
	return std::vector<BaseGenome*>();
}

void GenomeXYAB::Print()
{
	printf("<%d, %d, %d, %d>", values.x, values.y, values.a, values.b);
}
