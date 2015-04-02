#ifndef PANELREGISTER_H
#define PANELREGISTER_H


class PanelRegister
{
    public:
        PanelRegister();
        uint8_t getData();
        uint8_t newData;
    protected:
    private:
        PanelSwitch switchBit0;
        PanelSwitch switchBit1;
        PanelSwitch switchBit2;
};

#endif // PANELREGISTER_H
