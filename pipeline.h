#ifndef PIPELINE_H
#define PIPELINE_H


#include "car.h"
#include "collison.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class pipeline{
public:
	pipeline(){
		swapFlag = false;
		carList1 = new vector<car*>();
		carList2 = new vector<car*>();
		isCollision = false;
	}
	
	void addCar(car *c){
		(*getCurrentList()).push_back(c);
		//carList1.push_back(c);
		//cout << carList1->size() << "," << carList2->size() << endl;
	}

	void setIsCollision(bool b){
		isCollision = b;
	}

	bool getIsCollision(){
		return isCollision;
	}

	void calculate(){
		/*cout << "size:" << (*getOtherList()).size() << " " << (*getCurrentList()).size() << endl;
		if ((*getOtherList()).size() && (*getCurrentList()).size()){
			cout << "current" << endl;
			for (int i = 0; i<(*getCurrentList()).size(); ++i){
				(*getCurrentList())[i]->print();
			}
			cout << "other" << endl;
			for (int i = 0; i<(*getOtherList()).size(); ++i){
				(*getOtherList())[i]->print();
			}

		}*/
		if ((*getOtherList()).size() && (*getCurrentList()).size())
		for (int i = 0; i <(*getCurrentList()).size(); ++i){
			car *c = (*getCurrentList())[i];
			int index = -1;
			float distance = 10;
			for (int i = 0; i < (*getOtherList()).size(); ++i){
				if ((*getOtherList())[i]){
					float f = c->Distance(*(*getOtherList())[i]);
					if (f < distance){
						index = i;
						distance = f;
					}
				}
			}

			float time = 1.0f;// (float)1 / (float)8;
			//otherList里有car
			if (index != -1){
				if ((*getOtherList())[index]){
					car *before = (*getOtherList())[index];
					//cout << "speed_x" << c.loc_x << " " << before.loc_x << endl;
					double x = c->loc_x - before->loc_x;
					c->speed_x = x / time;
					//cout << "speed_x" << c->loc_x << " " << before->loc_x << " " << x << " " << time << " " << c->speed_x << endl;
					double y = c->loc_y - before->loc_y;
					c->speed_y = y / time;

					c->a_x = (c->speed_x - before->speed_x) / time;
					c->a_y = (c->speed_y - before->speed_y) / time;

				/*	if (fabs(x) > 10 || fabs(y) > 10){
						(*getCurrentList())[i] = NULL;
						delete c;
					}*/
				}

			}
			else{
				/*(*getCurrentList())[i] = NULL;
				delete c;*/
			}

		}
	}


	void detectCollision_1sec(){
		for (int i = 0; i < getCurrentList()->size(); ++i){
			//car *car_a = ((*getCurrentList())[i]);
			if ((*getCurrentList())[i]){
				car *temp_a = new car((*getCurrentList())[i]);
				//temp_a->loc_x += temp_a->speed_x;
				//temp_a->loc_y += temp_a->speed_y;
				//temp_a->speed_x += temp_a->a_x;
				//temp_a->speed_y += temp_a->a_y;
				int x1 = temp_a->loc_x;
				int y1 = temp_a->loc_y;

				for (int j = i + 1; j < getCurrentList()->size(); ++j){
					if ((*getCurrentList())[j]){
						car* temp_b = new car((*getCurrentList())[j]);

						double k1 = (double)temp_a->speed_y / (double)temp_a->speed_x;
						double k2 = (double)temp_b->speed_y / (double)temp_b->speed_x;
						int x2 = temp_b->loc_x;
						int y2 = temp_b->loc_y;
						int x = (k1*x1 - k2*x2 + y2 - y1) / (k1 - k2);
						int y = (k1*k2*(x1 - x2) + k1*y2 - k2*y1) / (k1 - k2);
						double t1 = fabs((double)((x - x1) / (temp_a->speed_x*0.8)));
						double t2 = fabs((double)((x - x2) / (temp_b->speed_x*0.8)));
						if (t1<10&&t2<10&&fabs(t1 - t2) <= 2.0){
							cout << "generate a collision:("<<i<<","<<j <<"),碰撞在("<<x<<","<<y<<"),("<<t1<<","<<t2<<")"<< endl;
							++collisionCount;
							isCollision = true;
							collision cl(i, j, x, y, t1, t2);
							lastCollision = collision(i, j, x, y, t1, t2);
							collisions.push_back(cl);
						}

						//temp_b->loc_x += temp_b->speed_x;
						//temp_b->loc_y += temp_b->speed_y;
						//temp_b->speed_x += temp_b->a_x;
						//temp_b->speed_y += temp_b->a_y;

						//cout << "first:";
						//judgeDistance(temp_a, temp_b);
						//detectCollison_2sec(temp_a, temp_b);
						delete temp_b;
					}
				}

				delete temp_a;
			}
		}
	}

	void detectCollison_2sec(car*car1,car*car2){
		car1->loc_x += car1->speed_x;
		car2->loc_y += car2->speed_y;
		//cout << "second:";
		judgeDistance(car1, car2);
	}

	void judgeDistance(car*car1,car*car2){
		double _x = fabs((double)car1->loc_x - (double)car2->loc_x);
		double _y = fabs((double)car1->loc_y - (double)car2->loc_y);
		//cout <<"in judge:"<< _x << " " << _y << " " << (car1->width + car2->width) / 2 << " " << (car1->height + car2->height) / 2 << endl;
		if ((_x <= (car1->width + car2->width) / 2) &&
			(_y <= (car1->height + car2->height) / 2)){
			++collisionCount;
			isCollision = true;
			cout << "a collision" << endl;

		}
			
	}

	void print(int i){
		cout << "----------------第" << i << "帧----------------"<<endl;
		for (int i = 0; i<(*getCurrentList()).size(); ++i){
			if ((*getCurrentList())[i]){
				cout << i << ":";
				(*getCurrentList())[i]->print();
			}
				
		}
		cout << "there are " << (*getCurrentList()).size() << " cars" << endl;
	}

	string toString(int id){
		string s = "";
		char a[256];
		sprintf_s(a, "----------------第%d帧----------------\n",id);
		s += string(a);
		for (int i = 0; i<(*getCurrentList()).size(); ++i){
			if ((*getCurrentList())[i]){
				char b[8];
				sprintf_s(b, "%d: ", i);
				//cout << i << ":";
				s += string(b);
				s+=(*getCurrentList())[i]->toString();
			}

		}
		return s;
		//cout << "there are " << (*getCurrentList()).size() << " cars" << endl;
	}

	void swap(){
		//把other里的销毁
		while ((*getOtherList()).size() > 0){
			car *temp = ((*getOtherList())[(*getOtherList()).size() - 1]);
			(*getOtherList()).pop_back();
			if (temp)
				delete temp;
		}
		
		swapFlag = (!swapFlag);
	}

	void recordInFile(){
		//char filename[] = ""; // 此处写入文件名 
		//fstream fout;// ("record.txt");
		//fout.open("record.txt", iostream::out);
		////vector<collision>::iterator it = collisions.begin();
		//for (vector<collision>::iterator it = collisions.begin(); it != collisions.end(); it++)
		//	fout << "generate a collision:(" << (*it).car1_index << "," << (*it).car2_index << "),碰撞在(" 
		//		<< (*it).loc_x << "," << (*it).loc_y << "),(" << (*it).time1 << "," << (*it).time2 << ")" << endl;
		//fout.close();
	}

	vector<car*>* getCurrentList(){
		if (!swapFlag)
			return carList1;
		else
			return carList2;
	}

	vector<car*>* getOtherList(){
		if (swapFlag)
			return (carList1);
		else
			return (carList2);
	}

	~pipeline(){
		if (carList1->size()>0){
			while (carList1->size() > 0){
				car *temp = ((*carList1)[carList1->size() - 1]);
				carList1->pop_back();
				delete temp;
			}
		}
		if (carList2->size() > 0){
			while (carList2->size() > 0){
				car *temp = ((*carList1)[carList2->size() - 1]);
				carList2->pop_back();
				delete temp;
			}
		}

		if (carList1)
			delete carList1;
		if (carList2)
			delete carList2;
	}
	
	static int collisionCount;
	vector<collision> collisions;
	collision lastCollision;
private:
	vector<car*> *carList1;
	vector<car*> *carList2;
	
	bool swapFlag;
	bool isCollision;
};

int pipeline::collisionCount = 0;


#endif