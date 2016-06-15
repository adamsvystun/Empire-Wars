#pragma once
#include <iostream>
#include <exception>

struct empireGetExeption : public std::exception {
	virtual const char* what() const throw();
};

struct fileOpenFail : public std::exception {
	virtual const char* what() const throw();
};

struct planetCreateFail : public std::exception {
	virtual const char* what() const throw();
};

struct asteroidCreateFail : public std::exception {
	virtual const char* what() const throw();
};

struct empireCreateFail : public std::exception {
	virtual const char* what() const throw();
};

struct addNodeFail : public std::exception {
	virtual const char* what() const throw();
};

struct battleRunning : public std::exception {
	virtual const char* what() const throw();
};

struct staticShip : public std::exception {
	virtual const char* what() const throw();
};