#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeComponent.h"
#include "SkillTreeWidget.generated.h"

UCLASS()
class SKILLTREEUI_API USkillTreeWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize() override;

    UPROPERTY(BlueprintReadWrite, Category = "SkillTree")
    USkillTreeComponent* SkillComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
    TSubclassOf<class USkillNodeWidget> SkillNodeWidgetClass;

    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* NodeContainer;

    UPROPERTY(meta = (BindWidgetOptional))
    class UTextBlock* PointsText;

    UFUNCTION()
    void RefreshFromComponent();

    UFUNCTION(BlueprintCallable)
    bool TrySelectNode(FName NodeID);

    UFUNCTION(BlueprintCallable)
    void SetSkillComponent(USkillTreeComponent* InComp);

protected:
    FName SelectedNodeID;

    UFUNCTION(BlueprintCallable)
    bool ConfirmPurchase();

    void ShowNotification(const FString& Msg);

    UFUNCTION()
    void OnNodeStateChanged_Internal(FName NodeID);     
};

