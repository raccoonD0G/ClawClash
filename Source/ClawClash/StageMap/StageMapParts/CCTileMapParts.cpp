// Fill out your copyright notice in the Description page of Project Settings.


#include "CCTileMapParts.h"

void UCCTileMapParts::SetTileMapPos(FIntVector2 NewTileMapPos)
{
	TileMapPos = NewTileMapPos;
}

FIntVector2 UCCTileMapParts::GetTileMapPos() const
{
	return TileMapPos;
}

void UCCTileMapParts::Init(FIntVector2 NewTileMapPos)
{
	TileMapPos = NewTileMapPos;
}

