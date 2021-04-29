#include <opencv2/opencv.hpp>
#include <algorithm>


std::vector<cv::Point2f> orderVerticies(cv::RotatedRect& rect)
{
    /*
    Function orders verticies in clockwise direction,
    it returns a vector [topLeft, topRight, bottomRight, bottomLeft]
    */
    cv::Point2f topLeft;
    cv::Point2f topRight;
    cv::Point2f bottomLeft;
    cv::Point2f bottomRight;

    std::vector<cv::Point2f> clockwiseOrderVerticies;
    std::vector<cv::Point2f> verticiesVect(4);
    rect.points(verticiesVect.data());
  
    std::sort(verticiesVect.begin(), verticiesVect.end(),
        [](const cv::Point2f& vertex1, const cv::Point2f vertex2)
        {
            return vertex1.x < vertex2.x;
        }
    );

    // Compare the y-coordinate of most left points
    if (verticiesVect[0].y < verticiesVect[1].y)
    {
        topLeft = verticiesVect[0];
        bottomLeft = verticiesVect[1];
    }
    else
    {
        topLeft = verticiesVect[1];
        bottomLeft = verticiesVect[0];
    }

    // Evaluate the distance from topLeft to other two points, the point to which the distance is greater is by pythagoras theorem the bottomRight point
    if (sqrt((topLeft.x - verticiesVect[2].x) * (topLeft.x - verticiesVect[2].x) + (topLeft.y - verticiesVect[2].y) * (topLeft.y - verticiesVect[2].y)) 
        >
        sqrt((topLeft.x - verticiesVect[3].x) * (topLeft.x - verticiesVect[3].x) + (topLeft.y - verticiesVect[3].y) * (topLeft.y - verticiesVect[3].y)))
    {
        bottomRight = verticiesVect[2];
        topRight = verticiesVect[3];
    }
    else
    {
        bottomRight = verticiesVect[3];
        topRight = verticiesVect[2];
    }

    // Append points to vector in specified order
    clockwiseOrderVerticies.push_back(topLeft);
    clockwiseOrderVerticies.push_back(topRight);
    clockwiseOrderVerticies.push_back(bottomRight);
    clockwiseOrderVerticies.push_back(bottomLeft);

    return clockwiseOrderVerticies;
}

void sortContours_from_Left_to_Right(std::vector<std::vector<cv::Point>>& contours)
{
    std::sort(contours.begin(), contours.end(),
        [](const std::vector<cv::Point>& cont1, const std::vector<cv::Point>& cont2)
        {
            return cont1[0].x < cont2[0].x;
        }
    );
}

float computeReference(const std::vector<cv::Point2f>& verticies, const float referenceCoinDiameter)
{
    cv::Point2f topLeft = verticies[0];
    cv::Point2f topRight = verticies[1];

    return sqrt((topLeft.x - topRight.x) * (topLeft.x - topRight.x) +
        (topLeft.y - topRight.y) * (topLeft.y - topRight.y)) / referenceCoinDiameter;
}

std::vector<float> computeWidthAndHeight(const std::vector<cv::Point2f>& verticies, const float pixelsPerMetric)
{
    /*
    Function computes width and height of object using intialized reference pixelsPerMetric [pixels/mm],
    since our reference coin is always at left most position the value passed to this function should never be unintilaized
    */
    std::vector<float> dimensions;
    if (pixelsPerMetric == -1)
    {
        // If pixelsPerMetric is unintilaized abort()
        std::cerr << "Coin is not the left most object on the picture"<<std::endl;
        return std::vector<float>();
    }
    else
    {
        cv::Point2f topLeft = verticies[0];
        cv::Point2f topRight = verticies[1];
        cv::Point2f bottomLeft = verticies[3];

        dimensions.push_back(sqrt((topLeft.x - topRight.x) * (topLeft.x - topRight.x) 
                                + (topLeft.y - topRight.y) * (topLeft.y - topRight.y)) / pixelsPerMetric); // object width in mm

        dimensions.push_back(sqrt((topLeft.x - bottomLeft.x) * (topLeft.x - bottomLeft.x)
                                + (topLeft.y - bottomLeft.y) * (topLeft.y - bottomLeft.y)) / pixelsPerMetric); // object height in mm
    }

    return dimensions;
}