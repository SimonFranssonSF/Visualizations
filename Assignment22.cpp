//--------------------------------------------- // 
#include "stdafx.h"                             // <--- this include is necessary in order to use precompiled headers in visual studio. 
//--------------------------------------------- //      it must be the first commmand in each cpp file
#include "Assignment22.h"                  // <--- include your header file
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
#include <stdlib.h>                                             //
                                                //
IMPLEMENT_GEOX_CLASS( Assignment22, 0 )    // <--- names the class, sets version number to zero.
{                                               //      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( Assignment22 );       // <--- starts the properties declartion

	ADD_SEPARATOR("Datafield");
    ADD_STRING_PROP(dataFilename, 0);
    ADD_NOARGS_METHOD(Assignment22::plotData);

}                                               //
                                                //
QWidget* Assignment22::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}                                                // remark: do not forget to register your class in InitGeoX.cpp
                                                //         (have a look at that file for details)
                                                //
                                                //
Assignment22::Assignment22()          // <--- constructor - set reasonable default parameters
{                                               //
	dataFilename = "..\experiments\Cars.csv";
	x_distance = 20;
	axis_height = 30;
}                                               //
                                                //
Assignment22::~Assignment22()         // <--- destructor - often empty for our experiments
{
                                               //
}                                               //
                                                //                                               //
void Assignment22::plotData()
{
	//Load scalar field
    std::ifstream file(dataFilename);
    std::string str;
    std::getline(file, str);
	std::istringstream ss(str);
	std::string token;
	std::vector<string> cols;
	while(std::getline(ss, token, ',')) {
	    output << token << " | ";
	    cols.push_back(token);
	}

	int dataDimensions = cols.size();
	vector<float>* data = new vector<float>[dataDimensions];
	float minMax[1000][2];


	bool firstTime = true;
    while (std::getline(file, str))
    {
    	int counter = 0;
    	std::istringstream ss(str);
        while(std::getline(ss, token, ',')) {
        	float tempFloat = atof(token.c_str());
        	data[counter].push_back(tempFloat);
        	if(tempFloat < minMax[counter][0] || firstTime == true){
        		minMax[counter][0] = tempFloat;
        	}
        	if(tempFloat > minMax[counter][1] || firstTime == true){
        		minMax[counter][1] = tempFloat;
        	}
        	counter++;
		}
		firstTime = false;
    }

    int origin = -(x_distance*dataDimensions)/2;
    for(int i = 0; i < dataDimensions; ++i){
    	viewer->addLine(makeVector2f(origin + i*x_distance, -axis_height/2), makeVector2f(origin + i*x_distance, axis_height/2));
    }

    float* scalars = new float[dataDimensions];
    for (int i = 0; i < dataDimensions; ++i)
    {
    	scalars[i] = (axis_height/(minMax[i][1]-minMax[i][0]));
    }

    int rows = data[0].size();
    Vector4f color =makeVector4f(80.0f,2.0f,20.0f,0.2f);
    for(int i = 0; i < rows; ++i){
    	//output << data[i].at(0) << " " << data[i].at(0) << " " << data[i].at(0) << " " << data[i].at(0) << " " << data[i].at(0) << " " << data[i].at(0) << " " <<  data[i].at(0) << " " <<  data[i].at(0) << "\n";
    	for(int j = 0; j < (dataDimensions-1); ++j){
    		float value1 = data[j].at(i);
    		float value2 = data[j+1].at(i);
    		viewer->addLine(origin + j*x_distance, (-axis_height/2)+(value1-minMax[j][0])*scalars[j], origin + (j+1)*x_distance, (-axis_height/2)+(value2-minMax[j+1][0])*scalars[j+1], color);
    	}
    }
    viewer->refresh();
}
