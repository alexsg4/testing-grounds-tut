// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "TP_ThirdPerson/PatrolRouteComponent.h"
#include "BehaviorTree/BlackboardComponent.h"



EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) 
{

	//TODO Protect against no Patrol Route component

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	
	auto AIController = OwnerComp.GetAIOwner();

	//Get patrol route
	auto ControlledPawn = AIController->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetWaypoints();
	if (!PatrolPoints.Num()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points!"));
	}

	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	
	auto NextIndex = (1 + Index) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}


