#include "MathUtilityForText.h"
#include <cmath>
#include <numbers>
#include <cassert>

//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) 
//{
//	//使用しないとエラーになるのでダミーにコピー
//	Vector3 dm;
//	dm = scale;
//	dm = rot;
//
//	//平行移動
//	Matrix4x4 result{
//		1.0f, 0.0f, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f, 0.0f,
//		0.0f, 0.0f, 1.0f, 0.0f,
//		translate.x,
//		translate.y,
//		translate.z,
//		1.0f};
//
//	return result;
//}

//Vector3の足し算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

//アフィン変換行列の作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate)
{
	//エラー対策(使用しない)
	Vector3 dm = scale;

	//回転
	Matrix4x4 RotateMatY = {cosf(rot.y), 0, -sinf(rot.y),
		0, 0, 1, 0, 0, 
		sinf(rot.y), 0, cosf(rot.y), 
		0, 0, 0, 0, 1};
	
	//平行移動行列の作成
	Matrix4x4 TranslateMat = {
	    1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};

	//X回転
	Matrix4x4 RotateMatX = {
		1, 0, 0, 0,
		0, cosf(rot.x), sinf(rot.x), 0,
		0, -sinf(rot.x), cosf(rot.x),0,
		0, 0, 0, 1};

	//スケール
	Matrix4x4 ScallMat = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1};

	//X回転*Y回転で回転行列を合成
	Matrix4x4 RotateMatAll = MatrixMultiply(RotateMatX, RotateMatY);


	//拡大*回転*平行移動でワールド変換行列に
	Matrix4x4 ansMat = MatrixMultiply(ScallMat, RotateMatAll);
	ansMat = MatrixMultiply(ansMat, TranslateMat);

	return ansMat;
}

float EaseInOut(float x1, float x2, float t) { 
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { 
	return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); 
}

// 2項演算子オーバーロード
// Vector3の足し算
const Vector3 operator+(const Vector3 & v1, const Vector3 & v2) { 
	Vector3 temp(v1);
	return temp += v2;
}

// 代入演算子オーバーロード
// Vector3の掛け算
const Vector3& operator*=(Vector3& v, float s) { 
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

// 2項演算子オーバーロード
// Vector3の掛け算
const Vector3 operator*(const Vector3& v, float s) { 
	Vector3 temp(v);
	return temp *= s;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) 
{ 
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);   // z軸
}

Matrix4x4 MakeRotateZMatrix(float theta) { 
	float sin = std::sin(theta);
	float cos = std::cos(theta);
	Matrix4x4 result{cos, sin, 0.0f, 0.0f,
					-sin, cos, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f};
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) { 
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
