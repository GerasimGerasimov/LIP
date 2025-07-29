#pragma once

#include "Indicator.h"
#include "Slots/ParametrControlSlotXN.h"

//общий класс для индикаторов содержащих слот, для чтения 1 конкретного параметра
//TODO УДАЛИТЬ дублирование кода с ControlIndicatorSlot
class Indicator1Parametr : public Indicator
{
protected:
    ParametrControlSlotXN parametrControl;
    bool updating = false;
public:
    void setParameter(std::string& param) override;
    bool update() override;
    void stopSlot() override;
    void startSlot() override;
};

