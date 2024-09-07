#include "Function.h"

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 Answer;
	Answer.m[0][0] = 1.0f;
	Answer.m[0][1] = 0.0f;
	Answer.m[0][2] = 0.0f;
	Answer.m[0][3] = 0.0f;
	Answer.m[1][0] = 0.0f;
	Answer.m[1][1] = std::cos(radian);
	Answer.m[1][2] = std::sin(radian);
	Answer.m[1][3] = 0.0f;
	Answer.m[2][0] = 0.0f;
	Answer.m[2][1] = -std::sin(radian);
	Answer.m[2][2] = std::cos(radian);
	Answer.m[2][3] = 0.0f;
	Answer.m[3][0] = 0.0f;
	Answer.m[3][1] = 0.0f;
	Answer.m[3][2] = 0.0f;
	Answer.m[3][3] = 1.0f;
	return Answer;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 Answer;
	Answer.m[0][0] = std::cos(radian);
	Answer.m[0][1] = 0.0f;
	Answer.m[0][2] = -std::sin(radian);
	Answer.m[0][3] = 0.0f;
	Answer.m[1][0] = 0.0f;
	Answer.m[1][1] = 1.0f;
	Answer.m[1][2] = 0.0f;
	Answer.m[1][3] = 0.0f;
	Answer.m[2][0] = std::sin(radian);
	Answer.m[2][1] = 0.0f;
	Answer.m[2][2] = std::cos(radian);
	Answer.m[2][3] = 0.0f;
	Answer.m[3][0] = 0.0f;
	Answer.m[3][1] = 0.0f;
	Answer.m[3][2] = 0.0f;
	Answer.m[3][3] = 1.0f;
	return Answer;
}


Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 Answer;
	Answer.m[0][0] = std::cos(radian);
	Answer.m[0][1] = std::sin(radian);
	Answer.m[0][2] = 0.0f;
	Answer.m[0][3] = 0.0f;
	Answer.m[1][0] = -std::sin(radian);
	Answer.m[1][1] = std::cos(radian);
	Answer.m[1][2] = 0.0f;
	Answer.m[1][3] = 0.0f;
	Answer.m[2][0] = 0.0f;
	Answer.m[2][1] = 0.0f;
	Answer.m[2][2] = 1.0f;
	Answer.m[2][3] = 0.0f;
	Answer.m[3][0] = 0.0f;
	Answer.m[3][1] = 0.0f;
	Answer.m[3][2] = 0.0f;
	Answer.m[3][3] = 1.0f;
	return Answer;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	{
		Matrix4x4 Answer;
		Answer.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
		Answer.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
		Answer.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
		Answer.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
		Answer.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
		Answer.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
		Answer.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
		Answer.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
		Answer.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
		Answer.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
		Answer.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
		Answer.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
		Answer.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
		Answer.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
		Answer.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
		Answer.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
		return Answer;
	}
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	/*Matrix4x4 result = Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));
	result.m[0][0] *= scale.x;
	result.m[0][1] *= scale.x;
	result.m[0][2] *= scale.x;
	result.m[1][0] *= scale.y;
	result.m[1][1] *= scale.y;
	result.m[1][2] *= scale.y;
	result.m[2][0] *= scale.z;
	result.m[2][1] *= scale.z;
	result.m[2][2] *= scale.z;
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;
	return result;*/
	Matrix4x4 result = MakeTranslateMatrix(translate);
	Matrix4x4 rotateXmatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYmatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZmatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZmatrix = Multiply(Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)), MakeRotateZMatrix(rotate.z));
	result.m[0][0] = scale.x * rotateXYZmatrix.m[0][0];
	result.m[0][1] = scale.x * rotateXYZmatrix.m[0][1];
	result.m[0][2] = scale.x * rotateXYZmatrix.m[0][2];
	result.m[1][0] = scale.y * rotateXYZmatrix.m[1][0];
	result.m[1][1] = scale.y * rotateXYZmatrix.m[1][1];
	result.m[1][2] = scale.y * rotateXYZmatrix.m[1][2];
	result.m[2][0] = scale.z * rotateXYZmatrix.m[2][0];
	result.m[2][1] = scale.z * rotateXYZmatrix.m[2][1];
	result.m[2][2] = scale.z * rotateXYZmatrix.m[2][2];
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;

	// w=1がデカルト座標系であるので(x,y,1)のベクトルとしてmatrixとの積をとる
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	// w=1がデカルト座標系であるので、w除算することで同次座標をデカルト座標に戻す
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}