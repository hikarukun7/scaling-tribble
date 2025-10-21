#pragma once

#include "Gmath.h"
#include <Windows.h>
#include <string>

//�~�̕\��
struct Circle
{
	Vector2d pos;
	float    radius;

	Circle() : pos(ZeroVec2d), radius(1.0f) {}
	Circle(Vector2d p, float r) : pos(p), radius(r) {}
};

//�Z�`�\��
struct  Box
{
	Vector2d pos;
	float width;
	float height;

	Box() : pos(ZeroVec2d), width(1.0f), height(1.0f) {}
	Box(Vector2d p, float w, float h) : pos(p), width(w), height(h) {}
};

//�����\��
struct Segment
{
	Vector2d start;
	Vector2d end;

	Segment() : start(ZeroVec2d), end(UnitVecX2d) {}
	Segment(const Vector2d& s, const Vector2d& e) : start(s), end(e) {}
};

//�_�Ɖ~�̏Փ�(���O�j����
bool detectPointToCircleCollision(Vector2d& p, Circle& c);
//�_�ƒZ�`�̏Փˁi���O�j����
bool detectPointToBoxCollision(Vector2d& p, Box& box);
//�~�Ɖ~�̏Փ˔���
bool detectCircleCollision(Circle& c1, Circle& c2);
//�~�Ɛ����̏Փ˔���
bool detectCircleToSegmentCollision(Circle& c, Segment& seg);

//�Z�`������ɕ���
void separateBoxToSegments(const Box& rect, Segment& left, Segment& right,
	Segment& top, Segment& bottom);

//�摜���\����
struct ImageDate
{
	HBITMAP img;
	int width;
	int height;

	ImageDate() : img(nullptr), width(0), height(0) {}
};

//�摜�ǂݍ���
bool loadImageData(const std::wstring& filePath, ImageData& imgData);
//�摜�J��
void releaseImageData(ImageData& imgData);