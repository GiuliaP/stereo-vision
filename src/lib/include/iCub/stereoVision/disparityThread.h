#include <iCub/stereoVision/stereoCamera.h>
#include <yarp/dev/PolyDriver.h>
#include <iCub/iKin/iKinFwd.h>
#include <yarp/dev/GazeControl.h>

#define LEFT    0
#define RIGHT   1

using namespace std; 
using namespace yarp::os; 
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace iCub::ctrl;
using namespace iCub::iKin;

/**
* \ingroup StereoVisionLib
*
* The class defining the disparity computation.
* It computes the depth map and it updates the icub's eye relative positions.
*/
class DisparityThread : public RateThread
{
private:

    StereoCamera *stereo;
    bool done;
    bool work;
    bool init;
    bool success;
    bool useCalibrated;
    bool useHorn;

    Matrix yarp_initLeft,yarp_initRight;
    Matrix yarp_H0;
    Semaphore* mutexDisp;
    PolyDriver* gazeCtrl;
    IGazeControl* igaze;

    iCubEye *LeyeKin;
    iCubEye *ReyeKin;
    yarp::dev::PolyDriver polyHead;
    yarp::dev::IEncoders *posHead;
    yarp::dev::IControlLimits *HctrlLim;

    yarp::dev::PolyDriver polyTorso;
    yarp::dev::IEncoders *posTorso;
    yarp::dev::IControlLimits *TctrlLim;

    Matrix H;
    Mat HL_root;
    Mat HR_root;

    double vergence_init;
    double version_init;

    string robotName;

    void buildRotTras(Mat & R, Mat & T, Mat & A);
    bool loadStereoParameters(yarp::os::ResourceFinder &rf, Mat &KL, Mat &KR, Mat &DistL, Mat &DistR, Mat &Ro, Mat &T);
    Matrix getCameraHGazeCtrl(int camera);
    Matrix getCameraH(yarp::sig::Vector head_angles,yarp::sig::Vector torso_angles, iCubEye *eyeKin, int camera);
    void printMatrixYarp(Matrix &A);
    void convert(Matrix& matrix, Mat& mat);
    void convert(Mat& mat, Matrix& matrix);

public:

    DisparityThread(yarp::os::ResourceFinder &rf, bool useHorn=true);
    ~DisparityThread(void) {};


    void setImages(Mat &left, Mat &right);
    void getDisparity(Mat &Disp);
    void getDisparityFloat(Mat &Disp);
    void getQMat(Mat &Q);
    void getMapper(Mat &Mapper);
    void getRectMatrix(Mat &RL);
    void triangulate(Point2f &pixel,Point3f &point) ;
    bool checkDone();
    void getRootTransformation(Mat & Trans,int eye=LEFT);
    bool isOpen();



    bool threadInit(void);
    void threadRelease(void);
    void run(void); 
    void onStop(void);
 
};
