#include "SkillTreeWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "SkillNodeWidget.h"
#include "Kismet/GameplayStatics.h"

bool USkillTreeWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    return true;
}

void USkillTreeWidget::SetSkillComponent(USkillTreeComponent* InComp)
{
    if (SkillComp)
    {
        SkillComp->OnSkillPointsChanged.RemoveAll(this);
        SkillComp->OnNodeStateChanged.RemoveAll(this);
    }

    SkillComp = InComp;

    if (SkillComp)
    {
        SkillComp->OnSkillPointsChanged.AddDynamic(this, &USkillTreeWidget::RefreshFromComponent);
        SkillComp->OnNodeStateChanged.AddDynamic(this, &USkillTreeWidget::OnNodeStateChanged_Internal);
    }

    RefreshFromComponent();
}

void USkillTreeWidget::OnNodeStateChanged_Internal(FName NodeID)
{
    RefreshFromComponent();
}

void USkillTreeWidget::RefreshFromComponent()
{
    if (!SkillComp) return;

    if (PointsText)
    {
        PointsText->SetText(FText::FromString(FString::Printf(TEXT("Puntos: %d"), SkillComp->CurrentSkillPoints)));
    }

    if (!NodeContainer) return;

    NodeContainer->ClearChildren();

    UGridPanel* Grid = Cast<UGridPanel>(NodeContainer);
    if (!Grid || !SkillNodeWidgetClass) return;

    TArray<FName> Branches;
    for (const FSkillNode& Node : SkillComp->Nodes)
    {
        if (!Branches.Contains(Node.Branch))
            Branches.Add(Node.Branch);
    }

    int32 MaxDepth = 0;
    for (const FSkillNode& Node : SkillComp->Nodes)
    {
        int32 Depth = 0;
        FName Parent = Node.ParentID;
        while (!Parent.IsNone())
        {
            const FSkillNode P = SkillComp->GetNodeByID(Parent);
            if (!P.NodeID.IsNone())
            {
                Depth++;
                Parent = P.ParentID;
            }
            else break;
        }
        MaxDepth = FMath::Max(MaxDepth, Depth);
    }

    for (int32 Col = 1; Col <= MaxDepth + 1; ++Col)
    {
        UTextBlock* Header = NewObject<UTextBlock>(this);
        Header->SetText(FText::FromString(FString::Printf(TEXT("Nivel %d"), Col)));
        Header->SetJustification(ETextJustify::Center);
        Header->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        UGridSlot* GridSlot = Grid->AddChildToGrid(Header, 0, Col);
        GridSlot->SetPadding(FMargin(8.f));
        GridSlot->SetHorizontalAlignment(HAlign_Center);
    }

    for (int32 Row = 0; Row < Branches.Num(); ++Row)
    {
        UTextBlock* BranchTitle = NewObject<UTextBlock>(this);
        BranchTitle->SetText(FText::FromName(Branches[Row]));
        BranchTitle->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        BranchTitle->SetJustification(ETextJustify::Right);

        UGridSlot* GridSlot = Grid->AddChildToGrid(BranchTitle, Row + 1, 0);
        GridSlot->SetPadding(FMargin(8.f));
        GridSlot->SetHorizontalAlignment(HAlign_Right);
    }

    for (const FSkillNode& Node : SkillComp->Nodes)
    {
        USkillNodeWidget* NodeWidget = CreateWidget<USkillNodeWidget>(this, SkillNodeWidgetClass);
        if (!NodeWidget) continue;

        NodeWidget->OwningTree = this;
        NodeWidget->InitializeFromNode(Node);

        int32 Row = Branches.IndexOfByKey(Node.Branch) + 1;
        int32 Level = 1;
        FName Parent = Node.ParentID;
        while (!Parent.IsNone())
        {
            const FSkillNode ParentNode = SkillComp->GetNodeByID(Parent);
            if (!ParentNode.NodeID.IsNone())
            {
                Level++;
                Parent = ParentNode.ParentID;
            }
            else break;
        }

        UGridSlot* GridSlot = Grid->AddChildToGrid(NodeWidget, Row, Level);
        GridSlot->SetPadding(FMargin(10.f));
        GridSlot->SetHorizontalAlignment(HAlign_Center);
        GridSlot->SetVerticalAlignment(VAlign_Center);
    }
}


bool USkillTreeWidget::TrySelectNode(FName NodeID)
{
    if (!SkillComp) return false;

    const FSkillNode Node = SkillComp->GetNodeByID(NodeID);
    if (Node.NodeID.IsNone())
    {
        ShowNotification(TEXT("Nodo no encontrado"));
        return false;
    }

    if (Node.State == ESkillState::Purchased)
    {
        ShowNotification(TEXT("Este nodo ya está adquirido"));
        return false;
    }

    if (Node.State == ESkillState::Locked)
    {
        ShowNotification(TEXT("Nodo bloqueado: desbloquea el anterior"));
        return false;
    }

    SelectedNodeID = NodeID;
    return ConfirmPurchase();
}

bool USkillTreeWidget::ConfirmPurchase()
{
    if (!SkillComp) return false;
    if (SelectedNodeID.IsNone()) return false;

    FString Fail;
    if (SkillComp->PurchaseNode(SelectedNodeID, Fail))
    {
        ShowNotification(TEXT("Mejora adquirida"));
        RefreshFromComponent();
        SelectedNodeID = NAME_None;
        return true;
    }
    else
    {
        ShowNotification(Fail);
        return false;
    }
}

void USkillTreeWidget::ShowNotification(const FString& Msg)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, Msg);
}



