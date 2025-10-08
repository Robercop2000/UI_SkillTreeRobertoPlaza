#include "SkillTreeComponent.h"
#include "Engine/World.h"

USkillTreeComponent::USkillTreeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USkillTreeComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentSkillPoints = StartingSkillPoints;

    if (Nodes.Num() == 0)
    {
        InitializeDefaultTree();
    }

    UpdateAvailableStates();
    OnSkillPointsChanged.Broadcast();
}

const FSkillNode USkillTreeComponent::GetNodeByID(FName NodeID) const
{
    for (const FSkillNode& N : Nodes)
    {
        if (N.NodeID == NodeID)
            return N;
    }

	return FSkillNode();
}

bool USkillTreeComponent::CanPurchaseNode(FName NodeID, FString& OutFailReason) const
{
    const FSkillNode Node = GetNodeByID(NodeID);

    if (Node.NodeID.IsNone())
    {
        OutFailReason = TEXT("Nodo no encontrado");
        return false;
    }

    if (Node.State == ESkillState::Purchased)
    {
        OutFailReason = TEXT("Nodo ya adquirido");
        return false;
    }

    if (Node.State != ESkillState::Available)
    {
        OutFailReason = TEXT("Nodo bloqueado — desbloquea el anterior");
        return false;
    }

    if (CurrentSkillPoints < Node.Cost)
    {
        OutFailReason = TEXT("No tienes suficientes puntos");
        return false;
    }

    return true;
}

bool USkillTreeComponent::PurchaseNode(FName NodeID, FString& OutFailReason)
{
    FSkillNode* Node = nullptr;

    for (FSkillNode& N : Nodes)
    {
        if (N.NodeID == NodeID)
        {
            Node = &N;
            break;
        }
    }

    if (!Node)
    {
        OutFailReason = TEXT("Nodo no encontrado");
        return false;
    }

    if (!CanPurchaseNode(NodeID, OutFailReason))
    {
        return false;
    }

    CurrentSkillPoints -= Node->Cost;
    Node->State = ESkillState::Purchased;

    OnSkillPointsChanged.Broadcast();
    OnNodeStateChanged.Broadcast(NodeID);

    UpdateAvailableStates();

    return true;
}

void USkillTreeComponent::AddSkillPoints(int32 Amount)
{
    CurrentSkillPoints += Amount;
    OnSkillPointsChanged.Broadcast();
}

void USkillTreeComponent::UpdateAvailableStates()
{
    for (FSkillNode& N : Nodes)
    {
        if (N.State == ESkillState::Purchased)
            continue;

        if (N.ParentID.IsNone())
        {
            N.State = ESkillState::Available;
        }
        else
        {
            const FSkillNode Parent = GetNodeByID(N.ParentID);

            if (!Parent.NodeID.IsNone())
            {
                if (Parent.State == ESkillState::Purchased)
                {
                    N.State = ESkillState::Available;
                }
                else
                {
                    N.State = ESkillState::Locked;
                }
            }
            else
            {
                N.State = ESkillState::Locked;
            }
        }
    }

    OnSkillPointsChanged.Broadcast();
}

void USkillTreeComponent::InitializeDefaultTree()
{
    Nodes.Empty();

    FSkillNode m1; m1.NodeID = "Move_1"; m1.DisplayName = FText::FromString("Speed I"); m1.Cost = 1; m1.ParentID = NAME_None; m1.Branch = "Movement";
    FSkillNode m2; m2.NodeID = "Move_2"; m2.DisplayName = FText::FromString("Speed II"); m2.Cost = 2; m2.ParentID = "Move_1"; m2.Branch = "Movement";

    FSkillNode h1; h1.NodeID = "Health_1"; h1.DisplayName = FText::FromString("Health I"); h1.Cost = 1; h1.ParentID = NAME_None; h1.Branch = "Health";
    FSkillNode h2; h2.NodeID = "Health_2"; h2.DisplayName = FText::FromString("Health II"); h2.Cost = 2; h2.ParentID = "Health_1"; h2.Branch = "Health";

    FSkillNode d1; d1.NodeID = "Dmg_1"; d1.DisplayName = FText::FromString("Damage I"); d1.Cost = 1; d1.ParentID = NAME_None; d1.Branch = "Damage";
    FSkillNode d2; d2.NodeID = "Dmg_2"; d2.DisplayName = FText::FromString("Damage II"); d2.Cost = 2; d2.ParentID = "Dmg_1"; d2.Branch = "Damage";

    Nodes.Add(m1); Nodes.Add(m2);
    Nodes.Add(h1); Nodes.Add(h2);
    Nodes.Add(d1); Nodes.Add(d2);

    UpdateAvailableStates();
}


