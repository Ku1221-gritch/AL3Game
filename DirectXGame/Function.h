#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>
#define _USE_MATHS_DEFINES
#include <cmath>

// 1.X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// 2.Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// 3.Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeIdentity4x4();
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
