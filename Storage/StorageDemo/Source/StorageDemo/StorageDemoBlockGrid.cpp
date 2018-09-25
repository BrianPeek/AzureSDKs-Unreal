// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StorageDemoBlockGrid.h"
#include "StorageDemoBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#include "was/storage_account.h"
#include "was/table.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AStorageDemoBlockGrid::AStorageDemoBlockGrid()
{
	// create a new unique identifier for the player
	uuid = utility::uuid_to_string(utility::new_uuid());

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "{0}: {1}"), FText::FromString(uuid.c_str()), FText::AsNumber(Score)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 3;
	BlockSpacing = 300.f;
}


void AStorageDemoBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AStorageDemoBlock* NewBlock = GetWorld()->SpawnActor<AStorageDemoBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
		}
	}

	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "{0}: {1}"), FText::FromString(uuid.c_str()), FText::AsNumber(Score)));
}


void AStorageDemoBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "{0}: {1}"), FText::FromString(uuid.c_str()), FText::AsNumber(Score)));

	// enter your own connection string below
	const utility::string_t storage_connection_string(_XPLATSTR("")); // <-- connection string
	azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);

	// create or get a reference to the table
	azure::storage::cloud_table_client table_client = storage_account.create_cloud_table_client();
	azure::storage::cloud_table table = table_client.get_table_reference(U("storagedemo"));
	table.create_if_not_exists();

	// create a new entity, setting the row key to the unique id
	azure::storage::table_entity entity(_XPLATSTR("StorageDemo"), uuid);

	// add columns to the row, specifically the score
	azure::storage::table_entity::properties_type& properties = entity.properties();
	properties.reserve(1);
	properties[_XPLATSTR("Score")] = azure::storage::entity_property(Score);

	// insert or update the entry in the table
	azure::storage::table_operation insert_operation = azure::storage::table_operation::insert_or_replace_entity(entity);
	azure::storage::table_result insert_result = table.execute(insert_operation);
}

#undef LOCTEXT_NAMESPACE
