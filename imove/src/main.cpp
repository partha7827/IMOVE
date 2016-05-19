#include <opencv2/opencv.hpp>
#include <opencv2/core/persistence.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "calibration/Calibration.hpp"
#include "image_processing/PeopleDetector.h"
#include "interface/Person.h"

const unsigned char U8_WHITE = 255;
const signed int NOKEY_ANYKEY = -1;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: <path to configuration file>" << std::endl;
		return EXIT_SUCCESS;
	}

	cv::FileStorage fs;
	fs.open(argv[1], cv::FileStorage::READ);
	int camera_device;
	fs["Camera_device"] >> camera_device;
	cv::Size resolution_camera;
	fs["Resolution_camera"] >> resolution_camera;
	cv::Size resolution_projector;
	fs["Resolution_projector"] >> resolution_projector;
	cv::Mat camera_projector_transformation;
	fs["Camera_projector_transformation"] >> camera_projector_transformation;
	unsigned int frames_projector_camera_delay;
	signed int int_frames_projector_camera_delay;
	fs["Frames_projector_camera_delay"] >> int_frames_projector_camera_delay;
	if (int_frames_projector_camera_delay < 0) {
		frames_projector_camera_delay = 0;
	} else {
		frames_projector_camera_delay = (unsigned int) int_frames_projector_camera_delay;
	}
	double percentage_projector_background_light;
	fs["Percentage_projector_background_light"] >> percentage_projector_background_light;
	float meter;
	fs["Meter"] >> meter;
	fs.release();

	const Calibration calibration = Calibration(resolution_projector, camera_projector_transformation, frames_projector_camera_delay, percentage_projector_background_light);
	PeopleDetector people_detector = PeopleDetector();
	
	cv::Mat frame_projector = cv::Mat::ones(resolution_projector.width, resolution_projector.height, CV_8UC3) * U8_WHITE;
	cv::namedWindow("Projector", cv::WINDOW_NORMAL);
	cv::moveWindow("Projector", 0, 0);
	cv::imshow("Projector", frame_projector);

	cv::namedWindow("Camera", cv::WINDOW_NORMAL);
	//cv::VideoCapture video_capture(camera_device);
	cv::VideoCapture video_capture("./imove/test/image_processing/IMG_0638.MOV");
	cv::Mat frame_camera;
	cv::moveWindow("Camera", 500, 0);

	cv::namedWindow("Projection", cv::WINDOW_NORMAL);
	cv::Mat frame_projection;
	cv::moveWindow("Projection", 1000, 0);
	cv::namedWindow("Frame", cv::WINDOW_NORMAL);
	while (cv::waitKey(1) == NOKEY_ANYKEY) {
		if (!video_capture.read(frame_camera)) {
			std::cerr << "Unable to read next frame." << std::endl;
			std::cerr << "Exiting..." << std::endl;
			return EXIT_FAILURE;
		}
		cv::imshow("Camera", frame_camera);
		calibration.createFrameProjectionFromFrameCamera(
			frame_projection,
			frame_camera
		);
		cv::imshow("Projection", frame_projection);
	 	people_detector.detectPeople(frame_camera);
		vector<Person> detected_people = people_detector.getDetectedPeople();
	}
 
	return EXIT_SUCCESS;
}
