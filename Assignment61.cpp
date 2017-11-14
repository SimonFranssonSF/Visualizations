//--------------------------------------------- // 
#include "stdafx.h"                             // <--- this include is necessary in order to use precompiled headers in visual studio. 
//--------------------------------------------- //      it must be the first commmand in each cpp file
#include "Assignment61.h"                  // <--- include your header file
//--------------------------------------------- // 
#include "Properties.h"                         // <--- this include file will import all properties available (used below)
#include "GeoXoutput.h"
#include "Properties.h"									// <--- this include file will import all properties available (used below)
#include "GLGeometryViewer.h"                         // <--- this include lets you //output text in the main window
//--------------------------------------------- //
#include <fstream>
#include <string>
#include <sstream>
#include <iostream> 
#include <stdlib.h>
#include <cmath>
#include <limits>
//
                                                //
IMPLEMENT_GEOX_CLASS( Assignment61, 0 )    // <--- names the class, sets version number to zero.
{                                               //      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( Assignment61 );       // <--- starts the properties declartion
	ADD_STRING_PROP(fileName, 0);
	
	ADD_SEPARATOR("Runge Kutta");
	ADD_INT32_PROP( rkSteps, 0 );
	ADD_FLOAT32_PROP( kernelSize, 0 );
	ADD_BOOLEAN_PROP(normalized, 0);
	ADD_SEPARATOR("LIC");
	ADD_STRING_PROP(textureName, 0);
	ADD_BOOLEAN_PROP(blackWhite, 0);
	ADD_SEPARATOR("output texture size");
	ADD_FLOAT32_PROP( xSizen, 0 );
	ADD_FLOAT32_PROP( ySizen, 0 );

	ADD_NOARGS_METHOD(Assignment61::LIC);
}                                               //
                                                //
QWidget* Assignment61::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}                                                // remark: do not forget to register your class in InitGeoX.cpp
                                                //         (have a look at that file for details)
                                                //
                                                //
Assignment61::Assignment61()          // <--- constructor - set reasonable default parameters
{
	xStart = 0.5; 
	yStart = 0.5;
	kernelSize = 50;
	eulerStepsize = 0.1f;
	eulerSteps = 100;
	rkStepsize = 0.05f;
	rkSteps = 30;
	minVelocity = 0.000001f;
	nbrOfLines = 1;
	fileName = "../experiments/ANoise2CT4.am";
	textureName = "../experiments/noise2.png";
	rows = 10;
	cols = 10;
	QImage image;
	gridWidth = 0.0f;
	gridHeight = 0.0f;
	grayVal = 0.0f;
	iWidth = 0;
	iHeight = 0;
	xSizen = 256;
	ySizen = 256;
	blackWhite = false;

	//fileName = "C:\\Qt\\4.8.6\\demos\\GeoX\\experiments\\Sink.am";

}                                              //
                                                //
Assignment61::~Assignment61()         // <--- destructor - often empty for our experiments
{
                                               //
}                                               //


void Assignment61::rungeKutta()
{

	float32 length = 0;

	float32 xTemp = xStart;
	float32 yTemp = yStart;

	StaticVector<unsigned int, 2> dims = field.dims();
    StaticVector<float, 2> min= field.boundMin();
    StaticVector<float, 2> max = field.boundMax();
 

	//viewer->addPoint(makeVector2f(xTemp, yTemp));
	float stepsTaken = 0.0f;
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

		//viewer->addLine(xTemp, yTemp, nextX, nextY);
		float32 currLength = sqrt(pow((nextX - xTemp),2) + pow((nextY+yTemp),2));
		if(currLength < minVelocity){
			viewer->refresh();
			break;
		}
		length += currLength;

		xTemp = nextX;
		yTemp = nextY;

				float xGet = 0.0f;
		float yGet = 0.0f;

		xGet = xTemp - min[0];
		yGet = yTemp - min[1];
	

		int posX = xGet/gridWidth;
		int posY = yGet/gridHeight;
		////output << "xGet: " << xGet << "gridWidth: " << gridWidth << "\n";
		if(posX >= iWidth){
			posX -= iWidth;
		}else if(posY >= iHeight){
			posY -= iHeight;
		}
		if(posX >= iWidth){
			posX -= iWidth;
		}else if(posY >= iHeight){
			posY -= iHeight;
		}

		grayVal += (qGray(image.pixel(posX, posY)));
		
		stepsTaken++;
		
		if(xTemp > max[0] || xTemp < min[0] || yTemp > max[1] || yTemp < min[1] || length > kernelSize){
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

		//viewer->addLine(xTemp, yTemp, nextX, nextY);
		float32 currLength = sqrt(pow((nextX - xTemp),2) + pow((nextY+yTemp),2));
		if(currLength < minVelocity){
			viewer->refresh();
			return;
		}
		length += currLength;

		xTemp = nextX;
		yTemp = nextY;

		float xGet = 0.0f;
		float yGet = 0.0f;

		xGet = xTemp - min[0];
		yGet = yTemp - min[1];

		int posX = xGet /gridWidth;
		int posY = yGet /gridHeight;

		if(posX >= iWidth){
			posX -= iWidth;
		}else if(posY >= iHeight){
			posY -= iHeight;
		}

		if(posX >= iWidth){
			posX -= iWidth;
		}else if(posY >= iHeight){
			posY -= iHeight;
		}
		////output<<"posX: " <<posX<<"\n";
		grayVal += (qGray(image.pixel(posX, posY)));

		stepsTaken++;
		
		if(xTemp > max[0] || xTemp < min[0] || yTemp > max[1] || yTemp < min[1] || length > kernelSize){
			viewer->refresh();
			break;
		}

		
	}

	grayVal /= stepsTaken;
}

namespace
{
    ///Returns the next power-of-two
    int32 NextPOT(int32 n)
    {
        n--;
        n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
        n |= n >> 2;   // and then or the results.
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;
        n++;           // The result is a number of 1 bits equal to the number
                       // of bits in the original number, plus 1. That's the
                       // next highest power of 2.
        return n;
    }
}


void Assignment61::LIC(){
	QImage imageTemp(textureName.c_str());
	image = imageTemp;

    //Get its (original) dimensions. Used as bounds later.
    float fWidth = (float)image.width();
    float fHeight = (float)image.height();

    //Resize to power-of-two and mirror.
    image = image.mirrored().scaled(NextPOT(image.width()), NextPOT(image.height()));

    //Get its new integer dimensions.
    iWidth = image.width();
    iHeight = image.height();

	if (!field.load(fileName))
    {
        //output << "Error loading field file " << fileName << "\n";
        return;
    }

	Vector2f max = field.boundMax();
	Vector2f min = field.boundMin();
	float32 xDiff = max[0]-min[0];
	float32 yDiff = max[1]-min[1];

	gridWidth = xDiff / (float)xSizen;
	gridHeight = yDiff / (float)ySizen;

	if(gridWidth > gridHeight){
		rkStepsize = gridHeight;
	}else{
		rkStepsize = gridWidth;
	}
	

	ScalarField2 Gray;
    Gray.init(makeVector2f(0, 0), makeVector2f(xSizen, ySizen), makeVector2ui(xSizen, ySizen));

	int i = 0;
	int j = 0;

	for(float32 x=field.boundMin()[0]; x<field.boundMax()[0]; x+=(gridWidth))
    {
		j = 0;
        for(float32 y=field.boundMin()[1]; y<field.boundMax()[1]; y+=(gridHeight))
        {
			grayVal = 0.0f;
            xStart = x;
			yStart = y;
			rungeKutta();

			grayVal = grayVal / 255.0f;

			if (blackWhite){
				grayVal += 0.5f;
				grayVal = floorf(grayVal);
				
			}

			if(i >= xSizen || j >= ySizen){
				j++;
				continue;
			}else{
				Gray.setNodeScalar(i, j, grayVal );
			}
			j++;
        }
		i++;
    }
	//output<<"Slut här\n";
	viewer->setTextureGray(Gray.getData());
	viewer->refresh();

}