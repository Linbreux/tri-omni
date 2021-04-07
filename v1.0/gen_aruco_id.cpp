#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    if (argc ==1 ){
        cout << "geef een ID mee, bv. ./a.out <ID> [output]" <<endl;
    }

    cv::Mat marker;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::aruco::drawMarker(dictionary, stoi(argv[1]), 200,marker, 1);
    if (argc == 2){
        cout << "marker aangemaakt met id: " << argv[1] << " maker.png" << endl;
        cv::imwrite("marker.png", marker);
    }if (argc == 3){
        cout << "marker aangemaakt met id: " << argv[1] << " " << *argv[2] << endl;
        cv::imwrite(argv[2], marker);

    }

}

