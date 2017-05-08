#ifndef BASE_GENOME_H
#define BASE_GENOME_H

#include <vector>

class BaseGenome
{
public:
	virtual ~BaseGenome();

	virtual BaseGenome* MutateRandom(int mutationStrength) = 0;
	virtual BaseGenome* GetRandomGenome(void) = 0;
	virtual int GetFitness(void) = 0;
	virtual int GetOptimalFitness(void) = 0;

	virtual void Print(void) = 0;

	virtual BaseGenome* GetBlendedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength) = 0;
	virtual BaseGenome* GetCombinedGenome(const std::vector<BaseGenome*>& parents, int mutationStrength) = 0;
	virtual std::vector<BaseGenome*> GetOnePointCrossoverGenome(const std::vector<BaseGenome*>& parents, float mutationChance) = 0;
};

inline BaseGenome::~BaseGenome()
{
}
#endif
