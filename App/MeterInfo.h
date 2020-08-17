#ifndef METERCONTEXT_H
#define METERCONTEXT_H

#include <QObject>
#include <Topology/Meter.h>
#include "PRIMEProcessor.h"
#include "DLMSProcessor.h"
#include <QVector>

class MeterInfo : public QObject
{
  Q_OBJECT

  PLCTool::Meter *mMeter;

  QDateTime lastActivity;

  QVector<Frame> frames;
  QVector<DlmsMessage> messages;

public:
  explicit MeterInfo(QObject *parent, PLCTool::Meter *meter);

  void pushFrame(Frame const &);
  void pushDlmsMessage(DlmsMessage const &);
  void pushCreds(QString password, QString ctx);

  inline QVector<Frame> *
  frameList(void)
  {
    return &this->frames;
  }

  inline QVector<DlmsMessage> *
  messageList(void)
  {
    return &this->messages;
  }

  inline PLCTool::Meter *
  meter(void) const
  {
    return this->mMeter;
  }

signals:
  void messageReceived(DlmsMessage);
  void frameReceived(Frame);
  void credentialsFound(QString, QString);

public slots:
};

#endif // METERCONTEXT_H