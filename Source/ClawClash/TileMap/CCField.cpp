// Fill out your copyright notice in the Description page of Project Settings.


#include "CCField.h"
#include "ClawClash/TileMap/CCStageMap.h"

UCCField::UCCField()
{
	FieldType = EFieldType::NoneField;
	StartPos = 0;
	Length = 0;
}

int32 UCCField::GetStartPos()
{
	return StartPos;
}

void UCCField::AddStartPos(int32 AddedNum)
{
	StartPos += AddedNum;
}

int32 UCCField::GetLength()
{
	return Length;
}

EFieldType UCCField::GetFieldType()
{
	return FieldType;
}

void UCCField::SetStartPos(int32 NewStartPos)
{
	StartPos = NewStartPos;
}

void UCCField::Init(int32 NewStartPos, int32 NewLength, EFieldType NewFieldType)
{
	FieldType = NewFieldType;
	StartPos = NewStartPos;
	Length = NewLength;
}
