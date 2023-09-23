#include "MTD_GameplayTags.h"

#define MTD_DEFINE(TAG_NAME, TAG, COMMENT) MTD_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_NAME, TAG, COMMENT);
namespace MTD_GameplayTags
{
	MTD_DEFINE(InputTag_Confirm, "InputTag.Confirm", "Generic confirm action.");
	MTD_DEFINE(InputTag_Cancel, "InputTag.Cancel", "Generic cancel action.");
}
#undef MTD_DEFINE

