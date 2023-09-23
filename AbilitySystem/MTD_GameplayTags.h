#pragma once

#include "NativeGameplayTags.h"

#define MTD_DECLARE(TAG_NAME) MTD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_NAME)
namespace MTD_GameplayTags
{
	MTD_DECLARE(InputTag_Confirm);
	MTD_DECLARE(InputTag_Cancel);
}
#undef MTD_DECLARE

