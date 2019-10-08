#include "CarManager.h"
#include "GameData.h"
#include "Engine/World.h"
#include "CarAI.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Trees.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/OutputDeviceDebug.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TrackSpline.h"

ACarManager::ACarManager() {
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	sphere->SetSphereRadius(1000.0f);
	sphere->SetCollisionProfileName("Start");
	sphere->OnComponentBeginOverlap.AddDynamic(this, &ACarManager::OnOverlap);
	sphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FClassFinder<UUserWidget> HUD_Finder(TEXT("WidgetBlueprint'/Game/Menu/HUDWidget.HUDWidget_C'"));
	HUD = CreateWidget<UUserWidget>(GetWorld(), HUD_Finder.Class);
}
void ACarManager::BeginPlay() {
	Super::BeginPlay();
	track->generate();
	trees->generate();

	m_data = Cast<UGameData>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (m_data) {
		cars.Emplace(spawn_a_car(0, false), m_data->lap_number);
		for (int i = 1; i < m_data->AI_car_number + 1; i++)
			cars.Emplace(spawn_a_car(i, true), m_data->lap_number);

		if (m_data->time_limit != 0.f)
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACarManager::OutOfTime, m_data->time_limit, false);

		if (m_data->type == RaceType::Duel || m_data->type == RaceType::Survival)
			for (auto car : cars)
				car.Get<0>()->fire_allowed = true;
		else
			for (auto car : cars)
				car.Get<0>()->fire_allowed = false;
	
		m_data->current_time = 0;
		m_data->laps_left = m_data->lap_number;
	}

	HUD->AddToViewport();
	start_time = std::chrono::high_resolution_clock::now();
}
void ACarManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (m_data) {
		m_data->current_time = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start_time).count();
	}
	USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
	if (!Savefile)
		Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));

	const FString command = FString::Printf(TEXT("update %f %f %d %d %f"), 
											m_data->current_time / 1000, 
											Savefile->record / 1000, 
											m_data->laps_left,
											cars[0].Get<0>()->get_gear(),
											cars[0].Get<0>()->get_speed());
	static FOutputDeviceDebug temp;
	HUD->CallFunctionByNameWithArguments(*command, temp, this, true);
}

void ACarManager::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (m_data->type != RaceType::Survival) {
		auto found = cars.FindByPredicate([OtherActor](auto const& tuple) {
			return OtherActor == tuple.Key;
		});
		if (found) {
			found->Get<1>()--;
			if (found->Get<1>() == 0)
				if (found->Get<0>() == cars[0].Get<0>())
					Win(found->Get<0>());
				else
					Lose();
			if (found->Get<0>() == cars[0].Get<0>() && m_data)
				m_data->laps_left = found->Get<1>();
		}
	}
}
void ACarManager::OutOfTime() {
	if (m_data->type == RaceType::Time)
		Lose();
}

void ACarManager::Win(ACar *car) {
	auto finish_time = std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - start_time).count();
	if (car == cars[0].Get<0>()) {
		USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
		Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
		if (!Savefile)
			Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));

		Savefile->Money += 400;
		if (finish_time < Savefile->record) {
			Savefile->record = finish_time;
			Savefile->Money += 200;
		}

		UGameplayStatics::SaveGameToSlot(Savefile, Savefile->SaveSlotName, Savefile->UserIndex);
		//UGameplayStatics::SetGamePaused(GetWorld(), true);
		if (VictoryWidget) {
			VictoryWidget->AddToViewport();
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACarManager::end_level, 3, false);
		} else
			end_level();
		WinEvent();
	} else
		Lose();
}
void ACarManager::Lose() {
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
	if (DefeatWidget) {
		DefeatWidget->AddToViewport();
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACarManager::end_level, 3, false);
	} else
		end_level();
	LoseEvent();
}
void ACarManager::end_level() {
	if (VictoryWidget) VictoryWidget->RemoveFromParent();
	if (DefeatWidget) DefeatWidget->RemoveFromParent();
	HUD->RemoveFromParent();
	//UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap");
}
void ACarManager::has_died(ACar *car) {
	if (car == cars[0].Get<0>())
		Lose();
	else if (m_data->type == RaceType::Survival || m_data->type == RaceType::Duel) {
		auto found = cars.FindByPredicate([car](auto const& tuple) {
			return car == tuple.Key;
		});
		if (found) {
			//cars.Remove(*found);
			if (cars.Num() <= 1)
				Win(cars[0].Get<0>());
		}
	}
}
class ACar* ACarManager::spawn_a_car(int index, bool ai) {
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	const float distance_bias = 1000;
	ACar *ret = nullptr;
	float distance = 0;
	float flip_flop = 1.f;
	while (!ret) {
		auto location = track->m_spline->GetLocationAtDistanceAlongSpline(flip_flop * distance + distance_bias, ESplineCoordinateSpace::World);
		//auto direction = track->m_spline->GetDirectionAtDistanceAlongSpline(flip_flop * distance + distance_bias, ESplineCoordinateSpace::World);
		//location += FVector(direction.Y, -direction.X, 0) * 400 * flip_flop;
		if (ai)
			ret = GetWorld()->SpawnActor<ACarAI>(location + FVector(0, 0, 100), FRotator(0, 90, 0), params);
		else
			ret = GetWorld()->SpawnActor<ACar>(location + FVector(0, 0, 100), FRotator(0, 90, 0), params);

		distance += 600;
		flip_flop = -flip_flop;
	}

	ret->PauseWidget = PauseWidget;
	ret->track = track;
	ret->EnableManagement(this);
	if (ai) {
		ret->update_name(*(TEXT("AI ") + FString::FromInt(index)));
		ret->SpawnDefaultController();
	} else {
		GetWorld()->GetFirstPlayerController()->UnPossess();
		GetWorld()->GetFirstPlayerController()->Possess(ret);
	}
	return ret;
}
/*
	auto location = track->m_spline->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
	auto rotation = track->m_spline->FindRotationClosestToWorldLocation(location, ESplineCoordinateSpace::World);
	while (!TeleportTo(location, rotation)) {
		auto direction = track->m_spline->FindDirectionClosestToWorldLocation(location, ESplineCoordinateSpace::World);
		location = track->m_spline->FindLocationClosestToWorldLocation(location - 50 * direction, ESplineCoordinateSpace::World);
		rotation = track->m_spline->FindRotationClosestToWorldLocation(location, ESplineCoordinateSpace::World);
	}

auto location = track->m_spline->GetLocationAtDistanceAlongSpline((i % 2 == 0 ? +1 : -1) * 1200 * (i + 1) / 2, ESplineCoordinateSpace::World);
auto car = GetWorld()->SpawnActor<ACar>(location + FVector((i % 2 == 0 ? +1 : -1) * 400, 0, 0),
										FRotator(0, 90, 0), params);
car->track = track;
car->EnableManagement(this);
*/