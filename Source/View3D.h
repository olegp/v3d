#pragma once

// by Oleg Podsechin
// when using this class in your project make sure you include
// the following libraries: opengl32.lib, glu32.lib 

#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Math3D.h"


// this class describes the input controller
// which allows the View3D to rotate the scene based
// on mouse input
class TrackBall
{
 	BOOL LeftButtonDown, RightButtonDown;
	Vect3 LastTransform, CurrentTransform;

	POINT LastPoint;

  HWND hWnd;

public:

  Vect3 GetTransform() { return CurrentTransform; }
  
  void SetTransform(const Vect3& transform) {
    CurrentTransform = transform;
  }

  Mat4 GetMatrix();

  // call this to attach the TrackBall to a window
  void SetWindow(HWND hWnd) { this->hWnd = hWnd; }

  // change these values to change the speed with which
  // the TrackBall zooms in and out and rotates the view
  // the default values are 0.5, 0.5, 1.0 and 20.0
  float XFactor, YFactor, ZFactor, WFactor;

  TrackBall() : LastTransform(Vect3(0.0f, 0.0f, -50.0f)), 
    CurrentTransform(LastTransform), LeftButtonDown(FALSE), RightButtonDown(FALSE),
    XFactor(0.5f), YFactor(0.5f), ZFactor(0.1f), WFactor(20.0f)
  {
  }

  TrackBall(const Vect3& transform) : LastTransform(transform),
    CurrentTransform(LastTransform), LeftButtonDown(FALSE), RightButtonDown(FALSE),
    XFactor(0.5f), YFactor(0.5f), ZFactor(0.1f), WFactor(20.0f)
  {
  }

  void OnMouseMove(int x, int y);
  void OnLButtonUp(int x, int y);
  void OnLButtonDown(int x, int y);
  void OnRButtonUp(int x, int y);
  void OnRButtonDown(int x, int y);
  void OnMouseWheel(short zDelta);
};


// this class describes the 3D view
class View3D
{
protected:

  friend class Font2D;
  friend class Font3D;

  HWND hWnd;
	HDC hDC;
	HGLRC hGLRC;


  TrackBall trackball;

  static int viewcount;
  static View3D* views[32];
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  // this is an index into views[]
  int viewnumber;

  // functions for managing multiple views
  bool AddView();
  void RemoveView();
  static View3D* GetView(HWND hWnd);

public:

  View3D();
  ~View3D();

  TrackBall GetTrackBall() { return trackball; }

  // creates a View3D from an HINSTANCE
  virtual bool Create(HINSTANCE hInstance);
  
  // creates a View3D from a window handle (HWND)
  // you must respons to windows messages yourself if you create the view
  // with this function
  virtual bool Create(HWND hWnd);
  
  // cleans up and frees the OpenGL context
  // called internally by the destructor
  virtual void Destroy();

  // returns the handle of the window the view renders to
  HWND GetWindow();
  
  // sets the default OpenGL states for this context
  // override if you want to change them
  virtual void SetDefaultStates();

  // processes a windows message
  // the messages processed are WM_SIZE, WM_PAINT and WM_DESTROY
  // this function is called only if the view is created with HINSTANCE
  // if you create View3D with HWND, you'll need to respond to these messages
  // yourself
  virtual int OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  
  // must be called when the window is resized
  // this is called internally by WndProc if the Create 
  // function was called with HINSTANCE
  // if you create View3D with HWND, you'll need to call this yourself
  virtual void OnResize(int width, int height);

  // must be called at the beginning of RenderScene
  virtual void BeginScene();

  // must be called at the end of RenderScene
  virtual void EndScene();

  // override this to render your own scenes
  virtual void RenderScene();
  
  // sets the camera position
  void SetView(const Mat4& view);

  // sets the projection matrix
  // for more info see Math3D::Perspective and Math3D::Ortho
  void SetProjection(const Mat4& projection);

  // sets the current coordinate transformation matrix
  // can affect rotation/translation/scaling
  void SetPosition(const Mat4& position);

  // sets the current current coordinate transformation vector
  // can affect the tranlation only
  void SetPosition(const Vect3& position);

  // sets the current color
  // 0.0f - minimum intensity (0)
  // 1.0f - maximum intensity (255)
  void SetColor(float r, float g, float b, float a = 1.0f);
  


  // these functions redirect the commands to TrackBall
  // if you haven't created the View3D with Create(HINSTANCE)
  // you will need to call these functions yourself
  virtual void OnMouseMove(int x, int y);
  virtual void OnLButtonUp(int x, int y);
  virtual void OnLButtonDown(int x, int y);
  virtual void OnRButtonUp(int x, int y);
  virtual void OnRButtonDown(int x, int y);
  virtual void OnMouseWheel(short zDelta);
};

// for more information on fonts in OpenGL check out
// http://www.opengl.org/developers/code/features/fontsurvey/

class Font2D
{
  int listbase;

public:
  
  // creates a bitmap font from system font
  // returns true if successful
  bool Create(View3D *view);

  // creates a bitmap font from the specified font
  bool Create(HFONT hFont, View3D *view);

  // draws text on the screen in the specified font
  void DrawString(Vect3& position, const char *string);

  void Destroy();

  Font2D();
  Font2D(View3D *view);
  Font2D(HFONT hFont, View3D *view);

  ~Font2D();
};


class Font3D
{
  int listbase;

public:
  
  // creates a 3d mesh font from system font
  bool Create(View3D *view);
  bool Create(HFONT hFont, View3D *view);

  // draws text on the screen in the specified font
  void DrawString(const char *string);

  void Destroy();

  Font3D();
  Font3D(View3D *view);
  Font3D(HFONT hFont, View3D *view);

  ~Font3D();

};


class Visual
{
protected:
  int list;

public:

  virtual void Render();

  virtual void Destroy();
  
  virtual ~Visual();
};

class Grid : public Visual
{
public:

  void Create(int squarecount, float squaresize);

  // draws a square grid
  // the object is drawn in the XZ plane
  // use SetPosition/Math3D::xyzRot or glRotate to rotate it
  static void Render(int squarecount, float squaresize);

  Grid(int squarecount, float squaresize) {
    Create(squarecount, squaresize);
  }
};

class Box : public Visual
{
public:

  void Create(float xscale, float yscale, float zscale);

  // draws a box
  static void Render(float xscale, float yscale, float zscale);

  Box(float xscale, float yscale, float zscale) {
    Create(xscale, yscale, zscale);
  }
};

class Plane : public Visual
{
  void Create(float xscale, float yscale);

  // draws a box
  static void Render(float xscale, float yscale);

  Plane(float xscale, float yscale) {
    Create(xscale, yscale);
  }

};

class Sphere : public Visual
{
public:

  void Create(float radius, int resolution);

  // draws a sphere
  static void Render(float radius, int resolution = 16);

  Sphere(float radius, int resolution) {
    Create(radius, resolution);
  }
};

class Cylinder : public Visual
{
public:
  void Create(float radiusa, float radiusb, float height, int resolution);

  // draws a cylinder
  // draw a cone by setting radiusb to 0
  static void Render(float radiusa, float radiusb, float height, int resolution);

  Cylinder(float radiusa, float radiusb, float height, int resolution) {
    Create(radiusa, radiusb, height, resolution);
  }
};



