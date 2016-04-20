#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

    //Change this values for threshold image
#define threshold_value 50
#define threshold_type CV_THRESH_BINARY
#define max_value 255

int main(int argc, char* argv[])
{
    Mat src = cv::imread(argv[1]);
    if(!src.data) cerr << "Problem loading image!!!" << endl;

    Mat gray;
    if (src.channels() == 3) cvtColor(src, gray, CV_BGR2GRAY);
    else gray = src;

    Mat threshold_img;
    cv::blur(gray, gray, cv::Size(5, 5));
    threshold( ~gray, threshold_img, threshold_value, max_value, threshold_type );
    imwrite("tables/binary.png", threshold_img);

    vector<Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(threshold_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Mat> rois;
    vector<vector<Point> > contours_poly_select;

    for (size_t i = 0; i < contours.size(); i++)
    {
        // find the area of each contour
        double area = contourArea(contours[i]);

        if(area < 10000) // value is randomly chosen, you will need to find that by yourself with trial and error procedure
	    continue;

        approxPolyDP( Mat(contours[i]), contours_poly[i], 100, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
	if(contours_poly[i].size() != 4)
	    continue;
        rois.push_back(src(boundRect[i]).clone());
	contours_poly_select.push_back(contours_poly[i]);
    }

    std::cout<<rois.size() <<", " <<contours_poly.size() <<std::endl;

    for(size_t i = 0; i < rois.size(); ++i)
    {
	//Each roi is a note_table
        imwrite("tables/note.png", rois[i]);
    }

    std::cout<<"So tu giac: " <<contours_poly_select.size() <<"\n";
    for(size_t i = 0; i < contours_poly_select.size(); ++i)
    {
	std::cout<<"So dinh: " <<contours_poly_select[i].size() <<"\n";
	if(contours_poly_select[i].size() == 4)
	{
	    std::cout<<"\n\nRectangle " <<i <<": \n";
	    size_t N = contours_poly_select[i].size();
	    for(size_t j = 0; j < N; ++j)
	    {
		line(src, contours_poly_select[i][j], contours_poly_select[i][(j+1)%4], Scalar(0, 0, 0), 5);
		std::cout<<"corner " <<j <<" x, y: " <<contours_poly_select[i][j].x <<", " <<contours_poly_select[i][j].y <<"\n";
	    }
	}
    }

    imwrite("tables/contours.png", src);
    imshow("contours", src);

    waitKey();
    return 0;
}
