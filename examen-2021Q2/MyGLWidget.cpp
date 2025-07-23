// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QTimer>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  canviaF = 0;
  numP = 0;
  angleP = 0.0;
  camPlanta = false;
  emit cam2(camPlanta);
  emit resP(numP);

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();

  connect(&timer, SIGNAL(timeout()), this, SLOT(moviment()));

}

void MyGLWidget::iniEscena ()
{
  ExamGLWidget::iniEscena();
}

void MyGLWidget::enviaPosFocus()
{
  ExamGLWidget::enviaPosFocus();
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.5;
  angleX = -0.5;
  camPlanta = false;
  ra = float(width())/height();
  fov = float(M_PI/3.0);
  zn = 15;
  zf = 65;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  ExamGLWidget::paintGL();
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pintem l'arbre
  pintaArbre (glm::vec3(10,0,10));

  // Pintem l'arbre
  pintaArbre (glm::vec3(20,0,5));

  // Pintem l'arbre
  pintaArbre (glm::vec3(25,0,15));


  glBindVertexArray(0);
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(10,0,10));
  TG = glm::rotate(TG, -angleP, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3 (escala*2, escala*2, escala*2));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camPlanta){
      View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
      View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
      View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
      //transate -centreescena
      View = glm::translate(View, -glm::vec3(15, 2, 10));

      glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }

  else
  {
    // Codi per a la viewMatrix de la Càmera-2
    View = glm::lookAt(glm::vec3(15, 2*radiEsc, 10), glm::vec3(15,0,10), glm::vec3(0,0,-1));
    /*
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(1, 0, 0));
    View = glm::rotate(View, float(M_PI), glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);
*/
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    // Codi per a la projectMatrix de la Càmera-2
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::canviCamera(){
  makeCurrent();

  camPlanta = !camPlanta;
  viewTransform();
  projectTransform();
  
  update();

}
void MyGLWidget::rotP(int n){
  makeCurrent();

  angleP = float(2*M_PI*n/32.0);
  modelTransformPatricio();
  numP = n;

  update();

}

void MyGLWidget::moviment(){
  makeCurrent();

  angleP += float(M_PI*11.25/180.0);
  modelTransformPatricio();
  numP++;
  if (numP == 33) numP = 0;
  emit resP(numP);

  update();

}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
      // ...
      angleP += float(M_PI*11.25/180.0);
      modelTransformPatricio();
      numP++;
      if (numP == 33) numP = 0;
      emit resP(numP);
      
    break;
	}
  case Qt::Key_F: {
    
      canviaF = 1 - canviaF;

      if (canviaF){
        //canvio a vec4 pq la view es mat4
        posFoc = View * glm::vec4(15,10,10,1);
        glUniform3fv (posfocusLoc, 1, &posFoc[0]);

      }
      else {
        posFoc = glm::vec3(0,0,0);
        glUniform3fv (posfocusLoc, 1, &posFoc[0]);

      }
      
    break;
	}
  case Qt::Key_C: {
      // ...
      canviCamera();
      emit cam2(camPlanta);
      
    break;
	}
  case Qt::Key_R: {
      // ...
      initializeGL();
    break;
	}
  case Qt::Key_S: {
      // ...
      if(timer.isActive())timer.stop();
      else timer.start(100);
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camPlanta)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}


