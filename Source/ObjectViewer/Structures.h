#include "Header.h"

//For representing a uniform matrix stored in a shader
struct Matrix
{
	int location; //Identifier in shader
	glm::mat4 value;
	glm::vec3 pivot;

	//Convenience functions for relative matrix operations around a pivot
	void translate(glm::vec3 _delta)
	{
		value = glm::translate(value, pivot);
		value = glm::translate(value, _delta);
		value = glm::translate(value, -pivot);
	}
	void rotate(float _delta, glm::vec3 _axis)
	{
		value = glm::translate(value, pivot);
		value = glm::rotate(value, _delta, _axis);
		value = glm::translate(value, -pivot);
	}
};

//For representing a uniform vector 3 stored in a shader
struct Vector
{
	int location; //Identifier in shader
	glm::vec3 value;
};

//For representing a uniform float stored in a shader
struct Float
{
	int location; //Identifier in shader
	float value;
};

//Used to store temporary key pressed or released values
//Match up to actions in the scene which must be acted upon
struct KeyMap
{
	bool zoomIn;
	bool zoomOut;
	bool panLeft;
	bool panRight;
	bool panUp;
	bool panDown;

	//Reset all the action values
	void Reset()
	{
		zoomIn = false;
		zoomOut = false;
		panLeft = false;
		panRight = false;
		panUp = false;
		panDown = false;
	}
};