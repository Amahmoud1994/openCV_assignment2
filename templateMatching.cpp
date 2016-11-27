#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;

int match_method = 5;
Mat src; Mat src_gray;
int max_thresh = 255;
RNG rng(12345);

Mat image_array[38];

/// Function Headers
Mat thresh_callback(int thresh,Mat image);
void MatchingMethod(int, void*,int index);
void readData()

{

  Mat src = imread("alphabet.jpg",1);
  Mat image(src);
  image_array[0] =Mat(image, Rect(1, 11, 8, 60));
  image_array[1] =Mat(image, Rect(10, 22, 45, 37));
  image_array[2] =Mat(image, Rect(56, 23, 45, 35));
  image_array[3] =Mat(image, Rect(102, 20, 45, 42));
  image_array[4] =Mat(image, Rect(148, 16, 40, 50));
  image_array[5] =Mat(image, Rect(189, 16, 40, 50));
  image_array[6] =Mat(image, Rect(230, 9, 40, 64));
  image_array[7] =Mat(image, Rect(271, 25, 24, 31));
  image_array[8] =Mat(image, Rect(296, 17, 24, 47));
  image_array[9] =Mat(image, Rect(321, 24, 31, 34));
  image_array[10] =Mat(image, Rect(353, 14, 31, 53));
  image_array[11] =Mat(image, Rect(385, 24, 64, 34));
  image_array[12] =Mat(image, Rect(450, 12, 64, 57));
  image_array[13] =Mat(image, Rect(515, 23, 78, 36));
  image_array[14] =Mat(image, Rect(594, 14, 78, 54));
  image_array[15] =Mat(image, Rect(673, 17, 43, 48));
  image_array[16] =Mat(image, Rect(717, 17, 43, 48));
  image_array[17] =Mat(image, Rect(761, 12, 38, 57));
  image_array[18] =Mat(image, Rect(800, 7, 38, 67));
  image_array[19] =Mat(image, Rect(839, 18, 55, 46));
  image_array[20] =Mat(image, Rect(895, 14, 39, 54));
  image_array[21] =Mat(image, Rect(935, 15, 40, 51));
  image_array[22] =Mat(image, Rect(976, 11, 36, 60));
  image_array[23] =Mat(image, Rect(1013, 16, 29, 49));
  image_array[24] =Mat(image, Rect(1043, 16, 39, 50));
  image_array[25] =Mat(image, Rect(1083, 27, 22, 28));
  image_array[26] =Mat(image, Rect(1106, 22, 33, 37));
  image_array[27] =Mat(image, Rect(1140, 21, 45, 40));
  image_array[28] =Mat(image, Rect(1186, 26, 21, 30));
  image_array[29] =Mat(image, Rect(1208, 6, 24, 70));
  image_array[30] =Mat(image, Rect(1233, 5, 36, 71));
  image_array[31] =Mat(image, Rect(1270, 6, 49, 70));
  image_array[32] =Mat(image, Rect(1320, 6, 31, 69));
  image_array[33] =Mat(image, Rect(1352, 16, 41, 49));
  image_array[34] =Mat(image, Rect(1394, 6, 40, 70));
  image_array[35] =Mat(image, Rect(1435, 6, 47, 70));
  image_array[36] =Mat(image, Rect(1483, 5, 47, 71));
  image_array[37] =Mat(image, Rect(1531, 2, 46, 78));
  image_array[29] =imread("1.jpg");
  image_array[30] =imread("2.jpg");
  image_array[31] =imread("3.jpg");
  image_array[32] =imread("4.jpg");
  image_array[33] =imread("55.jpg");
  image_array[34] =imread("6.jpg");
  image_array[35] =imread("7.jpg");
  image_array[36] =imread("8.jpg");
  image_array[37] =imread("9.jpg");
}
Mat q1Answer(Mat image)
{
  // Load the image in GRAY SCALE to simplify the conversion
      Mat img1 = image;
  // Create the result image with the same size as the Original image
      Mat dst (img1.size(), img1.type());

  // Accessing every pixel in the image and apply the threshold condition on its value
      for(int i=0;i<img1.rows; i++){
        for(int j=0;j<img1.cols*3;j++){
          // Setting the threshold to == 100 Because it is a convention when working with grey scaled images
          if(img1.at<uchar>(i,j)<100)
            dst.at<uchar>(i,j)= 0;
          else
            dst.at<uchar>(i,j)= 255;
        }
      }

      return dst;
}

/** @function main */
int main( int argc, char** argv )
{

  readData();
  // int minVal = 10;
  /// Load image and template
  img = imread("L1.jpg");

  Mat src = imread("alphabet.jpg");
  // img = q1Answer(img);
  medianBlur ( img, img, 11 );

  //  threshold( img, img, 100, 255,0 );
  // img = thresh_callback( 255,img );


  // cout<< templ.size()<<endl;
  // cout<< img.size()<<endl;

    // templ = image_array[0];
    Size sizeChar(90,140);//the dst image size,e.g.100x100
  for (int i = 0; i < 28; i++) {
    // medianBlur ( image_array[i], image_array[i], 1 );
    resize(image_array[i],image_array[i],sizeChar);//resize image
    // image_array[i] = thresh_callback(50,image_array[i] );

  }
  Size sizeNumbers(70,140);//the dst image size,e.g.100x100

    //  imshow("Sample", q1Answer(image_array[4]));
    for (int i = 28; i < 38; i++) {
      // medianBlur ( image_array[i], image_array[i], 1 );
      resize(image_array[i],image_array[i],sizeNumbers);//resize image
    // image_array[i] = thresh_callback(15,image_array[i] );
}

  for (int i = 0; i < 38; i++) {
    templ = image_array[i];
    medianBlur ( templ, templ, 7 );
    MatchingMethod(0,0,i);
  }
  /// Create windows
  namedWindow( "Source Image", CV_WINDOW_AUTOSIZE );
  //imshow("Source Image", image_array[19]);
  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int, void*,int index)
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  // cout<< result.size()<<endl;

  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, match_method );
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;
  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

    int matchingValue = (int)result.at<uchar>(Point(matchLoc.x , matchLoc.y));
    // std::cout << matchLoc <<  matchLoc.x << matchLoc.y << std::endl;

    if(matchingValue < 200)
        return;

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );

  img_display.copyTo( img );
  string s = "Char" + index;
  imshow(s,image_array[index]);
  imshow( "Source Image", img_display );
  std::cout << "Index of Character "<< index << std::endl;
  std::cout <<  matchingValue << std::endl;
  return ;
}
Mat thresh_callback(int thresh , Mat image)
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( image, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  // namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  // imshow( "Contours", drawing );
  return drawing;
}
