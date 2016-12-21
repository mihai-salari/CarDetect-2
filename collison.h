#ifndef COLLISION_H
#define COLLISION_H

class collision{
public :
	collision(){}
	collision(int i1, int i2, int x, int y, float t1, float t2){
		car1_index = i1;
		car2_index = i2;
		loc_x = x;
		loc_y = y;
		time1 = t1;
		time2 = t2;
	}

//private:
	int car1_index;
	int car2_index;
	int loc_x;
	int loc_y;
	float time1;
	float time2;
};

#endif