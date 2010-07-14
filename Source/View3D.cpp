// by Oleg Podsechin
#include "View3D.h"

int View3D::viewcount = 0;
View3D* View3D::views[32];


bool View3D::AddView()
{
  int i;
  for(i = 0; i < viewcount && views[i] != NULL; i ++);

  if(i < sizeof(views)/sizeof(views[0])) {
    viewnumber = i;
    views[i] = this;
    
    if(i == viewcount) viewcount ++;
    return true;
  }

  return false;
}

void View3D::RemoveView()
{
  views[viewnumber] = NULL;
  viewnumber = -1;
}

View3D* View3D::GetView(HWND hWnd)
{ 
  for(int i = 0; i < viewcount; i ++) {
    if(views[i] != NULL) {
      if(views[i]->GetWindow() != NULL) {
        if(views[i]->GetWindow() == hWnd)
          return views[i];
      }
    }
  }
  
  return NULL;
}


View3D::View3D() : hWnd(NULL), hDC(NULL), hGLRC(NULL), viewnumber(-1)
{

}

View3D::~View3D() {
  Destroy();
}

LRESULT CALLBACK View3D::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  View3D *view = GetView(hWnd);
  if(view != NULL)
    return view->OnMessage(uMsg, wParam, lParam);
  else
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


bool View3D::Create(HINSTANCE hInstance) 
{
  const char Name[] = "View3D";

  WNDCLASS wndClass = 
  { 
		CS_HREDRAW | CS_VREDRAW, 
    WndProc, 0, 0, hInstance,
    NULL, //LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON)),
    NULL, //LoadCursor(NULL, IDC_ARROW), 
    NULL, //(HBRUSH)GetStockObject(BLACK_BRUSH),
    NULL,
    Name 
	};
  
  RegisterClass(&wndClass);

  hWnd = CreateWindow(Name, Name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,						 
		0L, 0L, hInstance, 0L );

  if(hWnd != NULL && Create(hWnd)) {

    UpdateWindow(hWnd);
    ShowWindow(hWnd, SW_SHOWNORMAL);

    return true;
  }

  return false;
}

bool View3D::Create(HWND hWnd)
{
	hDC = GetDC(hWnd);

  BYTE colorbits = 32, zbits = 32;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  
		1,                            
		PFD_DRAW_TO_WINDOW |            
		PFD_SUPPORT_OPENGL |			
		PFD_DOUBLEBUFFER,               
		PFD_TYPE_RGBA,                  
		colorbits,                             
		0,0,0,0,0,0,                    
		0,0,                            
		0,0,0,0,0,                      
		zbits,                             
		0,                              
		0,                              
		PFD_MAIN_PLANE,                 
		0,                              
		0,0,0 
  };                        

	int nPixelFormat = ChoosePixelFormat(hDC, &pfd);

  if (nPixelFormat == 0) {
    // ChoosePixelFormat() failed
	  return false;
  }

  if (SetPixelFormat(hDC, nPixelFormat, &pfd) == FALSE) {
    // SetPixelFormat() failed
	  return false;
  }

  hGLRC = wglCreateContext(hDC);
  if(hGLRC == NULL) {
    // wglCreateContext() failed
	  return false;
  }

  if(wglMakeCurrent(hDC, hGLRC) == FALSE) {
    // wglMakeCurrent() failed
    return false;
  }

  SetDefaultStates();
  trackball.SetWindow(hWnd);

  AddView();
  return true;
}

void View3D::Destroy()
{
	wglMakeCurrent(NULL, NULL);
  if(hGLRC != NULL) {
    wglDeleteContext(hGLRC);
    hGLRC = NULL;
  }

  if(hWnd != NULL && hDC != NULL)
    ReleaseDC(hWnd, hDC);

  hDC = NULL;
  hWnd = NULL;

  trackball.SetWindow(NULL);

  RemoveView();
}

HWND View3D::GetWindow()
{
  return hWnd;
}

void View3D::SetDefaultStates()
{
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_DITHER);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);

  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_FOG_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glShadeModel(GL_SMOOTH);

  glDisable(GL_BLEND);

  glClearColor(0, 0, 0, 0);
  glClearDepth(1);

  glEnable(GL_COLOR_MATERIAL);
}


int View3D::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg) {
    case WM_MOUSEMOVE:
      OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

    case WM_LBUTTONUP:
      OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

    case WM_LBUTTONDOWN:
      OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

    case WM_RBUTTONUP:
      OnRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

    case WM_RBUTTONDOWN:
      OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      return 0;

//    case WM_MOUSEWHEEL:
//      OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
//      return 0;

    case WM_SIZE:
      OnResize(LOWORD(lParam), HIWORD(lParam));
      return 0;

//    case WM_PAINT:
//      RenderScene();
//      return 0;

    case WM_DESTROY:
      Destroy();
      return 0;
  }

  return (int)DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void View3D::OnResize(int width, int height)
{
  if(wglMakeCurrent(hDC, hGLRC) != FALSE) {
    glViewport(0, 0, width, height);
    SetProjection(Math3D::Perspective(PI/2.0f, 1.0f, 1000.0f, (float)height/(float)width));
  }
}

void View3D::BeginScene()
{
  if(wglMakeCurrent(hDC, hGLRC) != FALSE)
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void View3D::EndScene()
{
	glFlush();
	SwapBuffers(hDC);
}

void View3D::RenderScene()
{
  BeginScene();
  EndScene();
}

void View3D::SetView(const Mat4& view)
{

}


void View3D::SetProjection(const Mat4& projection)
{
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(&projection._11);
  glMatrixMode(GL_MODELVIEW);
}

void View3D::SetPosition(const Mat4& position)
{
  glLoadMatrixf(&position._11);
}

void View3D::SetPosition(const Vect3& position)
{
  Mat4 transform(Mat4::Identity());
  transform.SetPos(position);
  glLoadMatrixf(&transform._11);
}

void View3D::SetColor(float r, float g, float b, float a)
{
  glColor4f(r, g, b, a);
}


// redirect the messages to the trackball

void View3D::OnMouseMove(int x, int y)
{
  trackball.OnMouseMove(x, y);
}

void View3D::OnLButtonUp(int x, int y)
{
  trackball.OnLButtonUp(x, y);
}

void View3D::OnLButtonDown(int x, int y)
{
  trackball.OnLButtonDown(x, y);
}

void View3D::OnRButtonUp(int x, int y)
{
  trackball.OnRButtonUp(x, y);
}

void View3D::OnRButtonDown(int x, int y)
{
  trackball.OnRButtonDown(x, y);
}

void View3D::OnMouseWheel(short zDelta)
{
  trackball.OnMouseWheel(zDelta);
}


//*** TrackBall

void TrackBall::OnMouseMove(int x, int y) 
{
	if(LeftButtonDown) {
		CurrentTransform.x = LastTransform.x - float(y - LastPoint.y) * XFactor;
		CurrentTransform.y = LastTransform.y + float(x - LastPoint.x) * YFactor;
			
	} else if(RightButtonDown) {
		CurrentTransform.z = LastTransform.z - float(y - LastPoint.y) * ZFactor;
	}
}

void TrackBall::OnLButtonUp(int x, int y) 
{
	LeftButtonDown = FALSE;
	
	ReleaseCapture();
	ShowCursor(TRUE);

	LastTransform = CurrentTransform;
}

void TrackBall::OnLButtonDown(int x, int y) 
{
 	LeftButtonDown = TRUE;
	RightButtonDown = FALSE;

	LastPoint.x = x;
	LastPoint.y = y;
	
  if(hWnd) {
	  SetCapture(hWnd);
    // SetCursor(NULL);
  }
}

void TrackBall::OnRButtonUp(int x, int y) 
{
	RightButtonDown = FALSE;
	
	ReleaseCapture();
	ShowCursor(TRUE);
	
	LastTransform = CurrentTransform;
}

void TrackBall::OnRButtonDown(int x, int y) 
{
	RightButtonDown = TRUE;
	LeftButtonDown = FALSE;

	LastPoint.x = x;
	LastPoint.y = y;

  if(hWnd) { 
    SetCapture(hWnd);
    // SetCursor(NULL);
  }
}

#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif

void TrackBall::OnMouseWheel(short zDelta)
{
  CurrentTransform.z += (float(zDelta)/float(WHEEL_DELTA)) * WFactor;
  LastTransform.z = CurrentTransform.z;
}

Mat4 TrackBall::GetMatrix()
{
  Mat4 transform = Math3D::xyzRot(CurrentTransform.x, CurrentTransform.y, 0);
	transform.SetPos(*((Vect3 *)&transform._31) * float(CurrentTransform.z));
  return transform;
}


// Fonts


bool Font2D::Create(View3D *view)
{
  return Create((HFONT)GetStockObject(SYSTEM_FONT), view);
}

bool Font2D::Create(HFONT hFont, View3D *view)
{
  if(listbase != -1) Destroy();

  // make it the calling thread's current rendering context 
  wglMakeCurrent (view->hDC, view->hGLRC); 
   
  // make the system font the device context's selected font 
  HFONT hOldFont = (HFONT)SelectObject (view->hDC, hFont); 
   
  listbase = glGenLists(256);
  wglUseFontBitmaps (view->hDC, 0, 256, (GLsizei)listbase);

  SelectObject(view->hDC, hOldFont);
  return true;
}

  
void Font2D::DrawString(Vect3& position, const char *string)
{
  if(listbase != -1) {
    glRasterPos3f(position.x, position.y, position.z);

    glListBase(listbase); 
    // now draw the characters in a string 
    glCallLists((GLsizei)strlen(string), GL_UNSIGNED_BYTE, string); 
  }
}

void Font2D::Destroy()
{
  if(listbase != -1) {
    glDeleteLists(listbase, 256);
    listbase = -1;
  }
}

Font2D::Font2D() : listbase(-1)
{
  
}

Font2D::Font2D(View3D *view) : listbase(-1)
{
  Create(view);
}

Font2D::Font2D(HFONT hFont, View3D *view) : listbase(-1)
{
  Create(hFont, view); 
}

Font2D::~Font2D()
{
  Destroy();
}

bool Font3D::Create(View3D *view)
{
  return Create((HFONT)GetStockObject(SYSTEM_FONT), view);
}

bool Font3D::Create(HFONT hFont, View3D *view)
{
  if(listbase != -1) Destroy();

  // make it the calling thread's current rendering context 
  wglMakeCurrent (view->hDC, view->hGLRC); 
   
  // make the system font the device context's selected font 
  HFONT hOldFont = (HFONT)SelectObject (view->hDC, hFont); 
   
  GLYPHMETRICSFLOAT agmf[256];
  listbase = glGenLists(256);

  wglUseFontOutlines(view->hDC, 0, 256, listbase, 0.0f, 0.0f, WGL_FONT_POLYGONS, agmf);
  SelectObject(view->hDC, hOldFont);
  return true;
}

  
void Font3D::DrawString(const char *string)
{
  if(listbase != -1) {
    glListBase (listbase); 
    // now draw the characters in a string 
    glCallLists((GLsizei)strlen(string), GL_UNSIGNED_BYTE, string); 
  }
}

void Font3D::Destroy()
{
  if(listbase != -1) {
    glDeleteLists(listbase, 256);
    listbase = -1;
  }
}

Font3D::Font3D() : listbase(-1)
{
  
}

Font3D::Font3D(View3D *view) : listbase(-1)
{
  Create(view);
}

Font3D::Font3D(HFONT hFont, View3D *view) : listbase(-1)
{
  Create(hFont, view); 
}

Font3D::~Font3D()
{
  Destroy();
}

// Visuals

void Visual::Destroy()
{
  if(list != -1) {
    glDeleteLists(list, 1);
    list = -1;
  }
}

void Visual::Render() {
  if(list != -1)
    glCallList(list);
}

Visual::~Visual() {
  Destroy();
}

void Grid::Create(int squarecount, float squaresize)
{
  if(list == -1) Destroy();
  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  Render(squarecount, squaresize);
  glEndList();
}

void Grid::Render(int squarecount, float squaresize)
{
  float p = (float)squarecount * squaresize * 0.5f;

  glBegin(GL_LINES);

  for(int i = 0; i <= squarecount; i ++) {
    glVertex3f(-p + (float)i * squaresize, 0, -p);
    glVertex3f(-p + (float)i * squaresize, 0, p);

    glVertex3f(-p, 0, -p + (float)i * squaresize);
    glVertex3f(p, 0, -p + (float)i * squaresize);
  }

  glEnd();
}

void Box::Create(float xscale, float yscale, float zscale)
{
  if(list == -1) Destroy();
  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  Render(xscale, yscale, zscale);
  glEndList();
}

void Box::Render(float xscale, float yscale, float zscale)
{
  float a = xscale * 0.5f, b = yscale * 0.5f, c = zscale * 0.5f;

  glBegin(GL_QUADS);

  // top and bottom
  glNormal3f(0, 1, 0);

  glVertex3f(-a, b, -c);
  glVertex3f(-a, b, c);
  glVertex3f(a, b, c);
  glVertex3f(a, b, -c);

  glNormal3f(0, -1, 0);

  glVertex3f(a, -b, -c);
  glVertex3f(a, -b, c);
  glVertex3f(-a, -b, c);
  glVertex3f(-a, -b, -c);

  // right and left
  glNormal3f(0, 1, 0);

  glVertex3f(a, b, -c);
  glVertex3f(a, b, c);
  glVertex3f(a, -b, c);
  glVertex3f(a, -b, -c);

  glNormal3f(0, -1, 0);
 
  glVertex3f(-a, -b, -c);
  glVertex3f(-a, -b, c);
  glVertex3f(-a, b, c);
  glVertex3f(-a, b, -c);

  // back and front

  glNormal3f(0, 0, 1);

  glVertex3f(a, -b, c);
  glVertex3f(a, b, c);
  glVertex3f(-a, b, c);
  glVertex3f(-a, -b, c);

  glNormal3f(0, 0, -1);
 
  glVertex3f(-a, -b, -c);
  glVertex3f(-a, b, -c);
  glVertex3f(a, b, -c);
  glVertex3f(a, -b, -c);

  glEnd();
}

void Plane::Create(float xscale, float zscale)
{
  if(list == -1) Destroy();
  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  Render(xscale, zscale);
  glEndList();
}

void Plane::Render(float xscale, float zscale) 
{
  float a = xscale * 0.5f, b = zscale * 0.5f;

  glBegin(GL_QUADS);

  glNormal3f(0, 1, 0);

  glVertex3f(-a, 0, -b);
  glVertex3f(-a, 0, b);
  glVertex3f(a, 0, b);
  glVertex3f(a, 0, -b);

  glNormal3f(0, -1, 0);

  glVertex3f(a, 0, -b);
  glVertex3f(a, 0, b);
  glVertex3f(-a, 0, b);
  glVertex3f(-a, 0, -b);

  glEnd();  
}

void Sphere::Create(float radius, int resolution)
{
  if(list == -1) Destroy();
  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  Render(radius, resolution);
  glEndList();
}

void Sphere::Render(float radius, int resolution)
{
  GLUquadricObj *q = gluNewQuadric();
  gluQuadricNormals(q, GLU_SMOOTH);
  gluSphere(q, radius, resolution, resolution * 2);
  gluDeleteQuadric(q);
}

void Cylinder::Create(float radiusa, float radiusb, float height, int resolution)
{
  if(list == -1) Destroy();
  list = glGenLists(1);
  glNewList(list, GL_COMPILE);
  Render(radiusa, radiusb, height, resolution);
  glEndList();
}

void Cylinder::Render(float radiusa, float radiusb, float height, int resolution)
{
  GLUquadricObj *q = gluNewQuadric();
  gluQuadricNormals(q, GLU_SMOOTH);
  gluCylinder(q, radiusa, radiusb, height, resolution, radiusa == radiusb ? 1 : resolution);
  gluDeleteQuadric(q);

  // draw the caps
  // remove these lines if you don't want the cylinders to be capped
  GLUquadricObj *q2 = gluNewQuadric();
  gluQuadricNormals(q2, GLU_SMOOTH);
  gluDisk(q2, 0, radiusa, resolution, 1);
  gluDeleteQuadric(q2);

  if(radiusb > 0) {
    glTranslatef(0, 0, height);

    GLUquadricObj *q2 = gluNewQuadric();
    gluQuadricNormals(q2, GLU_SMOOTH);
    gluDisk(q2, 0, radiusb, resolution, 1);
    gluDeleteQuadric(q2);

    glTranslatef(0, 0, -height);
  }
}


