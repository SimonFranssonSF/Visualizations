//---------------------------------------------			// 
#include "stdafx.h"										// <--- this include is necessary in order to use precompiled headers in visual studio. 
//---------------------------------------------			//      it must be the first commmand in each cpp file
#include "Assignment21.h"								// <--- include your header file
//---------------------------------------------			// 
#include "Properties.h"									// <--- this include file will import all properties available (used below)
#include "GeoXOutput.h"									// <--- this include lets you output text in the main window
#include "GLGeometryViewer.h"
//---------------------------------------------			//
														//
														//
IMPLEMENT_GEOX_CLASS( Assignment21, 0 )					// <--- names the class, sets version number to zero.
{														//      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( Assignment21 );					// <--- starts the properties declartion

   ADD_INT32_PROP( points, 0 )

   ADD_NOARGS_METHOD(Assignment21::straightLine)       // <--- register method 1   (no version required, there is no data to be stored)
   ADD_NOARGS_METHOD(Assignment21::circle)        // <--- register method 2
   ADD_NOARGS_METHOD(Assignment21::hyperbola) // <--- register method 3
}														//
														//
QWidget* Assignment21::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}


														//
Assignment21::Assignment21()							// <--- constructor - set reasonable default parameters
{
	xOrigo = -25;
	yOrigo = 0;
	xSize = 20;
	ySize = 20;
	points = 40;
}

Assignment21::~Assignment21()							// <--- destructor - often empty for our experiments
{

}

void Assignment21::straightLine()						// <--- first experiment
{
	viewer->clear();
    float32 data[200][2];
	for(int i = 0; i < (points); ++i)
	{
		data[i][0] = i-xSize;
		data[i][1] = -i+ySize;
	}
	drawScatterPlot(data);
	drawParallelCoordPlot(data);
}

void Assignment21::circle()								// <--- second experiment
{
    float32 data[200][2];
	viewer->clear();
	for(int i = 0; i < points; ++i)
	{
		data[i][0] = 10*cos((2*3.14*i)/ (points-1) ); //x = r*cos(teta) + forflyttning
		data[i][1] = 10*sin((2*3.14*i)/ (points-1)); //y = r*sin(teta) + forflyttning
	}
	drawScatterPlot(data);
	drawParallelCoordPlot(data);
}

void Assignment21::hyperbola()							// <--- third experiment
{
    float32 data[200][2];
    viewer->clear();
    float32 h = -3.0f;
    float32 stepSize = 6.0f/(((float)points/2)-1);
	for(int i = 0; i < ((float)points/2); ++i)
	{
        data[i][0] =  (1*cosh(h))* cos(-3.14f/4.0f) + (1* sinh(h)) * sin(-3.14f/4.0f) + 5;
        data[i][1] =  -(1*cosh(h))*sin(-3.14f/4.0f) + (1* sinh(h)) * cos(-3.14f/4.0f) + 5;
        h += stepSize;
	}

    h = -3.0f;
    for(int i = ((float)points/2); i < points; ++i)
    {
    	//xˆ = x*cos(θ) + y*sin(θ) 
    	//ˆy = −x*sin(θ) + y*cos(θ)
        data[i][0] =  (-1*cosh(h))* cos(-3.14f/4.0f) + (-1* sinh(h)) * sin(-3.14f/4.0f) + 5;
        data[i][1] =  -(-1*cosh(h))*sin(-3.14f/4.0f) + (-1* sinh(h)) * cos(-3.14f/4.0f) + 5;
        h += stepSize;
    }
	drawScatterPlot(data);
	drawParallelCoordPlot(data);
}
														//
void Assignment21::drawScatterPlot(float32 data[200][2])		// <--- third experiment
{
	//output << "xOrigo: " << xOrigo << " yOrigo: " << yOrigo << " xzSize: " << xSize << " ySize: " << ySize;
	viewer->addLine(makeVector2f(xOrigo, yOrigo-ySize), makeVector2f(xOrigo,yOrigo+ySize));
	viewer->addLine(makeVector2f(xOrigo - xSize, yOrigo), makeVector2f(xOrigo + xSize, yOrigo));

	for(int i = 0; i < points; ++i)
	{
		Point2D p1(data[i][0]+xOrigo, data[i][1]+yOrigo);
		int p3Handle = viewer->addPoint(p1);
	}
	viewer->refresh();
}

void Assignment21::drawParallelCoordPlot(float32 data[200][2])	// <--- third experiment
{
	viewer->addLine(makeVector2f(0, yOrigo-ySize), makeVector2f(0, yOrigo+ySize));
	viewer->addLine(makeVector2f(xSize, yOrigo-ySize), makeVector2f(xSize, yOrigo+ySize));
	
	for(int i = 0; i < points; ++i)
	{
		viewer->addLine(makeVector2f(0, data[i][0]+yOrigo), makeVector2f(xSize, data[i][1]+yOrigo));
	}
	viewer->refresh();
}											

