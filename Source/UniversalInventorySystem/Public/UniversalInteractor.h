// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UniversalInteractor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, class UUniversalInteractable*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableLost);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSALINTERACTIONSYSTEM_API UUniversalInteractor : public UActorComponent
{
	GENERATED_BODY()

public:

	UUniversalInteractor();

	// How far should we check for interactable objects (in unreal units)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Universal Interaction System")
	float InteractionCheckDistance;

	// How much time (in seconds) between interaction checks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Universal Interaction System")
	float InteractionCheckRate;

	// If greater than 0, use a sphere instead of a line trace, specifies the radius of the interaction sphere trace (in unreal units)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Universal Interaction System")
	float InteractionSphereRadius;

	UPROPERTY(BlueprintReadWrite, Category = "Universal Interaction System")
	class UInteractionWidget* OwningInteractionWidget;

	UFUNCTION(BlueprintCallable, Category = "Universal Interaction System")
	void BeginInteract();

	UFUNCTION(BlueprintCallable, Category = "Universal Interaction System")
	void EndInteract();

	UFUNCTION(BlueprintPure, Category = "Universal Interaction System")
	class UUniversalInteractable* GetInteractable() const { return ViewedInteractableComponent; }

	UFUNCTION(BlueprintPure, Category = "Universal Interaction System")
	float GetInteractProgress();

	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnInteractableFound OnInteractableFound;
	
	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnInteractableLost OnInteractableLost;

protected:

	virtual void BeginPlay() override;
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;


	UFUNCTION(Server, Reliable)
	void ServerBeginInteract();

	UFUNCTION(Server, Reliable)
	void ServerEndInteract();


	void PerformInteractionCheck();

	void FoundNewInteractable(class UUniversalInteractable* Interactable);
	void CouldntFindInteractable();

	void Interact();

	UFUNCTION()
	void RefreshWidget();

	class UUniversalInteractable* ViewedInteractableComponent;
	FTimerHandle InteractionCheck_TimerHandle;
	FTimerHandle Interact_TimerHandle;
	bool bIsInteractHeld;
};
