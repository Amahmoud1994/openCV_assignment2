/**
 * @function moments_demo.cpp
 * @brief Demo code to calculate moments
 * @author OpenCV team
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 200;
RNG rng(3);

/// Function header
void thresh_callback(int, void* );

Matx33f getAffineMatrix( vector<Point2f> &src, vector<Point2f> &dst )
{
  Matx33f M(src[0].x,src[0].y,1,src[1].x,src[1].y,1,src[2].x,src[2].y,1);
  Matx33f mInv(M.inv().val);
  Matx31f a1(mInv * Matx31f(dst[0].x,dst[1].x,dst[2].x));
  Matx31f a2(mInv* Matx31f(dst[0].y,dst[1].y,dst[2].y));
  Matx31f a3(mInv* Matx31f(1,1,1));
  return Matx33f(a1.val[0],a1.val[1],a1.val[2],a2.val[0],a2.val[1],a2.val[2],a3.val[0],a3.val[1],a3.val[2]);
}

float clamp(float val, float lowerB, float higherB) {
	return val < lowerB ? lowerB : (val > higherB ? higherB : val);
}

void affineMatrixWrapper(Mat &src, Mat &dst, Matx33f &affineMatrix)
{

  for (int i = 0; i < dst.rows; i++) {
    for (int j = 0; j < dst.cols; j++) {
      Matx31f matrix(j,i,1);
      Matx31f points(affineMatrix * matrix);
      int x = (int)clamp(points.val[0],0,dst.cols);
      int y = (int)clamp(points.val[1],0,dst.rows);
      dst.at<Vec3b>(i,j) = src.at<Vec3b>(y, x);
    }

  }
}
Mat q4Answer()
{
  vector<Point2f> srcTri;
  vector<Point2f> dstTri;

  Matx33f affineMatrix;
  Mat src, warp_dst;

  /// Load the image
  src = imread( "L3.jpg" );

  /// Set the dst image the same type and size as src
  warp_dst = Mat( src.size(), src.type() );

  /// Set your 3 points to calculate the  Affine Transform

  dstTri.push_back(Point2f( 0, 0 ));
  dstTri.push_back(Point2f(931, 144));
  dstTri.push_back(Point2f(910, 671));

  srcTri.push_back(Point2f( 0, 0     ));
  srcTri.push_back(Point2f( src.cols - 1, 0 ));
  srcTri.push_back(Point2f( src.cols - 1, src.rows - 1));

  /// Get the Affine Transform
  affineMatrix = getAffineMatrix( srcTri, dstTri );

  affineMatrixWrapper(src,warp_dst,affineMatrix);

   return warp_dst;

}

/**
 * @function main
 */
int main( int, char** argv )
{
  /// Load source image and convert it to gray
  // Loading the L1 image which is the first part of Question 1
  // src = imread("L1.jpg");
  // Loading Frontal View of L3 image and the function used was from Assignment 1
  src =  q4Answer();

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  const char* source_window = "Source";
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );

  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/**
 * @function thresh_callback
 */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Get the moments
  vector<Moments> mu(contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }

  ///  Get the mass centers:
  vector<Point2f> mc( contours.size() );
  for( size_t i = 0; i < contours.size(); i++ )
     { mc[i] = Point2f( static_cast<float>(mu[i].m10/mu[i].m00) , static_cast<float>(mu[i].m01/mu[i].m00) ); }

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }

  /// Show in a window
  namedWindow( "Contours", WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );

  // Print the minimum number of Classes used to display the image
  printf("Number of classes is %d \n",(int) contours.size());

  /// Calculate the area with the moments 00 and compare with the result of the OpenCV function
  for( size_t i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point() );
       circle( drawing, mc[i], 4, color, -1, 8, 0 );
     }


}
