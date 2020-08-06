#include "Context.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <list>

#define COMPUTE_SIZE(sigma) (2 * ceil(sigma/2.5) + 1);
#define GAUSSIAN_COEF(sigma) (1. / (2 * M_PI * sigma * sigma));

static bool positionInMat(const cv::Mat& mat, int row, int col){
	return !(row < 0 || row >= mat.rows || col < 0 || col >= mat.cols);
}

static bool computeNeighborhood(Context *ctx, int row, int col, bool background){
	int kernel_row;  
	int current_row;  // row of current neigbhor
	int current_col;  // col of current neighbor
	double pixel[3] = {0};
	double sum = 0.;
	if (ctx == nullptr){
		return false;
	}
	kernel_row = -(ctx->kernel.rows / 2);
	for (int i = 0; i < ctx->kernel.rows; i++, kernel_row++){
		for (int j = 0, kernel_col = -(ctx->kernel.cols / 2); j < ctx->kernel.cols; j++, kernel_col++){
			current_row = row + kernel_row;
			current_col = col + kernel_col;
			if (!positionInMat(ctx->src, current_row, current_col)
				||(background && (ctx->depth.at<unsigned char>(current_row, current_col) > ctx->depth.at<unsigned char>(row, col)+ 1))){
				continue;
			}
			for (int p = 0; p < 3; p++){
				pixel[p] += ctx->src.at<cv::Vec3b>(current_row, current_col)[p] * ctx->kernel.at<double>(i, j);
			}
			sum+= ctx->kernel.at<double>(i, j);
		}
	}
	for (int p = 0; p < 3; p++){
		ctx->output.at<cv::Vec3b>(row, col)[p] = (unsigned char)(pixel[p]/sum);
	}
	return true;
}

static cv::Mat computeKernel(Context *ctx, double sigma){
	int rows;
	int cols;

	double gaussian_coeff;
	double dividende;
	double value;
	double sum;

	cv::Mat tmp;

    if (sigma <= 0. || ctx == nullptr){
        return cv::Mat();
    }
    
	sum = 0;
	rows = cols = COMPUTE_SIZE(sigma);
	gaussian_coeff = GAUSSIAN_COEF(sigma);
	dividende = (2 * sigma * sigma);
    tmp = cv::Mat_<double>(rows, cols);

	int x = -(rows / 2);
	for (int i = 0; i < rows; i++, x++) {
		int y = -(cols / 2);
		for (int j = 0; j < cols; j++, y++) {
			value = gaussian_coeff * exp(-((x * x) + (y * y)) / dividende);
			sum += value;
			tmp.at<double>(i,j) = value;
		}
	}
	return tmp;
}

cv::Mat getGaussianKernel(Context *ctx, int distance) {

    if (ctx == nullptr){
        return cv::Mat();
    }
	auto kernel = ctx->all_kernel.find(distance);

	distance++;
	if(kernel == ctx->all_kernel.end()){
		cv::Mat tmp = computeKernel(ctx, distance/6.);
		ctx->all_kernel.insert({distance, tmp});
		return tmp;
	}
   return kernel->second;
}

static void treatPoint(Context *ctx, std::list<cv::Point2i>&points_to_treat, int row, int col, int depth){
	if (!positionInMat(ctx->focus, row, col)
	|| ctx->focus.at<unsigned char>(row, col) != 0
	|| abs(ctx->depth.at<unsigned char>(row, col) - depth) > ctx->depth_difference){
		return;
	}

	points_to_treat.push_back(cv::Point2i(col, row));
	ctx->focus.at<unsigned char>(row, col) = 255;
}

static void computeObject(Context *ctx){
	int depth;
	int row;
	int col;
	cv::Point2i current_point;
	std::list<cv::Point2i> points_to_treat;

	if(ctx == nullptr || !positionInMat(ctx->focus, ctx->clic_row, ctx->clic_col)){
		return;
	}
	row = ctx->clic_row;
	col = ctx->clic_col;
	current_point = cv::Point2i(col, row);
	depth = ctx->depth.at<unsigned char>(row, col);
	points_to_treat.push_back(current_point);
	ctx->focus.setTo(cv::Scalar(0));

	for (; !points_to_treat.empty();){
		current_point = points_to_treat.front();
		points_to_treat.pop_front();
		ctx->focus.at<unsigned char>(current_point.y, current_point.x) = 255;
		treatPoint(ctx, points_to_treat, current_point.y - 1, current_point.x, depth);
		treatPoint(ctx, points_to_treat, current_point.y + 1, current_point.x, depth);
		treatPoint(ctx, points_to_treat, current_point.y, current_point.x - 1, depth);
		treatPoint(ctx, points_to_treat, current_point.y, current_point.x + 1, depth);
	}
}

bool openDepthImage(Context& ctx, const cv::String& inputImage){
	size_t positionExtension;
	cv::String name;
	cv::String extension;

	positionExtension = inputImage.find_last_of(inputImage, '.');
	if (positionExtension == inputImage.npos){
		return false;
	}

	name = inputImage.substr(0, positionExtension -3);
	extension = inputImage.substr(positionExtension - 3, inputImage.length());
	
	ctx.depth = cv::imread(name + "_depth" + extension, CV_LOAD_IMAGE_GRAYSCALE);

	return !ctx.depth.empty()
		&& (ctx.depth.rows == ctx.src.rows)
		&& (ctx.depth.cols == ctx.src.cols);
}

void computeOutputImage(Context *ctx){
	int depth;
	int distance;
    if(ctx == nullptr || !positionInMat(ctx->src, ctx->clic_row, ctx->clic_col)){
   		return;
    }
    computeObject(ctx); // object focus
	ctx->src.copyTo(ctx->output);
	
    depth = ctx->depth.at<unsigned char>(ctx->clic_row, ctx->clic_col); //averageDepth(ctx, ctx->focus);
	for (int i = 0; i < ctx->src.rows; i++){
		for (int j = 0; j < ctx->src.cols; j++){
			if (ctx->focus.at<unsigned char>(i,j) == 255){ // pixel fait partie du focus
				continue;
			}
			distance = (depth - ctx->depth.at<unsigned char>(i, j));
			ctx->kernel = getGaussianKernel(ctx, abs(distance));
			computeNeighborhood(ctx, i, j, abs(distance)==distance);
		}
	}
}

void displaySourceImage(const Context *ctx){
    cv::String window_name = "image";
    cv::namedWindow(window_name);
    cv::imshow(window_name, (ctx->src));
}

void displayOutputImage(const Context *ctx){
    cv::String window_name = "output";
    cv::namedWindow(window_name);
    cv::imshow(window_name, (ctx->output));
}

void displayFocusImage(const Context *ctx){
    cv::String window_name = "focus";
    cv::namedWindow(window_name);
    cv::imshow(window_name, (ctx->focus));
}

void exportOutputImage(const Context *ctx, const std::string& exportPath){
    std::string outputPath = "output/export_" + std::to_string(ctx->clic_col) + ' ' + std::to_string(ctx->clic_row) + ".png";
    cv::imwrite(outputPath, (ctx->output));
}

void ContextRelease(Context *ctx){
	if (ctx == nullptr){
		return;
	}
	
	ctx->src.release();
	ctx->output.release();
	ctx->depth.release();
	ctx->focus.release();
	for (std::pair<unsigned char, cv::Mat> element : ctx->all_kernel){
		element.second.release();
	}
}