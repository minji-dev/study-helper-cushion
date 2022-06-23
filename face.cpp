#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
#include "serialcomm.h"

using namespace std;
using namespace cv;

#define CAM_WIDTH 720
#define CAM_HEIGHT 480
int num_face, num_eye;

/** Function Headers */
void detectAndDisplay(Mat frame);
void detectAndDisplay_eye(Mat frame);
/** Global variables */
String face_cascade_name, eye_cascade_name;
CascadeClassifier face_cascade, eye_cascade;
String window_name = "Face detection";
CSerialComm serialComm;
char sleepStatus = '0';
char temp = '0';

/** @function main */
int main(int argc, const char** argv)
{
	VideoCapture cam(0);
	Mat frame;
	char buffer;

	if (!serialComm.connect("COM5")) //COM5번의 포트를 오픈한다. 실패할 경우 -1을 반환한다.
	{
		printf("connect faliled");
		return -1;
	}
	else
		printf("connect successed");

	face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
	eye_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
	if (!eye_cascade.load(eye_cascade_name)) { printf("--(!)Error loading eye cascade\n"); return -1; };

	cam.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);

	if (!cam.isOpened()) { printf("--(!)Error opening video cam\n"); return -1; }

	while (cam.read(frame))
	{
		num_face = 0;
		num_eye = 0;
		if (frame.empty())
		{
			printf(" --(!) No camd frame -- Break!");
			break;
		}

		detectAndDisplay(frame);
		detectAndDisplay_eye(frame);
		if ((num_face == 0) || (num_face == 1 && num_eye == 0))
		{
			sleepStatus = '1';
			if (temp == '0') {
				printf("face = %d eye = %d || sleep!! ", num_face, num_eye);
				serialComm.sendCommand('1');
				temp = '1';
			}
		}
		else
		{
			sleepStatus = '0';
			if (temp == '1') {
				printf("face = %d eye = %d || awake ", num_face, num_eye);
				serialComm.sendCommand('0');
				temp = '0';
			}
		}
		char c = (char)waitKey(22);
		if (c == 27) { break; }
	}
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		num_face++;
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2),
			0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
	}

	imshow(window_name, frame);
}

void detectAndDisplay_eye(Mat frame)
{
	std::vector<Rect> eyes;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	eye_cascade.detectMultiScale(frame_gray, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < eyes.size(); i++)
	{
		num_eye++;
		Point center(eyes[i].x + eyes[i].width / 2, eyes[i].y + eyes[i].height / 2);
		ellipse(frame, center, Size(eyes[i].width / 2, eyes[i].height / 2),
			0, 0, 360, Scalar(255, 0, 0), 4, 8, 0);
	}

	imshow(window_name, frame);
}