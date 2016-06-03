#include "ofMain.h"

#include <strstream>
#include "clockTester.h"
#include "demoChar.h"
#include "demoInt.h"
#include "demoFloat.h"
#include "demoBoolString.h"
#include "demoVec2.h"
#include "demoRotation.h"
#include "demoAdvanced.h"
#include "demoCallback.h"
#include "demoStack.h"

//========================================================================
int main( int argc,char *argv[] ){

	std::cout << "to start a demo, call:" << std::endl;
	std::cout << "\t'./example-BoostTween -d clock' for clockTester()" << std::endl;
	std::cout << "\t'./example-BoostTween -d char' for demoChar()" << std::endl;
	std::cout << "\t'./example-BoostTween -d int' for demoInt()" << std::endl;
	std::cout << "\t'./example-BoostTween -d float' for demoFloat()" << std::endl;
	std::cout << "\t'./example-BoostTween -d bool' for demoBoolString()" << std::endl;
	std::cout << "\t'./example-BoostTween -d vec2' for demoVec2()" << std::endl;
	std::cout << "\t'./example-BoostTween -d rotation' for demoRotation()" << std::endl;
	std::cout << "\t'./example-BoostTween -d advanced' for demoAdvanced()" << std::endl;
	std::cout << "\t'./example-BoostTween -d callback' for demoCallback()" << std::endl;
	std::cout << "\t'./example-BoostTween -d stack' for demoStack()" << std::endl;

	ofSetupOpenGL( 1280, 720, OF_WINDOW );
	
	bool found = false;
	for ( int i = 0; i < argc; ++i ) {
		std::string param = argv[i];
		if ( param.compare( "-d" ) == 0 && i < argc-1 ) {
			param = argv[i+1];
			if ( param.compare( "clock" ) == 0 ) {
				ofRunApp( new clockTester() );
				found = true;
			} else if ( param.compare( "char" ) == 0 ) {
				ofRunApp( new demoChar() );
				found = true;
			} else if ( param.compare( "int" ) == 0 ) {
				ofRunApp( new demoInt() );
				found = true;
			} else if ( param.compare( "float" ) == 0 ) {
				ofRunApp( new demoFloat() );
				found = true;
			} else if ( param.compare( "bool" ) == 0 ) {
				ofRunApp( new demoBoolString() );
				found = true;
			} else if ( param.compare( "vec2" ) == 0 ) {
				ofRunApp( new demoVec2() );
				found = true;
			} else if ( param.compare( "rotation" ) == 0 ) {
				ofRunApp( new demoRotation() );
				found = true;
			} else if ( param.compare( "advanced" ) == 0 ) {
				ofRunApp( new demoAdvanced() );
				found = true;
			} else if ( param.compare( "callback" ) == 0 ) {
				ofRunApp( new demoCallback() );
				found = true;
			} else if ( param.compare( "stack" ) == 0 ) {
				ofRunApp( new demoStack() );
				found = true;
			}
			break;
		}
			
	}
	
	if (!found) {
		ofRunApp( new demoAdvanced() );
	}
    
}
