#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <unordered_map>
#include "Context.hpp"
#include "Commands.hpp"

using namespace std;

int main(int argc, char ** argv){
	
	cv::String input_image;
	cv::String image_name;
	Context ctx;

	if(argc != 2){
		std::cout << "usage: " << argv[0] << " <my_image> " << std::endl;
		return 1;
	}

	ctx.depth_difference = DEFAULT_DEPTH_DIFFERENCE;

	std::cout << "load image ..." << std::endl;
	ctx.src = cv::imread(argv[1]);
	if((ctx.src).empty()){
		std::cout << "error loading " << argv[1] << endl;
		return 1;
	}

	ctx.display_focus = false;
	ctx.output = ctx.src.clone();

	if (!openDepthImage(ctx, argv[1])){
		cerr << " depth image not found" << endl;
		return 1;
	}

  	ctx.focus = cv::Mat(ctx.src.rows, ctx.src.cols, CV_8U);
	// image originale 
	cv::namedWindow("image", 1);

	cv::setMouseCallback("image", callBackMouse, &ctx);
	cv::imshow("image", (ctx.src));
	keyboard_command(&ctx);
	
	return 0;
}
