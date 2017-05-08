#ifndef GENOME_XYAB_H
#define GENOME_XYAB_H

#include "BaseGenome.h"

class GenomeXYAB : public BaseGenome
{
public:
	struct Values
	{
		int x, y, a, b;
	};

	GenomeXYAB(int x, int y, int a, int b);
	GenomeXYAB(Values values);
	GenomeXYAB(const GenomeXYAB& other);
	~GenomeXYAB();

	BaseGenome* MutateRandom(int mutationStrength) override;
	BaseGenome* GetRandomGenome(void) override;
	int GetFitness(void) override;
	int GetOptimalFitness(void) override;

	BaseGenome* GetBlendedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength) override;
	BaseGenome* GetCombinedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength) override;
	std::vector<BaseGenome*> GetOnePointCrossoverGenome(const std::vector<BaseGenome*>& parents, float mutationChance) override;

	void Print(void) override;

	Values values;
};

#endif
