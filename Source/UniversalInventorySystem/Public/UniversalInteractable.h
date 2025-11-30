// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UniversalInteractable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, class AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, class AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, class AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, class AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, class AActor*, Interactor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRefreshWidget);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSALINTERACTIONSYSTEM_API UUniversalInteractable : public UActorComponent
{
	GENERATED_BODY()

public:
	UUniversalInteractable();

	// The maximum distance that this interactable can be interacted from (in unreal units).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Universal Interaction System")
	float InteractionDistance;

	// How long it takes to interact with this interactable.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Universal Interaction System")
	float InteractionTime;

	// The name of the interactable to show in the UI.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Universal Interaction System")
	FText InteractableNameText;

	// The action to show in the UI (eg. Sit, Drink, Pickup).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Universal Interaction System")
	FText InteractableActionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Universal Interaction System")
	class UMaterialInterface* FocusedOverlayMaterial;

	virtual void BeginInteract(class AActor* Interactor);
	virtual void EndInteract(class AActor* Interactor);

	virtual void BeginFocus(class AActor* Interactor);
	virtual void EndFocus(class AActor* Interactor);

	virtual void Interact(class AActor* Interactor);

	UFUNCTION(BlueprintCallable, Category = "Universal Interaction System")
	void SetInteractableNameText(FText NewNameText);

	UFUNCTION(BlueprintCallable, Category = "Universal Interaction System")
	void SetInteractableActionText(FText NewActionText);

	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnInteract OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnBeginInteract OnBeginInteract;

	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnEndInteract OnEndInteract;

	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnBeginFocus OnBeginFocus;

	UPROPERTY(BlueprintAssignable, Category = "Universal Interaction System")
	FOnEndFocus OnEndFocus;

	FRefreshWidget RefreshWidget;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Universal Interaction System", meta =(DisplayName = "Can Interact"))
	bool CanInteract_Blueprint(class AActor* Interactor, class UUniversalInteractor* InteractorComponent, FText& OutErrorText);

	virtual bool CanInteract(class AActor* Interactor, class UUniversalInteractor* InteractorComponent, FText& OutErrorText);

};
