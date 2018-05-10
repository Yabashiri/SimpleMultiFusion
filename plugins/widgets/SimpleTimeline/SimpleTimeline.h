#ifndef __SIMPLETIMELINE_H__
#define __SIMPLETIMELINE_H__

#include <QWidget>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QLayout>
#include <QPainter>
#include <QToolButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QSpinBox>
#include <QBitmap>
#include <QTimer>
#include <QButtonGroup>

#include "./../../../pluginTool/Plugin.h"
#include "./../../../pluginTool/InterfacePlugin.h"
#include "./../../../interfaces/TimelineInterface.h"
#include "./../../../interfaces/MainWindowInterface.h"
#include "./../../../interfaces/PaintWidgetInterface.h"

#define STEP 10
#define DISPLACEMENT 10

class Frame: QFrame
{
	Q_OBJECT

	public:
		Frame(QWidget* parent):QFrame(parent)
        {
		}

        ~Frame(){}

	protected:
		int position;

	friend class LinesDraw;
};

class NumberDraw: public QFrame
{
    Q_OBJECT

    public:

        NumberDraw(QWidget* parent, int cFr):QFrame(parent), countFrames(cFr)
        {
        }

        void paintEvent(QPaintEvent *event)
        {
        }

        virtual void setLength(int value)
        {
            countFrames = value;
            update();
        }
    private:
        int countFrames;

};

class LinesDraw: public QFrame
{
	Q_OBJECT

	signals:
		void frameAdded(int pos);
        void simpleFrameAdded(int,int);
		void frameRemoved(int pos);
		void addEnabled(bool value);
		void removeEnabled(bool value);
		void posChanged(int value, bool played);
		void lengthChanged(int value);
        void enableFrameButton(int);
        void disableFrameButton(int);
        void frameButtonChanged(int);

	public:
		LinesDraw(QWidget* parent, int cP, int f):QFrame(parent), countPos(cP), fps(f), timer(this)
        {
			cursorPos = 0;
			maxFrame = 0;
            startPosX = -1;
            gap = 100;

            mainVLayout = new QVBoxLayout( this );
            mainVLayout->addStretch();
            mainVLayout->setSpacing(0);
            mainVLayout->setMargin(0);

            connect( &timer, SIGNAL( timeout() ), this, SLOT( play() ) );
		}

        ~LinesDraw(){}

		virtual void addFrame(int pos)
		{
			Frame* f = new Frame(this);
			f->setParent(this);
            f->position = pos;
            f->setStyleSheet("background-color: rgba(37, 55, 69, 125);");
            //f->setStyleSheet("background-color: rgba(0, 0, 255, 125);");
			frames.append(f);
			layout()->addWidget(f);
			update();
		}

		virtual void clear()
		{
			for(int i=0; i<frames.size(); i++)
				delete frames[i];
			frames.clear();
		}

		virtual int currentPos()
		{
			return cursorPos;
		}

		virtual void setMaxFrame(int max)
		{
			maxFrame = max;
		}

		virtual int getMaxFrame()
		{
			return maxFrame;
		}

		virtual int getLenght()
		{
			return countPos;
		}

        int getFps() {
			return fps;
		}

	public slots:

		virtual void setLenght(int value)
		{
			if(maxFrame > value)
				QMessageBox::warning(this,"Warning:",tr("You can't set length smallest than maximum frame!"));
			else
			{
				countPos = value;
				emit lengthChanged(value);
				update();
			}
		}

        virtual void changeFrame(int f)
        {
            cursorPos = frames[f-1]->position;
            emit posChanged(cursorPos, false);
        }

		virtual void setFps(int value)
		{
			fps = value;
		}

		virtual void play()
        {
            if(cursorPos < maxFrame)
			{
				cursorPos++;
				this->setEnabled(false);
				emit posChanged(cursorPos, true);
                emit frameButtonChanged(cursorPos);
				update();
                timer.start( 1000 / fps );
			}
			else
			{
				cursorPos=0;
				this->setEnabled(true);
				emit posChanged(cursorPos, false);
                emit frameButtonChanged(cursorPos);
				update();
				timer.stop();
			}
		}

		virtual void stop()
		{
			cursorPos=0;
			this->setEnabled(true);
			emit posChanged(cursorPos, false);
            emit frameButtonChanged(cursorPos);
			update();
			timer.stop();
		}

		virtual void pause()
		{
			update();
            timer.stop();
		}

		virtual void removeFrame()
		{
			int i = isHaveFrame();
			if(i != -1)
			{
				if(i == 0)
				{
					QMessageBox::warning(this,tr("You can't delete 0 frame!!!"), tr("You can't delete 0 frame!!!"));
					return;
				}
				delete frames[i];
				frames.remove(i);
			}
			emit addEnabled(true);
			emit removeEnabled(false);
			update();
			emit frameRemoved(cursorPos);
        }

        virtual void removeSimpleFrame()
        {
            int i = isHaveFrame();
            if(i != -1)
            {
                if(i == 0)
                {
                    QMessageBox::warning(this,tr("You can't delete 0 frame!!!"), tr("You can't delete 0 frame!!!"));
                    return;
                }
                delete frames[i];
                frames.remove(i);
            }
            emit addEnabled(true);
            emit removeEnabled(false);
            update();
            emit frameRemoved(cursorPos);

            int countFrames = frames.size();
            emit disableFrameButton(countFrames+1);
            emit enableFrameButton(1);

            if(countFrames == 1)
            {
                cursorPos = 0;
                emit posChanged(cursorPos, false);
                return;
            }

            countFrames--;
            gap = 100/countFrames;
            int current_gap = gap;

            for(int i=1; i<countFrames; i++)
            {
                int old_pos = frames[i]->position;
                cursorPos = current_gap;
                addFrame();
                cursorPos = old_pos;
                removeFrame();
                current_gap += gap;
            }

            int lastFrame = frames[countFrames]->position;

            if(lastFrame != 100)
            {
                cursorPos = 100;
                addFrame();
                cursorPos = lastFrame;
                removeFrame();
            }
            cursorPos = 0;
            emit posChanged(cursorPos, false);
        }

		virtual void addFrame()
        {
            Frame* f = new Frame(this);
            f->setParent(this);
            f->position = cursorPos;
            f->setStyleSheet("background-color: rgba(37, 55, 69, 125);");
            frames.append(f);
            layout()->addWidget(f);
            if(maxFrame < cursorPos)
                maxFrame = cursorPos;
            update();
            emit frameAdded(cursorPos);
		}

        virtual void copyNextFrame(int frame, int old_pos)
        {
            Frame* f = new Frame(this);
            f->setParent(this);
            f->position = frame;
            f->setStyleSheet("background-color: rgba(37, 55, 69, 125);");
            frames.append(f);
            layout()->addWidget(f);
            if(maxFrame < cursorPos)
                maxFrame = cursorPos;

            update();
            emit simpleFrameAdded(frame, old_pos);
        }

        virtual void addSimpleFrame()
        {
            int countFrames = frames.size();
            if(countFrames == 1 )
            {
                cursorPos = 100;
                addFrame();
                emit enableFrameButton(2);
                return;
            }
            else if(countFrames == 10)
                return;
            gap = 100/countFrames;
            int current_gap = gap;
            for(int i=1; i<countFrames; i++)
            {
                int old_pos = frames[i]->position;
                cursorPos = current_gap;
                copyNextFrame(cursorPos, old_pos);
                cursorPos = old_pos;
                removeFrame();
                current_gap += gap;
            }
            cursorPos = 100;
            addFrame();
            countFrames = frames.size();
            emit enableFrameButton(countFrames);
        }

    protected:

		int isHaveFrame()
		{
			for(int i=0; i< frames.size(); i++)
			{
				if(frames[i]->position == cursorPos)
					return i;
			}
			return -1;
        }

	private:
		int countPos;
		int cursorPos;
		int startPosX;
		int maxFrame;
        int gap;
		int fps;
        QTimer timer;
        QVBoxLayout *mainVLayout;
		QFrame* cursorF;
		QLabel* cursorL;
        QVector <Frame*> frames;

	friend class Timeline;

};

class Timeline:public QWidget, public TimelineInterface, public InterfacePlugin
{
	Q_OBJECT
	Q_INTERFACES( TimelineInterface )
	Q_INTERFACES( InterfacePlugin )

	signals:
		void frameChanged( int frame );
		void setActive(QString);
        void frameClicked(int);

	public:
		virtual void createPlugin(QObject *parent, QString idParent,plugin::PluginsManager *manager)
		{
			mainWin = MAINWINDOW(parent);
			if(mainWin!=0)
			{
				painter = PAINTWIDGETINTERFACE(mainWin->getPaintWidget());
				timelineWindow = new QDockWidget(mainWin);
				timelineWindow->setWindowTitle( tr( "Timeline" ) );
				this->setParent( timelineWindow );
				timelineWindow->setWidget(this);
				mainWin->addDockWidget( Qt::BottomDockWidgetArea, timelineWindow );

				connect( painter, SIGNAL( frameChanged( qreal ) ), this, SLOT( onStateChanged() ) );
				connect( painter, SIGNAL( allLayersChanged() ), this, SLOT( onStateChanged( ) ) );
				connect( painter, SIGNAL( layerSelected() ), this, SLOT( onStateChanged( ) ) );
				connect(lines, SIGNAL( frameAdded(int) ), this, SLOT( onAddPressed(int) ) );
                connect(lines, SIGNAL(simpleFrameAdded(int,int)), this, SLOT(onSimpleAdd(int,int)));
				connect(lines, SIGNAL( frameRemoved(int) ), this, SLOT( onRemovePressed(int) ) );
                connect(lines, SIGNAL( posChanged(int,bool) ), this, SLOT( onSetFrame(int,bool) ) );
                connect(this, SIGNAL(frameClicked(int)), lines, SLOT(changeFrame(int)));
                connect(lines, SIGNAL(frameButtonChanged(int)), this, SLOT(onPlayedFrameChanged(int)));

                initTimeline();

				connect( this, SIGNAL( setActive( QString ) ), getBeep(), SIGNAL( setActive( QString ) ) );

				manager->addPlugins(this, "Timeline");
			}

		}

		virtual QString getName()const
		{
			return "Timeline";
		}

		virtual int getFps() {
			return lines->getFps();
		}

		virtual QFrame* addSeparator()
		{
			QFrame* verticalLine = new QFrame(this);
	        verticalLine->setFrameShape(QFrame::VLine);
	        verticalLine->setFrameShadow(QFrame::Raised);
	        verticalLine->setMaximumHeight(24);
	        return verticalLine;
		}

        virtual void createFrameButton(int n, QHBoxLayout* framesLayout, bool visibility)
        {
            QToolButton* currentFrame = new QToolButton();
            QString frameToString = QString::number(n);
            QString s = ":/timeline/images/frame" + frameToString + ".png";
            QIcon icon;
            icon.addPixmap(QPixmap(s), QIcon::Normal, QIcon::Off);
            s = ":/timeline/images/frame" + frameToString + "-pressed.png";
            icon.addPixmap(QPixmap(s), QIcon::Active, QIcon::On);
            currentFrame->setMinimumSize(64,64);
            currentFrame->setCheckable(true);
            currentFrame->setStyleSheet("border: 0;");
            currentFrame->setIconSize(currentFrame->minimumSize());
            currentFrame->setIcon(icon);
            currentFrame->setToolTip( "Frame " + frameToString );
            currentFrame->setAutoRaise(true);
            currentFrame->setVisible(visibility);
            connect( currentFrame, SIGNAL( clicked() ), this, SLOT( onFrameClicked() ) );
            framesLayout->addWidget(currentFrame);
            frameButtons.addButton(currentFrame, n);
        }

		Timeline( plugin::PluginsManager *manager )
		{
			int fps = 24;
			countPos = 101;

            lines = new LinesDraw(this, countPos, fps);
            numbers = new NumberDraw(this, countPos);

			QVBoxLayout *mainVLayout = new QVBoxLayout( this );
            QHBoxLayout *toolHLayout = new QHBoxLayout( this );
            QHBoxLayout *framesLayout = new QHBoxLayout( this );

            mainVLayout->addLayout(toolHLayout);
            mainVLayout->addLayout(framesLayout);

            framesLayout->setAlignment(Qt::AlignLeft);
			connect(lines, SIGNAL(addEnabled(bool)), this, SLOT(addEnabled(bool)));
            connect(lines, SIGNAL(removeEnabled(bool)), this, SLOT(removeEnabled(bool)));

            mainVLayout->setContentsMargins(10,5,10,10);
			mainVLayout->setSpacing(4);
            toolHLayout->setMargin(0);

	        toolHLayout->addWidget(addSeparator());
	        toolHLayout->addSpacing(3);

            simpleAdd.setIcon( QIcon( ":/timeline/images/addFrame.png" ) );
            simpleAdd.setToolTip( "Simple Add");
            simpleAdd.setMinimumSize(32,32);
            simpleAdd.setIconSize(simpleAdd.minimumSize());
            simpleAdd.setAutoRaise(true);
            connect( &simpleAdd, SIGNAL( clicked() ), lines, SLOT( addSimpleFrame() ) );
            toolHLayout->addWidget(&simpleAdd);

            simpleRemove.setIcon( QIcon( ":/timeline/images/removeFrame.png" ) );
            simpleRemove.setToolTip("Simple Remove");
            simpleRemove.setMinimumSize(32,32);
            simpleRemove.setIconSize(simpleRemove.minimumSize());
            simpleRemove.setAutoRaise(true);
            connect( &simpleRemove, SIGNAL( clicked() ), lines, SLOT( removeSimpleFrame() ) );
            toolHLayout->addWidget(&simpleRemove);

            simpleAdd.setEnabled(true);
            simpleRemove.setEnabled(true);

	        toolHLayout->addSpacing(3);
	        toolHLayout->addWidget(addSeparator());
	        toolHLayout->addSpacing(3);

            QIcon icon;
            icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/play.png")), QIcon::Normal, QIcon::Off);
            icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/play_check.png")), QIcon::Active, QIcon::On);
            play.setMinimumSize(28,28);
            play.setCheckable(true);
            play.setStyleSheet("border: 0;");
            play.setIconSize(play.minimumSize());
            play.setIcon(icon);
            play.setToolTip( tr( "Play" ) );
            play.setAutoRaise(true);
            play.setAutoExclusive(true);
            connect( &play, SIGNAL( clicked() ), lines, SLOT( play()) );
            toolHLayout->addWidget(&play);

            icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/stop.png")), QIcon::Normal, QIcon::Off);
            icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/stop_check.png")), QIcon::Active, QIcon::On);
            stop.setMinimumSize(28,28);
            stop.setCheckable(true);
            stop.setStyleSheet("border: 0;");
            stop.setIconSize(stop.minimumSize());
            stop.setIcon( icon );
            stop.setToolTip( tr( "Stop" ) );
            stop.setAutoRaise(true);
            stop.setAutoExclusive(true);
            stop.setChecked(true);
            connect( &stop, SIGNAL( clicked() ), lines, SLOT( stop() ) );
            toolHLayout->addWidget(&stop);

            icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/pause.png")), QIcon::Normal, QIcon::Off);
            icon.addPixmap(QPixmap(QString::fromUtf8(":/timeline/images/pause_check.png")), QIcon::Active, QIcon::On);
            pause.setMinimumSize(28,28);
            pause.setCheckable(true);
            pause.setStyleSheet("border: 0;");
            pause.setIconSize(pause.minimumSize());
            pause.setIcon(icon);
            pause.setToolTip( tr( "Pause" ) );
            pause.setAutoRaise(true);
            pause.setAutoExclusive(true);
            connect( &pause, SIGNAL( clicked() ), lines, SLOT( pause() ) );
            toolHLayout->addWidget(&pause);

            toolHLayout->addSpacing(3);
            toolHLayout->addWidget(addSeparator());
			toolHLayout->addStretch();

            createFrameButton(1, framesLayout, true);
            frameButtons.button(1)->setChecked(true);
            for(int i=2; i<=10;i++)
                createFrameButton(i, framesLayout, false);
            connect(lines, SIGNAL(enableFrameButton(int)), SLOT(onFrameButtonEnabled(int)));
            connect(lines, SIGNAL(disableFrameButton(int)), SLOT(onFrameButtonDisabled(int)));

            framesLayout->setContentsMargins(0,2,0,5);
		}

		virtual ~Timeline()
		{
		}

		virtual void initTimeline()
		{
			emit setActive(getName());

			lines->clear();
			int max = painter->maxFrame();
			lines->setMaxFrame(max);
			if(max > countPos)
			{
				countPos = max+1;
				lines->setLenght(countPos);
			}

			int countFrames = painter->countFrames();
            for(int i=0; i<countFrames && i < 10; i++)
            {
                lines->addFrame(painter->getPositionFrame(i));
                frameButtons.button(i+1)->setVisible(true);
            }
            for(int i=countFrames+1; i<=10; i++)
                frameButtons.button(i)->setVisible(false);
		}

    private slots:

        void onPlayedFrameChanged(int pos)
        {
            int f = qFloor(pos/lines->gap);
            frameButtons.button(f+1)->setChecked(true);
        }

        void onFrameClicked()
        {
            int n = frameButtons.checkedId();
            emit frameClicked(n);
        }

        void onFrameButtonEnabled(int f)
        {
            QAbstractButton* button = frameButtons.button(f);
            button->setVisible(true);
            button->setChecked(true);
        }

        void onFrameButtonDisabled(int f)
        {
            QAbstractButton* button = frameButtons.button(f);
            button->setVisible(false);
        }

		void changeFps() {
			emit setActive(getName());
        }

		void onPlayPressed()
		{
			emit setActive(getName());
		}

		void onPausePressed()
		{
			emit setActive(getName());
		}

		void onStopPressed()
		{
			emit setActive(getName());
		}


		void onRemovePressed(int frame)
		{
			emit setActive(getName());
			painter->deleteFrame(frame);
		}

		void onSetFrame(int pos, bool played)
		{
			emit setActive(getName());
			if(pos == 0)
				stop.setChecked(true);
			painter->setFrame(pos, played);
		}

		void onLengthChanged(int value)
		{
			emit setActive(getName());
            countPos = value;
            numbers->setLength(value);
		}

		void onAddPressed(int frame)
		{
            emit setActive(getName());
            painter->addSimpleFrame(frame);
        }

        void onSimpleAdd(int frame, int old_pos)
        {
            emit setActive(getName());
            painter->copyNextFrame(frame, old_pos);
        }

		void onStateChanged()
		{
			initTimeline();
		}
	private:

		MainWindowInterface* mainWin;
        PaintWidgetInterface* painter;
        QDockWidget *timelineWindow;
        QToolButton simpleAdd;
        QToolButton simpleRemove;
        QToolButton play;
		QToolButton stop;
        QToolButton pause;
        int countPos;
        NumberDraw* numbers;
        LinesDraw* lines;
        QButtonGroup frameButtons;
};

#endif /* __SIMPLETIMELINE_H__ */
