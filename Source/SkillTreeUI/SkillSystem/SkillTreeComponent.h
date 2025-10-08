#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillNode.h"
#include "SkillTreeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillPointsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeStateChanged, FName, NodeID);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SKILLTREEUI_API USkillTreeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillTreeComponent();
    virtual void BeginPlay() override;

    // Public editable starting points
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    int32 StartingSkillPoints = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    int32 CurrentSkillPoints = 0;

    // All nodes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    TArray<FSkillNode> Nodes;

    // Events
    UPROPERTY(BlueprintAssignable)
    FOnSkillPointsChanged OnSkillPointsChanged;

    UPROPERTY(BlueprintAssignable)
    FOnNodeStateChanged OnNodeStateChanged;

    // API
    UFUNCTION(BlueprintCallable)
    const FSkillNode GetNodeByID(FName NodeID) const;

    UFUNCTION(BlueprintCallable)
    bool CanPurchaseNode(FName NodeID, FString& OutFailReason) const;

    UFUNCTION(BlueprintCallable)
    bool PurchaseNode(FName NodeID, FString& OutFailReason);

    UFUNCTION(BlueprintCallable)
    void AddSkillPoints(int32 Amount);

    UFUNCTION(BlueprintCallable)
    void InitializeDefaultTree();

protected:
    void UpdateAvailableStates();
};
