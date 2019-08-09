#pragma once
#include <memory>
#include <tuple>
#include <cmath>

class Rolling {
public:
	typedef std::shared_ptr<Rolling> SharedRolling;
	Rolling(SharedRolling S):SR(S){}
	
protected:
	SharedRolling SR;
	double L = 0;
	double X = 0;
	double Y = 0;
	double Angle = 0;

	double Rad = 3.141592 / 180.0;
	
	int N;
	
	static int C;
};

int Rolling::C = 0;