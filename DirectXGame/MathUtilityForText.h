#pragma once

#include <math.h>
#include "Matrix4x4.h"
#include "Vector3.h"

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Vector3& operator+=(Vector3& lhv, const Vector3& rhv);

//行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2);

//EaseInOut関数
float EaseInOut(float x1, float x2, float t);
float Lerp(float x1, float x2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

//2項演算子オーバーロード
//Vector3の足し算
const Vector3 operator+(const Vector3& v1, const Vector3& v2);

//代入演算子オーバーロード
//Vector3の掛け算
const Vector3& operator*=(Vector3& v, float s);

// 2項演算子オーバーロード
//Vector3の掛け算
const Vector3 operator*(const Vector3& v, float s);