//
//  main.cpp
//  Computer Vision A#1
//
//  Created by Shehab Mohamed on 9/28/16.
//  Copyright © 2016 Shehab Mohamed. All rights reserved.
//
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define PI 3.14159265

using namespace std;
using namespace cv;
//Geometry Option
void Geometry(Mat);
//Gray-Scale Option
void GrayScale(Mat);
//Histogram Option
//void Histogramer(Mat);

//Geometry Processing.
Mat Translate(Mat, int, int);
Mat Rotate(Mat, int, int);
Mat Scale(Mat, int, int);

//Gray-Scale Processing.
Mat ImageNegative(Mat, int, int);
Mat LogTransformation(Mat, int, int);
Mat PowerLaw(Mat, int, int);

//Histogram Processing.
//NOT DONE.
Mat HEqualization(Mat, int, int);

//User Interaction.
void SaveImage(Mat);

//Input Sample: sample.jpg
//  /Users/shehabmohamed/Desktop/sample.jpg
int main()
{
	cout<<"CS463: Computer Vision A#1.\n";
	cout<<"By Shehab Mohamed\n\n";
	string path;
	Mat SRC;
	
	//Prompting User to Enter Image Path.
	cout<<"Path of the Image: ";
	getline(cin, path);
	stringstream s(path);
	//cin>>path;
	SRC = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	
	//Validating till correct image path is inserted.
	while(!SRC.data)
	{
		cout<<"Could not find or open the image! \n";
		cout<<"Try to enter the path again: \n";
		cin>>path;
		SRC = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	}

	//Taking Inputs from User.
	int input;
	cout<<"Choose Type of Processing.\n";
	cout<<"1- Geometry\n2- Gray-Scale \n3- Histogram\n";
	cin>>input;
	switch(input)
	{
		case 1:
			Geometry(SRC);
			break;
		case 2:
			GrayScale(SRC);
			break;
		case 3:
			//Histogramer(SRC);
			break;
		default:
			cout<<"Invalid.\n";
			break;
	}
	
	
    return 0;
}

//Translation function
Mat Translate(Mat img, int r, int c)
{
	cout<<"Translating...\n";
    int x, y;
    Mat final = Mat::zeros(img.size(), img.type());
    cout<<"Enter translation in x-axis: \n";
    cin>>x;
    cout<<"Enter translation in y-axis: \n";
    cin>>y;
    for(int i=y; i<r; i++)
    {
        for(int j=x; j<c; j++)
        {
            final.at<uchar>(i,j) = img.at<uchar>(i,j);
        }
    }
    return final;
}

//Rotation function
Mat Rotate(Mat img, int r, int c)
{
	cout<<"Rotating.../n";
    int angle, px, py, x, y;
    Mat final = Mat::zeros(img.size(), img.type());
    cout<<"Enter angle of rotation in range of -90 to 90: \n";
    cin>>angle;
    double angle_radiant = -1*angle*PI/180;
    px = r/2;
    py = c/2;
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            x = int((i-px)*cos(angle_radiant))-int((j-py)*sin(angle_radiant)) + px;
            y = int((j-py)*cos(angle_radiant))+int((i-px)*sin(angle_radiant)) + py;
            //Checking values computed.
            cout<<"x, y: "<<x<<", "<<y<<endl;
            if(x < 0 || y < 0 || x > r || y > c)
                //If Pixel of the Image went out of boundries, give it a black color.
                final.at<uchar>(i,j) = 0;
            else
                final.at<uchar>(i,j) = img.at<uchar>(x,y);
        }
    }
    return final;
}

//Scaling function
Mat Scale(Mat img, int r, int c)
{
	cout<<"Scaling...\n";
    int r_factor, c_factor;
    Mat final;
    double f;
    cout<<"Note: To avoid any Memory-Exceptions and get proper scaling, 0.3 <= f <= 4.0\n";
    cout<<"Scale factor: ";
    cin>>f;
    r_factor = int(f*r);
    c_factor = int(f*c);
    if(f>1.0)
        //If factor > 1.0, the image is enlarged.
    {
        cout<<"Old Size: "<<r<<"x"<<c<<endl;
        cout<<"New Size: "<<r_factor<<"x"<<c_factor<<endl;
        final = Mat::zeros(r_factor, c_factor, img.type());
        //DST[r*(W*factor) + c] = SRC[(r/factor)*W + c/factor]
        
        for(int i=0; i<r_factor; i++)
        {
            for(int j=0; j<c_factor; j++)
            {
                final.at<uchar>(i, j) = img.at<uchar>(int(i/f), int(j/f));
            }
        }
        return final;
    }
    else
        if(f<1.0)
            //If factor < 1.0, the image is shrinked.
        {
            cout<<"Old Size: "<<r<<"x"<<c<<endl;
            cout<<"New Size: "<<r_factor<<"x"<<c_factor<<endl;
            final = Mat::zeros(r_factor, c_factor, img.type());
            //DST[(r/factor)*W + c/factor] = SRC[r*W + c]
            
            for(int i=0; i<r; i++)
            {
                for(int j=0; j<c; j++)
                {
                    final.at<uchar>((int)i*f, (int)j*f) = img.at<uchar>(i, j);
                }
            }
            return final;
        }
    //Else, there isn't any Scaling. Return Regular image.
    return img;
}

//Negative Image
Mat ImageNegative(Mat img, int r, int c)
{
	// S = L - 1 - r
	//L = 256
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			int s = 255 - img.at<uchar>(i,j);
			img.at<uchar>(i,j) = s;
		}
	}
	return img;
}
//Log Transformation. (Brighter image)
Mat LogTransformation(Mat img, int r, int c)
{
	unsigned int coefficient;
	
	// S = c*log(1+r)
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			if(img.at<uchar>(i, j) == 0)
				//To avoid throwing exception when dividing by zero.
				img.at<uchar>(i, j) = 1;
			coefficient = 255/log(1 + img.at<uchar>(i, j));
			int s = (int)coefficient*(int)log(1 + img.at<uchar>(i, j));
			img.at<uchar>(i, j) = s;
		}
	}
	
	return img;
}
//Power-Law Transformation.
Mat PowerLaw(Mat img, int r, int c)
{
	unsigned long long int coefficient;
	unsigned long long int L = 255.0;
	double GAMMA;
	cout<<"S = c.r^Gamma\n";
	cout<<"0.5 <= Gamma <= 25.0\n";
	cout<<"Gamma: ";
	cin>>GAMMA;
	// S = c.r^GAMMA
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			if(img.at<uchar>(i, j) == 0)
				//To avoid throwing exception when dividing by zero.
				img.at<uchar>(i, j) = 1;
			coefficient = L/pow((double)img.at<uchar>(i, j), GAMMA);
			//cout<<pow((double)img.at<uchar>(i, j), GAMMA)<<endl;
			cout<<coefficient<<endl;
			int s = (int)coefficient*(int)pow((double)img.at<uchar>(i, j), GAMMA);
			//cout<<s<<endl;
			img.at<uchar>(i, j) = s;
		}
	}
	
	return img;
}

//Histogram Equalization.
Mat HEqualization(Mat img, int r, int c)
{
//	struct Pixel;
//	{
//		int index;
//		int H;
//	}
	
	int SIZE = r*c;
	int *HE_Arr = new int[SIZE];
	
	delete [] HE_Arr;
	return img;
}

void Geometry(Mat SRC)
{
	Mat DST;
	cout<<"Which Transformations you want to apply to loaded image?\n";
	cout<<"1- Translate \n2- Rotate \n3- Scale\n";
	cout<<"Input Example: 1 0 1 (Translate & Scale Only)\n";
	int d1, d2, d3;
	bool transformed = false;
	string Names[3] = {"Translated", "Rotated", "Scaled"};
	string SavedFileName = "";
	scanf("%d %d %d", &d1, &d2, &d3);
	unsigned int imageH = SRC.rows;
	unsigned int imageW = SRC.cols;
	//system("clear");
	cout<<"Transforming Image...\n";
	if(d1)
	{
		DST = Translate(SRC, imageH, imageW);
		transformed = true;
		SavedFileName += Names[0];
	}
	if(d2 && transformed)
	{
		DST = Rotate(DST, imageH, imageW);
		SavedFileName += '-' + Names[1];
	}
	else if(d2 && !transformed)
	{
		DST = Rotate(SRC, imageH, imageW);
		transformed = true;
		SavedFileName += Names[1];
	}
	if(d3 && transformed)
	{
		DST = Scale(DST, imageH, imageW);
		SavedFileName += '-' + Names[2];
	}
	else if (d3 && !transformed)
	{
		DST = Scale(SRC, imageH, imageW);
		SavedFileName += Names[3];
	}
	
	cout<<"Image Transformation Done.\n";
	imshow("Original Image", SRC);
	imshow(SavedFileName, DST);
	waitKey(0);
	char save;
	cout<<"\nSave Modified Image? (Y/N)\n";
	cin>>save;
	if(save == 'y' || 'Y')
		imwrite(SavedFileName, DST);
	waitKey(0);
}

void GrayScale(Mat SRC)
{
	int d;
	int R = SRC.rows, C = SRC.cols;
	cout<<"Which Gray Level Transformation you want to apply to loaded image?\n";
	cout<<"1- PowerLaw \n2- ImageNegative \n3- LogTransformation\n";
	cin>>d;
	switch(d)
	{
		case 1:
			//NOT WORKING.
			PowerLaw(SRC, R, C);
			break;
		case 2:
			ImageNegative(SRC, R, C);
			break;
		case 3:
			LogTransformation(SRC, R, C);
			break;
		default:
			cout<<"Invalid.\n";
			break;
	}
	
}