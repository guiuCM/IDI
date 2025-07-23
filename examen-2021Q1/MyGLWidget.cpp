// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

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

  angleP = 0.0, 
  angleC1 = 0.0, 
  angleC2 = float((M_PI * 2.0)/3);
  angleC3 = float((M_PI * 4.0)/3);
  colFoc = glm::vec3 (1,1,1);
  pintar = false;
  color = false; 
  camPlanta = false;


  emit resP(1);
  emit canviCam2(camPlanta);


  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
 

  l = -radiEsc;
  r = radiEsc;
  b = -radiEsc;
  t = radiEsc;

  iniCamera ();

 
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  if (pintar){

     // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
    
  }
  else {

    // Pintem el cubs
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0, angleC1);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    modelTransformCub (5.0, angleC2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    modelTransformCub (6.0, angleC3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
   
  }
  
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  ExamGLWidget::modelTransformCub (1.0, 0.0);
  // En aquest mètode has de substituir aquest codi per construir la 
  // transformació geomètrica (TG) del cub usant els paràmetres adientment
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  
 
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angleP, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::rotate(TG, -float(M_PI /2.0), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (escala*2, escala*2, escala*2));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI /2.0), glm::vec3(1, 0, 0));
    View = glm::rotate(View, float(M_PI /2.0), glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::ortho(l, r, b, t, zn, zf);
  //Proj = glm::ortho(-10.f, 10.f, -10.f, 10.f, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::resizeGL (int w, int h) 
{

  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  ra = float(w)/float(h);

  if (ra > 1){
    l = -radiEsc * ra;
    r = radiEsc * ra;
  }
  if (ra < 1){
    b = -radiEsc/ra;
    t = radiEsc/ra;
  }
  
  
  projectTransform();
  glViewport(0,0,w,h);
}

void MyGLWidget::canviCamera(){
  makeCurrent();
  
  camPlanta = !camPlanta;
  viewTransform();
  projectTransform ();
  

  update();
}

void MyGLWidget::posP(int n){
  makeCurrent();
  if (n == 1) angleP = angleC1;
  else if (n == 2) angleP = angleC2;
  else angleP = angleC3;
  

  update();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      pintar = !pintar;
    break;
	}
  case Qt::Key_1: {
      posP(1);
      emit resP(1);
    break;
	}
  case Qt::Key_2: {
      posP(2);
      emit resP(2);
    break;
	}
  case Qt::Key_3: {
      posP(3);
      emit resP(3);
    break;
	}
  case Qt::Key_F: {
      color = !color;
      if (!color){
        colFoc = glm::vec3(1,1,1);
        glUniform3fv (colfocusLoc, 1, &colFoc[0]);
      }
      else {
        colFoc = glm::vec3(1,1,0);
        glUniform3fv (colfocusLoc, 1, &colFoc[0]);
      }
    break;
	}
  case Qt::Key_C: {
      canviCamera();
      emit canviCam2(camPlanta);
    break;
	}
  case Qt::Key_Right: {
      // ...
      if (angleP < float(M_PI * 2)) angleP += float((M_PI * 2.0)/3);
      else angleP = float((M_PI * 2.0)/3);

      if (angleC1 < float(M_PI * 2)) angleC1 += float((M_PI * 2.0)/3);
      else angleC1 = float((M_PI * 2.0)/3);

      if (angleC2 < float(M_PI * 2)) angleC2 += float((M_PI * 2.0)/3);
      else angleC2 = float((M_PI * 2.0)/3);

      if (angleC3 < float(M_PI * 2)) angleC3 += float((M_PI * 2.0)/3);
      else angleC3 = float((M_PI * 2.0)/3);

    break;
	}
  case Qt::Key_Left: {
      // ...
      if (angleP > 0.0) angleP -= float((M_PI * 2.0)/3);
      else angleP = float((M_PI * 4.0)/3);

      if (angleC1 > 0.0) angleC1 -= float((M_PI * 2.0)/3);
      else angleC1 = float((M_PI * 4.0)/3);

      if (angleC2 > 0.0) angleC2 -= float((M_PI * 2.0)/3);
      else angleC2 = float((M_PI * 4.0)/3);

      if (angleC3 > 0.0) angleC3 -= float((M_PI * 2.0)/3);
      else angleC3 = float((M_PI * 4.0)/3);
    break;
	}
  case Qt::Key_R: {
      // ...
      initializeGL();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}



