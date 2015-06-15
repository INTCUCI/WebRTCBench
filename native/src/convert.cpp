#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <fstream>
#include <vector>

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

const char* WIN_RF = "Reference";

void help()
{
	cout << endl;
	cout << "/////////////////////////////////////////////////////////////////////////////////" << endl;
	cout << "This program measures Frame Loss Rate" << endl;
	cout << "USAGE: ./convert input output outputwidth outputheight" << endl;
	cout << "For example: ./tag video/Megamind.avi test.y4m 640 480" << endl;
	cout << "/////////////////////////////////////////////////////////////////////////////////" << endl << endl;
}

int main(int argc, char **argv)
{

	if(argc != 5)
	{
		help();
		return -1;
	}
	
	namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(WIN_RF, 400       , 0);

    const string inputVideoName = argv[1];
    int owidth = atoi(argv[3]);
    int oheight = atoi(argv[4]);

    int frameNum = -1;          // Frame counter

    VideoCapture inputVideo(inputVideoName);
    
    string::size_type pAt = inputVideoName.find_last_of('.');
    const string name = argv[2];
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));

    if (!inputVideo.isOpened())
    {
        cout  << "Could not open reference " << inputVideoName << endl;
        return -1;
    }
    
    Size refS = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
                     
	char EXT[] = {(char)(ex & 0xff), (char)((ex & 0xff00) >> 8), (char)((ex & 0xff0000) >> 16), (char)((ex & 0xff000000) >> 24), 0};

    cout << "input video frame resolution: Width=" << refS.width << "  Height=" << refS.height
         << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;

    Mat _frameReference, frameReference;
    
    VideoWriter output;
cout << name << endl;
	int fcc = CV_FOURCC('I','4','2','0');
    output.open("./output/"+name, fcc, 30, Size(owidth, oheight), true);

    
    if (!output.isOpened())
    {
        cout  << "Could not open output video " << name << endl;
        return -1;
    }
	
	inputVideo >> _frameReference;
    for(;;) //Show the image captured in the window and repeat
    {
        inputVideo >> _frameReference;

        if (_frameReference.empty() )
        {
            cout << " < < <  Video over!  > > > ";
            break;
        }
        
        resize(_frameReference, frameReference, Size(owidth, oheight));

        ++frameNum;
        cout << "Frame: " << frameNum << "# " << endl;
        
        int number = frameNum;
        int num = 0;
        
        output << frameReference;

        ////////////////////////////////// Show Image /////////////////////////////////////////////
        imshow(WIN_RF, frameReference);
        //imshow(WIN_UT, frameUnderTest);

        char c = (char)cvWaitKey(33);
        if (c == 27) break;
    }
	
	return 0;
}
