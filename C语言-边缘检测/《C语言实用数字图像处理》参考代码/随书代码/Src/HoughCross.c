#include <math.h>

#define PI 3.141592

/*--- hough_cross --- ‚Q’¼ü‚ÌŒğ“_‚ğ‹‚ß‚é ------------------------------------
	rho1, theta1:	’¼ü‚P
	rho2, theta2:	’¼ü‚Q
	x, y:			Œğ“_
---------------------------------- ------------------------------------------*/
void hough_cross(double theta1, double rho1, double theta2, double rho2,
	double *x, double *y)
{
	double d, t1, t2;
	
	t1 = theta1 * PI / 180.0;
	t2 = theta2 * PI / 180.0;
	d = sin(t1-t2);
	if (d == 0) return;
	*x = (rho2 * sin(t1) - rho1 * sin(t2)) / d;
	*y = (rho1 * cos(t2) - rho2 * cos(t1)) / d;
}
