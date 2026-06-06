
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS.h"
#include "TPSBaseWeaponGame.generated.h"


UCLASS()
class TPS_API ATPSBaseWeaponGame : public AActor
{
	GENERATED_BODY()
	
	/*METHODS*/

public:	
	ATPSBaseWeaponGame();

	void StartFire();
	void StopFire();
	void Reload();

	bool IsCanReload() const;
	void DecreaseAmmo();
	bool IsClipEmpty() const;
	void SwitchIsReload();

	void MakeShot();
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;


	FVector GetMuzzleWorldLocation() const;

	/* Trace Shoot */
	void MakeTraceHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

	/* Projectile Shoot */
	UFUNCTION (BlueprintImplementableEvent)
	void CreateProjectile(FVector StartLocation, FRotator StartRotation);

	UFUNCTION(BlueprintCallable)
	bool AddAmmo(FAmmoData AmmoToAdd);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FAmmoData GetCurrentAmmo() const
	{
		return CurrentAmmo;
	}


protected:
	virtual void BeginPlay() override;

	/*PROPERTIES*/

public:
	UPROPERTY (VisibleAnywhere, BlueprintReadWrite)
	bool bIsNeedAmmo = false;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TPS|Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Weapons")
	UAnimationAsset* FireAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Weapons")
	UAnimationAsset* ReloadAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPS|Weapons")
	float TimeBetweenShots = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Weapons")
	FTimerHandle ShotTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Weapons")
	FTimerHandle ReloadTimerHandle;

	bool ReloadInProgress = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPS|Weapons")
	FName MuzzleSocketName = "MuzzleFlash";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Weapons")
	float TraceMaxDistance = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TPS|Weapons")
	FAmmoData DefaultAmmo{ 30, 3 };
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TPS|Debug")
	bool bIsTraceShoot = true;


private:

	FAmmoData CurrentAmmo;

};
