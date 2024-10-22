#include "GameplayAbilities/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelFromXP(int32 XP)
{
	int Level = 1;
	bool bSearchint = true;
	while (bSearchint)
	{
		//说明没有配置1级以后的数据
		if (LevelUpRequirementXPS.Num() < Level)
		{
			return Level;
		}

		if (XP >= LevelUpRequirementXPS[Level - 1])
		{
			Level++;
		}
		else
		{
			bSearchint = false;
		}
	}
	return Level;
}
