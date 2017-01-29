#pragma once

#include "ItfSwitcheable.generated.h"

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