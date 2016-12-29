#include "stdafx.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Capture.h"

#include "CinderOpenCV.h"

#include "IRenderMode.h"
#include "PassThrough.h"
#include "Sharpen.h"
#include "Sobel.h"
#include "Threshold.h"
#include "Tracking.h"

using namespace ci;
using namespace ci::app;

class ocvCaptureApp : public App {
public:
	void setup() override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void resize() override;

	void mouseMove(MouseEvent event) override {
		m_mouse_loc = event.getPos();
	}

private:
	CaptureRef			mCapture;
	gl::TextureRef		mTexture;
	std::vector<std::unique_ptr<IRenderMode>> m_modes;
	std::mutex render_mode_mutex;
	decltype(m_modes)::iterator current_render_mode;
	glm::ivec2 m_mouse_loc;

    ci::CameraOrtho			mCam;

	cv::Scalar GetColorAtPosition(glm::ivec2 position);
};

void ocvCaptureApp::setup()
{
	try {
		m_modes.emplace_back(new PassThrough());
		m_modes.emplace_back(new Sharpen());
		m_modes.emplace_back(new Sobel());
		m_modes.emplace_back(new Threshold());
		m_modes.emplace_back(new Tracking());

		current_render_mode = m_modes.begin();

		mCapture = Capture::create(640, 480);
		mCapture->start();

        auto ul = Color(1, 1, 1);
        auto ll = Color(1, 1, 1);
        auto ur = Color(0, 0, 0);
        auto lr = Color(0, 0, 0);

        mCam.setOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	}
	catch (...) {
		console() << "Failed to initialize capture" << std::endl;
	}
}

void ocvCaptureApp::keyDown(KeyEvent event)
{
	if (event.getChar() == KeyEvent::KEY_SPACE)
	{
		std::unique_lock<std::mutex> lock(render_mode_mutex);
		current_render_mode++;
		if (current_render_mode == m_modes.end())
		{
			current_render_mode = m_modes.begin();
		}
	}
	else if (event.getChar() == KeyEvent::KEY_ESCAPE)
	{
		quit();
	}
    else
    {
        auto tracking = dynamic_cast<Tracking *>(current_render_mode->get());
        if (tracking)
        {
            if (event.getCode() == KeyEvent::KEY_UP)
            {
                tracking->Increment();
            }
            else if (event.getCode() == KeyEvent::KEY_DOWN)
            {
                tracking->Decrement();
            }
        }
    }
}

void ocvCaptureApp::update()
{
	if (mCapture && mCapture->checkNewFrame())
	{
		auto input(toOcv(*mCapture->getSurface()));
		auto output = cv::Mat{};

		if (current_render_mode != m_modes.end())
		{
			std::unique_lock<std::mutex> lock(render_mode_mutex);
			(*current_render_mode)->Compute(input, output);
		}
		else
		{
			output = input;
		}

		mTexture = gl::Texture::create(fromOcv(output), gl::Texture::Format().loadTopDown());
	}
}


void ocvCaptureApp::draw()
{
	auto window_size = getWindowSize();
	gl::ScopedViewport scopedViewport(ivec2(0), window_size);
	gl::clear(Color(0,0,0));

    gl::setMatrices(mCam);
    if (mTexture)
    {
    	Rectf drawRect(1.0f, 1.0f, -1.0f, -1.0f);
    	gl::draw(mTexture, drawRect);
    }
    
    gl::setMatricesWindow(app::getWindowSize());

	auto color = GetColorAtPosition(m_mouse_loc);
    gl::drawString("x,y: " + std::to_string(m_mouse_loc.x) + " " + std::to_string(m_mouse_loc.y), glm::vec2(0.0f, 0.0f), ColorA(1, 0, 1, 1));
	gl::drawString("RGB: " + std::to_string(color[0]) + " " + std::to_string(color[1]) + " " + std::to_string(color[2]), glm::vec2(0.0f, 20.0f), ColorA(1, 0, 1, 1));

	auto hsv_color = cv::Mat{};
	cv::cvtColor(cv::Mat(1, 1, CV_8UC3, color), hsv_color, cv::COLOR_BGR2HSV);
    auto ss = std::stringstream{};
    ss << "HSV: " << hsv_color;
	gl::drawString(ss.str(), glm::vec2(0.0f, 40.0f), ColorA(1, 0, 1, 1));

    auto tracking = dynamic_cast<Tracking *>(current_render_mode->get());
    if (tracking)
    {
        gl::drawString("Range: " + std::to_string(tracking->GetRange()), glm::vec2(0.0f, 60.0f), ColorA(1, 0, 1, 1));
    }
}

void ocvCaptureApp::resize()
{
}

cv::Scalar ocvCaptureApp::GetColorAtPosition(glm::ivec2 position)
{
	unsigned char color_bytes[3];
	gl::readPixels(position.x, app::getWindowSize().y - position.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color_bytes);

	return cv::Scalar{ (double)color_bytes[0], (double)color_bytes[1], (double)color_bytes[2] };
}

CINDER_APP(ocvCaptureApp, RendererGl)
