

#include "UniversalInteractable.h"


UUniversalInteractable::UUniversalInteractable()
{
	InteractionDistance = 200.f;
	InteractionTime = 0.f;
	InteractableNameText = FText::FromString("Object");
	InteractableActionText = FText::FromString("Interact");
	FocusedOverlayMaterial = nullptr;
	bAutoActivate = true;
}

void UUniversalInteractable::BeginInteract(class AActor* Interactor)
{
	OnBeginInteract.Broadcast(Interactor);
}

void UUniversalInteractable::EndInteract(class AActor* Interactor)
{
	OnEndInteract.Broadcast(Interactor);
}

void UUniversalInteractable::BeginFocus(class AActor* Interactor)
{
	if (!GetOwner())
	{
		return;
	}

	TArray<UMeshComponent*> MeshComponents;
	GetOwner()->GetComponents(UMeshComponent::StaticClass(), MeshComponents);

	for (UMeshComponent* MeshComp : MeshComponents)
	{
		MeshComp->SetOverlayMaterial(FocusedOverlayMaterial);
	}
}

void UUniversalInteractable::EndFocus(class AActor* Interactor)
{
	if (!GetOwner())
	{
		return;
	}

	TArray<UMeshComponent*> MeshComponents;
	GetOwner()->GetComponents(UMeshComponent::StaticClass(), MeshComponents);

	for (UMeshComponent* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetOverlayMaterial(nullptr);
		}
	}
}

void UUniversalInteractable::Interact(class AActor* Interactor)
{
	OnInteract.Broadcast(Interactor);
}

void UUniversalInteractable::SetInteractableNameText(FText NewNameText)
{
	InteractableNameText = NewNameText;
	RefreshWidget.Broadcast();
}

void UUniversalInteractable::SetInteractableActionText(FText NewActionText)
{
	InteractableActionText = NewActionText;
	RefreshWidget.Broadcast();
}


bool UUniversalInteractable::CanInteract_Blueprint_Implementation(class AActor* Interactor, class UUniversalInteractor* InteractorComponent, FText& OutErrorText)
{
	return CanInteract(Interactor, InteractorComponent, OutErrorText);
}

bool UUniversalInteractable::CanInteract(class AActor* Interactor, class UUniversalInteractor* InteractorComponent, FText& OutErrorText)
{
	return true;
}


