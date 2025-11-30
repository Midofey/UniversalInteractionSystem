// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSALINTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UpdateInteractionWidget(class UUniversalInteractable* InteractionComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateInteractionWidget();

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	class UUniversalInteractable* OwningInteractionComponent;

	
};
