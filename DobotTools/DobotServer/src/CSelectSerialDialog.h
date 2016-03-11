#ifndef CSELECTSERIALDIALOG_H
#define CSELECTSERIALDIALOG_H

#include <QDialog>

class CSelectSerialDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CSelectSerialDialog(QWidget *parent = 0);
    ~CSelectSerialDialog();
    QString getSelectedSerial(void);
private:
    void initLayout(void);
private slots:
    void onCurrentIndexChanged(const QString &text);
private:
    QString selectedSerial;
};

#endif // CSELECTSERIALDIALOG_H
