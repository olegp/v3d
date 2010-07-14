// by Oleg Podsechin

#include "View3D.h"

#include <time.h> // this is here only for the animation

// an example of how class View3D may be used

class MyView : public View3D {

  Font2D *font2;
  Font3D *font3;
  Cylinder *visual;

public:

  void SetDefaultStates() {
    // set the default states
    View3D::SetDefaultStates();

    // override the backgroudn color
    glClearColor(0.5, 0.5, 0.5, 1);

    // add a light, because View3D doesn't add one for us
    float position[] = {1, 1, 1, 0};
    float ambient[] = {0.5, 0.5, 0.5, 1};
    float diffuse[] = {1, 1, 1, 1};

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, diffuse);
    glEnable(GL_LIGHT0);

    const char *fontname = "Arial";

    LOGFONT logfont;

    logfont.lfHeight        = -12;
    logfont.lfWidth         = 0;
    logfont.lfEscapement    = 0;
    logfont.lfOrientation   = logfont.lfEscapement;
    logfont.lfWeight        = FW_NORMAL;
    logfont.lfItalic        = FALSE;
    logfont.lfUnderline     = FALSE;
    logfont.lfStrikeOut     = FALSE;
    logfont.lfCharSet       = ANSI_CHARSET;
    logfont.lfOutPrecision  = OUT_DEFAULT_PRECIS;
    logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logfont.lfQuality       = DEFAULT_QUALITY;
    logfont.lfPitchAndFamily = FF_DONTCARE|DEFAULT_PITCH;
    strcpy(logfont.lfFaceName, fontname);

    HFONT hFont = CreateFontIndirect(&logfont);

    // let's create a 2D bitmapped font which always faces the viewer
    font2 = new Font2D(hFont, this);

    // let's create a 3D font which moves with other geometry in the scene
    font3 = new Font3D(hFont, this);
    
    visual = new Cylinder(4, 4, 6, 8);

    DeleteObject(hFont);
  }

  void StartTimer() {
    SetTimer(hWnd, 1, 50, NULL);

  }

  int OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
      case WM_TIMER:
        RenderScene();
        return 0;
    }

    return View3D::OnMessage(uMsg, wParam, lParam);
  }

  void RenderScene() {
    BeginScene();

    Mat4 cam2world = trackball.GetMatrix(), world2cam;
    Mat4::InverseOrthogonal(cam2world, world2cam);
    SetPosition(world2cam); //== glTranslatef(...);

    glDisable(GL_LIGHTING);

    SetColor(0, 0, 0); //== glColor3f(...);
    Grid::Render(10, 2.f);

    glEnable(GL_LIGHTING);

    // draw something animated

    int count = 8;
    float delta = 2.0f, size = 2.0f;
    float set = (delta * (count -1)) * 0.5f;
    int t = (int)clock();

    for(int i = 0; i < count; i ++) {
      for(int j = 0; j < count; j ++) {
        float v = sinf((float)(t * (i + 1) * (j + 1)) * 0.0001f);
        float h = 3.0f + 2.0f * v;

        glPushMatrix();
        Vect3 p(-set + delta * (float)i, h * 0.5f, -set + delta * (float)j);
        glTranslatef(p.x, p.y, p.z);

        float c = (v * 0.5f + 0.5f);
        SetColor(c, 0.2f, 1.0f - c);
        Box::Render(size, h, size);
        glPopMatrix();
      }
    }

    glPushMatrix();
    glTranslatef(0, 10, 0);
    glColor3f(1, 1, 0);
    Sphere::Render(4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 10, 5);
    glColor3f(1, 0, 0);
    visual->Visual::Render();
    
    // the method below produces the same results but is slower
    // if at all possible (that is if your Visuals are static and don't change with time)
    // try to Create the Visuals only once, the method below creates a new Cylinder every frame
    // Cylinder::Render(4, 4, 6, 8); 
    glPopMatrix();


    glPushMatrix();

    glDisable(GL_LIGHTING);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST); // draw the text on top of everything
    glColor3f(0, 0, 1); // set the text color

    font2->DrawString(Vect3(10, 0, 10), "bitmap font");

    glTranslatef(-15, 20, 0);
    glScalef(10, 10, 10);
    font3->DrawString("outline font");

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    EndScene();
  }

  void Destroy() {

    delete visual;

    delete font2;
    delete font3;

    KillTimer(hWnd, 1);

    // we are the only window in the application -> exit
    PostQuitMessage(0);

    // call the underlying Destroy method
    View3D::Destroy();
  }
};



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  MyView view1;

  if(!view1.Create(hInstance)) return FALSE;

  // set the size we want
  SetWindowPos(view1.GetWindow(), NULL, 0, 0, 512, 384, SWP_NOMOVE | SWP_NOZORDER);
  view1.StartTimer();

  // remove the comments to display a second window
  MyView view2;

  if(!view2.Create(hInstance)) return FALSE;

  // set the size we want
  SetWindowPos(view2.GetWindow(), NULL, 0, 0, 512, 384, SWP_NOMOVE | SWP_NOZORDER);
  view2.StartTimer();
  

 	MSG msg;
  while(GetMessage(&msg, NULL, 0, 0 ) > 0) { 
    TranslateMessage(&msg); 
    DispatchMessage(&msg); 
  } 
 
  // return the exit code to the system. 
  return (int)msg.wParam; 
}
