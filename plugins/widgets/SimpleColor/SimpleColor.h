//Простая палитра из 16 цветов
//Красноставская Яна, 2018

#ifndef SIMPLECOLOR_H_
#define SIMPLECOLOR_H_

#include <QtGui>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QGridLayout>
#include <QPainterPath>
#include <QVector>
#include "qdrawutil.h"

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/ColorEditorInterface.h"

class SingleColor:public QWidget
{
    Q_OBJECT

    signals:
        void colorChanged(QColor);

    public slots:
        void onColorChanged(const QColor c)
        {
            if(this->color!=c)
                drawBorder(QColor("#f0f0f0")); //делает цвет неактивным
            else
                drawBorder(Qt::black); //делает цвет активным
        }

    public:
        SingleColor(QColor c):color(c),pm(20,20),pmp(&pm)
        {
            pmp.fillRect( 0, 0, 20, 20, color ); //квадрат цвета
            drawBorder(QColor("#f0f0f0"));

            QPalette pal = palette();
            pal.setBrush( backgroundRole(), QBrush( pm ) );
            setAutoFillBackground( true );
            setPalette( pal );

            setMinimumSize( QSize( 20, 20 ) );
            setMaximumSize( QSize( 20, 20 ) );
        }

        void mousePressEvent( QMouseEvent *event )
        {
            emit colorChanged(color);
        }

    private:
        void drawBorder(QColor c)
        {
            QPainterPath p;
            p.addRect(0, 0, 19, 19);
            pmp.setPen(QPen(c, 1)); //обводка (по умолчанию цвета фона)
            pmp.drawPath(p);

            QPalette pal = palette();
            pal.setBrush( backgroundRole(), QBrush( pm ) );
            setAutoFillBackground( true );
            setPalette( pal );
        }

        QColor color;
        QPixmap pm; //область рисования виджета
        QPainter pmp; //инструмент рисования виджета
};

class SimpleColor:public QWidget, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES( InterfacePlugin )

    signals:
        void setActive(QString);
        void penChanged(QPen);
        void brushChanged(QBrush);
        void colorChanged(QColor);

    public slots:
        void onColorChanged(const QColor &c)
        {
            _pen.setColor(c);
            _brush.setStyle(Qt::SolidPattern);
            _brush.setColor(c);
            emit penChanged(_pen);
            emit brushChanged(_brush);
            emit setActive(getName());
            emit colorChanged(c);
        }

        void setPen(const QPen &pen)
        {
            if(pen == _pen)
                return;
            _pen = pen;
            emit colorChanged(_pen.color());
        }

        void onObjectCreated()
        {
            if( !mainWin->getProcessSignals() ) return;
            painter->setPen(_pen);
        }

        void onStateChanged( )
        {
            if( !mainWin->getProcessSignals() ) return;
            setPen(painter->pen());
        }

	public:
		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
        {
            connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );
            if(idParent.contains("Main"))
            {
                mainWin = MAINWINDOW(parent);
                if(mainWin!=0)
                {
                    painter = PAINTWIDGETINTERFACE( mainWin->getPaintWidget() );
                    connect( this, SIGNAL( penChanged(QPen) ),
                            mainWin, SLOT( onPenChanged(QPen) ) );
                    connect( this, SIGNAL( brushChanged(QBrush)),
                            mainWin, SLOT( onBrushChanged(QBrush) ) );
                    connect(painter, SIGNAL(figureSelected(int,int)),
                            this, SLOT(onStateChanged())); //синхронизируемся с изменением пера
                    connect(painter, SIGNAL(objectCreated()),
                            this, SLOT(onObjectCreated())); //чтобы при создании фигуры использовалась наша кисть
                }
            }
            else if(idParent.contains("ToolBox"))
			{
                _manager = manager;
                ToolBoxInterface* tool = TOOLBOX(parent);
                if(tool!=0)
                {
                    this->setParent(tool);
                    this->setToolTip( "Palette");
                    hl = new QHBoxLayout;
                    QVBoxLayout *mainLayout = &(tool->getMainLayout());
                    mainLayout->addLayout(hl);
                    hl->addWidget( this );
                    manager->addPlugins(this, "SimpleColor");

                    createPalette();
                }
            }
            else if(idParent.contains("Selection"))
            {
                selection = GSRINTEFACE(parent);
            }
		}

		virtual QString getName()const
		{
            return "SimpleColor";
        }

        SimpleColor( plugin::PluginsManager * manager )
        {
            setMinimumSize( QSize( 64, 180 ) );
            setMaximumSize( QSize( 64, 180 ) );
		}

        virtual ~SimpleColor()
		{
            SingleColor* s;
            while(!colors.empty())
            {
                s = colors.takeFirst();
                delete s;
            }
            ///на всякий случай
        }

    private:

        void createColor(const QColor &c, int row, int column)
        {
            SingleColor* s;
            s = new SingleColor(c);
            connect( s, SIGNAL( colorChanged(QColor) ),
                    this, SLOT( onColorChanged(QColor) ));
            connect(this, SIGNAL(colorChanged(QColor)),
                    s, SLOT(onColorChanged(QColor)));
            colors.append(s);
            layout->addWidget(s,row,column);
        }

        void createPalette()
        {
            layout = new QGridLayout;
            layout->setSpacing(0);

            createColor(Qt::yellow,0,0);
            createColor(Qt::cyan,0,1);
            createColor(Qt::red,1,0);
            createColor(Qt::green,1,1);
            createColor(Qt::blue,2,0);
            createColor(Qt::magenta,2,1);
            createColor(Qt::darkRed,3,0);
            createColor(Qt::darkGreen,3,1);
            createColor(Qt::darkBlue,4,0);
            createColor(Qt::darkCyan,4,1);
            createColor(Qt::darkMagenta,5,0);
            createColor(Qt::darkYellow,5,1);
            createColor(Qt::white,6,0);
            createColor(Qt::black,6,1);
            createColor(Qt::lightGray,7,0);
            createColor(Qt::darkGray,7,1);

            this->setLayout(layout);
            this->show();
        }

		GSRInterface* selection;
		MainWindowInterface* mainWin;/// Указатель на область рисования(PaintWidget)
        PaintWidgetInterface* painter;/// Указатель на панель инструментов(ToolBox)
        QObject* penEditor;
        plugin::PluginsManager *_manager;
        QGridLayout* layout;
        QHBoxLayout* hl;
        QVector<SingleColor*> colors; ///список цветов
        QPen _pen;
        QBrush _brush;
};


#endif
