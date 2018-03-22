#pragma once
struct SCordinate
{
	double x;
	double y;
	SCordinate(double a,double b){x=a;y=b;}
	SCordinate(){}
};
class CTSPMap
{
public:
	vector<SCordinate> m_vecCity;
	double m_dBestPossibleRoute;
private:
	int m_iCityNum;
	int m_iMapWidth;
	int m_iMapHeight;
private:
	void CreateCityCircle();
	double CaculateAtoB(SCordinate& a,SCordinate& b);
	void CaculatePossibleRouteLength();
public:
	CTSPMap(void);
	~CTSPMap(void);
	double CaculateRouteLenght(vector<int>& route);

};

