

#include "Weapons/TPSBaseWeaponGame.h"
#include "Player/TPSBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC( LogTPSBaseWeapon, All, All)


ATPSBaseWeaponGame::ATPSBaseWeaponGame()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>( "Weapon Mesh" );
	SetRootComponent(WeaponMesh);

}

void ATPSBaseWeaponGame::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);

	CurrentAmmo = DefaultAmmo;

}

bool ATPSBaseWeaponGame::AddAmmo(FAmmoData AmmoToAdd)
{
	if (DefaultAmmo.Clips == CurrentAmmo.Clips)
	{
		return false;
	}

	if (DefaultAmmo.Clips >= AmmoToAdd.Clips)
	{
		if (CurrentAmmo.Clips + AmmoToAdd.Clips >= DefaultAmmo.Clips)
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
		}
		else
		{
			CurrentAmmo.Clips += AmmoToAdd.Clips;
		}
	}
	else 
	{
		CurrentAmmo.Clips = DefaultAmmo.Clips;
	}
	bIsNeedAmmo = false;
	return true;

}

void ATPSBaseWeaponGame::StartFire()
{
	if (ReloadInProgress ) return;

	if (CurrentAmmo.Bullets == 0)
		Reload();

	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ATPSBaseWeaponGame::MakeShot, TimeBetweenShots, true, 0.2f);
}

void ATPSBaseWeaponGame::StopFire()
{
	if (!IsValid(this))
		return;
	if (ShotTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	}
}

void ATPSBaseWeaponGame::Reload()
{
	
	if (IsClipEmpty())
	{
		UE_LOG(LogTPSBaseWeapon, Warning, TEXT("No clips"));
		StopFire();
		bIsNeedAmmo = true;
		return;
	}

	if (CurrentAmmo.Bullets == DefaultAmmo.Bullets)
	{
		return;
	}

	bIsNeedAmmo = false;
	ReloadInProgress = true;

	WeaponMesh->PlayAnimation(ReloadAnim, false);
	float lReloadAnimTime = ReloadAnim->GetPlayLength();

	GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &ATPSBaseWeaponGame::SwitchIsReload, 1.0f, false, lReloadAnimTime);

	CurrentAmmo.Clips--;
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;

}

bool ATPSBaseWeaponGame::IsCanReload() const
{
	return false;
}

void ATPSBaseWeaponGame::DecreaseAmmo()
{
	CurrentAmmo.Bullets--;

	if (CurrentAmmo.Bullets == 0 && !ReloadInProgress)
	{
		UE_LOG(LogTPSBaseWeapon, Warning, TEXT("Clip is empty"));

		StopFire();
		Reload();

		return;
	}
}

bool ATPSBaseWeaponGame::IsClipEmpty() const
{
	return CurrentAmmo.Clips == 0;
}

void ATPSBaseWeaponGame::SwitchIsReload()
{
	ReloadInProgress = !ReloadInProgress;
}

//
void ATPSBaseWeaponGame::MakeShot()
{
	if (!WeaponMesh || !FireAnim)
	{
		UE_LOG(LogTPSBaseWeapon, Error, TEXT("WeaponMesh or FireAnim is null in MakeShot()"));
		return;
	}

	//UE_LOG(LogTPSBaseWeapon, Warning, TEXT("Bam-bam"));
	FVector lTraceStart, lTraceEnd;
	GetTraceData(lTraceStart, lTraceEnd);

	if (bIsTraceShoot)
	{
		/* Line Trace Shoot */
		FHitResult lHitResult;
		MakeTraceHit(lHitResult, lTraceStart, lTraceEnd);
	}
	else
	{
		/* Projectile Shoot */
		FRotator lMuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
		CreateProjectile(lTraceStart, lMuzzleRotation);
	}


	WeaponMesh->PlayAnimation(FireAnim, false);

	DecreaseAmmo();
}

//
bool ATPSBaseWeaponGame::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	const auto lBaseCharacter = Cast<ATPSBaseCharacter>(GetOwner());
	if (!lBaseCharacter)
		return false;

	TraceStart = GetMuzzleWorldLocation();

	FVector lAimPoint;
	FHitResult lHit;
	if (lBaseCharacter->GetAimPoint(lHit, lAimPoint, TraceMaxDistance))
		return false;

	FVector lShootDirection = (lAimPoint - TraceStart).GetSafeNormal();
	if (lShootDirection.IsNearlyZero())
	{
		lShootDirection = lBaseCharacter->GetBaseAimRotation().Vector();
	}
	
	TraceEnd = TraceStart + lShootDirection * TraceMaxDistance;
	return true;
}

//
FVector ATPSBaseWeaponGame::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

void ATPSBaseWeaponGame::MakeTraceHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld())
		return;

	FCollisionQueryParams lParams;
	lParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, lParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, 1.f, 1.f);
}

