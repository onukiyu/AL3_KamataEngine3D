#include "MathUtilityForText.h"

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) 
{
	//使用しないとエラーになるのでダミーにコピー
	Vector3 dm;
	dm = scale;
	dm = rot;

	//平行移動
	Matrix4x4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x,
		translate.y,
		translate.z,
		1.0f};

	return result;
}