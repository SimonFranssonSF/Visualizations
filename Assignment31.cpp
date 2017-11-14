//--------------------------------------------- // 
#include "stdafx.h"                             // <--- this include is necessary in order to use precompiled headers in visual studio. 
//--------------------------------------------- //      it must be the first commmand in each cpp file
#include "Assignment31.h"                  // <--- include your header file
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
IMPLEMENT_GEOX_CLASS( Assignment31, 0 )    // <--- names the class, sets version number to zero.
{                                               //      increment later if you add properties and still want to read old example files
   BEGIN_CLASS_INIT( Assignment31 );       // <--- starts the properties declartion

	ADD_SEPARATOR("Datafield");
    ADD_STRING_PROP(ScalarfieldFilename, 0);
    ADD_FLOAT32_PROP( isoValue, 0 );
    ADD_FLOAT32_PROP( nrOfIsolines, 0 );
    ADD_BOOLEAN_PROP(midPointD, 0);

    ADD_NOARGS_METHOD(Assignment31::drawGrid);
    ADD_NOARGS_METHOD(Assignment31::marchingSquare);
	ADD_NOARGS_METHOD(Assignment31::marchingSecondSquare);



}                                               //
                                                //
QWidget* Assignment31::createViewer()
{
    viewer = new GLGeometryViewer();
    return viewer;
}                                                // remark: do not forget to register your class in InitGeoX.cpp
                                                //         (have a look at that file for details)
                                                //
                                                //
Assignment31::Assignment31()          // <--- constructor - set reasonable default parameters
{                                               //
	ScalarfieldFilename = "../experiments/WideGrid.am";
	x_distance = 20;
	axis_height = 30;
	isoValue = 1;
	ScalarField2 field;
	midPointD = true;
	nrOfIsolines = 5.0f; 
	red = 200.0f;
	green = 20.0f;
	blue = 20.0f;
	alpha = 1.0f;
}                                              //
                                                //
Assignment31::~Assignment31()         // <--- destructor - often empty for our experiments
{
                                               //
}                                               //
                                                //                                               //
void Assignment31::drawGrid()
{
	viewer->clear();
	 //Load scalar field
    field.load(ScalarfieldFilename);
    

    StaticVector<unsigned int, 2> dims = field.dims();
    StaticVector<float, 2> min= field.boundMin();
    StaticVector<float, 2> max = field.boundMax();

    output << " MIN: " << min << "Max: " << max;
    output << "DIM: "<< dims[0] << "x" << dims[1];
	//output << "\n" << field.nodeScalar(250, 250)<<"\n";


    for (int i = 0; i < dims[0]; ++i)
    {
    	for (int j = 0; j < dims[1]; ++j)
	    {
	    	StaticVector<float, 2> pos = field.nodePosition(i,j);
	    	StaticVector<float, 2> posRight = field.nodePosition(i+1,j);
	    	StaticVector<float, 2> posDown = field.nodePosition(i,j+1);
	    	if(j == dims[1]-1 && i == dims[0] - 1){

	    	}else if(i == dims[0]-1){
	    		viewer->addLine(makeVector2f(pos[0], pos[1]), makeVector2f(posDown[0],posDown[1]));
	    	}else if(j == dims[1]-1){
	    		viewer->addLine(makeVector2f(pos[0], pos[1]), makeVector2f(posRight[0],posRight[1]));
	    	}else{
	    		viewer->addLine(makeVector2f(pos[0], pos[1]), makeVector2f(posRight[0],posRight[1]));
	    		viewer->addLine(makeVector2f(pos[0], pos[1]), makeVector2f(posDown[0],posDown[1]));
	    	}
	    }
    }
    viewer->refresh();
}

void Assignment31::marchingSquare(){
	field.load(ScalarfieldFilename);
	Vector4f color = nextColour();

	StaticVector<unsigned int, 2> dims = field.dims();
    StaticVector<float, 2> min= field.boundMin();
    StaticVector<float, 2> max = field.boundMax();

	output << " MIN: " << min << "Max: " << max;
	output << "DIM: " << dims[0] << "x" << dims[1];

	
	//output << "\n" << field.nodeScalar(250, 250) << "\n";
    //Ga igenom cell for cell och kollar alla vertices
	for (int i = 0; i < dims[0]-1; ++i)
    {
		
    	for (int j = 0; j < dims[1]-1; ++j)
	    {
	    	float32 vertices[4];
	    	vertices[0] = field.nodeScalar(i,j);
	    	vertices[1] = field.nodeScalar(i+1,j);
	    	vertices[2] = field.nodeScalar(i+1,j+1);
			vertices[3] = field.nodeScalar(i, j + 1);

			output << "\n" << vertices[0] << " " << vertices[1] << " " << vertices[2] << " " << vertices[3] << "\n";    
			

	    	StaticVector<float, 2> verticesPos[4];
	    	verticesPos[0] = field.nodePosition(i,j);
	    	verticesPos[1] = field.nodePosition(i+1,j);
	    	verticesPos[2] = field.nodePosition(i+1,j+1);
	    	verticesPos[3] = field.nodePosition(i,j+1);

	    	//markerar vertices med 1 eller 0
	    	int markedVertices[4] = {0,0,0,0};
	    	int positiveIndices = 0; 

	    	if (vertices[0] >= isoValue)
	    	{
	    	  	markedVertices[0] = 1;
	    	  	positiveIndices +=1;  
	    	}
	    	if (vertices[1] >= isoValue)
	    	{
	    	  	markedVertices[1] = 1;
	    	  	positiveIndices +=1;   
	    	} 
	    	if (vertices[2] >= isoValue)
	    	{
	    	  	markedVertices[2] = 1;
	    	  	positiveIndices +=1;   
	    	} 
	    	if (vertices[3] >= isoValue)
	    	{
	    	  	markedVertices[3] = 1;
	    	  	positiveIndices +=1;   
	    	}  
	    	//output << "positiveIndices: " << positiveIndices; 

	    	//4 olika fall som kan intraffa som svarar for hur vi ska dra isolinjen
	    	if(positiveIndices == 0 || positiveIndices == 4){
			
	    	//fall utan isolinje, alla vertices ar storre eller mindre 
	    		//continue;

	    	}else if(positiveIndices == 1 || positiveIndices == 3){
			//	output << "Corner";
	    		//output << "INNE 2";

			
	    		for (int i = 0; i < (sizeof(vertices)/sizeof(vertices[0])); ++i)
	    		{
	    			if((vertices[i] < isoValue && positiveIndices == 3) || (vertices[i] > isoValue && positiveIndices == 1)){
	    				//output << "\nINNE 3";
	    				//vi vet att vertices[i] ar vardet som ar ensamt
	    				int i1 = i+1;
	    				int i2 = i+3;
	    				if(i1 > 3) 
	    					i1 -= 4;
	    				if(i2 > 3)
	    					i2 -=4;

	    				//output << "INNE 1";

	    				//vertices[i]
	    				//vertices[i1]


	    				float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i1][0] - verticesPos[i][0]))/(vertices[i1]-vertices[i]);
	    				float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i1][1] - verticesPos[i][1]))/(vertices[i1] - vertices[i]);


	    				float32 intersectionPoint2X = ((isoValue - vertices[i])*(verticesPos[i2][0] - verticesPos[i][0]))/(vertices[i2]-vertices[i]);
	    				float32 intersectionPoint2Y = ((isoValue - vertices[i])*(verticesPos[i2][1] - verticesPos[i][1]))/(vertices[i2]-vertices[i]);   
	    				
	    				//output << "X: " << intersectionPoint1X << " Y: " << intersectionPoint1Y << "\n";
	    				//output << "X2: " << intersectionPoint2X << " Y2: " << intersectionPoint2Y << "\n";


						viewer->addLine(intersectionPoint1X + verticesPos[i][0], intersectionPoint1Y + verticesPos[i][1], intersectionPoint2X + verticesPos[i][0], intersectionPoint2Y + verticesPos[i][1], color);
	    			} 
	    		}
	    	}else if( (markedVertices[0]+markedVertices[1] == 2 || markedVertices[0] + markedVertices[3] == 2 || markedVertices[2] + markedVertices[3] == 2 || markedVertices[1] + markedVertices[2] == 2) ){
				
				if(markedVertices[0]+markedVertices[1] == 2 || markedVertices[0]+markedVertices[1] == 0){
	    			int i = 0;
	    			int i1 = 1;
    				int i2 = 2;
    				int i3 = 3;

    				float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i3][0] - verticesPos[i][0]))/(vertices[i3]-vertices[i]);
    				float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i3][1] - verticesPos[i][1]))/(vertices[i3] - vertices[i]);


    				float32 intersectionPoint2X = ((isoValue - vertices[i1])*(verticesPos[i2][0] - verticesPos[i1][0]))/(vertices[i2]-vertices[i1]);
    				float32 intersectionPoint2Y = ((isoValue - vertices[i1])*(verticesPos[i2][1] - verticesPos[i1][1]))/(vertices[i2]-vertices[i1]);   
	    			
	    			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1],color);
	    			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1],color);
	    		}else if((markedVertices[0]+markedVertices[3] == 2 || markedVertices[0]+markedVertices[3] == 0)){
	    			int i = 0;
	    			int i1 = 1;
    				int i2 = 2;
    				int i3 = 3;
    				
    				float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i1][0] - verticesPos[i][0]))/(vertices[i1]-vertices[i]);
    				float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i1][1] - verticesPos[i][1]))/(vertices[i1] - vertices[i]);


    				float32 intersectionPoint2X = ((isoValue - vertices[i2])*(verticesPos[i3][0] - verticesPos[i2][0]))/(vertices[i3]-vertices[i2]);
    				float32 intersectionPoint2Y = ((isoValue - vertices[i2])*(verticesPos[i3][1] - verticesPos[i2][1]))/(vertices[i3]-vertices[i2]);   
	    			
	    			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i2][0], intersectionPoint2Y+verticesPos[i2][1],color);
	    		}

	    	}else{
				
	    		if(midPointD){
	    			midPointDecider(vertices, isoValue, verticesPos, markedVertices);
	    		}else{
	    			asymDecider(vertices, isoValue, verticesPos, markedVertices);
	    		}
	    	}
	    }

    }
    viewer->refresh();
}



void Assignment31::marchingSecondSquare(){
	field.load(ScalarfieldFilename);


	StaticVector<unsigned int, 2> dims = field.dims();
	StaticVector<float, 2> min = field.boundMin();
	StaticVector<float, 2> max = field.boundMax();

	//output << " MIN: " << min << "Max: " << max;
	//output << "DIM: " << dims[0] << "x" << dims[1];



	float maxScalar = field.nodeScalar(0, 0);
	float minScalar = field.nodeScalar(0, 0);

	for (int i = 0; i < dims[0] - 1; ++i)
	{

		for (int j = 0; j < dims[1] - 1; ++j)
		{
			if (maxScalar <  field.nodeScalar(i, j))
			{
				maxScalar = field.nodeScalar(i, j);
			}
			if (minScalar > field.nodeScalar(i, j)){
				minScalar = field.nodeScalar(i, j);
			}
				
		}
	}

	float isoStep = (maxScalar - minScalar) / (float)nrOfIsolines;

	isoValue = minScalar + isoStep;
	blue = 1.0f;
	red = 0.0f;
	green = 0.2f;

	float colorStep = 1.0f / (float) nrOfIsolines;
	output << "ColorStep: " << colorStep << "\n";

	Vector4f color = nextColour();
	for (int i = 0; i <= nrOfIsolines; i++)
	{


		//output << "\n" << field.nodeScalar(250, 250) << "\n";
		//Ga igenom cell for cell och kollar alla vertices
		for (int i = 0; i < dims[0] - 1; ++i)
		{

			for (int j = 0; j < dims[1] - 1; ++j)
			{
			
				float32 vertices[4];
				vertices[0] = field.nodeScalar(i, j);
				vertices[1] = field.nodeScalar(i + 1, j);
				vertices[2] = field.nodeScalar(i + 1, j + 1);
				vertices[3] = field.nodeScalar(i, j + 1);


				StaticVector<float, 2> verticesPos[4];
				verticesPos[0] = field.nodePosition(i, j);
				verticesPos[1] = field.nodePosition(i + 1, j);
				verticesPos[2] = field.nodePosition(i + 1, j + 1);
				verticesPos[3] = field.nodePosition(i, j + 1);

				//markerar vertices med 1 eller 0
				int markedVertices[4] = { 0, 0, 0, 0 };
				int positiveIndices = 0;

				if (vertices[0] >= isoValue)
				{
					markedVertices[0] = 1;
					positiveIndices += 1;
				}
				if (vertices[1] >= isoValue)
				{
					markedVertices[1] = 1;
					positiveIndices += 1;
				}
				if (vertices[2] >= isoValue)
				{
					markedVertices[2] = 1;
					positiveIndices += 1;
				}
				if (vertices[3] >= isoValue)
				{
					markedVertices[3] = 1;
					positiveIndices += 1;
				}
				//output << "positiveIndices: " << positiveIndices; 

				//4 olika fall som kan intraffa som svarar for hur vi ska dra isolinjen
				if (positiveIndices == 0 || positiveIndices == 4){

					//fall utan isolinje, alla vertices ar storre eller mindre 
					//continue;

				}
				else if (positiveIndices == 1 || positiveIndices == 3){
					//	output << "Corner";
					//output << "INNE 2";


					for (int i = 0; i < (sizeof(vertices) / sizeof(vertices[0])); ++i)
					{
						if ((vertices[i] < isoValue && positiveIndices == 3) || (vertices[i] > isoValue && positiveIndices == 1)){
							//output << "\nINNE 3";
							//vi vet att vertices[i] ar vardet som ar ensamt
							int i1 = i + 1;
							int i2 = i + 3;
							if (i1 > 3)
								i1 -= 4;
							if (i2 > 3)
								i2 -= 4;

							//output << "INNE 1";

							//vertices[i]
							//vertices[i1]


							float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i1][0] - verticesPos[i][0])) / (vertices[i1] - vertices[i]);
							float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i1][1] - verticesPos[i][1])) / (vertices[i1] - vertices[i]);


							float32 intersectionPoint2X = ((isoValue - vertices[i])*(verticesPos[i2][0] - verticesPos[i][0])) / (vertices[i2] - vertices[i]);
							float32 intersectionPoint2Y = ((isoValue - vertices[i])*(verticesPos[i2][1] - verticesPos[i][1])) / (vertices[i2] - vertices[i]);

							//output << "X: " << intersectionPoint1X << " Y: " << intersectionPoint1Y << "\n";
							//output << "X2: " << intersectionPoint2X << " Y2: " << intersectionPoint2Y << "\n";


							viewer->addLine(intersectionPoint1X + verticesPos[i][0], intersectionPoint1Y + verticesPos[i][1], intersectionPoint2X + verticesPos[i][0], intersectionPoint2Y + verticesPos[i][1],color);
						}
					}
				}
				else if ((markedVertices[0] + markedVertices[1] == 2 || markedVertices[0] + markedVertices[3] == 2 || markedVertices[2] + markedVertices[3] == 2 || markedVertices[1] + markedVertices[2] == 2)){

					if (markedVertices[0] + markedVertices[1] == 2 || markedVertices[0] + markedVertices[1] == 0){
						int i = 0;
						int i1 = 1;
						int i2 = 2;
						int i3 = 3;

						float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i3][0] - verticesPos[i][0])) / (vertices[i3] - vertices[i]);
						float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i3][1] - verticesPos[i][1])) / (vertices[i3] - vertices[i]);


						float32 intersectionPoint2X = ((isoValue - vertices[i1])*(verticesPos[i2][0] - verticesPos[i1][0])) / (vertices[i2] - vertices[i1]);
						float32 intersectionPoint2Y = ((isoValue - vertices[i1])*(verticesPos[i2][1] - verticesPos[i1][1])) / (vertices[i2] - vertices[i1]);

						viewer->addLine(intersectionPoint1X + verticesPos[i][0], intersectionPoint1Y + verticesPos[i][1], intersectionPoint2X + verticesPos[i1][0], intersectionPoint2Y + verticesPos[i1][1],color);
					}
					else if ((markedVertices[0] + markedVertices[3] == 2 || markedVertices[0] + markedVertices[3] == 0)){
						int i = 0;
						int i1 = 1;
						int i2 = 2;
						int i3 = 3;

						float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i1][0] - verticesPos[i][0])) / (vertices[i1] - vertices[i]);
						float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i1][1] - verticesPos[i][1])) / (vertices[i1] - vertices[i]);


						float32 intersectionPoint2X = ((isoValue - vertices[i2])*(verticesPos[i3][0] - verticesPos[i2][0])) / (vertices[i3] - vertices[i2]);
						float32 intersectionPoint2Y = ((isoValue - vertices[i2])*(verticesPos[i3][1] - verticesPos[i2][1])) / (vertices[i3] - vertices[i2]);

						viewer->addLine(intersectionPoint1X + verticesPos[i][0], intersectionPoint1Y + verticesPos[i][1], intersectionPoint2X + verticesPos[i2][0], intersectionPoint2Y + verticesPos[i2][1],color);
					}

				}
				else{

					if (midPointD){
						midPointDecider(vertices, isoValue, verticesPos, markedVertices);
					}
					else{
						asymDecider(vertices, isoValue, verticesPos, markedVertices);
					}
				}
			}

		}
		isoValue += isoStep;
		blue -= colorStep;
		red += colorStep;
		color = nextColour();
	}
	viewer->refresh();



}


void Assignment31::midPointDecider(float32 vertices[4], float32 isoValue, StaticVector<float, 2> verticesPos[4], int markedVertices[4]){

	Vector4f color = nextColour();
	
	int i = 0;
	int i1 = 1;
	int i2 = 2;
	int i3 = 3;
		
	float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i1][0] - verticesPos[i][0]))/(vertices[i1]-vertices[i]);
	float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i1][1] - verticesPos[i][1]))/(vertices[i1] - vertices[i]);


	float32 intersectionPoint2X = ((isoValue - vertices[i1])*(verticesPos[i2][0] - verticesPos[i1][0]))/(vertices[i2]-vertices[i1]);
	float32 intersectionPoint2Y = ((isoValue - vertices[i1])*(verticesPos[i2][1] - verticesPos[i1][1]))/(vertices[i2]-vertices[i1]);

	float32 intersectionPoint3X = ((isoValue - vertices[i2])*(verticesPos[i3][0] - verticesPos[i2][0]))/(vertices[i3]-vertices[i2]);
	float32 intersectionPoint3Y = ((isoValue - vertices[i2])*(verticesPos[i3][1] - verticesPos[i2][1]))/(vertices[i3]-vertices[i2]);   

	float32 intersectionPoint4X = ((isoValue - vertices[i])*(verticesPos[i3][0] - verticesPos[i][0]))/(vertices[i3]-vertices[i]);
	float32 intersectionPoint4Y = ((isoValue - vertices[i])*(verticesPos[i3][1] - verticesPos[i][1]))/(vertices[i3]-vertices[i]);

	float32 midPoint = ((vertices[0]+vertices[1]+vertices[2]+vertices[3])/4.0f) ;

	if(midPoint > isoValue){
		if(markedVertices[0] == 1 && markedVertices[2] == 1){
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1],color);
			viewer->addLine(intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1],color);
		}else if(markedVertices[1] == 1 && markedVertices[3] == 1){
			viewer->addLine(intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1],color);
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1],color);
		}
	}else{
		if(markedVertices[0] == 0 && markedVertices[2] == 0){
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1],color);
			viewer->addLine(intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1]),color;
		}else if(markedVertices[1] == 0 && markedVertices[3] == 0){
			viewer->addLine(intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1],color);
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1],color);
		}
	}
}

void Assignment31::asymDecider(float32 vertices[4], float32 isoValue, StaticVector<float, 2> verticesPos[4], int markedVertices[4]){
	
	Vector4f color = nextColour();

	int i = 0;
	int i1 = 1;
	int i2 = 2;
	int i3 = 3;
		
	float32 intersectionPoint1X = ((isoValue - vertices[i])*(verticesPos[i1][0] - verticesPos[i][0]))/(vertices[i1]-vertices[i]);
	float32 intersectionPoint1Y = ((isoValue - vertices[i])*(verticesPos[i1][1] - verticesPos[i][1]))/(vertices[i1] - vertices[i]);


	float32 intersectionPoint2X = ((isoValue - vertices[i1])*(verticesPos[i2][0] - verticesPos[i1][0]))/(vertices[i2]-vertices[i1]);
	float32 intersectionPoint2Y = ((isoValue - vertices[i1])*(verticesPos[i2][1] - verticesPos[i1][1]))/(vertices[i2]-vertices[i1]);

	float32 intersectionPoint3X = ((isoValue - vertices[i2])*(verticesPos[i3][0] - verticesPos[i2][0]))/(vertices[i3]-vertices[i2]);
	float32 intersectionPoint3Y = ((isoValue - vertices[i2])*(verticesPos[i3][1] - verticesPos[i2][1]))/(vertices[i3]-vertices[i2]);   

	float32 intersectionPoint4X = ((isoValue - vertices[i])*(verticesPos[i3][0] - verticesPos[i][0]))/(vertices[i3]-vertices[i]);
	float32 intersectionPoint4Y = ((isoValue - vertices[i])*(verticesPos[i3][1] - verticesPos[i][1]))/(vertices[i3]-vertices[i]);

	float32 asym;
	asym = ( (vertices[0]*vertices[2]) - (vertices[1]*vertices[3]) )/ ( (vertices[2]+vertices[0]-vertices[1]-vertices[3]) );
	

	if (isoValue < asym)
	{
		if(markedVertices[0] == 1 && markedVertices[2] == 1){
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1],color);
			viewer->addLine(intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1], color);
		}else if(markedVertices[1] == 1 && markedVertices[3] == 1){
			viewer->addLine(intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1],color);
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1],color);
		}
	}else{
		if(markedVertices[0] == 0 && markedVertices[2] == 0){
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1],color);
			viewer->addLine(intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1],color);
		}else if(markedVertices[1] == 0 && markedVertices[3] == 0){
			viewer->addLine(intersectionPoint2X+verticesPos[i1][0], intersectionPoint2Y+verticesPos[i1][1], intersectionPoint3X+verticesPos[i2][0], intersectionPoint3Y+verticesPos[i2][1],color);
			viewer->addLine(intersectionPoint1X+verticesPos[i][0], intersectionPoint1Y+verticesPos[i][1], intersectionPoint4X+verticesPos[i][0], intersectionPoint4Y+verticesPos[i][1],color);
		}
	}
}


Vector4f  Assignment31::nextColour(){

	Vector4f color = makeVector4f(200.0f, 20.0f, 20.0f, 1.0f);

	if (0 <= red && red <= 255 && 0 <= blue && blue <= 255 && 0 <= green && green <= 255 && 0 <= alpha && alpha <= 1){
		color = makeVector4f(red, green, blue, alpha);
	}
	else
	{
		if (0 > red || red > 255){
			output << "Red should be between 0 and 255.\n";
		}
		if (0 > green || green > 255){
			output << "Green should be between 0 and 255.\n";
		}
		if (0 > blue || blue > 255){
			output << "Blue should be between 0 and 255.\n";
		}
		if (0 > alpha || alpha > 1){
			output << "Alpha should be between 0 and 1.\n";
		}

	}



	return color;
}