#include <opencv2/opencv.hpp>

// Constants 

cv::RNG rng(12345);
cv::Scalar boxColor = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
cv::Scalar pointColor = cv::Scalar(rng.uniform(256, 0), rng.uniform(0, 256), rng.uniform(0, 256));
cv::Scalar contourColor = cv::Scalar(rng.uniform(256, 0), rng.uniform(256, 0), rng.uniform(256,0));

void drawRectangle(const cv::Mat& image, const std::vector<cv::Point2f>& verticies)
{
	for (int j{ 0 }; j < sizeof(verticies) / sizeof(verticies[0]); j++)
	{
		// Draw Sides and Verticies of rectange
		cv::line(image, verticies[j], verticies[(j + 1) % 4], boxColor, 2);
		cv::circle(image, verticies[j], 3, pointColor, 2);
	}
}

std::string formatData(const float data)
{
	// Convert to string and append units
	std::string dataString = std::to_string(roundf(data * 100) / 100);
	dataString.erase(dataString.find_last_not_of('0') + 1, std::string::npos);
	dataString += "mm";

	return dataString;
}

void drawData(const cv::Mat& image, const std::vector<cv::Point2f>& verticies, float objectWidth, float objectHeight)
{
	// Grab verticies
	cv::Point2f topLeft = verticies[0];
	cv::Point2f topRight = verticies[1];
	cv::Point2f bottomRight = verticies[2];

	// Convert and format data
	std::string widthString = formatData(objectWidth);
	std::string heightString = formatData(objectHeight);

	// Compute position
	cv::Point2f widthTextPosition = topLeft + (topRight - topLeft) / 2;
	cv::Point2f heightTextPosition = topRight + (bottomRight - topRight) / 2;

	// Draw width and height values at computed positions
	cv::putText(image, widthString, widthTextPosition - cv::Point2f(widthString.length()/2, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, cv::Scalar(200, 200, 250), 1);
	cv::putText(image, heightString, heightTextPosition + cv::Point2f(10, 0), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.6, cv::Scalar(200, 200, 250), 1);

}