//-------------------------------------------- // 
#ifndef Assignment22H                     // <--- don't forget the "guards"
#define Assignment22H                     // <--- use a unique name, for example FileNameH, as shown here
//-------------------------------------------- //
#include "Experiment.h"                        // <--- include the base class
#include "LinearAlgebra.h"
#include "GLGeometryViewer.h"
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
class Assignment22 : public Experiment    
{                                              //
    GEOX_CLASS(Assignment22)              // <--- use the macro to create registration code
                                               //
//Constructor / Destructor                     //
public:                                        //
    Assignment22();                       // <--- the constructor is used to intialize the parameters
    virtual ~Assignment22();              // <--- the destructor is used to destroy resources; often empty for our experiments
    string dataFilename;
    void plotData();
    virtual QWidget* createViewer();

                                               //
//Methods                                      //
protected:
  GLGeometryViewer* viewer;                                        //
                                               //
//Attributes                                   //
private:
  float x_distance;
  float axis_height;                                       //
                       // <---
};                                             //
                                               //
#endif                                         
