#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"


#define END 30


using namespace std;
using namespace cv;


void thinningGuoHallIteration(cv::Mat& im, int iter)
{
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1); 

    for (int i = 1; i < im.rows; i++)
    {
        for (int j = 1; j < im.cols; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1); 
            uchar p9 = im.at<uchar>(i-1, j-1);

            int C  = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
                     (!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
            int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
            int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
            int N  = N1 < N2 ? N1 : N2;
            int m  = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);

            if (C == 1 && (N >= 2 && N <= 3) & m == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void thinningGuoHall(cv::Mat& im)
{
    im /= 255;

    cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningGuoHallIteration(im, 0);
        thinningGuoHallIteration(im, 1);
        cv::absdiff(im, prev, diff);
        im.copyTo(prev);
    } 
    while (cv::countNonZero(diff) > 0);

    im *= 255;
}



int absolute_val(int a)
{
	if(a>0)
	{
		return a;
	}
	else 
	{
		return -1*a;
	}
}


int main(int argc,char* argv[])
{
	 VideoCapture cap("vd22.mp4"); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

   double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;

    //namedWindow("MyVideo",CV_WINDOW_NORMAL); //create a window called "MyVideo"
    //namedWindow("MyVideo2",CV_WINDOW_NORMAL);
    //namedWindow("Original",CV_WINDOW_NORMAL);
    Mat image,image1;
    
    cap.read(image);
    resize(image,image,Size(image.cols/3,image.rows/3));
    cvtColor(image,image,CV_BGR2GRAY);
    //for(int i=0;i<image.rows;i++)
    //{
		//for(int j=0;j<image.cols;j++)
		//{
			//image.at<uchar>(i,j)-=30;
			//if(image.at<uchar>(i,j)<0)//checking if pixel value is negative
			//{
				//image.at<uchar>(i,j)=0;
			//}
		//}
	//}
				
    //equalizeHist(image,image1);
    cout<<image.channels()<<endl;
    //cout<<image<<endl;
    //imshow("image",image);
    /*if (image.channels() == 1) { // gray-level image
		image.at<uchar>(j,i)= 255;
	} else if (image.channels() == 3) { // color image
	image.at<cv::Vec3b>(j,i)[0]= 255;
	image.at<cv::Vec3b>(j,i)[1]= 255;
	image.at<cv::Vec3b>(j,i)[2]= 255;
	}*/
	//cout<<image.at<cv::Vec3b>(0,0)[0]<<endl;
	
    

    while (1)
    {
		int K=0;
		//scanf("%d",&K);
        Mat frame,diff,frame2;
        int di1,di2,di3;


        bool bSuccess = cap.read(frame); // read a new frame from video
        
        
         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        resize(frame,frame,Size(frame.cols/3,frame.rows/3));
        cvtColor(frame,frame,CV_BGR2GRAY);
		equalizeHist(frame,frame2);
        imshow("Original",frame);
      for(int i=0;i<frame.rows;i++)
      {
		  for(int j=0;j<frame.cols;j++)
		  {  
       /* if (frame.channels() == 3) { // color image
			 
			 di1=image.at<cv::Vec3b>(i,j)[0]-frame.at<cv::Vec3b>(i,j)[0];
			 di2=image.at<cv::Vec3b>(i,j)[1]-frame.at<cv::Vec3b>(i,j)[1];
			 di3=image.at<cv::Vec3b>(i,j)[2]-frame.at<cv::Vec3b>(i,j)[2];
			 di1=absolute_val(di1);
			 di2=absolute_val(di2);
			 di3=absolute_val(di3);
			 
			 if(di1>END || di2>END || di3>END)
			{
				
	frame.at<cv::Vec3b>(i,j)[0]= 0;
	frame.at<cv::Vec3b>(i,j)[1]= 0;
	frame.at<cv::Vec3b>(i,j)[2]= 0;
	}
	else
	{
		frame.at<cv::Vec3b>(i,j)[0]= 255;
	frame.at<cv::Vec3b>(i,j)[1]= 255;
	frame.at<cv::Vec3b>(i,j)[2]= 255;
}
}*/
		
			//frame.at<uchar>(i,j)-=10;
			//if(frame.at<uchar>(i,j)<0)//checking if pixel value is negative
			//{
				//frame.at<uchar>(i,j)=0;
			//}

		di1=image.at<uchar>(i,j)-frame.at<uchar>(i,j);
		di1=absolute_val(di1);
		if(di1>END)
		{
			frame.at<uchar>(i,j)=255;
		}
		else 
		{
			frame.at<uchar>(i,j)=0;
		}
		/*di2=image1.at<uchar>(i,j)-frame2.at<uchar>(i,j);
		di2=absolute_val(di2);
		if(di2>END)
		{
			frame2.at<uchar>(i,j)=255;
		}
		else 
		{
			frame2.at<uchar>(i,j)=0;
		}*/
}}
	//cout<<image-frame<<endl;	
		
	/*else if (image.channels() == 3) { // color image
	image.at<cv::Vec3b>(j,i)[0]= 255;
	image.at<cv::Vec3b>(j,i)[1]= 255;
	image.at<cv::Vec3b>(j,i)[2]= 255;
	}*/
	
/*	Mat skel(frame.size(), CV_8UC1, cv::Scalar(0));
    Mat temp;
    Mat eroded;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
 
bool done;		
do
{
  cv::erode(frame, eroded, element);
  cv::dilate(eroded, temp, element); // temp = open(img)
  cv::subtract(frame, temp, temp);
  cv::bitwise_or(skel, temp, skel);
  eroded.copyTo(frame);
 
  done = (cv::countNonZero(frame) == 0);
} while (!done);
*/
	Mat dis;
		 //morphological opening (removes small objects from the foreground)
	erode(frame, dis, getStructuringElement(MORPH_ELLIPSE, Size(4,4)) );	

  dilate( frame, dis, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) ); 
        imshow("MyVideo2", frame);
		thinningGuoHall(frame);
   //morphological closing (removes small holes from the foreground)
  //dilate( frame, dis, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
 // erode(frame, dis, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        imshow("MyVideo", frame); //show the frame in "MyVideo" window
		//imshow("MyVideo2",dis);
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }
	
	return 0;
	}


