// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void initializeGL();
    virtual void paintGL();
    virtual void modelTransformMorty ();
    virtual void modelTransformFantasma (float posX);
    virtual void modelTransformDiscoBall ();

    void carregaShaders ();
    void iniFocus();

  private:
    int printOglError(const char file[], int line, const char func[]);
    float graus = 0.0;
    float rotMF = 0.0;

    // uniform locations
    GLuint colLoc;
    
};
