#include "GenomeQueens.h"
#include "StaticXorShift.h"
#include <cassert>
#include <algorithm>

#include <iostream>
#include <iterator>

GenomeQueens::GenomeQueens(int columnCount)
	: m_columns(columnCount)
{
	values.resize(m_columns);
}

GenomeQueens::~GenomeQueens()
{
}

BaseGenome* GenomeQueens::MutateRandom(int mutationStrength)
{
	return nullptr;
}

BaseGenome* GenomeQueens::GetBlendedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength)
{
	return nullptr;
}

BaseGenome* GenomeQueens::GetCombinedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength)
{
	return nullptr;
}

BaseGenome* GenomeQueens::GetRandomGenome()
{
	GenomeQueens* genome = new GenomeQueens(m_columns);
	
	for(unsigned int i = 0; i < m_columns; ++i)
	{
		genome->values[i] = StaticXorShift::GetIntInRange(0, m_columns - 1);
	}

	return genome;
}

int GenomeQueens::GetFitness()
{
	int clashes = 0;

	for (int column = 0; column < m_columns; ++column)
	{
		for (int queenNum = column + 1; queenNum < m_columns; ++queenNum)
		{
			// check if queen is in same row
			if (values[column] == values[queenNum])
				++clashes;

			// check if queen is in same diagonal
			if (abs(column - queenNum) == abs(values[column] - values[queenNum]))
				++clashes;
		}
	}

	return clashes;
}

int GenomeQueens::GetOptimalFitness()
{
	return 0;
}

std::vector<BaseGenome*> GenomeQueens::GetOnePointCrossoverGenome(const std::vector<BaseGenome*>& parents, float mutationChance)
{
	assert(parents.size() == 2);

	int splittingPoint = StaticXorShift::GetIntInRange(0, m_columns - 1);

	GenomeQueens* firstParent = dynamic_cast<GenomeQueens*>(parents[0]);
	GenomeQueens* secondParent = dynamic_cast<GenomeQueens*>(parents[1]);

	GenomeQueens* firstChild = new GenomeQueens(m_columns);
	GenomeQueens* secondChild = new GenomeQueens(m_columns);

	for (unsigned int i = 0; i < m_columns; ++i)
	{
		if(i < splittingPoint)
		{
			firstChild->values[i] = firstParent->values[i];
			secondChild->values[i] = secondParent->values[i];
		}
		else
		{
			firstChild->values[i] = secondParent->values[i];
			secondChild->values[i] = firstParent->values[i];
		}
	}

	return std::vector<BaseGenome*> { firstChild, secondChild };
}

void GenomeQueens::Print()
{
	printf("<");

	for (unsigned int i = 0; i < m_columns; ++i)
	{
		printf("%d", values[i]);

		if (i != m_columns - 1)
			printf(", ");
	}

	printf(">");
}
