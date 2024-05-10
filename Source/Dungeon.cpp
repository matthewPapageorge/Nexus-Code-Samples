// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon.h"

ADungeon::ADungeon()
	: m_RootComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) }
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADungeon::AddRoom(ADungeonRoom* Room)
{
	m_RoomsArray.Add(Room);
	Room->AttachToComponent(m_RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ADungeon::BeginPlay()
{
	Super::BeginPlay();
	
}


