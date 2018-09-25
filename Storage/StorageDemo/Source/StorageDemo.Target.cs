// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class StorageDemoTarget : TargetRules
{
	public StorageDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("StorageDemo");
		bForceEnableExceptions = true;
	}
}
