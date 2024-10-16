// Copyright 2020-2022 Rapyuta Robotics Co., Ltd.

#include "Robots/RRBaseRobotROSController.h"

// RapyutaSimulationPlugins
#include "Robots/RRBaseROS2Interface.h"
#include "Robots/RRBaseRobot.h"
#include "logUtilities.h"
#include "Robots/RRRobotROS2Interface.h"



void ARRBaseRobotROSController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    auto* robot = Cast<ARRBaseRobot>(InPawn);
    if (robot)
    {
        robot->InitROS2Interface();
        ROS2Interface = static_cast<URRBaseROS2Interface*>(robot->ROS2Interface.Get());    }
    else
    {
        URRBaseROS2InterfaceComponent* ROS2InterfaceComponent = InPawn->FindComponentByClass<URRBaseROS2InterfaceComponent>();
        if (ROS2InterfaceComponent)
        {
            ROS2Interface = ROS2InterfaceComponent->ROS2Interface;
            if (ROS2Interface)
            {
                ROS2Interface->Initialize(InPawn);
            }
            else
            {
                UE_LOG_WITH_INFO(LogRapyutaCore, Warning, TEXT("Pawn does not have ROS2Interface as a member."));
            }
        }
        else
        {
            UE_LOG_WITH_INFO(LogRapyutaCore, Warning, TEXT("Pawn does not have ROS2InterfaceComponent as a child component."));
        }
    }
}

void ARRBaseRobotROSController::OnUnPossess()
{
    auto* robot = GetPawn<ARRBaseRobot>();
    if (robot)
    {
        robot->DeInitROS2Interface();
    }
    else
    {
        if (ROS2Interface)
        {
            ROS2Interface->DeInitialize();
        }
        else
        {
            UE_LOG_WITH_INFO(LogRapyutaCore, Log, TEXT("Pawn does not have ROS2Interface as a member."));
        }
    }
    Super::OnUnPossess();
}
