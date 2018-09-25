using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;

public class AzureStorage : ModuleRules
{
	public AzureStorage(ReadOnlyTargetRules Target) : base(Target)
	{
		string plugin = Path.GetFullPath(Path.Combine(ModuleDirectory, ".."));
		Log.TraceInformation("AzureStorage Plugin path: " + plugin);

		string include = Path.Combine(plugin, "Source", "Public");
		PublicIncludePaths.Add(include);
		Log.TraceInformation("AzureStorage Include path: " + include);

		string lib = Path.Combine(plugin, "Binaries", "ThirdParty", Target.Platform.ToString());
		PublicLibraryPaths.Add(lib);
		Log.TraceInformation("AzureStorage Lib path: " + lib);

#if UE_4_20_OR_LATER
		DirectoryReference installDir;
		if(WindowsExports.TryGetVSInstallDir(WindowsCompiler.VisualStudio2017, out installDir))
		{
			Log.TraceInformation("VS Path: " + installDir.FullName);

			// update if using a different compiler than the default
			string compilerVersion = "14.13.26128";
			string path = Path.Combine(installDir.FullName, @"VC\Tools\MSVC\" + compilerVersion + @"\atlmfc");
			PrivateIncludePaths.Add(Path.Combine(path, "include"));
			string arch = string.Empty;
			if(Target.Platform == UnrealTargetPlatform.Win64)
				arch = "x64";
			else if(Target.Platform == UnrealTargetPlatform.Win32)
				arch = "x86";
			else
				Log.TraceError("Unsupported target platform");

			PublicLibraryPaths.Add(Path.Combine(path, "lib", arch));
		}
		else
			Log.TraceError("Failed to find Visual Studio install path");

		PublicDefinitions.Add("_NO_WASTORAGE_API");
		PublicDefinitions.Add("CPPREST_FORCE_PPLX=0");
#else
		Definitions.Add("_NO_WASTORAGE_API");
#endif

		PublicAdditionalLibraries.AddRange(new string[]
		{
			"wastorage.lib",
			"cpprest_2_10.lib",
			"zlib.lib",
			"bcrypt.lib",
			"xmllite.lib",
			"crypt32.lib",
			"winhttp.lib",
			"libeay32.lib",
			"ssleay32.lib",
			"boost_system-vc140-mt.lib",
			"atls.lib"
		});

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	}
}
