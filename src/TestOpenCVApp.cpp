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

using namespace ci;
using namespace ci::app;

class ocvCaptureApp : public App {
public:
	void setup() override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void resize() override;

private:
	CaptureRef			mCapture;
	gl::TextureRef		mTexture;
	std::vector<std::unique_ptr<IRenderMode>> m_modes;
	std::mutex render_mode_mutex;
	decltype(m_modes)::iterator current_render_mode;
};

void ocvCaptureApp::setup()
{
	try {
		m_modes.emplace_back(new PassThrough());
		m_modes.emplace_back(new Sharpen());
		m_modes.emplace_back(new Sobel());
		m_modes.emplace_back(new Threshold());

		current_render_mode = m_modes.begin();

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
