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

	int splittingPoint = StaticXorShift::GetIntInRange(1, m_columns - 2);

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

	// mutation
	for (int i = 0; i < 2; ++i)
	{
		if (StaticXorShift::GetPercentage() < mutationChance)
		{
			for(size_t valueIndex = 0; valueIndex < m_columns; ++valueIndex)
			{
				int modifier = StaticXorShift::GetIntInRange(-(m_columns - 1), m_columns - 1);

				if(StaticXorShift::GetPercentage() < 1.0f / static_cast<float>(m_columns))
				{
					if (i == 0)
					{
						firstChild->values[valueIndex] += modifier;

						if (firstChild->values[valueIndex] >= m_columns)
							firstChild->values[valueIndex] -= m_columns;

						if (firstChild->values[valueIndex] < 0)
							firstChild->values[valueIndex] = -firstChild->values[valueIndex];
					}
					else
					{
						secondChild->values[valueIndex] += modifier;

						if (secondChild->values[valueIndex] >= m_columns)
							secondChild->values[valueIndex] -= m_columns;

						if (secondChild->values[valueIndex] < 0)
							secondChild->values[valueIndex] = -secondChild->values[valueIndex];
					}
				}
			}
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
