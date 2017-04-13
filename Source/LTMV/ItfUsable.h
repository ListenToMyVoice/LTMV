#pragma once

#include "ItfUsable.generated.h"

UINTERFACE(Blueprintable)
class LTMV_API UItfUsable : public UInterface {
    GENERATED_UINTERFACE_BODY()
};

class LTMV_API IItfUsable {
    GENERATED_IINTERFACE_BODY()

public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
    void Use();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pressable")
	void Press();

};