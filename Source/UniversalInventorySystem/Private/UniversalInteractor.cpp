
#include "UniversalInteractor.h"
#include "UniversalInteractable.h"
#include "InteractionWidget.h"


UUniversalInteractor::UUniversalInteractor()
{
	InteractionCheckDistance = 1000.f;
	InteractionCheckRate = 0.1f;
	InteractionSphereRadius = 0.f;
	bAutoActivate = true;
}


void UUniversalInteractor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(InteractionCheck_TimerHandle, this, &UUniversalInteractor::PerformInteractionCheck, InteractionCheckRate, true);
}


void UUniversalInteractor::Activate(bool bReset)
{
	GetWorld()->GetTimerManager().SetTimer(InteractionCheck_TimerHandle, this, &UUniversalInteractor::PerformInteractionCheck, InteractionCheckRate, true);
}


void UUniversalInteractor::Deactivate()
{
	CouldntFindInteractable();
	GetWorld()->GetTimerManager().ClearTimer(InteractionCheck_TimerHandle);
}

void UUniversalInteractor::ServerBeginInteract_Implementation()
{
	BeginInteract();
}


void UUniversalInteractor::ServerEndInteract_Implementation()
{
	EndInteract();
}

void UUniversalInteractor::PerformInteractionCheck()
{
	if (!GetOwner())
	{
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->GetController())
	{
		return;
	}

	FVector EyesLoc;
	FRotator EyesRot;

	OwnerPawn->GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	FVector Start = EyesLoc;
	FVector End = Start + (EyesRot.Vector() * InteractionCheckDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerPawn);

	FHitResult HitResult;

	const bool bUseSphereTrace = InteractionSphereRadius > 0.f;

	if (bUseSphereTrace)
	{ 
		GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat(), ECC_Visibility, FCollisionShape::MakeSphere(InteractionSphereRadius));
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
	}

	if (HitResult.GetActor())
	{
		if (UUniversalInteractable* Interactable = Cast<UUniversalInteractable>(HitResult.GetActor()->GetComponentByClass(UUniversalInteractable::StaticClass())))
		{
			float Distance = FVector::Distance(GetOwner()->GetActorLocation(), HitResult.Location);
			if (Interactable != GetInteractable() && Distance <= Interactable->InteractionDistance && Interactable->IsActive())
			{
				FoundNewInteractable(Interactable);
			}
			else if (Distance > Interactable->InteractionDistance && GetInteractable())
			{
				CouldntFindInteractable();
			}
			return;
		}
	}
	CouldntFindInteractable();
}


void UUniversalInteractor::FoundNewInteractable(class UUniversalInteractable* Interactable)
{
	if (bIsInteractHeld)
	{
		EndInteract();
	}

	if (UUniversalInteractable* OldInteractable = GetInteractable())
	{
		OldInteractable->RefreshWidget.RemoveDynamic(this, &UUniversalInteractor::RefreshWidget);
		OldInteractable->EndFocus(GetOwner());
	}

	ViewedInteractableComponent = Interactable;
	Interactable->BeginFocus(GetOwner());
	Interactable->RefreshWidget.AddDynamic(this, &UUniversalInteractor::RefreshWidget);
	OnInteractableFound.Broadcast(Interactable);
	RefreshWidget();
}


void UUniversalInteractor::CouldntFindInteractable()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(Interact_TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(Interact_TimerHandle);
	}

	if (UUniversalInteractable* OldInteractable = GetInteractable())
	{
		OldInteractable->EndFocus(GetOwner());
		OldInteractable->RefreshWidget.RemoveDynamic(this, &UUniversalInteractor::RefreshWidget);
		if (bIsInteractHeld)
		{
			EndInteract();
		}
	}


	ViewedInteractableComponent = nullptr;
	OnInteractableLost.Broadcast();
	RefreshWidget();
}

void UUniversalInteractor::BeginInteract()
{
	if (!GetOwner()->HasAuthority())
	{
		ServerBeginInteract();
	}


	bIsInteractHeld = true;
	FText ErrorInteractText;

	if (UUniversalInteractable* Interactable = GetInteractable())
	{
		if (!Interactable->CanInteract_Blueprint(GetOwner(), this, ErrorInteractText))
		{
			return;
		}

		Interactable->BeginInteract(GetOwner());

		if (FMath::IsNearlyZero(GetInteractable()->InteractionTime))
		{
			Interact();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(Interact_TimerHandle, this, &UUniversalInteractor::Interact, Interactable->InteractionTime, false);
		}
	}
}

void UUniversalInteractor::EndInteract()
{
	if (!GetOwner()->HasAuthority())
	{
		ServerEndInteract();
	}

	bIsInteractHeld = false;

	GetWorld()->GetTimerManager().ClearTimer(Interact_TimerHandle);

	if (GetInteractable())
	{
		GetInteractable()->EndInteract(GetOwner());
	}
}


float UUniversalInteractor::GetInteractProgress()
{
	if (GetInteractable())
	{
		if (FMath::IsNearlyZero(GetInteractable()->InteractionTime))
		{
			return 0.f;
		}
		else
		{
			return GetWorld()->GetTimerManager().GetTimerElapsed(Interact_TimerHandle) / GetInteractable()->InteractionTime;
		}
	}
	return 0.f;
}

void UUniversalInteractor::Interact()
{
	GetWorld()->GetTimerManager().ClearTimer(Interact_TimerHandle);

	if (UUniversalInteractable* Interactable = GetInteractable())
	{
		Interactable->Interact(GetOwner());
	}
}

void UUniversalInteractor::RefreshWidget()
{
	if (OwningInteractionWidget)
	{
		OwningInteractionWidget->UpdateInteractionWidget(GetInteractable());
	}
}
