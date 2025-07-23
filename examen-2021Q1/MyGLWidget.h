#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
  // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void resizeGL (int width, int height);

  private:
    int printOglError(const char file[], int line, const char func[]);
    float angleP, angleC1, angleC2, angleC3;
    glm::vec3 colFoc;
    bool pintar, color, camPlanta;
    float l,r,b,t;

  public slots:
    void canviCamera();
    void posP(int n);

  signals:
    void canviCam2(bool);
    void resP(int);

};
