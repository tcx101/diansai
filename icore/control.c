#include  "headfile.h"
uint16_t X_target,Y_target,speed=1;
_Bool AB_finish_flag=0;
_Bool AB_flag;
void control_AB_X()
{
	X_target+=speed;
	
	servo1.target=X_target;
	
	if(X_target>=cal_mid(point_data.B1[0],point_data.B2[0]))
	{
    X_target=cal_mid(point_data.B1[0],point_data.B2[0]);
		AB_finish_flag=1;
		AB_flag=0;
	}
	
}

void control_AB_Y()
{
	
	Y_target=AB_line[X_target];
	servo2.target=Y_target;
	
}



void control_mid()
{
	X_target=point_data.mid[0];
	Y_target=point_data.mid[1];
	

}

void control_A()
{
	X_target=cal_mid(point_data.A1[0],point_data.A2[0]);
	Y_target=AB_line[X_target];
}

void control_A_B()
{
	control_AB_X();
	control_AB_Y();
	
}
