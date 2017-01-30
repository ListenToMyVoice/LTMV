#pragma once

#include "ItfSwitcheable.generated.h"

USTRUCT(BlueprintType, Category = "ToggleActor")
struct FSwitcheable {
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    AActor* _actor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ToggleActor")
    TArray<FName> _components;

    FSwitcheable() {
        _actor = nullptr;
        _components = {};
    }
};

UINTERFACE(Blueprintable)
class PROYECTO_API UItfSwitcheable : public UInterface {
    GENERATED_UINTERFACE_BODY()
};

class PROYECTO_API IItfSwitcheable {
    GENERATED_IINTERFACE_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    int SwitchState();
};