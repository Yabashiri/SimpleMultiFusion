//Простая палитра из 16 цветов
//Красноставская Яна, 2018

#ifndef COLORPICKER_H_
#define COLORPICKER_H_

#include <QtGui>
#include <QWidget>
#include <QPainter>
#include <QColorDialog>
#include <QMouseEvent>
#include "qdrawutil.h"

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/ToolBoxInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/BrushEditorInterface.h"

class ColorPicker:public QWidget, public InterfacePlugin
{
    Q_OBJECT
    Q_INTERFACES( InterfacePlugin )

    signals:
        void setActive(QString);
        void penChanged(QPen);
        void brushChanged(QBrush);

    public slots:
        void onColorChanged(const QColor &c)
        {
            _pen.setColor(c);
            _brush.setStyle(Qt::SolidPattern);
            _brush.setColor(c);
            emit penChanged(_pen);
            emit brushChanged(_brush);
            emit setActive(getName());
        }

        void setPen(const QPen &pen)
        {
            if(pen == _pen)
                return;
            _pen = pen;
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
                    this->setToolTip( "Color Picker");
                    hl = new QHBoxLayout;
                    QVBoxLayout *mainLayout = &(tool->getMainLayout());
                    mainLayout->addLayout(hl);
                    hl->addWidget( this );
                    manager->addPlugins(this, "ColorPicker");
                }
            }
            else if(idParent.contains("Selection"))
            {
                selection = GSRINTEFACE(parent);
            }
		}

        virtual QString getName() const
		{
            return "ColorPicker";
		}

        ColorPicker( plugin::PluginsManager * manager )
        {
            QPixmap pm( 64, 64 );

            QPainter pmp( &pm );
                pmp.fillRect( 0, 0, 64, 64, Qt::white );
            pmp.end();

            QPalette pal = palette();
            pal.setBrush( backgroundRole(), QBrush( pm ) );
            setAutoFillBackground( true );
            setPalette( pal );

            setMinimumSize( QSize( 64, 64 ) );
		}

        virtual ~ColorPicker()
        {
        }

        void mousePressEvent( QMouseEvent *event )
        {
            QColor newColor = QColorDialog::getColor(_pen.color(), this);
            if( newColor.isValid() )
            {
                onColorChanged(newColor);
                update();
            }
        }

    private:

        void paintEvent( QPaintEvent *event )
        {
            QPainter p( this );

            p.setRenderHints( QPainter::Antialiasing |
                              QPainter::TextAntialiasing |
                              QPainter::SmoothPixmapTransform );

            qDrawShadePanel( &p, 0, 0, width(), height(), palette(), true, 3 );

            p.setPen( Qt::NoPen );
            p.fillRect( 3, 3, width() - 6, height() - 6, prepareBrushToPaint( _brush, rect() ) );
        }

		GSRInterface* selection;
		MainWindowInterface* mainWin;/// Указатель на область рисования(PaintWidget)
        PaintWidgetInterface* painter;/// Указатель на панель инструментов(ToolBox)
        QObject* penEditor;
        plugin::PluginsManager *_manager;
        QHBoxLayout* hl;
        QPen _pen;
        QBrush _brush;
};


#endif
