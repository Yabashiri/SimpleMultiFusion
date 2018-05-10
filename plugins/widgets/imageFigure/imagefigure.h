#ifndef IMAGEFIGURE_H
#define IMAGEFIGURE_H
#include <QObject>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/ImageFigureInterface.h"
#include "./../../../interfaces/GSRInterface.h"
#include "./../../../interfaces/RPWInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

class ImageFigure:public QWidget, public InterfacePlugin,  public ImageFigureInterface
{
    Q_OBJECT
    Q_INTERFACES( ImageFigureInterface )
    Q_INTERFACES( InterfacePlugin )

    private:

        MainWindowInterface* mainWin;
        PaintWidgetInterface* painter;
        /*GSRInterface* selection;
        RPWInterface* realPainter;*/
        QAction* Image;
        //QAction* Redo;
        //int position;
        //int maxLength;
        //QVector <QObject* > history;

    public slots:

        /*void addToHistory(QString event)
        {
            //QMessageBox::warning(this,"dsa",event);
            int count = history.size();
            if( position != count-1 )
            {

                for(int i = position+1; i<count; i++ )
                    delete history[i];

                for(int i = count-1; i>position; i-- )
                    history.remove(i);

                history.append(realPainter->getUndo());
                position++;
            }
            else if( maxLength!= count)
            {
                history.append(realPainter->getUndo());
                position++;
            }
            else if(maxLength == count)
            {
                delete history[0];
                history.remove(0);
                history.append(realPainter->getUndo());
            }

            if(position == count-1)
                Redo->setEnabled(false);

            if(position > 0)
                Undo->setEnabled(true);
        }

        void onUndo()
        {
            if(position > 0)
            {
                position--;
                //QMessageBox::warning(this,"Trace1",QString::number(position));
                realPainter->setFromUndo(history[position]);
            }

            if(position == 0)
                Undo->setEnabled(false);
            int count = history.size();
            if(position != count-1 && count != 1)
                Redo->setEnabled(true);
        }

        void onRedo()
        {
            int count = history.size();
            if(position != count-1 && count != 1)
            {
                position++;
                realPainter->setFromUndo(history[position]);
            }

            if(position == count-1 || count == 1)
                Redo->setEnabled(false);

            if(position > 0)
                Undo->setEnabled(true);
        }*/

    public:

        virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
        {

                mainWin = MAINWINDOW(parent);
                if(mainWin!=0)
                {
                    painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());


                    QMenu* editMenu = mainWin->getObjectMenu();
                    Image = editMenu->addAction(  tr( "&Загрузить изображение" ) );
                    Image->setIcon( QIcon( ":/main/images/load.png" ) );
                    connect( Image, SIGNAL( triggered( bool ) ), painter, SLOT( showRectImage()));
                    manager->addPlugins(this, "ImageFigure");
                }

        }

        virtual QString getName()const
        {
            return "ImageFigure";
        }

        ImageFigure( plugin::PluginsManager *manager ){}

        ~ImageFigure(){}


};



#endif // IMAGEFIGURE_H
