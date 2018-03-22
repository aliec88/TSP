#include "StdAfx.h"
#include "TSPMap.h"


void CTSPMap::CreateCityCircle()
{
	const int margin=70;
	double Radius;
	if (m_iMapHeight<m_iMapWidth)
	{
		Radius=m_iMapHeight/2-margin;
	}else
	{
		Radius=m_iMapWidth/2-margin;
	}
	SCordinate origin(m_iMapWidth/2.0,m_iMapHeight/2.0-margin/4);
	double angele=0;
	double segment=2*PI/m_iCityNum;
	SCordinate thiscity;
	while(angele<2*PI)
	{
		thiscity.x=Radius*cos(angele)+origin.x;
		thiscity.y=Radius*sin(angele)+origin.y;
		m_vecCity.push_back(thiscity);
		angele+=segment;
	}
}

double CTSPMap::CaculateAtoB(SCordinate& a,SCordinate& b)
{
	double xDist=a.x-b.x;
	double yDist=a.y-b.y;
	return sqrt(xDist*xDist+yDist*yDist);
}

void CTSPMap::CaculatePossibleRouteLength()
{
	for (int i=0;i<m_vecCity.size()-1;i++)
	{
		m_dBestPossibleRoute+=CaculateAtoB(m_vecCity[i],m_vecCity[i+1]);
		m_dBestPossibleRoute+=EPSILON;
	}
	m_dBestPossibleRoute+=CaculateAtoB(m_vecCity[0],m_vecCity[CITYNUMBERS-1]);
}

CTSPMap::CTSPMap(void)
{
	m_iMapHeight=WINDOW_HEIGHT;
	m_iMapWidth=WINDOW_WIDTH;
	m_iCityNum=CITYNUMBERS;
	m_dBestPossibleRoute=0;
	CreateCityCircle();
	CaculatePossibleRouteLength();
}




CTSPMap::~CTSPMap(void)
{
}

double CTSPMap::CaculateRouteLenght(vector<int>& route)
{
	double total=0;
	for (int i=0;i<route.size()-1;i++)
	{
		total+=CaculateAtoB(m_vecCity[route[i]],m_vecCity[route[i+1]]);
	}
	total+=CaculateAtoB(m_vecCity[route[0]],m_vecCity[route[route.size()-1]]);
	return total;
}
