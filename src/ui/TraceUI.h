//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"
#include "../scene/texture.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider*			m_attenuationConstantSlider;
	Fl_Slider*			m_attenuationLinearSlider;
	Fl_Slider*			m_attenuationQuadraticSlider;
	Fl_Slider*			m_ambientLightSlider;
	Fl_Slider*			m_ThresholdSlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;

	Fl_Button*          m_depthFieldButton;
	Fl_Button*          m_softShadowButton;
	Fl_Button*          m_motionBlurButton;
	Fl_Button*          m_glossyReflectionButton;

	Fl_Button*			m_useBackgroundButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	double		getAttenuationConstant();
	double		getAttenuationLinear();
	double		getAttenuationQuadratic();
	double		getAmbientLight();
	double		getThreshold();

private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	double		m_nAttenuationConstant;
	double		m_nAttenuationLinear;
	double		m_nAttenuationQuadratic;
	double		m_nAmbientLight;
	double		m_nThreshold;

	bool		m_nUseBackground = false;
	Texture*	backgroundTexturePtr = nullptr;

	bool        m_nDepthField       = false;
	bool        m_nSoftShadow       = false;
	bool        m_nMotionBlur       = false;
	bool        m_nGlossyReflection = false;

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_load_background(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_attenuationConstantSlides(Fl_Widget* o, void* v);
	static void cb_attenuationLinearSlides(Fl_Widget* o, void* v);
	static void cb_attenuationQuadraticSlides(Fl_Widget* o, void* v);
	static void cb_ambientLightSlides(Fl_Widget* o, void* v);
	static void cb_thresholdSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);

	static void cb_useBackground(Fl_Widget* o, void* v);

	static void cb_depthField(Fl_Widget* o, void* v);
	static void cb_softShadow(Fl_Widget* o, void* v);
	static void cb_motionBlur(Fl_Widget* o, void* v);
	static void cb_glossyReflection(Fl_Widget* o, void* v);
};

#endif
