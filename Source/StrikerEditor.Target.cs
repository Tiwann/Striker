using UnrealBuildTool;


public class StrikerEditorTarget : TargetRules
{
	public StrikerEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.Add("Striker");
		ExtraModuleNames.Add("EnhancedInput");
	}
}
