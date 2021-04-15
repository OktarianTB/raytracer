//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"

static bool done;
char* TraceUI::scenePath = nullptr;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );
	scenePath = newfile;

	if (newfile != NULL) {
		char buf[256];

		pUI->raytracer->backgroundTexturePtr = pUI->backgroundTexturePtr;
		pUI->raytracer->useBackground = pUI->m_nUseBackground;

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}

void TraceUI::cb_load_background(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);
	char* filename = fl_file_chooser("Load Background Image", "*.bmp", nullptr);

	if (filename != nullptr)
	{
		delete[] pUI->backgroundTexturePtr;
		pUI->backgroundTexturePtr = new Texture(filename);

		fl_alert("Make sure that 'Use Background' is selected in the settings now!");
	}
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_attenuationConstantSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAttenuationConstant = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_attenuationLinearSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAttenuationLinear = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_attenuationQuadraticSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAttenuationQuadratic = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_ambientLightSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAmbientLight = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_thresholdSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nThreshold = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_useBackground(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nUseBackground = bool(((Fl_Check_Button*)o)->value());
}



void TraceUI::cb_softShadow(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nSoftShadow = bool(((Fl_Check_Button*)o)->value());
}

void TraceUI::cb_motionBlur(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nMotionBlur = bool(((Fl_Check_Button*)o)->value());
}

void TraceUI::cb_glossyReflection(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nGlossyReflection = bool(((Fl_Check_Button*)o)->value());
}

void TraceUI::cb_depthField(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepthField = bool(((Fl_Check_Button*)o)->value());
}

void TraceUI::cb_focalLengthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nFocalLength = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_apertureSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAperture = double(((Fl_Slider*)o)->value());
}



void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );
		pUI->m_traceGlWindow->show();
		pUI->raytracer->traceSetup(width, height);

		// CUSTOM ADDED BY OKTO
		pUI->raytracer->max_depth = pUI->m_nDepth;
		pUI->raytracer->getScene()->distAttenConstCoeff = pUI->m_nAttenuationConstant;
		pUI->raytracer->getScene()->distAttenLinearCoeff = pUI->m_nAttenuationLinear;
		pUI->raytracer->getScene()->distAttenQuadraticCoeff = pUI->m_nAttenuationQuadratic;
		pUI->raytracer->getScene()->threshold = pUI->m_nThreshold;

		pUI->raytracer->backgroundTexturePtr = pUI->backgroundTexturePtr;
		pUI->raytracer->useBackground = pUI->m_nUseBackground;

		// CUSTOM ADDED BY ALUA
		pUI->raytracer->getScene()->softShadow = pUI->m_nSoftShadow;
		pUI->raytracer->getScene()->motionBlur = pUI->m_nMotionBlur;
		pUI->raytracer->getScene()->glossyReflection = pUI->m_nGlossyReflection;
		pUI->raytracer->getScene()->depthField = pUI->m_nDepthField;

		pUI->raytracer->getScene()->focalLength = pUI->m_nFocalLength;
		pUI->raytracer->getScene()->aperture = pUI->m_nAperture;
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

double TraceUI::getAttenuationConstant()
{
	return m_nAttenuationConstant;
}

double TraceUI::getAttenuationLinear()
{
	return m_nAttenuationLinear;
}

double TraceUI::getAttenuationQuadratic()
{
	return m_nAttenuationQuadratic;
}

double TraceUI::getAmbientLight()
{
	return m_nAmbientLight;
}

double TraceUI::getThreshold()
{
	return m_nThreshold;
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Load Background...",	FL_ALT + 'b', (Fl_Callback*)TraceUI::cb_load_background },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 250;
	m_nAttenuationConstant = 0.0;
	m_nAttenuationLinear = 0.0;
	m_nAttenuationQuadratic = 0.0;
	m_nAmbientLight = 0.2;
	m_nThreshold = 0.0;

	m_mainWindow = new Fl_Window(100, 40, 365, 400, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 320, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);

		// install slider attenuation constant
		m_attenuationConstantSlider = new Fl_Value_Slider(10, 80, 180, 20, "Attenuation Constant");
		m_attenuationConstantSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_attenuationConstantSlider->type(FL_HOR_NICE_SLIDER);
		m_attenuationConstantSlider->labelfont(FL_COURIER);
		m_attenuationConstantSlider->labelsize(12);
		m_attenuationConstantSlider->minimum(0);
		m_attenuationConstantSlider->maximum(1);
		m_attenuationConstantSlider->step(0.01);
		m_attenuationConstantSlider->value(m_nAttenuationConstant);
		m_attenuationConstantSlider->align(FL_ALIGN_RIGHT);
		m_attenuationConstantSlider->callback(cb_attenuationConstantSlides);

		// install slider attenuation linear
		m_attenuationLinearSlider = new Fl_Value_Slider(10, 105, 180, 20, "Attenuation Linear");
		m_attenuationLinearSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_attenuationLinearSlider->type(FL_HOR_NICE_SLIDER);
		m_attenuationLinearSlider->labelfont(FL_COURIER);
		m_attenuationLinearSlider->labelsize(12);
		m_attenuationLinearSlider->minimum(0);
		m_attenuationLinearSlider->maximum(1);
		m_attenuationLinearSlider->step(0.01);
		m_attenuationLinearSlider->value(m_nAttenuationLinear);
		m_attenuationLinearSlider->align(FL_ALIGN_RIGHT);
		m_attenuationLinearSlider->callback(cb_attenuationLinearSlides);

		// install slider attenuation quadratic
		m_attenuationQuadraticSlider = new Fl_Value_Slider(10, 130, 180, 20, "Attenuation Quadratic");
		m_attenuationQuadraticSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_attenuationQuadraticSlider->type(FL_HOR_NICE_SLIDER);
		m_attenuationQuadraticSlider->labelfont(FL_COURIER);
		m_attenuationQuadraticSlider->labelsize(12);
		m_attenuationQuadraticSlider->minimum(0);
		m_attenuationQuadraticSlider->maximum(1);
		m_attenuationQuadraticSlider->step(0.01);
		m_attenuationQuadraticSlider->value(m_nAttenuationQuadratic);
		m_attenuationQuadraticSlider->align(FL_ALIGN_RIGHT);
		m_attenuationQuadraticSlider->callback(cb_attenuationQuadraticSlides);

		// install slider ambient light
		m_ambientLightSlider = new Fl_Value_Slider(10, 155, 180, 20, "Ambient Light");
		m_ambientLightSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ambientLightSlider->type(FL_HOR_NICE_SLIDER);
		m_ambientLightSlider->labelfont(FL_COURIER);
		m_ambientLightSlider->labelsize(12);
		m_ambientLightSlider->minimum(0);
		m_ambientLightSlider->maximum(1);
		m_ambientLightSlider->step(0.01);
		m_ambientLightSlider->value(m_nAmbientLight);
		m_ambientLightSlider->align(FL_ALIGN_RIGHT);
		m_ambientLightSlider->callback(cb_ambientLightSlides);

		// install slider threshold
		m_ThresholdSlider = new Fl_Value_Slider(10, 180, 180, 20, "Threshold");
		m_ThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ThresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_ThresholdSlider->labelfont(FL_COURIER);
		m_ThresholdSlider->labelsize(12);
		m_ThresholdSlider->minimum(0);
		m_ThresholdSlider->maximum(30);
		m_ThresholdSlider->step(0.01);
		m_ThresholdSlider->value(m_nThreshold);
		m_ThresholdSlider->align(FL_ALIGN_RIGHT);
		m_ThresholdSlider->callback(cb_thresholdSlides);

		// install slider focal length
		m_ThresholdSlider = new Fl_Value_Slider(150, 245, 120, 20, "Focal length");
		m_ThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ThresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_ThresholdSlider->labelfont(FL_COURIER);
		m_ThresholdSlider->labelsize(12);
		m_ThresholdSlider->minimum(0);
		m_ThresholdSlider->maximum(5);
		m_ThresholdSlider->step(0.01);
		m_ThresholdSlider->value(m_nFocalLength);
		m_ThresholdSlider->align(FL_ALIGN_RIGHT);
		m_ThresholdSlider->callback(cb_focalLengthSlides);

		// install slider aperture
		m_ThresholdSlider = new Fl_Value_Slider(150, 270, 120, 20, "Aperture");
		m_ThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_ThresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_ThresholdSlider->labelfont(FL_COURIER);
		m_ThresholdSlider->labelsize(12);
		m_ThresholdSlider->minimum(0);
		m_ThresholdSlider->maximum(1);
		m_ThresholdSlider->step(0.01);
		m_ThresholdSlider->value(m_nAperture);
		m_ThresholdSlider->align(FL_ALIGN_RIGHT);
		m_ThresholdSlider->callback(cb_apertureSlides);

		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_softShadowButton = new Fl_Check_Button(10, 205, 100, 25, "&Soft Shadow");
		m_softShadowButton->user_data((void*)(this));
		m_softShadowButton->value(false);
		m_softShadowButton->callback(cb_softShadow);

		m_glossyReflectionButton = new Fl_Check_Button(150, 205, 100, 25, "&Glossy Reflection");
		m_glossyReflectionButton->user_data((void*)(this));
		m_glossyReflectionButton->value(false);
		m_glossyReflectionButton->callback(cb_glossyReflection);

		m_motionBlurButton = new Fl_Check_Button(10, 225, 100, 25, "&Motion Blur");
		m_motionBlurButton->user_data((void*)(this));
		m_motionBlurButton->value(false);
		m_motionBlurButton->callback(cb_motionBlur);

		m_depthFieldButton = new Fl_Check_Button(10, 245, 100, 25, "&Depth of Field");
		m_depthFieldButton->user_data((void*)(this));
		m_depthFieldButton->value(false);
		m_depthFieldButton->callback(cb_depthField);

		m_useBackgroundButton = new Fl_Check_Button(10, 305, 100, 25, "&Use Background Image");
		m_useBackgroundButton->user_data((void*)(this));
		m_useBackgroundButton->value(false);
		m_useBackgroundButton->callback(cb_useBackground);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);

    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}