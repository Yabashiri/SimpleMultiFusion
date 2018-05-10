#ifndef __SIMPLE_PALETTE_INTERFACE__
#define __SIMPLE_PALETTE_INTERFACE__

#include <QString>

class SimplePaletteInterface
{
    public:
        virtual QString getTestString( QString param ) const = 0;
        virtual void printMessage( QString message ) = 0;

        SimplePaletteInterface() {}
        virtual ~SimplePaletteInterface() {}
};

//И, что главное, не забудьте обьявить ваш интерфейс интерфейсом:
Q_DECLARE_INTERFACE( SimplePaletteInterface, "com.alex.test.SimplePaletteInterface/1.0")

#endif // __SIMPLE_PALETTE_INTERFACE__
