//======================================================================
//
// RealsenseでRGB画像，距離画像を取得するプログラム
//
//                                           Shuichi Akizuki, Keio Univ.
//======================================================================
#include <stdio.h>
#include <stdlib.h>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

using namespace cv;
using namespace std;

Mat frame_to_mat(const rs2::frame& f);
Mat depth_frame_to_meters(const rs2::pipeline& pipe, const rs2::depth_frame& f);


int main(int argc, char * argv[])
{
    system("rm -fr image");
    system("rm -fr depth");
    system("mkdir image");
    system("mkdir depth");

    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;


    rs2::config cfg; //カメラの設定の定義
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    rs2::pipeline_profile selection = pipe.start(cfg);
    auto sensor = selection.get_device().first<rs2::depth_sensor>();
    auto scale =  sensor.get_depth_scale();

    cerr<<"Scale: "<<scale<<endl;

    // Camera warmup
    rs2::frameset frames;
    for( int i=0 ; i<30 ; i++ ){
       frames = pipe.wait_for_frames(); 
    }

    rs2::align align(RS2_STREAM_COLOR);

    int cnt = 0;
    char name_c[256], name_d[256];
    while(1){
        rs2::frameset data = pipe.wait_for_frames();  
        auto aligned_frames = align.process(data);   // RGB画像に対してDepth画像を位置合わせ
        rs2::frame depth_vis = color_map(aligned_frames.get_depth_frame());
        rs2::frame depth = aligned_frames.get_depth_frame();
        rs2::frame color = aligned_frames.get_color_frame();

        // rs2のフレームからcv::Matへの変換
        auto im_c = frame_to_mat(color);            //RGB画像
        auto im_d = frame_to_mat(depth);            //距離画像
        auto im_d_vis = frame_to_mat(depth_vis);    //可視化用の距離画像
        im_d *= 1000.0*scale; // 画素値をミリスケールに変換

        // Update the window with new data
        imshow("Depth", im_d_vis);
        imshow("RGB", im_c);

        waitKey(1);

        sprintf( name_c,"image/image%05d.png", cnt );
        sprintf( name_d,"depth/depth%05d.png", cnt );

        imwrite( name_c, im_c);
        imwrite( name_d, im_d);
        cerr<<"Frame: "<<cnt<<endl;
        cnt++;
    }

    return 0;
}

// Convert rs2::frame to cv::Mat
cv::Mat frame_to_mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        //cerr<<"RS2_FORMAT_BGR8"<<endl;
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        //cerr<<"RS2_FORMAT_RGB8"<<endl;
        auto r = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        cv::cvtColor(r, r, CV_BGR2RGB);
        return r;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        //cerr<<"RS2_FORMAT_Z16"<<endl;
        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8)
    {
        //cerr<<"RS2_FORMAT_Y8"<<endl;
        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);;
    }

    throw std::runtime_error("Frame format is not supported yet!");
}
