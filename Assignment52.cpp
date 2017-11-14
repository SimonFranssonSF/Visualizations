//--------------------------------------------- // 
#include "stdafx.h"                             // <--- this include is necessary in order to use precompiled headers in visual studio. 
//--------------------------------------------- //      it must be the first commmand in each cpp file
#include "Assignment52.h"                  // <--- include your header file
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
#include <cmath>

//
                                                //
IMPLEMENT_GEOX_CLASS( Assignment52, 0 )    // <--- names the class, sets version number to zero.
{                                               //      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( Assignment52 );       // <--- starts the properties declartion
	ADD_STRING_PROP(fileName, 0);

	ADD_SEPARATOR("Start position");
    ADD_FLOAT32_PROP( xStart, 0 );
	ADD_FLOAT32_PROP( yStart, 0 );

	
	ADD_SEPARATOR("Runge Kutta");
	ADD_FLOAT32_PROP( rkStepsize, 0 );
	ADD_INT32_PROP( rkSteps, 0 );
	ADD_FLOAT32_PROP( maxDistance, 0 );
	ADD_FLOAT32_PROP( minVelocity, 0 );
	ADD_FLOAT32_PROP( nbrOfLines, 0 );
	ADD_FLOAT32_PROP( rows, 0 );
	ADD_FLOAT32_PROP( cols, 0 );
	ADD_BOOLEAN_PROP(forward, 0);
	ADD_BOOLEAN_PROP(backward, 0);
	ADD_BOOLEAN_PROP(normalized, 0);
	ADD_NOARGS_METHOD(Assignment52::rungeKutta);
	ADD_NOARGS_METHOD(Assignment52::randomLines);
	ADD_NOARGS_METHOD(Assignment52::gridSeed);
	ADD_NOARGS_METHOD(Assignment52::magSeed);
}                                               //
                                                //
QWidget* Assignment52::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}                                                // remark: do not forget to register your class in InitGeoX.cpp
                                                //         (have a look at that file for details)
                                                //
                                                //
Assignment52::Assignment52()          // <--- constructor - set reasonable default parameters
{
	xStart = 0.5; 
	yStart = 0.5;
	maxDistance = 50;
	eulerStepsize = 0.1f;
	eulerSteps = 100;
	rkStepsize = 0.05;
	rkSteps = 30;
	minVelocity = 0.01f;
	nbrOfLines = 1;
	fileName = "..\experiments\ANoise2CT4.am";
	rows = 10;
	cols = 10;
	//fileName = "C:\\Qt\\4.8.6\\demos\\GeoX\\experiments\\Sink.am";

}                                              //
                                                //
Assignment52::~Assignment52()         // <--- destructor - often empty for our experiments
{
                                               //
}                                               //


void Assignment52::rungeKutta()
{
	if (!field.load(fileName))
    {
        output << "Error loading field file " << fileName << "\n";
        return;
    }

	float32 length = 0;

	float32 xTemp = xStart;
	float32 yTemp = yStart;

	StaticVector<unsigned int, 2> dims = field.dims();
    StaticVector<float, 2> min= field.boundMin();
    StaticVector<float, 2> max = field.boundMax();

	viewer->addPoint(makeVector2f(xTemp, yTemp));
	
	if(forward== true && backward == false)
	{
		for(int i = 0; i < rkSteps; ++i)
		{
			Vector2f vec = field.sample(xTemp,yTemp);
			if(vec[0] == 0.0f && vec[1] == 0.0f){
				viewer->refresh();
				return;
			}
			Vector2f vec2;
			Vector2f vec3;
			Vector2f vec4;
			if(normalized)
			{
				vec.normalize();
			}
			vec2 = field.sample(xTemp+(rkStepsize/2)*vec[0], yTemp+(rkStepsize/2)*vec[1]);
			if(normalized)
			{
				vec2.normalize();
			}
			vec3 = field.sample(xTemp+(rkStepsize/2)*vec2[0], yTemp+(rkStepsize/2)*vec2[1]);
			if(normalized)
			{
				vec3.normalize();
			}
			vec4 = field.sample(xTemp+rkStepsize*vec3[0], yTemp+rkStepsize*vec3[1]);
			if(normalized)
			{
				vec4.normalize();
			}
	
			float nextX = xTemp + rkStepsize*(vec[0]/6.0f + vec2[0]/3.0f + vec3[0]/3.0f + vec4[0]/6.0f);
			float nextY = yTemp + rkStepsize*(vec[1]/6.0f + vec2[1]/3.0f + vec3[1]/3.0f + vec4[1]/6.0f);

			viewer->addLine(xTemp, yTemp, nextX, nextY);
			float currLength = sqrt(pow((nextX - xTemp),2) + pow((nextY+yTemp),2));
			if(currLength < minVelocity){
				viewer->refresh();
				return;
			}
			length += currLength;

			xTemp = nextX;
			yTemp = nextY;
		
			if(xTemp > max[0] || xTemp < min[0] || yTemp > max[1] || yTemp < min[1] || length > maxDistance){
				viewer->refresh();
				return;
			}
		}
	}else if(backward == true && forward == true){
		for(int i = 0; i < rkSteps/2; ++i)
		{
			Vector2f vec = field.sample(xTemp,yTemp);
			if(vec[0] == 0.0f && vec[1] == 0.0f){
				viewer->refresh();
				break;
			}
			Vector2f vec2;
			Vector2f vec3;
			Vector2f vec4;
			if(normalized)
			{
				vec.normalize();
			}
			vec2 = field.sample(xTemp+(rkStepsize/2)*vec[0], yTemp+(rkStepsize/2)*vec[1]);
			if(normalized)
			{
				vec2.normalize();
			}
			vec3 = field.sample(xTemp+(rkStepsize/2)*vec2[0], yTemp+(rkStepsize/2)*vec2[1]);
			if(normalized)
			{
				vec3.normalize();
			}
			vec4 = field.sample(xTemp+rkStepsize*vec3[0], yTemp+rkStepsize*vec3[1]);
			if(normalized)
			{
				vec4.normalize();
			}
	
			float nextX = xTemp + rkStepsize*(vec[0]/6.0f + vec2[0]/3.0f + vec3[0]/3.0f + vec4[0]/6.0f);
			float nextY = yTemp + rkStepsize*(vec[1]/6.0f + vec2[1]/3.0f + vec3[1]/3.0f + vec4[1]/6.0f);

			viewer->addLine(xTemp, yTemp, nextX, nextY);
			float32 currLength = sqrt(pow((nextX - xTemp),2) + pow((nextY+yTemp),2));
			if(currLength < minVelocity){
				viewer->refresh();
				break;
			}
			length += currLength;

			xTemp = nextX;
			yTemp = nextY;
		
			if(xTemp > max[0] || xTemp < min[0] || yTemp > max[1] || yTemp < min[1] || length > maxDistance){
				break;
			}
		}

		xTemp = xStart;
		yTemp = yStart;

		for(int i = 0; i < rkSteps/2; ++i)
		{
			Vector2f vec = field.sample(xTemp,yTemp);
			if(vec[0] == 0.0f && vec[1] == 0.0f){
				viewer->refresh();
				return;
			}
			Vector2f vec2;
			Vector2f vec3;
			Vector2f vec4;
			if(normalized)
			{
				vec.normalize();
			}
			vec2 = field.sample(xTemp-(rkStepsize/2)*vec[0], yTemp-(rkStepsize/2)*vec[1]);
			if(normalized)
			{
				vec2.normalize();
			}
			vec3 = field.sample(xTemp-(rkStepsize/2)*vec2[0], yTemp-(rkStepsize/2)*vec2[1]);
			if(normalized)
			{
				vec3.normalize();
			}
			vec4 = field.sample(xTemp-rkStepsize*vec3[0], yTemp-rkStepsize*vec3[1]);
			if(normalized)
			{
				vec4.normalize();
			}
	
			float nextX = xTemp - rkStepsize*(vec[0]/6.0f + vec2[0]/3.0f + vec3[0]/3.0f + vec4[0]/6.0f);
			float nextY = yTemp - rkStepsize*(vec[1]/6.0f + vec2[1]/3.0f + vec3[1]/3.0f + vec4[1]/6.0f);

			viewer->addLine(xTemp, yTemp, nextX, nextY);
			float32 currLength = sqrt(pow((nextX - xTemp),2) + pow((nextY+yTemp),2));
			if(currLength < minVelocity){
				viewer->refresh();
				return;
			}
			length += currLength;

			xTemp = nextX;
			yTemp = nextY;
		
			if(xTemp > max[0] || xTemp < min[0] || yTemp > max[1] || yTemp < min[1] || length > maxDistance){
				viewer->refresh();
				return;
			}
		}
	}else if(backward == true && forward == false){
		for(int i = 0; i < rkSteps; ++i)
		{
			Vector2f vec = field.sample(xTemp,yTemp);
			if(vec[0] == 0.0f && vec[1] == 0.0f){
				viewer->refresh();
				return;
			}
			Vector2f vec2;
			Vector2f vec3;
			Vector2f vec4;

			vec2 = field.sample(xTemp-(rkStepsize/2)*vec[0], yTemp-(rkStepsize/2)*vec[1]);
			vec3 = field.sample(xTemp-(rkStepsize/2)*vec2[0], yTemp-(rkStepsize/2)*vec2[1]);
			vec4 = field.sample(xTemp-rkStepsize*vec3[0], yTemp-rkStepsize*vec3[1]);
	
			float nextX = xTemp - rkStepsize*(vec[0]/6.0f + vec2[0]/3.0f + vec3[0]/3.0f + vec4[0]/6.0f);
			float nextY = yTemp - rkStepsize*(vec[1]/6.0f + vec2[1]/3.0f + vec3[1]/3.0f + vec4[1]/6.0f);

			viewer->addLine(xTemp, yTemp, nextX, nextY);

			xTemp = nextX;
			yTemp = nextY;

			float32 currLength = sqrt(pow((nextX - xTemp),2) + pow((nextY+yTemp),2));
			if(currLength < minVelocity){
				viewer->refresh();
				return;
			}
			length += currLength;
		
			if(xTemp > max[0] || xTemp < min[0] || yTemp > max[1] || yTemp < min[1] || length > maxDistance){
				viewer->refresh();
				return;
			}
		}
	}else{
		output << "Need to specify forward RK, backward RK or both.";
	}



	/*for(float32 x=field.boundMin()[0]; x<=field.boundMax()[0]; x+=0.2)
    {
        for(float32 y=field.boundMin()[1]; y<=field.boundMax()[1]; y+=0.2)
        {
            Vector2f vec = field.sample(x,y);
            vec.normalize();

            viewer->addLine(x, y, x + 0.1f*vec[0], y + 0.1f*vec[1]);
        }
    }*/

	/*int p1Handle = viewer->addPoint(makeVector2f(xStart, yStart));
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
	}*/
	viewer->refresh();
} 

void Assignment52::randomLines(){
	if (!field.load(fileName))
    {
        output << "Error loading field file " << fileName << "\n";
        return;
    }
	Vector2f max = field.boundMax();
	Vector2f min = field.boundMin();
	float xDiff = max[0]-min[0];
	float yDiff = max[1]-min[1];

	for(int i = 0; i < nbrOfLines; ++i)
	{
		
		xStart = (float)rand()/(float)RAND_MAX;
		xStart = (xStart * xDiff)+min[0];
		yStart = (float)rand()/(float)RAND_MAX;
		yStart = (yStart * yDiff)+min[1];
		rungeKutta();
	}
}

void Assignment52::gridSeed(){
	if (!field.load(fileName))
    {
        output << "Error loading field file " << fileName << "\n";
        return;
    }

	Vector2f max = field.boundMax();
	Vector2f min = field.boundMin();
	float32 xDiff = max[0]-min[0];
	float32 yDiff = max[1]-min[1];

	for(float32 x=field.boundMin()[0]; x<field.boundMax()[0]; x+=(xDiff/cols))
    {
        for(float32 y=field.boundMin()[1]; y<field.boundMax()[1]; y+=(yDiff/rows))
        {
            xStart = x;
			yStart = y;
			rungeKutta();
        }
    }
}

void Assignment52::magSeed(){
	if (!field.load(fileName))
    {
        output << "Error loading field file " << fileName << "\n";
        return;
    }
	Vector2f max = field.boundMax();
	Vector2f min = field.boundMin();
	float32 xDiff = max[0]-min[0];
	float32 yDiff = max[1]-min[1];

	float matrix[100][100];
	int i = 0;
	int j = 0;
	float totMagn = 0;

	for(float32 x=field.boundMin()[0]; x<field.boundMax()[0]; x+=(xDiff/cols))
    {
		j = 0;
        for(float32 y=field.boundMin()[1]; y<field.boundMax()[1]; y+=(yDiff/rows))
        {
			float magn = 0;
			for(int i = 0; i < 10; ++i){
				float xRand = (float)rand()/(float)RAND_MAX;
				xRand = (xRand * (xDiff/cols))+x;
				float yRand = (float)rand()/(float)RAND_MAX;
				yRand = (yRand * (yDiff/rows))+y;
				output << "Xr: " << xRand << " Yr: " << yRand <<"\n";

				Vector2f vec = field.sample(xRand,yRand);
				float length = sqrt(pow(vec[0],2) + pow(vec[1],2));
				magn += length;

			}
			magn = magn/10;
			totMagn += magn;
            matrix[i][j] = magn;
			j += 1;
        }
		i += 1;
    }
	output << "---------------------i,j--------------" << i << ", " << j << "\n";
	for(int k = 0; k < i; ++k){
		for(int l = 0;l < j; ++l){
			float pro = matrix[k][l]/totMagn;
			int linesInCell = floor(nbrOfLines*pro+0.5f);
			for(int m = 0; m < linesInCell; ++m){
				float xCell = (float)rand()/(float)RAND_MAX;
				xCell = (xCell * ((k+1)*(xDiff/cols)-k*(xDiff/cols)))+(k*(xDiff/cols))+field.boundMin()[0];
				float yCell = (float)rand()/(float)RAND_MAX;
				yCell = (yCell * ((l+1)*(yDiff/rows))-((l)*(yDiff/rows))) + (l)*(yDiff/rows)+field.boundMin()[1];

				xStart = xCell;
				yStart = yCell;
				output << "X: " << yCell << " yDiff: " << yDiff << " rows: " << rows << " l "<< l << "\n";
				rungeKutta();
			}
		}
	}
}
