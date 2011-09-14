#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int _brilho = 100;
Mat image, img_aux, image2;
int histSize = 256;


void histograma_img_tons_de_cinza(double a, double b){
    Mat hist_origin, img_balanceada;

    image.convertTo(img_balanceada, CV_8U, a, b);
    imshow("Imagem Original", img_balanceada);

    calcHist(&img_balanceada, 1, 0, Mat(), hist_origin, 1, &histSize, 0);
    Mat histImage_origin = Mat::ones(160, 260, CV_8U)*255;

    normalize(hist_origin, hist_origin, 0, histImage_origin.rows, CV_MINMAX, CV_32F);

    histImage_origin = Scalar::all(255);
    int binW = cvRound((double)histImage_origin.cols/histSize);

    for( int i = 0; i < histSize; i++ )
        rectangle( histImage_origin, Point(i*binW, histImage_origin.rows),
                   Point((i+1)*binW, histImage_origin.rows - cvRound(hist_origin.at<float>(i))),
                   Scalar::all(0), -1, 8, 0 );
    imshow("Histograma Original", histImage_origin);

}


void atualizar_brilho( int arg, void* ){
    int brilho = _brilho - 100;
    double a, b;

        double delta = -128/100;
        a = (256.-delta*2)/255.;
        b = a*brilho + delta;

    histograma_img_tons_de_cinza(a,b);

    Mat img_eq, hist;

    img_aux.convertTo(img_eq, CV_8U, a, b);
    imshow("Imagem Equalizada", img_eq);


    calcHist(&img_eq, 1, 0, Mat(), hist, 1, &histSize, 0);
    Mat histImage = Mat::ones(160, 260, CV_8U)*255;

    normalize(hist, hist, 0, histImage.rows, CV_MINMAX, CV_32F);

    histImage = Scalar::all(255);
    int binW = cvRound((double)histImage.cols/histSize);

    for( int i = 0; i < histSize; i++ )
        rectangle( histImage, Point(i*binW, histImage.rows),
                   Point((i+1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
                   Scalar::all(0), -1, 8, 0 );
    imshow("Histograma Equalizado", histImage);
}

int main( int argc, char** argv ){
     image = imread("baboon.jpg", 0);
     equalizeHist(image, img_aux);
    namedWindow("Imagem Equalizada", 0);
    namedWindow("Histograma Equalizado", 0);
    namedWindow("Imagem Original",0);
    namedWindow("Histograma Original", 0);

    createTrackbar("Equalização", "Imagem Original", &_brilho, 200, atualizar_brilho);

    atualizar_brilho(0, 0);
    waitKey();

    return 0;
}

