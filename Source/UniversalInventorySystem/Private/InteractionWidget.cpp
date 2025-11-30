// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"
#include "UniversalInteractor.h"

void UInteractionWidget::UpdateInteractionWidget(class UUniversalInteractable* InteractionComponent)
{
	OwningInteractionComponent = InteractionComponent;
	OnUpdateInteractionWidget();
}

