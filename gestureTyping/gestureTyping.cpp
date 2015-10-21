#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include<imgproc\imgproc.hpp>
#include<cv.h>
#include "opencv2/ml/ml.hpp"
#include<iostream>

using namespace std;
using namespace cv;

Mat_<Vec3b> output(400,400, Vec3b(255,255,255));
	
	Point P;
	Point oldP;
int main()

{  VideoCapture cap(0);
    if(!cap.isOpened()) 
	return -1;

	Mat_<Vec3b> img1(400,400, Vec3b(0,180,160));
	Mat_<Vec3b> img2(400,400, Vec3b(100,255,255));
	imshow("LB",img1);
	imshow("UB",img2);
	while(1)
{
    Mat image,img;
    cap >> img;
	resize(img,image,Size(400,400));
	Mat dst;

	inRange(image,img1,img2,dst);

	

	Mat dil,can;
//    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
	 Mat element = getStructuringElement( 1,Size( 31, 31 ), Point( 10, 10) );
	//inRange(image,hsv1,hsv2,dst);
    dilate( dst,dil, element);
   // dilate( dil,dil, element);

	//Canny(dil,can,0,100,3);
//	cvtColor(can, can, CV_BGR2GRAY);    
	//findContours( can, contours, hierarchy,2,CV_CHAIN_APPROX_NONE, Point(0, 0) );
//	findContours( can, contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	
    //  Mat drawing = Mat::zeros( can.size(), CV_8UC3 );

 



	static const int thickness = 1;//CV_FILLED - filled contour
static const int lineType = 8;//8:8-connected,  4:4-connected line, CV_AA: anti-aliased line.
Scalar           color = CV_RGB(255, 20, 20); // line color - light red

//Segmented image
Mat Segmented = dil > 128;

////////////////////////////////////////////////////////////////////
/// Find contours - use old style C since C++ version has bugs. 

//Target image for sketching contours of segmentation
Mat             drawing;
cvtColor(dil ,   drawing, CV_GRAY2RGB);
IplImage        drawingIpl = drawing; //just clone header with no copying of data

//Data containers for FindContour
IplImage        SegmentedIpl = Segmented;//just clone header with no copying of data
CvMemStorage*   storage = cvCreateMemStorage(0);
CvSeq*          contours = 0;
int             numCont = 0;
int             contAthresh = 45;

numCont = cvFindContours(&SegmentedIpl, storage, &contours, sizeof(CvContour),
    CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
if(numCont==2)
{
for(int y=0;y<output.rows;y++)
{
    for(int x=0;x<output.cols;x++)
    {
        // get pixel
     
		output.at<Vec3b>(Point(x,y))[0] = 255;
		output.at<Vec3b>(Point(x,y))[1] = 255;
		output.at<Vec3b>(Point(x,y))[2] = 255;

  
    }
}
}
for (; contours != 0; contours = contours->h_next)
{
     CvRect rect = cvBoundingRect(contours, 0); //extract bounding box for current contour

                     //drawing rectangle
                     cvRectangle(&drawingIpl,                  
                                  cvPoint(rect.x, rect.y),    
                                  cvPoint(rect.x+rect.width, rect.y+rect.height),
                                  cvScalar(0, 0, 255, 0),
                                  2, 8, 0); 
					 P.x =  cvPoint(rect.x+rect.width/2, rect.y+rect.height/2).x;
					 P.y =  cvPoint(rect.x+rect.width/2, rect.y+rect.height/2).y;

					
					 output.at<Vec3b>(P.y,400-P.x) = 0;

					 output.at<Vec3b>(P.y+1,400-P.x) = 0;
					 output.at<Vec3b>(P.y-1,400-P.x) = 0;
					 output.at<Vec3b>(P.y,400-P.x+1) = 0;
					 output.at<Vec3b>(P.y,400-P.x-1) = 0;
					 output.at<Vec3b>(P.y+1,400-P.x+1) = 0;
					 output.at<Vec3b>(P.y-1,400-P.x-1) = 0;
					 output.at<Vec3b>(P.y+1,400-P.x-1) = 0;
					 output.at<Vec3b>(P.y-1,400-P.x+1) = 0;
					 P.x = 400-P.x;
					
					  line(output, P, oldP, 'r', 2, 8,0);
					 oldP.x = P.x;
					 oldP.y = P.y;
					 
	
	//cvDrawContours(&drawingIpl, contours, color, color, -1, thickness, lineType, cvPoint(0, 0));
}

/// Show in a window
string win_name = "Contour";
//Mat drawing = cvarrToMat(&drawingIpl); //Mat(&IplImage) is soon to be deprecated OpenCV 3.X.X
namedWindow(win_name, CV_WINDOW_NORMAL);
imshow(win_name, drawing);
	imshow("x   ",image);
	
	
	imshow("g",drawing);
	imshow("output",output);

	cout<<numCont<<endl;
waitKey(10);
	
   if(waitKey(27) >= 0) break;
}
	return 0;

}
