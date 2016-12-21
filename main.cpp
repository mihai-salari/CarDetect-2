#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <list>
#include <iostream>
#include <string>
#include <direct.h>
#include <iomanip>  
#include <fstream>  
//#include <iomanip>
//#include <sstream>
//#include <stdlib.h>
#include "pipeline.h"



using namespace cv;
using namespace std;

int car_height = 30;
int car_width = 60;
pipeline m_pipeline;


int main(int argc, char* argv[])
{
	CvScalar color[5] = { (128, 0, 0, 0), (255, 0, 0, 0), (251, 113, 67, 0), (246, 187, 109, 0), (248, 244, 178, 0) };
	CvScalar color1(128, 0, 0, 0);
	//CvScalar color2(255, 0, 0, 0);
	//CvScalar color3(251, 113, 67, 0);
	//CvScalar color4(246, 187, 109, 0);
	//CvScalar color5(248, 244, 178, 0);



	int FRAME = 2;
	int WIDTH = 10;
	int HEIGHT = 8;



	for (int i = 0; i < argc; ++i){
		cout << i << " " << argv[i] << endl;
	}

	if (argc > 2){
		FRAME = atoi(argv[1]);
		WIDTH = atoi(argv[2]);
		HEIGHT = atoi(argv[3]);
	}

	IplImage* pFrame = NULL;
	IplImage* pFrImg = NULL;
	IplImage* pBkImg = NULL;
	CvMat* pFrameMat = NULL;
	CvMat* pFrMat = NULL;
	CvMat* pBkMat = NULL;
	CvCapture* pCapture = NULL;

	CvPoint pt1, pt2, pt3, pt4, pt5;
	pt1.x = 310;//(视频中左下点)
	pt1.y = 100;
	pt2.x = 550;//(视频中右上点)
	pt2.y = 400;


	//IplImage *dst = NULL;
	int nFrmNum = 0;

	char filename[64];

	int latestTime = 0;


	cvNamedWindow("背景图", 1);
	cvNamedWindow("前景图", 1);
	cvNamedWindow("原视频", 1);

	cvMoveWindow("原视频", 10, 100);
	cvMoveWindow("背景图", 600, 100);
	cvMoveWindow("前景图", 600, 100);

	char*videoname2 = "E:/video/video1.avi";
	if(argc>2)
		videoname2 = argv[4];// // 14 00014 screen3 screen3.avi

	string s(videoname2);
	int sign = 0;
	for (int i = s.size() - 1; i >= 0; --i)
		if (s[i] == '.'){
			sign = i;
			break;
		}

	char *cc = new char[sign + 1];
	for (int i = 0; i < sign; ++i)
		cc[i] = s[i];
	cc[sign] = '\0';
	string fname=string(cc)+string("/");
	cout << fname << endl;

	;

	ofstream outfile(fname + string("data.txt"));
	//int tttt;
	if (!((_mkdir(cc) == 0)))
	{
		//cout << tttt << endl;
		cout << "mkdir fail" << endl;
		system("pause");
		return 0;
	}

	if (!(pCapture = cvCaptureFromFile(videoname2)))
	{
		fprintf(stderr, "Can not open video file %s\n", argv[1]);
		system("pause");
		return -2;
	}

	IplConvKernel * kenel1 = cvCreateStructuringElementEx(3, 1, 1, 0, CV_SHAPE_RECT, NULL);
	IplConvKernel * kenel2 = cvCreateStructuringElementEx(1, 3, 0, 1, CV_SHAPE_RECT, NULL);

	bool isStart = false;

	CvFont font1;
	cvInitFont(&font1, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0, 0, 2, 8);

	int numFrames = (int)cvGetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_COUNT);
	cout << "all:" << numFrames << endl;
	CvRect rect1(pt1.x,pt1.y,pt2.x,pt2.y);

	cout << "before" << endl;

	while (pFrame = cvQueryFrame(pCapture))
	{
		nFrmNum++;
		//cout << nFrmNum << endl;

		if (numFrames%FRAME == 0){
			
			if (nFrmNum == numFrames -FRAME){
				cout << "in create 1:"<<m_pipeline.collisions.size() << endl;
				for (int i = 0; i < m_pipeline.collisions.size(); ++i){
					int x = m_pipeline.collisions[i].loc_x;
					int y = m_pipeline.collisions[i].loc_y;
					double tts = fabs(m_pipeline.collisions[i].time1 - m_pipeline.collisions[i]
						.time2);
					if (tts<0.4)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(128, 0, 0), -1);
					else if (tts >= 0.4&&tts<0.8)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(255, 0, 0), -1);
					else if (tts >= 0.8&&tts<1.2)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(251, 113, 67), -1);
					else if (tts >= 1.2&&tts<1.6)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(246, 187, 109), -1);
					else if (tts >= 1.6)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(248, 244, 178), -1);
					//cvCircle(pFrame, CvPoint(x,y), 2, CV_RGB(255, 0, 0), 3);
					cout << "xxxx" << i << endl;
				}
				outfile.close();
				cvSaveImage((fname+string("冲突分布图.jpg")).c_str(), pFrame);
				cout << "end" << endl;
			}
		}
		else{
			
			if (nFrmNum == numFrames-FRAME-1){
				cout << "in create 2:" << m_pipeline.collisions.size() << endl;
				for (int i = 0; i < m_pipeline.collisions.size(); ++i){
					int x = m_pipeline.collisions[i].loc_x;
					int y = m_pipeline.collisions[i].loc_y;
					double tts = fabs(m_pipeline.collisions[i].time1 - m_pipeline.collisions[i].time2);
					if (tts<0.4)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(128, 0, 0), -1);
					else if (tts >= 0.4&&tts<0.8)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(255, 0, 0), -1);
					else if (tts >= 0.8&&tts<1.2)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(251, 113, 67), -1);
					else if (tts >= 1.2&&tts<1.6)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(246, 187, 109), -1);
					else if (tts >= 1.6)
						cvCircle(pFrame, CvPoint(x, y), 9, CV_RGB(248, 244, 178), -1);
					//cvCircle(pFrame, CvPoint(x,y), 2, CV_RGB(255, 0, 0), 3);
					cout << "yyyy" << i << endl;
				}
				outfile.close();
				cvSaveImage((fname+string("冲突分布图.jpg")).c_str(), pFrame);
				cout << "end" << endl;
			}
		}

		if (nFrmNum==1)
		{
			//cvSetImageROI(pFrame, rect1);
			pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
			pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 1);
			pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
			pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
			pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

			cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
			cvConvert(pFrImg, pFrameMat);
			cvConvert(pFrImg, pFrMat);
			cvConvert(pFrImg, pBkMat);

			//cvSaveImage("coll125.jpg", pFrame);
		}
		else if (nFrmNum % FRAME == 0)
		{
		/*	if (nFrmNum > 1000)
				break;*/

			//bool f = false;
			if (nFrmNum > 300)
				isStart = true;

			//if (nFrmNum > 300){
			//	char c;
			//	cin >> c;
			//	if (c == 's')
			//		f = true;
			//}
			//cvCircle(pFrame, Point(100, 100), 3, CV_RGB(255, 0, 0), 3);


			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
			cvConvert(pFrImg, pFrameMat);

			cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);


			cvRectangle(pFrame, pt1, pt2, CV_RGB(255, 0, 0), 2, 8, 0);


			cvAbsDiff(pFrameMat, pBkMat, pFrMat);

			cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);


			cvRunningAvg(pFrameMat, pBkMat, 0.02, 0);

			cvErode(pFrImg, pFrImg, kenel2, 2);
			cvErode(pFrImg, pFrImg, kenel1, 2);
			cvDilate(pFrImg, pFrImg, kenel1,14);
			cvErode(pFrImg, pFrImg, kenel1, 6);
			cvDilate(pFrImg, pFrImg, kenel2, 6);
			cvErode(pFrImg, pFrImg, kenel2, 3);


			CvMemStorage* storage = cvCreateMemStorage();  
			CvSeq* contours;     
			std::list<CvRect> rects;  

			cvFindContours(pFrImg, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); 
			int count = 0;     


			int cnt = 1;

			cvRectangle(pFrImg, cvPoint(0, 0),
				cvPoint(15, 15),
				CV_RGB(255, 255, 255),
				1,
				8,
				0);

			cvRectangle(pFrImg, pt1,pt2,CV_RGB(255, 255, 255),2,8,0);
			/*if (isStart){
				for (int i = 3; i < 7;++i){
					Point p1(i * 100, 0);
					Point p2(i * 100, 400);
					cvLine(pFrImg, p1, p2, CV_RGB(255, 255, 255));
				}
				for (int i = 1; i < 5; ++i){
					Point p1(0,i * 100);
					Point p2(600,i * 100);
					cvLine(pFrImg, p1, p2, CV_RGB(255, 255, 255));
				}
			}*/
			int cnt1 = 0;
			while (contours != NULL)
			{
				CvRect contourRect = cvBoundingRect(contours, 0);

				//cvZero(dst);
		
				if ((contourRect.width > WIDTH) && (contourRect.height >HEIGHT)){  //6
					if ((contourRect.x + contourRect.width / 2)> pt1.x && (contourRect.x + contourRect.width / 2) < pt2.x &&
						(contourRect.y + contourRect.height / 2 > pt1.y) && (contourRect.y + contourRect.height / 2) < pt2.y){
						//cvDrawContours(pFrImg, contours, cv::Scalar(255, 255, 255), cv::Scalar(255, 255, 255), 3/* -1, CV_FILLED, 8*/);
						char a[2];
						_itoa(cnt1, a, 10);
						++cnt1;
						cvRectangle(pFrImg, cvPoint(contourRect.x, contourRect.y),cvPoint(contourRect.x + contourRect.width, contourRect.y + contourRect.height),
							CV_RGB(255, 255, 255),1,8,0);
						cvPutText(pFrImg, a, cvPoint(contourRect.x + contourRect.width / 2, contourRect.y + contourRect.height/2+8), &font1, CV_RGB(255, 255, 255));

						if (isStart){
							car *c = new car(contourRect);
							m_pipeline.addCar(c);
						}

					}
				}
				contours = contours->h_next;   
			}

			if (isStart){
				m_pipeline.calculate();
				outfile<< m_pipeline.toString(nFrmNum);
				m_pipeline.print(nFrmNum);
				m_pipeline.detectCollision_1sec();

				m_pipeline.swap();

				if (m_pipeline.getIsCollision()){
					double sec = nFrmNum / 24;
					if (sec - latestTime < 1.2){
						cout << m_pipeline.collisionCount << " " << m_pipeline.collisions.size() << endl;
						--m_pipeline.collisionCount;
						m_pipeline.collisions.pop_back();
					}
					else{
						latestTime = sec;
						int s = (int)sec;
						double tts = fabs(m_pipeline.lastCollision.time1 - m_pipeline.lastCollision.time2);
						if (sec <= 60){
							sprintf(filename, "collison_%d_%d秒tts_%.2f.jpg", m_pipeline.collisionCount,s,tts);
						}
						else{
							sprintf(filename, "collison_%d_%d分%d秒tts_%.2f.jpg", m_pipeline.collisionCount,(int)s/60,s%60,tts);
						}
						cout << filename << endl;
						if (tts<0.4)
							cvCircle(pFrame, CvPoint(m_pipeline.lastCollision.loc_x, m_pipeline.lastCollision.loc_y), 9, CV_RGB(128, 0, 0), -1);
						else if (tts >= 0.4&&tts<0.8)
							cvCircle(pFrame, CvPoint(m_pipeline.lastCollision.loc_x, m_pipeline.lastCollision.loc_y), 9, CV_RGB(255, 0, 0), -1);
						else if (tts >= 0.8&&tts<1.2)
							cvCircle(pFrame, CvPoint(m_pipeline.lastCollision.loc_x, m_pipeline.lastCollision.loc_y), 9, CV_RGB(251, 113, 67), -1);
						else if (tts >= 1.2&&tts<1.6)
							cvCircle(pFrame, CvPoint(m_pipeline.lastCollision.loc_x, m_pipeline.lastCollision.loc_y), 9, CV_RGB(246, 187, 109), -1);
						else if (tts >= 1.6)
							cvCircle(pFrame, CvPoint(m_pipeline.lastCollision.loc_x, m_pipeline.lastCollision.loc_y), 9, CV_RGB(248, 244, 178), -1);
						cvSaveImage((fname+string(filename)).c_str(), pFrame);
						
					}
					m_pipeline.setIsCollision(false);
					
				}
			}


			cvConvert(pBkMat, pBkImg);

			cvShowImage("背景图", pBkImg);
			cvShowImage("前景图", pFrImg);
			cvShowImage("原视频", pFrame);

			if (cvWaitKey(100) >= 0)
				break;
		}

	}



	//m_pipeline.recordInFile();

	cvDestroyWindow("原视频");
	cvDestroyWindow("背景图");
	cvDestroyWindow("前景图");

	cvReleaseImage(&pFrImg);
	cvReleaseImage(&pBkImg);
	cvReleaseMat(&pFrameMat);
	cvReleaseMat(&pFrMat);
	cvReleaseMat(&pBkMat);
	cvReleaseCapture(&pCapture);
	//m_pipeline.~pipeline();
	delete[]cc;

	cout << endl << "共发生" << pipeline::collisionCount << "次碰撞" << endl;

	//system("pause");

	return 0;
}

