#ifndef __TRAITEMENT_CONTEXT__
#define __TRAITEMENT_CONTEXT__

#include <opencv2/core/core.hpp>
#include <unordered_map>

#define DEFAULT_DEPTH_DIFFERENCE 10

/**
 * @brief represents a context which will be the  user data in opencv
 * the context contains the context contains all the data necessary for the user: the last mouse click position, all computed images...
 * a map is also used to improve the speed of calculations
*/
typedef struct {
    bool display_focus;        // if we want to display in a window the focused object
    int clic_row;              // the y position of the last mouse click
    int clic_col;              // the x position of the last mouse click 
	int depth_difference;      // the max difference of depth between two pixels to be considered as the same object
    cv::Mat focus;             // the current focused object
    cv::Mat output;            // the output image
	cv::Mat kernel;            // the last gaussian kernel
	cv::Mat depth;             // the image representing depth of all pixels
    cv::Mat src;               // the input image
	std::unordered_map<unsigned char, cv::Mat> all_kernel; // contains all computed kernel in the program to avoid to compute them again
} Context;

/**
 * @brief compute the outputImage according to all images of the context
 * @param ctx the context which contanins all images
*/
void computeOutputImage(Context *ctx);

/**
 * @brief load the depth image of the input image 
 * @param ctx the context which contains all images
 * @param inputImage the input image
 * @return true if we found and load the depth image
 *         false otherwise
*/
bool openDepthImage(Context& ctx, const cv::String& inputImage);

/**
 * @brief export the output image in the exportPath
 * @param ctx the context which contains the output image
 * @param exportPath the path where the image should be exported
*/
void exportOutputImage(const Context *ctx, const std::string& exportPath);

/**
 * @brief display the source image of the context
 * @param ctx the context which contains the source image
*/
void displaySourceImage(const Context *ctx);

/**
 * @brief display the output image of the context
 * @param ctx the context which contains the output image
*/
void displayOutputImage(const Context *ctx);


/**
 * @brief display the focused object of the context
 * @param ctx the context which contains the focused object
*/
void displayFocusImage(const Context *ctx);


/**
 * @brief get the the gaussian kernel corresponding to the distance
 * compute the kernel if it was not found
 * update the field kernel of the context with the asked kernel
 * @param ctx the context which contains the all
 * @param distance the distance 
 * @return the distance corresponding to the calculated kernel
*/
cv::Mat getGaussianKernel(Context *ctx, int distance);


/**
 * @brief release the allocated memory
 * @param ctx the context which contains all images
*/
void ContextRelease(Context *ctx);
#endif