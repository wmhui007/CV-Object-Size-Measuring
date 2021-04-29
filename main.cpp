#include "preProccesing.hpp"
#include "postProcessing.hpp"
#include "display.hpp"
#include <iostream>

#define BLUR_COUNTER 3
#define REFERENCE_COIN_DIAMETER 25.75f
#define COUNTER_AREA_TRESHOLD 700

float pixelsPerMetric = -1.0f; // unit: [pixels/mm]

int main()
{
	try
	{
		std::string imagePath = "test_image2.jpg";
		cv::Mat inputGrayImg = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
		cv::Mat labeledImg = cv::Mat::zeros(inputGrayImg.size(), CV_8UC3);

		// 1.Preprocessing:
		
		// Remove Noise
		for (int i{ 0 }; i < BLUR_COUNTER; i++)
			gaussianBlur(inputGrayImg); // cv::GaussianBlur(grayImg, grayImg, cv::Size(3, 3), 0, 0); // 
			
		// Edge Detection
		cv::Canny(inputGrayImg,inputGrayImg, 50 , 100 ,3);

		// Apply Dilation and Erossion to patch(fill in) any gaps in detected edges
		dilation(inputGrayImg, 3);
		erossion(inputGrayImg, 3);


		// 2.Postprocessing:
		
		// Find Contours:
		std::vector<std::vector<cv::Point>>contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(inputGrayImg, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		sortContours_from_Left_to_Right(contours);

		// Compute and Display Dimensions 
		int index = 0;
		for (auto contour : contours)
		{
			if (cv::contourArea(contour) > COUNTER_AREA_TRESHOLD)
			{
				// Compute Smallest Rotated Bounding Box around Object
				cv::RotatedRect rect = cv::minAreaRect(contour);
				std::vector<cv::Point2f> verticies = orderVerticies(rect);

				// If reference not defined yet, define it, using leftmost object (Reference Coin)
				if (pixelsPerMetric == -1.0f)
					pixelsPerMetric = computeReference(verticies, REFERENCE_COIN_DIAMETER);
					
				// Compute width and height of current object
				std::vector<float> dims = computeWidthAndHeight(verticies, pixelsPerMetric);

				// Draw 
				drawRectangle(labeledImg, verticies);
				drawData(labeledImg, verticies, dims[0], dims[1]);
				cv::drawContours(labeledImg, contours, index, contourColor, 2, cv::LINE_8, hierarchy, 0);
			}
			index++;
		}
		
		// 3.Show Results:
		std::cout << "Displaying Results\n";
		cv::imshow("Results", labeledImg);
		int k = cv::waitKey(0);
	}
	catch (...)
	{
		std::cerr << "Exception in main\n";
	}
	return 0;
}

