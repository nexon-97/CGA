#pragma once
#include <core/Defines.hpp>

//Matrix 4 X 4

namespace cga
{
namespace math
{

class Matrix
{
public:
	//constructors
	Matrix() {}
	Matrix(float val);
	Matrix(Matrix & mat);

	// Matrix operations
	Matrix & SetZero();
	Matrix & SetIdentity();

	Matrix & SetRotationX(float angle);
	Matrix & SetRotationY(float angle);
	Matrix & SetRotationZ(float angle);
	Matrix & SetRotationAngleAxis(float angle, float x, float y, float z);

	Matrix & SetScale(float scale);
	Matrix & SetScale(float scaleX, float scaleY, float scaleZ);
	Matrix & SetScale(float * pScale);
	Matrix & SetScale(glm::vec3f& scaleVec);

	Matrix & SetTranslation(float x, float y, float z);
	Matrix & SetTranslation(float *pTrans);
	Matrix & SetTranslation(glm::vec3f& vec);

	Matrix & SetPerspective(float fovY, float aspect, float nearPlane, float farPlane);

	Matrix Transpose();
	Matrix Inverse();

	Matrix operator + (Matrix & mat);
	Matrix & operator += (Matrix & mat);
	Matrix operator - (Matrix & mat);
	Matrix &operator -= (Matrix & mat);

	Matrix operator * (Matrix & mat);
	Matrix operator * (float k);
	Matrix & operator *= (float k);

	glm::vec4f operator * (glm::vec4f& vec);

	Matrix & operator = (Matrix & mat);

	//data members
	float m[4][4];
};

}
}
