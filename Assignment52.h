//-------------------------------------------- // 
#ifndef Assignment52H                     // <--- don't forget the "guards"
#define Assignment52H                     // <--- use a unique name, for example FileNameH, as shown here
//-------------------------------------------- //
#include "Experiment.h"                        // <--- include the base class
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"

#include <limits>
#include "Field2.hpp"
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
class Assignment52 : public Experiment    
{                                              //
    GEOX_CLASS(Assignment52)              // <--- use the macro to create registration code
                                               //
//Constructor / Destructor                     //
public:                                        //
    Assignment52();                       // <--- the constructor is used to intialize the parameters
    virtual ~Assignment52();              // <--- the destructor is used to destroy resources; often empty for our experiments
    virtual QWidget* createViewer();
	void rungeKutta();
	void randomLines();
	void magSeed();
	void gridSeed();
	string fileName;
	bool forward;
	bool backward;
	bool normalized;
	float stepSize;
	float xPos;
	float yPos;
	int maxSteps;
	float maxLineLength;


                                               //
//Methods                                      //
protected:
  GLGeometryViewer* viewer;                                        //
                                               //
//Attributes                                   //
private:
  float xStart;
  float yStart;        
  float maxDistance;
  float minVelocity;
  float nbrOfLines;
  float eulerStepsize;
  float rows;
  float cols;
  int eulerSteps;
  int rkSteps;
  float rkStepsize;
  VectorField2 field;
                       // <---
};                                             //
                                               //
#endif                                         
