#include "MainWindow.h"

#include "ui_MainWindow.h"
#include "Support.h"

#include <QClipboard>
#include <QFileDialog>
#include <QSettings>
#include <QScrollBar>

#include <QtDebug>

#include <sstream>

static const QColor
    brown(130, 85, 0),
    orange(244, 165, 0),
    pink(252, 0, 235);

static const std::array BuiltinColors{
        QColor(Qt::black),
        QColor(Qt::white),
        QColor(Qt::red),
        QColor(Qt::green),
        QColor(Qt::blue),
        QColor(Qt::yellow),
        orange,
        brown,
        pink
    };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionPlain->setData( QVariant::fromValue(BoardBackground(BoardBackground::Type::Plain)) );
    ui->actionLines->setData( QVariant::fromValue(BoardBackground(BoardBackground::Type::Lines)) );
    ui->actionGrid->setData( QVariant::fromValue(BoardBackground(BoardBackground::Type::Grid)) );

    Q_ASSERT( BuiltinColors.size() <= 9 );
    auto iconSize = ui->colorToolBar->iconSize();
    unsigned i=0;
    for( auto &color : BuiltinColors ) {
        QPixmap pix(iconSize);
        pix.fill(color);
        QAction *action = ui->colorToolBar->addAction(QIcon( std::move(pix) ), "Color");

        action->setData( color );
        char keyStr[2] = "1";
        keyStr[0] += i++;
        action->setShortcut( QKeySequence( QString(keyStr) ) );
        connect( action, SIGNAL(triggered()), this, SLOT(setPenColor()) );

        addAction(action);
    }

    addAction(ui->actionExit_Full_Screen);

    for( auto &action : ui->menuBar->actions() ) {
        adoptMenuActions(this, action);
    }

    statusPenSize = new QLabel();
    ui->statusBar->addWidget(statusPenSize);
    statusPenColor = new QLabel();
    ui->statusBar->addWidget(statusPenColor);

    updateStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::performPaste() {
    QPixmap pixmap = qGuiApp->clipboard()->pixmap();

    if( !pixmap.isNull() ) {
        ui->board->newBackground( std::move(pixmap) );
    }
}

void MainWindow::resetBoard() {
    ui->board->clearBoard();
}

void MainWindow::toggleFullScreen() {
    if( isFullScreen() ) {
        unimportantWidgetsVisibility(true);
        showNormal();
    } else {
        unimportantWidgetsVisibility(false);
        showFullScreen();
    }
}

void MainWindow::exitFullScreen() {
    if( !isFullScreen() )
        return;

    toggleFullScreen();
}

void MainWindow::setPenColor() {
    auto senderAction = dynamic_cast<QAction *>(sender());
    Q_ASSERT(senderAction != nullptr); // setPenColor called not by signal, not from action
    ui->board->setPenColor(senderAction->data().value<QColor>());
    updateStatusBar();
}

void MainWindow::backgroundType() {
    auto senderAction = dynamic_cast<QAction *>(sender());
    Q_ASSERT(senderAction != nullptr); // backgroundType called not by signal, not from action
    ui->board->newBackground(senderAction->data().value<BoardBackground>());
}

void MainWindow::loadImage() {
    QSettings settings;
    QVariant lastDir = settings.value("LastOpenedFile");

    QFileDialog dlg(this, "Open background image", lastDir.isValid() ? lastDir.toString() : nullptr);
    dlg.setAcceptMode( QFileDialog::AcceptMode::AcceptOpen );
    dlg.setFileMode( QFileDialog::FileMode::ExistingFile );
    dlg.setOption( QFileDialog::ReadOnly );

    if( dlg.exec() ) {
        QString filename = dlg.selectedFiles()[0];
        ui->board->newBackground(QPixmap(filename));

        settings.setValue("LastOpenedFile", filename);
    }
}

void MainWindow::saveImageAs() {
    QSettings settings;
    QVariant lastDir = settings.value("LastSavedFile");

    QFileDialog dlg(this, "Save image", lastDir.isValid() ? lastDir.toString() : nullptr);
    dlg.setAcceptMode( QFileDialog::QFileDialog::AcceptSave );
    dlg.setFileMode( QFileDialog::FileMode::AnyFile );

    if( dlg.exec() ) {
        QString filename = dlg.selectedFiles()[0];
        saveImage(filename);
    }
}

void MainWindow::saveImage() {
    if( savedImage.isEmpty() )
        saveImageAs();
    else
        saveImage(savedImage);
}

void MainWindow::saveImage(QString filename) {
    QSettings settings;
    const QPixmap &image = ui->board->getForeground();
    image.save(filename, nullptr);

    savedImage = filename;
    settings.setValue("LastSavedFile", filename);
    ui->actionSaveImage->setEnabled(true);
    ui->actionSaveImage->setShortcuts( ui->actionSaveImageAs->shortcuts() );
    ui->actionSaveImageAs->setShortcuts( {} );
}

void MainWindow::extraWindow(bool enable) {
    if( enable ) {
        if( secondary )
            return;

        secondary = new SecondaryWindow(this, ui->board);
        secondary->show();
    } else {
        if( !secondary )
            return;

        secondary->close();
        secondary = nullptr;
        ui->actionSecond_Window->setChecked(false);
    }
}

void MainWindow::penSizeChanged() {
    updateStatusBar();
}

void MainWindow::widgetNeedsScroll(QPointF delta) {
    ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()-delta.x());
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value()-delta.y());
}

// Private methods
void MainWindow::unimportantWidgetsVisibility(bool show) {
    ui->menuBar->setVisible(show);
    ui->colorToolBar->setVisible(show);
    ui->statusBar->setVisible(show);
}

void MainWindow::updateStatusBar() {
    QPixmap color( statusPenColor->size().height(), statusPenColor->size().height() );
    color.fill( ui->board->getPenColor() );
    statusPenColor->setPixmap( std::move(color) );

    std::stringstream formatter;
    formatter<<"Pen size: "<<ui->board->getPenSize();
    statusPenSize->setText(formatter.str().c_str());
}
