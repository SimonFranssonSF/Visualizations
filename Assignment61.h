//-------------------------------------------- // 
#ifndef Assignment61H                     // <--- don't forget the "guards"
#define Assignment61H                     // <--- use a unique name, for example FileNameH, as shown here
//-------------------------------------------- //
#include "Experiment.h"                        // <--- include the base class
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"

#include <limits>
#include "Field2.hpp"
#include <vector>
                     // <--- for Vector3f, Matrix3f
//-------------------------------------------- //
                                               //
                                               //
///                                            //
/// This is an example experiment.             //
///                                            //
/// The code is meant to demonstrate how       //
///  to use the GeoX framework                 //
///                                            //
class Assignment61 : public Experiment    
{                                              //
    GEOX_CLASS(Assignment61)              // <--- use the macro to create registration code
                                               //
//Constructor / Destructor                     //
public:                                        //
    Assignment61();                       // <--- the constructor is used to intialize the parameters
    virtual ~Assignment61();              // <--- the destructor is used to destroy resources; often empty for our experiments
    virtual QWidget* createViewer();
	void rungeKutta();
	void LIC(); //line integral convolution
	string fileName;
	string textureName;
	bool normalized;
	bool blackWhite;
	float stepSize;
	float xPos;
	float yPos;
	int maxSteps;
	float maxLineLength;
	float xSizen;
	float ySizen;


                                               //
//Methods                                      //
protected:
  GLGeometryViewer* viewer;                                        //
                                               //
//Attributes                                   //
private:
	
  float gridWidth;
  float gridHeight;
  QImage image;
  float xStart;
  float yStart;        
  float kernelSize;
  float minVelocity;
  float nbrOfLines;
  float eulerStepsize;
  float rows;
  float cols;
  int eulerSteps;
  int rkSteps;
  float rkStepsize;
  VectorField2 field;
  float grayVal;
  int iWidth;
  int iHeight;
                       // <---
};                                             //
                                               //
#endif                                         
