#include "MathUtilityForText.h"
#include <cmath>
#include <numbers>

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

	//X回転*Y回転で回転行列を合成
	Matrix4x4 RotateMatAll = MatrixMultiply(RotateMatX, RotateMatY);


	//回転*平行移動だけをワールド変換行列に
	Matrix4x4 ansMat = MatrixMultiply(RotateMatAll, TranslateMat);

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