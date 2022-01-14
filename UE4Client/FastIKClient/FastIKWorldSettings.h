#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "FastIKWorldSettings.generated.h"

UCLASS()
class FASTIKCLIENT_API AFastIKWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "custom", meta = (ClampMin = "1", ClampMax = "60", UIMin = "1", UIMax = "60"))
		int Rot_Step_Size = 20;
};
