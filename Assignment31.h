//-------------------------------------------- // 
#ifndef Assignment31H                     // <--- don't forget the "guards"
#define Assignment31H                     // <--- use a unique name, for example FileNameH, as shown here
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
class Assignment31 : public Experiment    
{                                              //
    GEOX_CLASS(Assignment31)              // <--- use the macro to create registration code
                                               //
//Constructor / Destructor                     //
public:                                        //
    Assignment31();                       // <--- the constructor is used to intialize the parameters
    virtual ~Assignment31();              // <--- the destructor is used to destroy resources; often empty for our experiments
    string ScalarfieldFilename;
    void drawGrid();
    void marchingSquare();
	void marchingSecondSquare();
    float32 isoValue;
    bool midPointD;
    void midPointDecider(float32 vertices[4], float32 isoValue, StaticVector<float, 2> verticesPos[4], int markedVertices[4]);
    void asymDecider(float32 vertices[4], float32 isoValue, StaticVector<float, 2> verticesPos[4], int markedVertices[4]);
    virtual QWidget* createViewer();
    float nrOfIsolines;

                                               //
//Methods                                      //
protected:
  GLGeometryViewer* viewer;                                        //
                                               //
//Attributes                                   //
private:
  float x_distance;
  float axis_height;
  ScalarField2 field;        
  float red;
  float blue;
  float green;
  float alpha;
  Vector4f nextColour();
                       // <---
};                                             //
                                               //
#endif                                         
