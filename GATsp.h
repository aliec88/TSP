#pragma once
#include "TSPMap.h"
struct SChromosome
{
	vector<int> m_vecGenBit;
	double m_dFitness;
	SChromosome(){m_dFitness=0;}
	SChromosome(int genlength);
	bool operator<(const SChromosome& right){return this->m_dFitness<right.m_dFitness;}
};
enum SelectType{ROULETTE,TOURNAMENT,ALT_TOURNAMENT,SUS};
enum Scaletype{NONE,RANK,SIGMA,BOLTZMAN};
enum CrossoverType{PMX,OBX,PBX};
enum MutationType{EM,DM,SM,IM};
class CGATsp
{
private:
	CTSPMap* m_pMap;
	vector<SChromosome>m_vecPopulation;
	SelectType m_eSelecttype;
	Scaletype  m_eScaletype;
	CrossoverType m_eCrossovertype;
	MutationType  m_eMutationtype;
	bool m_bElitism;
	double m_dTotalFitness;
	double m_dShortestRoute;
	int m_iShortestPos;
	double m_dLongestRoute;
	double m_dBestFitness;
	double m_dWorstFitness;
	double m_dAverageFitness;
	double m_dSigma;
	double m_dBoltzmanTemp;
	bool m_bSus;
	bool m_bStart;
	int m_iEpoch;
private:
	void CreateStartingPopulation();
	void CaculatePopulationFitness();
	void CaculateBestWorst();
	void Reset();
	//select
	SChromosome& RouletteSelect();
	SChromosome& TournamentSelect();
	SChromosome& ALT_TournamentSelect();
	SChromosome& SelectTypeChoose(SelectType st);
	//scale
	void FitnessRank(vector<SChromosome>&schrom);
	void FitnessSigma(vector<SChromosome>&schrom);
	void FitnessBoltzman(vector<SChromosome>&schrom);
	void FitnessType(Scaletype sl);
	//crossover:
	void CrossoverPMX(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2);
	void CrossoverOBX(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2);
	void CrossoverPBX(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2);
	void Crossover(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2,CrossoverType ct);
	//mutation
	void MutateEM(vector<int>&schrom);
	void MutateDM(vector<int>&schrom);
	void MutateSM(vector<int>&schrom);
	void MutateIM(vector<int>&schrom);
	void Mutation(vector<int>&schrom,MutationType mt);
	//Elitism
	void Elitism(int numcopy,vector<SChromosome>& vecBabyPopulation);
	void SUSelection(vector<SChromosome>& vecBabyPopulation);
public:
	CGATsp(void);
	~CGATsp(void);
	void Rander(HDC surface,int width,int height);
	void SetSelectType(SelectType s){m_eSelecttype=s;}
	void SetScaleType(Scaletype s){m_eScaletype=s;}
	void SetCrossoverType(CrossoverType c){m_eCrossovertype=c;}
	void SetMutationType(MutationType m){m_eMutationtype=m;}
	void SetElitism(bool b){m_bElitism=b;}
	void SetSus(bool b){m_bSus=b;}
	bool Started(){return m_bStart;}
	void Stop();
	void Run();
	void Epoch();
};

