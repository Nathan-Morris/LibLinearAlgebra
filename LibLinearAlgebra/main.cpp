#include "vec.h"
#include "mat.h"
#include "equnode.h"

using namespace std;
using namespace nmath;

/*
 4w + 1x + 2y - 3z= -16
-3w + 3x - 1y + 4z=  20
-1w + 2x + 5y + 1z=  -4
 5w + 4x + 3y - 1z= -10
*/


int main(int argc, char* argv[]) {
	mat<4, 4, double> A = {
		{  4,  1,  2, -3 },
		{ -3,  3, -1,  4 },
		{ -1,  2,  5,  1 },
		{  5,  4,  3, -1 }
	};

	mat<4, 1, double> B = { 
		{ -16 },
		{  20 },
		{  -4 },
		{ -10 }
	};

	auto C = ( A.inverse() ) * B;

	cout << C << endl;
}