//-------------------------------------------- // 
#ifndef Assignment21H                     // <--- don't forget the "guards"
#define Assignment21H                     // <--- use a unique name, for example FileNameH, as shown here
//-------------------------------------------- //
#include "Experiment.h"                        // <--- include the base class
#include "LinearAlgebra.h"                     // <--- for Vector3f, Matrix3f
#include "GLGeometryViewer.h"
//-------------------------------------------- //
                                               //
                                               //
///                                            //
/// This is an example experiment.             //
///                                            //
/// The code is meant to demonstrate how       //
///  to use the GeoX framework                 //
///                                            //
class Assignment21 : public Experiment		   // <--- derive your experiment from base class <Experiment>
{                                              //
    GEOX_CLASS(Assignment21)				   // <--- use the macro to create registration code
                                               //
//Constructor / Destructor                     //
public:                                        //
    Assignment21();							   // <--- the constructor is used to intialize the parameters
    virtual ~Assignment21();				   // <--- the destructor is used to destroy resources; often empty for our experiments
    virtual QWidget* createViewer();

//Methods                                      //
public:                                        //
    void straightLine();                       // <--- Creates 2d-data in a container which creates an straight line with a negative slope
    void circle();							   // <--- Creates 2d-data in a container which creates an circle
    void hyperbola();                          // <--- Creates 2d-data in a container which creates an hyperbola
	void drawScatterPlot(float data[21][2]);		   // <--- Draws a scatterplot of the data sent to the method
	void drawParallelCoordPlot(float data[21][2]);  // <--- Draws a Parallel coordinate plot of the data sent to the method
                                               //
//Attributes
protected:
	GLGeometryViewer* viewer;

private:                                       //
	float32 xOrigo;
	float32 yOrigo;
	float32 xSize;
	float32 ySize;
  int32 points;
};                                             //
                                               //
#endif                                         //
