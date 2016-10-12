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
//Geometry Processing.
Mat Translate(Mat, int, int, int&, int&);
Mat Rotate(Mat, int, int, int, int);
Mat Scale(Mat, int, int);

//Gray-Scale Processing.
Mat ImageNegative(Mat, int, int);
Mat LogTransformation(Mat, int, int);
Mat LogInverse(Mat, int, int);
Mat PowerLaw(Mat, int, int);

//Histogram Processing.
Mat HEqualization(Mat, int, int);

////////////////////////////////////////////////////////////////
//  Input Sample: /Users/shehabmohamed/Desktop/sample.jpg
//  Input Sample2: /Users/shehabmohamed/Desktop/Test.jpg
////////////////////////////////////////////////////////////////

//Original Values of Translation X-axis, Y-axis.
//If User decides to translate then rotate. Then we shift angle of rotation Angle + X, Angle + Y
int X, Y = 0;

int main()
{
	cout<<"CS463: Fund. Of Computer Vision A#1.\n";
	cout<<"By Shehab Mohamed\n\n";
	string path;
	Mat SRC, DST;
	string FileName="";
	int d1, d2, d3;
	bool transformed = false;
	string Names[3] = {"Translated", "Rotated", "Scaled"};
	
	//Prompting User to Enter Image Path.
	cout<<"Path of the Image: ";
	getline(cin, path);
	stringstream s(path);
	SRC = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	
	//Validating till correct image path is inserted.
	while(!SRC.data)
	{
		cout<<"Could not find or open the image! \n";
		cout<<"Try to enter the path again: \n";
		cin>>path;
		SRC = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
	}

	imshow("Original Image", SRC);
	//Taking Inputs from User.
	int input;
	unsigned int imageH = SRC.rows, imageW = SRC.cols;
	int px = imageH/2;
	int py = imageW/2;
	cout<<"Choose Type of Processing.\n";
	cout<<"1- Geometry\n2- Gray-Scale \n3- Histogram Equalization\n";
	cin>>input;
	printf("\n");
	
	if(input == 1)
	{
		cout<<"Which Transformations you want to apply to loaded image?\n";
		cout<<"1- Translate \n2- Rotate \n3- Scale\n";
		cout<<"Input Example: 1 0 1 (Translate & Scale Only)\n";
		scanf("%d %d %d", &d1, &d2, &d3);
		printf("\n");
		if(d1)
		{
			DST = Translate(SRC, imageH, imageW, X, Y);
			transformed = true;
			FileName += Names[0];
		}
		if(d2 && transformed)
		{
			imageH = DST.rows;
			imageW = DST.cols;
			px = px + X;
			py = py + Y;
			DST = Rotate(DST, imageH, imageW, px, py);
			FileName += '-' + Names[1];
		}
		else if(d2 && !transformed)
		{
			DST = Rotate(SRC, imageH, imageW, px, py);
			transformed = true;
			FileName += Names[1];
		}
		if(d3 && transformed)
		{
			imageH = DST.rows;
			imageW = DST.cols;
			DST = Scale(DST, imageH, imageW);
			FileName += '-' + Names[2];
		}
		else if (d3 && !transformed)
		{
			DST = Scale(SRC, imageH, imageW);
			FileName += Names[2];
		}

	}
	else if(input == 2)		//Grey-Level Transformation
	{
		int d;
		cout<<"Which Gray Level Transformation you want to apply to loaded image?\n";
		cout<<"1- Power Law \n2- Image Negative \n3- Log Transformation\n4- Inverse Log Transformation\n";
		cin>>d;
		switch(d)
		{
			case 1:
				DST = PowerLaw(SRC, imageH, imageW);
				FileName = "PowerLaw Transformation";
				break;
			case 2:
				DST = ImageNegative(SRC, imageH, imageW);
				FileName = "Image-Negative Transformation";
				break;
			case 3:
				DST = LogTransformation(SRC, imageH, imageW);
				FileName = "Log Transformation";
				break;
			case 4:
				DST = LogInverse(SRC, imageH, imageW);
				FileName = "Inverse Log Transformation";
				break;
			default:
				cout<<"Invalid.\n";
				break;
		}
		
	}
	else if(input == 3)		//Histogram Equalization
	{
		DST = HEqualization(SRC, imageH, imageW);
		FileName = "Histogram Equalized";
	}
	
	// Uncomment the following lines if images generated should be saved.
	//		char save;
	//		cout<<"\nSave Modified Image? (Y/N)\n";
	//		cin>>save;
	//		if(save == 'y' || 'Y')
	//			imwrite(SavedFileName, DST);
	//		waitKey(0);
	
	imshow(FileName, DST);
	waitKey(0);
	
    return 0;
}
/************************** Geometry Processing **************************/
//Translation function
Mat Translate(Mat img, int r, int c, int &X, int &Y)
{
	cout<<"Translating...\n";
	int x, y;
	Mat final = Mat::zeros(img.size(), img.type());
	cout<<"Enter translation in x-axis: \n";
	cin>>x;
	cout<<"Enter translation in y-axis: \n";
	cin>>y;
	X = x;
	Y = y;
	for(int i=0; i<r-y; i++)
	{
		for(int j=0; j<c-x; j++)
		{
			final.at<uchar>(i+y, j+x) = img.at<uchar>(i, j);
		}
	}
	return final;
}
//Rotation function
Mat Rotate(Mat img, int r, int c, int px, int py)
{
	cout<<"Rotating...\n";
    int angle, x, y;
	int PX, PY;
	cout<<"Enter the arbitrary point of rotation for the image. \n";
	cout<<"If Image should rotate on the center. Enter -1 in Px & Py \n";
	cout<<"Px: ";
	cin>>PX;
	cout<<"Py: ";
	cin>>PY;
	if(PX != -1 || PY != -1)
	{
		px = PX;
		py = PY;
	}
    Mat final = Mat::zeros(img.size(), img.type());
    cout<<"Enter angle of rotation in range of -90 to 90: \n";
    cin>>angle;
    double angle_radiant = -1*angle*PI/180;
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            x = int((i-px)*cos(angle_radiant))-int((j-py)*sin(angle_radiant)) + px;
            y = int((j-py)*cos(angle_radiant))+int((i-px)*sin(angle_radiant)) + py;
            //Checking values computed.
            //cout<<"x, y: "<<x<<", "<<y<<endl;
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
/************************** Grey-Level Processing **************************/
//Negative Image
Mat ImageNegative(Mat img, int r, int c)
{
	// S = L - 1 - r
	//L = 256
	Mat final = img;
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			int s = 255 - final.at<uchar>(i,j);
			final.at<uchar>(i,j) = s;
		}
	}
	return final;
}
//Log Transformation. (Brighter image)
Mat LogTransformation(Mat img, int r, int c)
{
	double coefficient;
	
	// S = c*log(1+r)
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			if(img.at<uchar>(i, j) == 0)
				//To avoid throwing exception when dividing by zero.
				img.at<uchar>(i, j) = 1;
			coefficient = 255/log(256.0);	// c = S/(log(1+L)
			double s = coefficient*log(1 + img.at<uchar>(i, j));
			cout<<s<<endl;
			img.at<uchar>(i, j) = s;
		}
	}
	return img;
}
//Inverse Log Transformation. (Darker image)
Mat LogInverse(Mat img, int r, int c)
{
	float coefficient;

	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			if(img.at<uchar>(i, j) == 0)
				//To avoid throwing exception when dividing by zero.
				img.at<uchar>(i, j) = 1;
			
			coefficient = 255.0/log(255.0 + 1.0);
			int s = (pow(exp(img.at<uchar>(i, j)), 1/coefficient)) - 1;
			cout<<s<<endl;
			img.at<uchar>(i, j) = s;
		}
	}
	
	return img;
}
//Power-Law Transformation.
Mat PowerLaw(Mat img, int r, int c)
{
	double GAMMA;
	cout<<"S = c.r^Gamma\n";
	cout<<"0.05 <= Gamma <= 5.0\n";
	cout<<"Gamma: ";
	cin>>GAMMA;
	double coefficient = pow(255.0, 1.0 - GAMMA);
	// S = c.r^GAMMA
	
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			double s = coefficient*(pow(double(img.at<uchar>(i, j)), GAMMA));
			cout<<s<<endl;
			img.at<uchar>(i, j) = s;
		}
	}
	
	return img;
}
/************************** Histogram Processing **************************/
//Histogram Equalization.
Mat HEqualization(Mat img, int r, int c)
{
	int Old_Intensity[256] = {0};
	int New_Intensity[256] = {0};

	double N = r*c;
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			int p = img.at<uchar>(i, j);
			Old_Intensity[p]++;
		}
	}
	double New_p = 0.0;
	for(int k=0; k<256; k++)
	{
		//printf("Pixel Intensity Value (%d) : NoOfPixels (%d)\n", k, Old_Intensity[k]);
		New_p += Old_Intensity[k];
		New_Intensity[k] = (New_p/N)*255.0;
		//printf("New_Intensity[%d] = %d\n",k, New_Intensity[k]);
	}
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			int p = img.at<uchar>(i, j);
			img.at<uchar>(i, j) = New_Intensity[p];
		}
	}
	return img;
}