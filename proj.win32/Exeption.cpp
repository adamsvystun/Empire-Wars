#include "Exeptions.h"

const char* empireGetExeption::what() const throw()
{
	return "Could not get pointer to the empire";
}

const char* fileOpenFail::what() const throw()
{
	return "Could not open the file";
}

const char* planetCreateFail::what() const throw()
{
	return "Could not create the planet. Try again...";
}

const char* asteroidCreateFail::what() const throw()
{
	return "Could not create the asteroid. Try again...";
}

const char* empireCreateFail::what() const throw()
{
	return "Could not create the empire. Try again...";
}

const char* addNodeFail::what() const throw()
{
	return "Could not add node to the empire. Try again...";
}

const char* battleRunning::what() const throw()
{
	return "Not a save point. Battle is running. Try again...";
}

const char* staticShip::what() const throw()
{
	return "Not a save point. Ship is moving. Try again...";
}