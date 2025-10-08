#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillNode.h"
#include "SkillNodeWidget.generated.h"

UCLASS()
class SKILLTREEUI_API USkillNodeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "SkillNode")
    FName NodeID;

    UPROPERTY(meta = (BindWidgetOptional))
    class UButton* NodeButton;

    UPROPERTY(meta = (BindWidgetOptional))
    class UTextBlock* NodeLabel;

    UPROPERTY(BlueprintReadWrite)
    class USkillTreeWidget* OwningTree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillNode")
    float HoldDuration = 1.5f;

    virtual bool Initialize() override;

    void InitializeFromNode(const FSkillNode& Node);

protected:
    UFUNCTION()
    void OnPressed();

    UFUNCTION()
    void OnReleased();

    FTimerHandle HoldTimerHandle;

    void ConfirmHeldPurchase();
};
