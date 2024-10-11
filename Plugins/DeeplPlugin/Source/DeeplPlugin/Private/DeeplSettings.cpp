// Copyright 2023 W³odzimierz Iwanowski. All Rights Reserved.

#include "DeeplSettings.h"

UDeeplSettings::UDeeplSettings(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer),
  Endpoint("https://api-free.deepl.com/v2/translate") // by default we use end point that is specified in documentation
{
}
