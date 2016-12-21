#ifndef CAR_H
#define CAR_H

#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace std;


class car{
public:
	car(){
		loc_x = loc_y = height = width = area = speed = acceleration = 0;
	}

	car(CvRect rec){
		loc_x = rec.x + rec.width / 2;
		loc_y = rec.y + rec.height / 2;
		height = rec.height;
		width = rec.width;
		area = width*height;
		speed = speed_x=speed_y=a_x=a_y=acceleration = 0;
	}

	car(car*c){
		loc_x = c->loc_x;
		loc_y = c->loc_y;
		height = c->height;
		width = c->width;
		speed_x = c->speed_x;
		speed_y = c->speed_y;
		a_x = c->a_x;
		a_y = c->a_y;
		acceleration = c->acceleration;
		area = c->area;
		speed = c->speed;
	}

	float Distance(car c){
		int s = (loc_x - c.loc_x)*(loc_x - c.loc_x) + (loc_y - c.loc_y)*(loc_y - c.loc_y);
		return sqrt(s); 
	}

	void print(){
		cout << "(" << loc_x << "," << loc_y << ")--[" << height << "," << width << "]   ";
		cout << "v_x: " << speed_x << ", " << "v_y: " << speed_y << "   ";
		cout << "a_x: " << a_x << ", a_y: " << a_y << endl;
			//cout<< "v:" << speed << "," << speed_x << "," << speed_y << ",a:" << acceleration <<","<<a_x<<","<<a_y<< endl;
	}

	string toString(){
		char a[512];
		sprintf(a, "(%d,%d)--[%d,%d]   v_x: %.2f, v_y: %.2f   a_x: %.2f, a_y: %.2f\n",
			loc_x, loc_y, height, width, speed_x, speed_y, a_x, a_y);
		return string(a);
	}

	~car(){

	}

	int loc_x;
	int loc_y;
	int height;
	int width;
	int area;
	float speed;
	float speed_x;
	float speed_y;
	float a_x;
	float a_y;
	float acceleration;
};


#endif