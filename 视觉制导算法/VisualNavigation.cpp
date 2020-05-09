#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>
#include <fstream>
using namespace cv;
using namespace std;
int main(int argc, const char* argv[]) {


	Mat process_img = imread("C:/Users/admin/Desktop/Test2.jpg", IMREAD_GRAYSCALE);
	double sumGrayScale = 0;
	for (int k = 0; k < process_img.rows* process_img.cols; k++) {
		sumGrayScale += process_img.data[k];
	}
	int allThePixels = process_img.rows * process_img.cols;
	double averageGrayScale = sumGrayScale / allThePixels;
	for (int j = 0; j < process_img.rows * process_img.cols; j++) {
		if (process_img.data[j] <= averageGrayScale) {
			process_img.data[j] = 0;
		}
		else {
			process_img.data[j] = 255;
		}
	}

	int leftMargin[1000] = {0};
	int rightMargin[1000] = {0};
	int midPoints[1000] = {0};
	for (int i = 0; i < process_img.rows; i++) {
		//find bilateral margins
		//left margin
		for(int p1 = process_img.cols / 2; p1 > 0; p1--) {
			if ((process_img.data[process_img.cols * i + p1] - process_img.data[process_img.cols * i + p1 - 1]) != 0 && leftMargin[i] == 0) {
				leftMargin[i] = p1;
			}
		}
		//right margin
		for (int p2 = process_img.cols / 2; p2 < process_img.cols; p2++) {
			if ((process_img.data[process_img.cols * i + p2] - process_img.data[process_img.cols * i + p2 - 1]) != 0 && rightMargin[i] == 0) {
				rightMargin[i] = p2;
			}
			if (p2 == process_img.cols - 2 && rightMargin[i] == 0) {
				rightMargin[i] = process_img.cols-1;
			}
		}
		midPoints[i] = (leftMargin[i] + rightMargin[i]) / 2;
	}
	ofstream outfile;
	outfile.open("marginData.dat", ios::out);
	outfile << "leftMargin  midPoints  rightMargin" << endl;
	for (int i = 0; i < process_img.rows; i++) {
			outfile << leftMargin[i] << "      " << midPoints[i] << "      " << rightMargin[i] << endl;
	}
	outfile.close();

	

	
	for (int l = 0; l < process_img.rows; l++) {
		if (midPoints[l] != 0) {
			if (process_img.data[l * process_img.cols + midPoints[l] - 1] == 0 && process_img.data[l * process_img.cols + midPoints[l] + 1] == 0) {
				process_img.data[l * process_img.cols + midPoints[l]] = 255;
			}
			else if (process_img.data[l * process_img.cols + midPoints[l] - 1] == 255 && process_img.data[l * process_img.cols + midPoints[l] + 1] == 255) {
				process_img.data[l * process_img.cols + midPoints[l]] = 0;
			}
		}
	}
	
	//图片的中心作为车当前的位置
	int central = process_img.cols / 2;
	//道路中心线作为车当前的位置
	int central1 = midPoints[process_img.rows - 1];
	int centralPix2 = 1;
	double centralPix = 1;
	while (central1 == 0) {
		centralPix++;
		centralPix2++;
		central1 = midPoints[process_img.rows-centralPix2];
	}

	int calPixNumber = 50;
	while (midPoints[process_img.rows - calPixNumber] == 0) {
		calPixNumber++;
	}
	int deviPixel1 = midPoints[process_img.rows - calPixNumber] - central1;
	

	int deviPixel = midPoints[process_img.rows-calPixNumber]-central;
	double calPixNum = (double)calPixNumber;
	double deviAngel = atan((deviPixel / calPixNum));

	double deviAngel1 = atan(deviPixel1 / (calPixNum - centralPix));
	if (deviPixel > 0) {
		cout << "若以图片中央作为小车行驶的中心，则：" << endl;
		cout << "在从下往上数" << calPixNumber << "个像素处，" << endl;
		cout << "偏离了" << deviPixel << "个像素" << endl;
		cout << "车应当往右"<<endl;
		cout << "偏离角度为(弧度)：" << deviAngel << endl;
	}else {
		cout << "若以图片中央作为小车行驶的中心，则：" << endl;
		cout << "在从下往上数" << calPixNumber << "个像素处，" << endl;
		cout << "偏离了" << -deviPixel << "个像素" << endl;
		cout << "车应当往左" << endl;
		cout << "偏离角度为(弧度)：" << -deviAngel << endl;

	}
	if (deviPixel1 > 0) {
		cout << "若以道路中心线作为小车行驶的中心，则：" << endl;
		cout << "在从下往上数" << calPixNumber << "个像素处，" << endl;
		cout << "偏离了" << deviPixel1 << "个像素" << endl;
		cout << "车应当往右" << endl;
		cout << "偏离角度为(弧度)：" << deviAngel1 << endl;
	
	
	}
	else {
		cout << "若以道路中心线作为小车行驶的中心，则：" << endl;
		cout << "在从下往上数" << calPixNumber << "个像素处，" << endl;
		cout << "偏离了" << -deviPixel1 << "个像素" << endl;
		cout << "车应当往左" << endl;
		cout << "偏离角度为(弧度)：" << -deviAngel1 << endl;
	}

	//
	double Angle[1000] = {0};
	int devi = 0;
	ofstream outfile1;
	outfile1.open("Angle.dat", ios::out);
	for (int i = 0; i < process_img.rows; i++) {
		devi = midPoints[i] - central;
		double height= process_img.rows - i;
		Angle[i] = atan(devi / height);
		outfile1 <<Angle[i]<< endl;
	}
	outfile1.close();

	//cvNamedWindow("process_img");
	imshow("process_img", process_img);
	waitKey();
	return 0;

}