//-------------------------------------------- // 
#ifndef Assignment41H                     // <--- don't forget the "guards"
#define Assignment41H                     // <--- use a unique name, for example FileNameH, as shown here
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
class Assignment41 : public Experiment    
{                                              //
    GEOX_CLASS(Assignment41)              // <--- use the macro to create registration code
                                               //
//Constructor / Destructor                     //
public:                                        //
    Assignment41();                       // <--- the constructor is used to intialize the parameters
    virtual ~Assignment41();              // <--- the destructor is used to destroy resources; often empty for our experiments
    virtual QWidget* createViewer();
	void euler();
	void rungeKutta();
   

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
  float eulerStepsize;
  int eulerSteps;
  int rkSteps;
  float rkStepsize;
                       // <---
};                                             //
                                               //
#endif                                         
