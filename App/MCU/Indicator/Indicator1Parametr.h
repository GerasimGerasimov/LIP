#pragma once

#include "Indicator.h"
#include "Slots/ParametrControlSlot.h"

//общий класс для индикаторов содержащих слот, для чтения 1 конкретного параметра
class Indicator1Parametr : public Indicator
{
protected:
    ParametrControlSlot parametrControl;
public:
    void setParameter(std::string& param) override;
    bool update() override;
    void stopSlot() override;
    void startSlot() override;
};

