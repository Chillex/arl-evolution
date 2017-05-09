#ifndef GENOME_QUEENS_H
#define GENOME_QUEENS_H

#include "BaseGenome.h"

class GenomeQueens : BaseGenome
{
public:

	GenomeQueens(int columnCount);
	~GenomeQueens();

	BaseGenome* MutateRandom(int mutationStrength) override;
	BaseGenome* GetRandomGenome(void) override;
	int GetFitness(void) override;
	int GetOptimalFitness(void) override;

	BaseGenome* GetBlendedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength) override;
	BaseGenome* GetCombinedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength) override;
	std::vector<BaseGenome*> GetOnePointCrossoverGenome(const std::vector<BaseGenome*>& parents, float mutationChance) override;

	void Print(void) override;

	std::vector<int> values;
private:
	int m_columns;
};

#endif
