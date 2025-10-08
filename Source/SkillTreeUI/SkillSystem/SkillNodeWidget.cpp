#include "SkillNodeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SkillTreeWidget.h"
#include "TimerManager.h"

bool USkillNodeWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (NodeButton)
    {
        NodeButton->OnPressed.AddDynamic(this, &USkillNodeWidget::OnPressed);
        NodeButton->OnReleased.AddDynamic(this, &USkillNodeWidget::OnReleased);
    }

    return true;
}

void USkillNodeWidget::InitializeFromNode(const FSkillNode& Node)
{
    NodeID = Node.NodeID;

    if (NodeLabel)
    {
        FText Label = Node.DisplayName.IsEmpty() ? FText::FromName(Node.NodeID) : Node.DisplayName;
        FText CostText = FText::Format(FText::FromString("{0}\n({1} pts)"), Label, FText::AsNumber(Node.Cost));
        NodeLabel->SetText(CostText);

        FSlateColor SlateColor;
        switch (Node.State)
        {
        case ESkillState::Locked: SlateColor = FSlateColor(FLinearColor::Gray); break;
        case ESkillState::Available: SlateColor = FSlateColor(FLinearColor::Yellow); break;
        case ESkillState::Purchased: SlateColor = FSlateColor(FLinearColor::Green); break;
        default: SlateColor = FSlateColor(FLinearColor::White); break;
        }
        NodeLabel->SetColorAndOpacity(SlateColor);
    }

    if (NodeButton)
    {
        NodeButton->SetIsEnabled(Node.State == ESkillState::Available);
        NodeButton->SetToolTipText(Node.Description);
    }
}

void USkillNodeWidget::OnPressed()
{
    if (!OwningTree || NodeID.IsNone()) return;

    GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this,
        &USkillNodeWidget::ConfirmHeldPurchase, HoldDuration, false);

    GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,
        FString::Printf(TEXT("Mantén pulsado para confirmar %s..."), *NodeID.ToString()));
}

void USkillNodeWidget::OnReleased()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(HoldTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);

        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
            FString::Printf(TEXT("Cancelado: %s"), *NodeID.ToString()));
    }
}

void USkillNodeWidget::ConfirmHeldPurchase()
{
    if (OwningTree)
    {
        if (OwningTree->TrySelectNode(NodeID))
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
                FString::Printf(TEXT("¡Confirmado! Mejora %s adquirida"), *NodeID.ToString()));
        }
    }
}

