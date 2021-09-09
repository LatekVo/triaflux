#include <array>
#include <cmath>
#include <math.h>
#include <vector>

const float PI = 3.14159; // for rad conversion

class Triangle {
	float base = 1;
	float tip_x = 0, tip_y = 0;

	//results
	float bari_x = 0, bari_y = 0;
	float ortho_x = 0, ortho_y = 0;
	float flux = 0; //diff
public:
	float a = 0, b = 0, c = 0;

	//A - left bot
	//B - right bot
	//C - found pt

	Triangle(float aplha, float beta, float theta) {
		
		// lhs / mid = sin a / sin b
		float lhs = base * (std::sin(a) / std::sin(b));
		
		// actual pos of lhs
		tip_x = lhs * std::cos(a); // y = 0
		tip_y = lhs * std::sin(a); // y = 0

		//verts = [0,0] [1,0] [x, y]
		bari_x = (tip_x + 1.f) / 3.f;
		bari_y = tip_y / 3.f;

		//slopes, perpendiculars
		float AC,  CB,  BA;
		float PAC, PCB, PBA;
		
		//NOTE: check all of this, as it's currently 0:30 and im barely thinking

		// AC
		if (tip_x != 0) {
			AC = tip_y / tip_x;
			PAC = -1 / AC;
		} else {
			AC = 0;
			PAC = 1;
		}

		// CB
		if (base - tip_x != 0) {
			CB = (0 - tip_y) / (base - tip_x);
			PCB = -1 / CB;
		} else {
			CB = 0;
			PCB = 1;
		}

		// BA
		BA = 0;
		PBA = 0;

		//altitude functions
		// y - Y = m * (x - X)
		// m = perpendicular to the slope of the side opposite to XY
		
		// y - tip_y = PBA * (x - tip_x)
		// y = PBA * (x - tip_x) + tip_y
		
		// y - 0 = PAC * (x - 1)
		// y = PAC * (x - 1)
	
		//note: i have no idea how this works, this is too hard to comperhand to me
		// x = (x1 * tan(α) + x2 * tan(β) + x3 * tan(γ)) / (tan(α) + tan(β) + tan(γ))
		// y = (y1 * tan(α) + y2 * tan(β) + y3 * tan(γ)) / (tan(α) + tan(β) + tan(γ))

		ortho_x = (0 + 1 * std::tan(b) + tip_x * std::tan(c)) / (std::tan(a) + std::tan(b) + std::tan(c));
		ortho_y = (0 +  				 tip_y * std::tan(c)) / (std::tan(a) + std::tan(b) + std::tan(c));
	}
};

int main() {

	std::vector<std::vector<Triangle>> trigList;

	//using the AAS rule
	float mid = 1; // sides (rhs is drawn from lhs' tip to mid's tip
	for (float a = 90; a <= 178; a++) {
		std::vector<Triangle> oneAng;
		for (float b = 1; b < 180 - a - 1; b++) {
			// a + b + c = 180
			float c = 180 - a - b;
			
			oneAng.push_back(Triangle(a / 180 * PI, b / 180 * PI, c / 180 * PI));

		}
	}

	return 0;
}
