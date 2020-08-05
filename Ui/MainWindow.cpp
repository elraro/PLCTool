#include "MainWindow.h"
#include "ui_MainWindow.h"


QSaneMdiSubWindow::QSaneMdiSubWindow(QString const &name)
{
  this->name = name;
}


void
QSaneMdiSubWindow::closeEvent(QCloseEvent *)
{
  emit closed(this->name);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->model = new TopologyModel(nullptr, this);
  this->ui->topologyView->setModel(this->model);

  this->connectAll();
}

void
MainWindow::connectAll(void)
{
  connect(
        this->ui->actionConnect,
        SIGNAL(toggled(bool)),
        this,
        SIGNAL(toggleStart()));

  connect(
        this->ui->actionSettings,
        SIGNAL(triggered(bool)),
        this,
        SIGNAL(openModemDialog()));
}

QSaneMdiSubWindow *
MainWindow::openWindow(
    QString const &name,
    QString const &title,
    QWidget *widget)
{
  QSaneMdiSubWindow *subWindow;

  if (findWindow(name) != nullptr)
    return nullptr;

  subWindow = new QSaneMdiSubWindow(name);
  subWindow->setWidget(widget);
  subWindow->setAttribute(Qt::WA_DeleteOnClose);
  subWindow->setWindowTitle(title);
  this->windowMap[name] = subWindow;

  connect(
        subWindow,
        SIGNAL(closed(QString)),
        this,
        SLOT(onCloseSubWindow(QString)));

  this->ui->mdiArea->addSubWindow(subWindow);

  subWindow->show();

  return subWindow;
}

bool
MainWindow::closeWindow(QString const &name)
{
  QMdiSubWindow *window = this->findWindow(name);

  if (window != nullptr)
    return false;

  return window->close();
}

QSaneMdiSubWindow *
MainWindow::findWindow(QString const &name) const
{
  if (this->windowMap.find(name) != this->windowMap.end())
    return this->windowMap[name];

  return nullptr;
}

void
MainWindow::setSubNet(const PLCTool::SubNet *sn)
{
  this->model->setSubNet(sn);
}

bool
MainWindow::connectState(void) const
{
  return this->ui->actionConnect->isChecked();
}

void
MainWindow::setConnectState(bool state)
{
  this->ui->actionConnect->setChecked(state);
}

void
MainWindow::notifySubNetChanges(void)
{
  this->model->notifyLayout();
  this->ui->topologyView->resizeColumnToContents(0);
  this->ui->topologyView->resizeColumnToContents(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

///////////////////////////////// Slots ////////////////////////////////////////

void
MainWindow::onCloseSubWindow(QString subWindow)
{
  emit closeSubWindow(subWindow);
  this->windowMap[subWindow] = nullptr;
}