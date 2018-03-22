#include "StdAfx.h"
#include "GATsp.h"


void CGATsp::CreateStartingPopulation()
{
	m_vecPopulation.clear();
	for (int i=0;i<POPULATION;i++)
	{
		SChromosome chrom(CITYNUMBERS);
		m_vecPopulation.push_back(chrom);
	}
	m_iEpoch=0;
}

void CGATsp::CaculatePopulationFitness()
{
	double shortest=99999999999.0;
	double longest=-999999.0;
	for (int i=0;i<m_vecPopulation.size();i++)
	{
		double tour=m_pMap->CaculateRouteLenght(m_vecPopulation[i].m_vecGenBit);
		m_vecPopulation[i].m_dFitness=tour;
		if (tour<shortest)
		{
			m_dShortestRoute=tour;
			shortest=tour;
		}
		if (tour>longest)
		{
			m_dLongestRoute=tour;
			longest=tour;
		}
	}
	for (int i=0;i<m_vecPopulation.size();i++)
	{
		m_vecPopulation[i].m_dFitness=m_dLongestRoute-m_vecPopulation[i].m_dFitness;
	}
	CaculateBestWorst();
}

void CGATsp::CaculateBestWorst()
{
	double best=-99999999.0;
	double worst=999999999.0;
	for (int i=0;i<m_vecPopulation.size();i++)
	{
		if (m_vecPopulation[i].m_dFitness>best)
		{
			best=m_vecPopulation[i].m_dFitness;
			m_iShortestPos=i;
			m_dBestFitness=best;
		}
		if (m_vecPopulation[i].m_dFitness<worst)
		{
			worst=m_vecPopulation[i].m_dFitness;
			m_dWorstFitness=worst;
		}
		m_dTotalFitness+=m_vecPopulation[i].m_dFitness;
	}
	m_dAverageFitness=m_dTotalFitness/(double)POPULATION;
}

void CGATsp::Reset()
{
	
	m_dTotalFitness=0;
	m_dShortestRoute=0;
	m_dLongestRoute=0;
	m_iShortestPos=0;
	m_dBestFitness=0;
	m_dWorstFitness=0;
	m_dAverageFitness=0;
	m_dSigma=1;
	m_dBoltzmanTemp=2*CITYNUMBERS;
	
	
}

SChromosome& CGATsp::RouletteSelect()
{
	double fslice=RandDouble()*m_dTotalFitness;
	double t=0;
	int pos=0;
	for (int i=0;i<m_vecPopulation.size();i++)
	{
		t+=m_vecPopulation[i].m_dFitness;
		if (t>fslice)
		{
			pos=i;
			break;
		}
	}
	return m_vecPopulation[pos];
}

SChromosome& CGATsp::TournamentSelect()
{
	int pos=0;
	double f=0;
	int select=0;
	for (int i=0;i<TOURNAMENT_NUM;i++)
	{
		pos=RandInt(0,POPULATION);
		if (m_vecPopulation[pos].m_dFitness>f)
		{
			f=m_vecPopulation[pos].m_dFitness;
			select=pos;
		}
	}
	return m_vecPopulation[select];
}

SChromosome& CGATsp::ALT_TournamentSelect()
{
	int pos1=RandInt(0,POPULATION);
	int pos2=RandInt(0,POPULATION);
	while(pos1==pos2)
	{
		pos2=RandInt(0,POPULATION);
	}
	double r=RandDouble();
	if (r<ALT_TOURNAMENT_CT)
	{
		if (m_vecPopulation[pos1].m_dFitness>m_vecPopulation[pos2].m_dFitness)
		{
			return m_vecPopulation[pos1];
		}else
		{
			return m_vecPopulation[pos2];
		}
	}else
	{
		if (m_vecPopulation[pos1].m_dFitness<m_vecPopulation[pos2].m_dFitness)
		{
			return m_vecPopulation[pos1];
		}else
		{
			return m_vecPopulation[pos2];
		}
	}
}

SChromosome& CGATsp::SelectTypeChoose(SelectType st)
{
	switch(st)
	{
	case ROULETTE:
		return RouletteSelect();
		break;
	case TOURNAMENT:
		return TournamentSelect();
		break;
	case ALT_TOURNAMENT:
		return ALT_TournamentSelect();
		break;
	
	}
}

void CGATsp::FitnessRank(vector<SChromosome>&schrom)
{
	sort(schrom.begin(),schrom.end());
	for (int i=0;i<schrom.size();i++)
	{
		schrom[i].m_dFitness=i;
	}
	m_dTotalFitness=0;
	CaculateBestWorst();
}

void CGATsp::FitnessSigma(vector<SChromosome>&schrom)
{
	double total=0;
	for (int i=0;i<schrom.size();i++)
	{
		total+=(schrom[i].m_dFitness-m_dAverageFitness)*(schrom[i].m_dFitness-m_dAverageFitness);
	}
	m_dSigma=sqrt(total/(double)POPULATION);
	for(int i=0;i<schrom.size();i++)
	{
		double old=schrom[i].m_dFitness;
		schrom[i].m_dFitness=(old-m_dAverageFitness)/(2*m_dSigma);
	}
	m_dTotalFitness=0;
	CaculateBestWorst();
}

void CGATsp::FitnessBoltzman(vector<SChromosome>&schrom)
{
	m_dBoltzmanTemp-=BOLTZMAN_DT;
	if (m_dBoltzmanTemp<BOLTZMAN_MIN_TEMP)
	{
		m_dBoltzmanTemp=BOLTZMAN_MIN_TEMP;
	}
	vector<double>expBoltz;
	double aver=0;
	for (int i=0;i<schrom.size();i++)
	{
		expBoltz.push_back(exp(schrom[i].m_dFitness/m_dBoltzmanTemp));
		aver+=expBoltz[i];
	}
	aver/=POPULATION;
	for (int i=0;i<schrom.size();i++)
	{
		schrom[i].m_dFitness=expBoltz[i]/aver;
	}
	m_dTotalFitness=0;
	CaculateBestWorst();
}

void CGATsp::FitnessType(Scaletype sl)
{
	switch(sl)
	{
	case NONE:
		break;
	case RANK:
		FitnessRank(m_vecPopulation);
		break;
	case SIGMA:
		FitnessSigma(m_vecPopulation);
		break;
	case BOLTZMAN:
		FitnessBoltzman(m_vecPopulation);
		break;
	}
}

void CGATsp::CrossoverPMX(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2)
{
	baby1=mum;
	baby2=dad;
	vector<int>::iterator iter1,iter2;
	if (RandDouble()<CROSSOVER_RATE || mum!=dad)
	{
		int posbegin=RandInt(0,mum.size()/2);
		int posend=RandInt(mum.size()/2,mum.size());
		for (int i=posbegin;i<=posend;i++)
		{
			iter1=find(baby1.begin(),baby1.end(),mum[i]);
			iter2=find(baby1.begin(),baby1.end(),dad[i]);
			swap(*iter1,*iter2);
			iter1=find(baby2.begin(),baby2.end(),mum[i]);
			iter2=find(baby2.begin(),baby2.end(),dad[i]);
			swap(*iter1,*iter2);
		}
	}
}

void CGATsp::CrossoverOBX(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2)
{
	baby1=mum;
	baby2=dad;
	if (RandDouble()>CROSSOVER_RATE || mum==dad)
	{
		return;
	}
	vector<int>pos;
	vector<int>tempval;
	int segment=RandInt(4,mum.size());
	int segnumber=mum.size()/segment;
	segment=mum.size()/segnumber;
	int r;
	for(int i=0;i<segnumber;i++)
	{
		if (segment*(i+1)>mum.size())
		{
			r=RandInt(segment*i,mum.size());

		}else
		{
			r=RandInt(segment*i,segment*(i+1));
		}
		pos.push_back(r);
		tempval.push_back(mum[r]);
	}
	int crosspos=0;
	for (int i=0;i<baby2.size();i++)
	{
		for(int j=0;j<tempval.size();j++)
		{
			if (baby2[i]==tempval[j])
			{
				baby2[i]=tempval[crosspos];
				crosspos++;
				break;
			}
		}
	}
	tempval.clear();
	crosspos=0;
	for (int i=0;i<pos.size();i++)
	{
		tempval.push_back(dad[pos[i]]);
	}
	for (int i=0;i<baby1.size();i++)
	{
		for (int j=0;j<tempval.size();j++)
		{
			if (baby1[i]==tempval[j])
			{
				baby1[i]=tempval[crosspos];
				crosspos++;
				break;
			}
		}
	}
}

void CGATsp::CrossoverPBX(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2)
{
	
	if (RandDouble()>CROSSOVER_RATE || mum==dad)
	{
		baby1=mum;
		baby2=dad;
		return;
	}
	baby1.assign(mum.size(),-1);
	baby2.assign(dad.size(),-1);
	vector<int>pos;
	vector<int>tempvalmum;
	vector<int>tempvaldad;
	int segment=RandInt(4,mum.size());
	int segnumber=mum.size()/segment;
	segment=mum.size()/segnumber;
	int r;
	for(int i=0;i<segnumber;i++)
	{
		if (segment*(i+1)>mum.size())
		{
			r=RandInt(segment*i,mum.size());

		}else
		{
			r=RandInt(segment*i,segment*(i+1));
		}
		pos.push_back(r);
		tempvalmum.push_back(mum[r]);
		tempvaldad.push_back(dad[r]);
	}
	for (int i=0;i<pos.size();i++)
	{
		baby1[pos[i]]=tempvalmum[i];
		baby2[pos[i]]=tempvaldad[i];
	}
	vector<int>::iterator iter;
	for (int i=0;i<dad.size();i++)
	{
		iter=find(baby1.begin(),baby1.end(),dad[i]);
		if (iter==baby1.end())
		{
			for (int j=0;j<baby1.size();j++)
			{
				if (baby1[j]==-1)
				{
					baby1[j]=dad[i];
					break;
				}
			}
		}
	}
	for (int i=0;i<mum.size();i++)
	{
		iter=find(baby2.begin(),baby2.end(),mum[i]);
		if (iter==baby2.end())
		{
			for (int j=0;j<baby2.size();j++)
			{
				if (baby2[j]==-1)
				{
					baby2[j]=mum[i];
					break;
				}
			}
		}
	}
}

void CGATsp::Crossover(const vector<int>&mum,const vector<int>&dad,vector<int>&baby1,vector<int>&baby2,CrossoverType ct)
{
	switch(ct)
	{
	case PMX:
		CrossoverPMX(mum,dad,baby1,baby2);
		break;
	case OBX:
		CrossoverOBX(mum,dad,baby1,baby2);
		break;
	case PBX:
		CrossoverPBX(mum,dad,baby1,baby2);
		break;
	}
}

void CGATsp::MutateEM(vector<int>&schrom)
{
	if(RandDouble()<MUTATION_RATE)
	{
		int pos1=RandInt(0,schrom.size());
		int pos2=RandInt(0,schrom.size());
		while(pos1==pos2)
		{
			pos2=RandInt(0,schrom.size());
		}
		swap(schrom[pos1],schrom[pos2]);
	}
}

void CGATsp::MutateDM(vector<int>&schrom)
{
	if (RandDouble()<MUTATION_RATE)
	{
		int posbegin=RandInt(0,schrom.size()-1);
		int posend=RandInt(0,schrom.size());
		while(posend<=posbegin)
		{
			posend=RandInt(0,schrom.size());
		}
		vector<int>::iterator iterbegin=schrom.begin()+posbegin;
		vector<int>::iterator iterend=schrom.begin()+posend;
		vector<int>temp;
		temp.assign(iterbegin,iterend);
		schrom.erase(iterbegin,iterend);
		int posrand=RandInt(0,schrom.size());
		schrom.insert(schrom.begin()+posrand,temp.begin(),temp.end());
	}
}

void CGATsp::MutateSM(vector<int>&schrom)
{
	if (RandDouble()<MUTATION_RATE)
	{
		int posbegin=RandInt(0,schrom.size()-1);
		int posend=RandInt(0,schrom.size());
		while(posend<=posbegin)
		{
			posend=RandInt(0,schrom.size());
		}
		int span=posend-posbegin;
		int numberofswap=span;
		while(--numberofswap)
		{
			vector<int>::iterator iter1=schrom.begin();
			vector<int>::iterator iter2=schrom.begin();
			advance(iter1,posbegin+RandInt(0,span));
			advance(iter2,posbegin+RandInt(0,span));
			swap(*iter1,*iter2);
		}
	}
}

void CGATsp::MutateIM(vector<int>&schrom)
{
	if (RandDouble()<MUTATION_RATE)
	{
		int pos=RandInt(0,schrom.size());
		vector<int>::iterator iter=schrom.begin()+pos;
		int temp=*iter;
		schrom.erase(iter);
		iter=schrom.begin()+RandInt(0,schrom.size());
		schrom.insert(iter,temp);
	}
}

void CGATsp::Mutation(vector<int>&schrom,MutationType mt)
{
	switch(mt)
	{
	case EM:
		MutateEM(schrom);
		break;
	case DM:
		MutateDM(schrom);
		break;
	case SM:
		MutateSM(schrom);
		break;
	case IM:
		MutateIM(schrom);
		break;
	}
}

void CGATsp::Elitism(int numcopy,vector<SChromosome>& vecBabyPopulation)
{
	sort(m_vecPopulation.begin(),m_vecPopulation.end());
	for (int i=0;i<numcopy;i++)
	{
		vecBabyPopulation.push_back(m_vecPopulation[POPULATION-i-1]);
	}
}

void CGATsp::SUSelection(vector<SChromosome>& vecBabyPopulation)
{
	if (m_dWorstFitness<0)
	{
		for (int i=0;i<m_vecPopulation.size();i++)
		{
			m_vecPopulation[i].m_dFitness+=fabs(m_dWorstFitness);
		}
		CaculateBestWorst();
	}
	int curgen=0;
	double sum=0;
	int numtoadd=POPULATION-vecBabyPopulation.size();
	double pointergap=m_dTotalFitness/(double)numtoadd;
	double ptr=RandDouble()*pointergap;
	while(vecBabyPopulation.size()<numtoadd)
	{
		for (sum+=m_vecPopulation[curgen].m_dFitness;sum>ptr;ptr+=pointergap)
		{
			vecBabyPopulation.push_back(m_vecPopulation[curgen]);
			if (vecBabyPopulation.size()==numtoadd)
			{
				return;
			}
		}
		curgen++;
	}
}

CGATsp::CGATsp(void)
{
	m_pMap=new CTSPMap();
	CreateStartingPopulation();
	m_eSelecttype=TOURNAMENT;
	m_eScaletype=NONE;
	m_eCrossovertype=PMX;
	m_eMutationtype=IM;
	m_dTotalFitness=0;
	m_dShortestRoute=0;
	m_dLongestRoute=0;
	m_iShortestPos=0;
	m_dBestFitness=0;
	m_dWorstFitness=0;
	m_dAverageFitness=0;
	m_dSigma=1;
	m_dBoltzmanTemp=2*CITYNUMBERS;
	m_bElitism=true;
	m_bSus=false;
	m_bStart=true;
	m_iEpoch=0;
	//CaculatePopulationFitness();
}


CGATsp::~CGATsp(void)
{
	delete m_pMap;
}

void CGATsp::Rander(HDC surface,int width,int height)
{
	for (int i=0;i<m_pMap->m_vecCity.size();i++)
	{
		int x=(int)(m_pMap->m_vecCity[i].x);
		int y=(int)(m_pMap->m_vecCity[i].y);
		Ellipse(surface,x-CITYSIZE,y+CITYSIZE,x+CITYSIZE,y-CITYSIZE);
	}
	wstring s=TEXT("最佳路径长度：           ")+dtos(m_pMap->m_dBestPossibleRoute);
	TextOut(surface,0,0,s.c_str(),s.size());
	int posx=(int)m_pMap->m_vecCity[m_vecPopulation[m_iShortestPos].m_vecGenBit[0]].x;
	int posy=(int)m_pMap->m_vecCity[m_vecPopulation[m_iShortestPos].m_vecGenBit[0]].y;
	MoveToEx(surface,posx,posy,NULL);
	for (int i=0;i<CITYNUMBERS;i++)
	{
		int linex=(int)m_pMap->m_vecCity[m_vecPopulation[m_iShortestPos].m_vecGenBit[i]].x;
		int liney=(int)m_pMap->m_vecCity[m_vecPopulation[m_iShortestPos].m_vecGenBit[i]].y;
		LineTo(surface,linex,liney);
		wstring s=itos(i);
		TextOut(surface,linex,liney,s.c_str(),s.size());
	}
	LineTo(surface,posx,posy);
	wstring r=TEXT("本时代最佳路径长度：")+dtos(m_dShortestRoute);
	TextOut(surface,0,20,r.c_str(),r.size());
	wstring e=TEXT("时代：")+itos(m_iEpoch);
	TextOut(surface,0,40,e.c_str(),e.size());
}

void CGATsp::Stop()
{
	if (m_bStart)
	{
		m_bStart=false;
	}else
	{
		m_bStart=true;
	}
}

void CGATsp::Run()
{
	CreateStartingPopulation();
	m_bStart=true;
}

void CGATsp::Epoch()
{
	Reset();
	CaculatePopulationFitness();
	if (m_dShortestRoute<=m_pMap->m_dBestPossibleRoute)
	{
		m_bStart=false;
		return;
	}
	FitnessType(m_eScaletype);
	if (m_dSigma==0)
	{
		return;
	}
	vector<SChromosome> babypopulation;
	if (m_bSus)
	{
		vector<SChromosome>temppopulaton;
		SUSelection(temppopulaton);
		for (int i=0;i<temppopulaton.size();i+=2)
		{
			SChromosome baby1,baby2;
			Crossover(temppopulaton[i].m_vecGenBit,temppopulaton[i+1].m_vecGenBit,baby1.m_vecGenBit,baby2.m_vecGenBit,m_eCrossovertype);
			Mutation(baby1.m_vecGenBit,m_eMutationtype);
			Mutation(baby2.m_vecGenBit,m_eMutationtype);
			babypopulation.push_back(baby1);
			babypopulation.push_back(baby2);
		}
	}else
	{
		if (m_bElitism)
		{
			Elitism(2,babypopulation);
		}
		SChromosome mum,dad;
		while(babypopulation.size()<POPULATION)
		{
			mum=SelectTypeChoose(m_eSelecttype);
			dad=SelectTypeChoose(m_eSelecttype);
			SChromosome baby1,baby2;
			Crossover(mum.m_vecGenBit,dad.m_vecGenBit,baby1.m_vecGenBit,baby2.m_vecGenBit,m_eCrossovertype);
			Mutation(baby1.m_vecGenBit,m_eMutationtype);
			Mutation(baby2.m_vecGenBit,m_eMutationtype);
			babypopulation.push_back(baby1);
			babypopulation.push_back(baby2);
		}
	}
	m_vecPopulation=babypopulation;
	m_iEpoch++;
}

SChromosome::SChromosome(int genlength):m_dFitness(0)
{
	vector<int>::iterator iter;
	int pos;
	for (int i=0;i<genlength;i++)
	{
		bool done=true;
		while(done)
		{
			pos=RandInt(0,CITYNUMBERS);
			iter=find(m_vecGenBit.begin(),m_vecGenBit.end(),pos);
			if (iter==m_vecGenBit.end())
			{
				m_vecGenBit.push_back(pos);
				done=false;
			}
		}
		
	}
}
