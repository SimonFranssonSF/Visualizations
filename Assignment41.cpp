//--------------------------------------------- // 
#include "stdafx.h"                             // <--- this include is necessary in order to use precompiled headers in visual studio. 
//--------------------------------------------- //      it must be the first commmand in each cpp file
#include "Assignment41.h"                  // <--- include your header file
//--------------------------------------------- // 
#include "Properties.h"                         // <--- this include file will import all properties available (used below)
#include "GeoXOutput.h"
#include "Properties.h"									// <--- this include file will import all properties available (used below)
#include "GLGeometryViewer.h"                         // <--- this include lets you output text in the main window
//--------------------------------------------- //
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream> 
#include <stdlib.h>                       

//
                                                //
IMPLEMENT_GEOX_CLASS( Assignment41, 0 )    // <--- names the class, sets version number to zero.
{                                               //      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( Assignment41 );       // <--- starts the properties declartion

	ADD_SEPARATOR("Start position");
    ADD_FLOAT32_PROP( xStart, 0 );
	ADD_FLOAT32_PROP( yStart, 0 );
	ADD_FLOAT32_PROP( maxDistance, 0 );
	ADD_SEPARATOR("Euler");
	ADD_FLOAT32_PROP( eulerStepsize, 0 );
	ADD_INT32_PROP( eulerSteps, 0 );
	ADD_SEPARATOR("Runge Kutta");
	ADD_FLOAT32_PROP( rkStepsize, 0 );
	ADD_INT32_PROP( rkSteps, 0 );
	ADD_NOARGS_METHOD(Assignment41::euler);
	ADD_NOARGS_METHOD(Assignment41::rungeKutta);


}                                               //
                                                //
QWidget* Assignment41::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}                                                // remark: do not forget to register your class in InitGeoX.cpp
                                                //         (have a look at that file for details)
                                                //
                                                //
Assignment41::Assignment41()          // <--- constructor - set reasonable default parameters
{
	xStart = 1; 
	yStart = 0;
	maxDistance = 5;
	eulerStepsize = 0.1f;
	eulerSteps = 100;
	rkStepsize = 0.3;
	rkSteps = 30;

}                                              //
                                                //
Assignment41::~Assignment41()         // <--- destructor - often empty for our experiments
{
                                               //
}                                               //

void Assignment41::euler()
{
	int p1Handle = viewer->addPoint(makeVector2f(xStart, yStart));
	float32 xTemp = xStart;
	float32 yTemp = yStart;
	Vector4f colorEuler = makeVector4f(1.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < eulerSteps; ++i){
		float32 nextStepX = xTemp+eulerStepsize*-yTemp;
		float32 nextStepY = yTemp + eulerStepsize*(xTemp/2);
		viewer->addLine(xTemp, yTemp, nextStepX, nextStepY, colorEuler);
		
		xTemp= nextStepX;
		yTemp = nextStepY;

		viewer->addPoint(makeVector2f(xTemp,yTemp));

	}
	viewer->refresh();
}

void Assignment41::rungeKutta()
{
	int p1Handle = viewer->addPoint(makeVector2f(xStart, yStart));
	float32 xTemp = xStart;
	float32 yTemp = yStart;
	Vector4f colorRK = makeVector4f(0.0f, 1.0f, 0.0f, 1.0f);
	for(int i=0; i< rkSteps; ++i)
	{
		float32 arr1[2];
		float32 arr2[2];
		float32 arr3[2];
		float32 arr4[2];
		
		arr1[0] = -yTemp;
		arr1[1] = xTemp/2;
		
		arr2[0] = -(yTemp+rkStepsize/2*arr1[1]);
		arr2[1] = (xTemp+rkStepsize/2*arr1[0])/2;

		arr3[1] = (xTemp+rkStepsize/2*arr2[0])/2;
		arr3[0] = -(yTemp+rkStepsize/2*arr2[1]);

		arr4[1] = (xTemp + rkStepsize*arr3[0])/2;
		arr4[0] = -(yTemp + rkStepsize*arr3[1]);

		float32 xNext = xTemp + rkStepsize *(arr1[0]/6 + arr2[0]/3 + arr3[0]/3 + arr4[0]/6);
		float32 yNext = yTemp + rkStepsize * (arr1[1]/6 + arr2[1]/3 + arr3[1]/3 + arr4[1]/6);

		viewer->addLine(xTemp, yTemp, xNext, yNext, colorRK);

		xTemp = xNext;
		yTemp = yNext;

		viewer->addPoint(makeVector2f(xTemp, yTemp));
	}
	viewer->refresh();
}                                              
