#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void modelTransformPatricio ();
    virtual void enviaPosFocus ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();

  private:
    int printOglError(const char file[], int line, const char func[]);
    int canviaF, numP; 
    float angleP;
    bool camPlanta;
    QTimer timer;

  public slots:
    void canviCamera();
    void rotP(int n);
    void moviment();

  signals:
    void cam2(bool);
    void resP(int);

};
