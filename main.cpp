#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);

/** Global variables */
CascadeClassifier face_cascade;


int main(int argc, const char** argv)
{
    String face_cascade_name = "resources/haarcascade_frontalface_default.xml";

    //-- 1. Load the cascades
    if (!face_cascade.load(face_cascade_name))
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };

    const string path = "resources/video_face.MOV";
    VideoCapture inputVideo(path);

    // get video resolution
    int frameWidth = inputVideo.get(CAP_PROP_FRAME_WIDTH);
    int frameHeight = inputVideo.get(CAP_PROP_FRAME_HEIGHT);

    // create videwriter obj
    VideoWriter outputVideo("output.MOV", VideoWriter::fourcc('M','J','P','G'), 30, Size(frameWidth, frameHeight));


    // -- 2. Read a video stream
    if (!inputVideo.isOpened())
    {
        cout << "Error opening stream or file!" << endl;
    }
    else
    {
        int fps = inputVideo.get(30);
        cout << "Frames per second: " << fps;
        int frame_count = inputVideo.get(30);
        cout << "Frame count: " << frame_count;
    }

    // read the frames to the last frame
    while (inputVideo.isOpened())
    {
        Mat video;
        inputVideo.read(video);
        detectAndDisplay(video);

        if (!outputVideo.isOpened())
        {
            cout << "Could not open the output video for write." << endl;
            return -1;
        }

        outputVideo.write(video);

        int key = waitKey(25);
        if (key == 'q' || waitKey(25) >= 0)
        {
            cout << "Force quit by Q" << endl;
            break;
        }
    }


    outputVideo.release();
    inputVideo.release();

    destroyAllWindows();

    return 0;
}

/* function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces);

    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        rectangle(frame, faces[i].tl(), faces[i].br(), Scalar(0, 0, 255), 3);

        Mat faceROI = frame_gray(faces[i]);
    }

    //-- Show what you got
    imshow("Capture - Face detection", frame);
}