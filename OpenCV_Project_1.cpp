#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
void nothing(int pos, void *)
{

}
void FindFrame(Mat mask, Mat ori)
{
	Mat a;
	vector<vector<Point>>contours;
	vector<Vec4i>hireachy;
	findContours(mask, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
	for (int i = 0; i < size(contours); i++)
	{
		//drawContours(ori, contours, i, Scalar(0, 0, 255), 3, 8);
		if (contourArea(contours[i], false) > 1300) // 除去微小元素干扰
		{
			Rect rec = boundingRect(contours[i]);
			if (150 * rec.height > 100 * rec.width && 100 * rec.height < 150 * rec.width) // 判断图形近似为正方形，去除天空干扰
			{
				rectangle(ori, rec, Scalar(0, 0, 255), 2, 8);
				putText(ori, "aim 0.03", Point(rec.tl().x, rec.tl().y - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
			}
		}
	}
	imshow("Frame", ori);
}
int main()
{
	VideoCapture capture;
	capture.open("video.mp4");
	if (!capture.isOpened())
	{
		printf("erroe\n");
		return -1;
	}
	Mat frame,a, b;
	namedWindow("Frame");
	namedWindow("output");
	while (capture.read(frame))
	{
		Mat kernel1 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3), Point(-1, -1));
		inRange(frame, Scalar(116, 0, 0), Scalar(255, 255, 74), a); // 分理处球
		erode(a, b, kernel1);
		//imshow("video", frame);
		imshow("output", b);
		FindFrame(b, frame);
		waitKey(30);
	}
}