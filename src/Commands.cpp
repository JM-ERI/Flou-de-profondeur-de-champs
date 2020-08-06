#include "Commands.hpp"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#define DEPTH_DIFFERENCE_POWER 1
#define MIN_DEPTH_POWER 0
#define MAX_DEPTH_POWER 255

void keyboard_command(Context* ctx){

	while (true){
		switch (cv::waitKey()){
			case 'q':
				std::cout << "quit" << std::endl;
				ContextRelease(ctx);
				cv::destroyAllWindows();
				return;
			case 'o':
				ctx->display_focus = !ctx->display_focus;
				if (!ctx->display_focus){
					cv::destroyWindow("focus");
				}
				else{
					displayFocusImage(ctx);
				}
				std::cout << (ctx->display_focus ? "display" : "hide") << " focus" << std::endl;
				break;
			case 's':
                std::cout << "saving output image..." << std::endl;
				exportOutputImage(ctx, "output/");
				break;
			case '+':
			case 'p':
				ctx->depth_difference = std::min(ctx->depth_difference + DEPTH_DIFFERENCE_POWER, MAX_DEPTH_POWER);
				std::cout << "depth difference:" << ctx->depth_difference << std::endl;
				break;
			case '-':
			case 'm':
				ctx->depth_difference = std::max(ctx->depth_difference - DEPTH_DIFFERENCE_POWER, MIN_DEPTH_POWER);
				std::cout << "depth difference:" << ctx->depth_difference << std::endl;
				break;
			default:
				;
		}
	}
}

void callBackMouse(int event, int x, int y, int flags, void* userdata){
	Context *ctx;
	//cout << x << " " << y << endl;
	if(userdata == nullptr || event != cv::EVENT_LBUTTONDOWN){
		return;
	}
    ctx = (Context *)userdata;
	ctx->clic_row = y;
	ctx->clic_col = x;
	computeOutputImage(ctx);
	displayOutputImage(ctx);
	if (ctx->display_focus){
		displayFocusImage(ctx);
	}
	
}