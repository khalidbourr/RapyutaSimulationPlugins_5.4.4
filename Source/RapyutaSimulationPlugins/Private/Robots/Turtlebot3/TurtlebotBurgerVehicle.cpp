// Copyright 2020-2022 Rapyuta Robotics Co., Ltd.

#include "Robots/Turtlebot3/TurtlebotBurgerVehicle.h"

#include "Robots/RRRobotROS2Interface.h"
//#include "RRBaseRobotROSController.generated.h"
#include "Robots/Turtlebot3/RRTurtlebotROS2Interface.h"
#include "Robots/RRBaseRobotROSController.h"



ATurtlebotBurgerVehicle::ATurtlebotBurgerVehicle()
{
    SetupDefaultRootSkeletal();
}

ATurtlebotBurgerVehicle::ATurtlebotBurgerVehicle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    ROS2InterfaceClass = URRTurtlebotROS2Interface::StaticClass();
    SetupDefaultRootSkeletal();
    LidarComponent = CreateDefaultSubobject<URR2DLidarComponent>(TEXT("LidarComp"));
    LidarComponent->SetRelativeLocation(FVector(0, 0, 17.2));
    LidarComponent->bIgnoreSelf = true;
    LidarComponent->SetupAttachment(SkeletalMeshComp);
}

void ATurtlebotBurgerVehicle::SetupDefaultRootSkeletal()
{
    // [SkeletalMeshComp], due to the support for being configured in certain [ATurtlebotBurgerVehicle]'s BP classes,
    // needs to be created in ctor, thus its name must be also a constant literal.
    // Reference: AWheeledVehiclePawn
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    SkeletalMeshComp->SetCollisionProfileName(UCollisionProfile::Vehicle_ProfileName);
    SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMeshComp->SetCanEverAffectNavigation(true);
    SkeletalMeshComp->SetIsReplicated(true);
    SkeletalMeshComp->SetGenerateOverlapEvents(true);
    AddOwnedComponent(SkeletalMeshComp);
    // [SkeletalMeshComp] -> NEW ROOT
    // This is in ctor, thus no need to use [SetRootComponent()]
    RootComponent = SkeletalMeshComp;
    DefaultRoot->SetupAttachment(SkeletalMeshComp);

    AIControllerClass = ARRBaseRobotROSController::StaticClass();
}

void ATurtlebotBurgerVehicle::PreInitializeComponents()
{
    Super::PreInitializeComponents();
    if (IsDynamicRuntimeRobot() && DefaultRoot->GetAttachParent())
    {
        // NOTE: This requires [DefaultRoot] to have been attached to some NEW Root, thus without need to promote its child
        DefaultRoot->DestroyComponent();
    }
    // else must keep for static child BP legacy support
}

void ATurtlebotBurgerVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATurtlebotBurgerVehicle, SkeletalMeshComp);
}
