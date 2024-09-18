// Fill out your copyright notice in the Description page of Project Settings.


#include "CCHillField.h"

void UCCHillField::CreateTile()
{
	Super::CreateTile();
	CreatNoneBasicFieldTile(EFieldType::HillField);
}

void UCCHillField::CreateSprite()
{
	Super::CreateSprite();

}
