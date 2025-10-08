#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillNode.generated.h"

UENUM(BlueprintType)
enum class ESkillState : uint8
{
    Locked UMETA(DisplayName = "Locked"),
    Available UMETA(DisplayName = "Available"),
    Purchased UMETA(DisplayName = "Purchased")
};

USTRUCT(BlueprintType)
struct FSkillNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName NodeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Cost = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ParentID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ESkillState State = ESkillState::Locked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Branch;

    FSkillNode() {}
};
