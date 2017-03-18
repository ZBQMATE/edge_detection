#include "cvs.h"
#include <math.h>

using namespace std;

double* cvs::plot(unsigned char ima[], msk ft) {
	
	double output[(height - ft.size + 1) * (width - ft.size + 1)];
	
	double filter_sum = 0;
	for (int c = 0; c < height - ft.size + 1; c++) {
		for (int r = 0; r < width - ft.size + 1; r++) {
			
			for (int mc = 0; mc < ft.size; mc++) {
				for (int mr = 0; mr < ft.size; mr++) {
					
					filter_sum = filter_sum + ft.map[mc*ft.size+mr] * ima[(c+mc)*width+r+mr];
					
				}
			}
			
			output[c*(width-ft.size+1) + r] = filter_sum;
			filter_sum = 0;
			
		}
	}
	
	double *opt;
	opt = output;
	
	return opt;
}