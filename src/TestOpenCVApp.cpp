#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h"

#include "CinderOpenCV.h"

using namespace ci;
using namespace ci::app;

class ocvCaptureApp : public App {
public:
	void setup();
	void keyDown(KeyEvent event);
	void update();
	void Sharpen(cv::Mat &input, cv::Mat &output);
	void FFT(cv::Mat &input, cv::Mat &output);
	void draw();
	void resize();

	CaptureRef			mCapture;
	gl::TextureRef		mTexture;
	int modes = 4;
};

constexpr int GetMaxModes() { return 5; }

void ocvCaptureApp::setup()
{
	try {
		mCapture = Capture::create(640, 480);
		mCapture->start();
	}
	catch (...) {
		console() << "Failed to initialize capture" << std::endl;
	}
}

void ocvCaptureApp::keyDown(KeyEvent event)
{
	if (event.getChar() == KeyEvent::KEY_SPACE)
	{
		modes++;
		if (modes >= GetMaxModes())
		{
			modes = 0;
		}
	}
	else if (event.getChar() == KeyEvent::KEY_ESCAPE)
	{
		quit();
	}
}

void ocvCaptureApp::update()
{
	if (mCapture && mCapture->checkNewFrame()) {
		cv::Mat input(toOcv(*mCapture->getSurface())), output;

		switch (modes)
		{
		case 0:
			cv::Sobel(input, output, CV_8U, 1, 0);
			break;
		case 1:
			cv::threshold(input, output, 128, 255, CV_8U);
			break;
		case 2:
			cv::Laplacian(input, output, CV_8U);
			break;
		case 3:
			output = input;
			break;
		case 4:
			Sharpen(input, output);
			break;
		}

		mTexture = gl::Texture::create(fromOcv(output), gl::Texture::Format().loadTopDown());
	}
}

void ocvCaptureApp::Sharpen(cv::Mat &input, cv::Mat &output)
{
	auto kern = cv::Mat{ (cv::Mat_<char>(3,3) << 0, -1,  0,
			                                    -1,  5, -1,
			                                     0, -1,  0) };
	cv::filter2D(input, output, CV_8U, kern);
}

void ocvCaptureApp::draw()
{
	auto window_size = getWindowSize();
	gl::ScopedViewport scopedViewport(ivec2(0), window_size);
	gl::clear(Color(1,1,1));
	if (mTexture)
	{
		gl::setMatricesWindow(window_size);
		Rectf drawRect(static_cast<float>(window_size.x), 0.0f, 0.0f, static_cast<float>(window_size.y));
		gl::draw(mTexture, drawRect);
	}
}

void ocvCaptureApp::resize()
{
}


CINDER_APP(ocvCaptureApp, RendererGl)
