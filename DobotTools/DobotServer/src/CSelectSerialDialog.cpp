#include "CSelectSerialDialog.h"
#include <QtWidgets>
#include <QSerialPortInfo>

CSelectSerialDialog::CSelectSerialDialog(QWidget *parent)
  : QDialog(parent),
    selectedSerial("")
{
    setWindowTitle("Serial port not found");
    initLayout();
}

CSelectSerialDialog::~CSelectSerialDialog()
{

}

void CSelectSerialDialog::initLayout(void)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel *label = new QLabel(this);
    layout->addWidget(label);
    label->setText("Serial port not found!\nSelect other serial ports?");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QComboBox *comboBox = new QComboBox(this);
    comboBox->setObjectName("SerialList");
    layout->addWidget(comboBox);
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo portInfo, availablePorts) {
        comboBox->addItem(portInfo.portName());
    }
    selectedSerial = comboBox->currentText();

    QHBoxLayout *hLayout = new QHBoxLayout();
    layout->addLayout(hLayout);

    QPushButton *okBtn = new QPushButton(this);
    QPushButton *cancelBtn = new QPushButton(this);

    hLayout->addWidget(okBtn);
    hLayout->addWidget(cancelBtn);

    okBtn->setText("OK");
    cancelBtn->setText("Cancel");
    if (comboBox->count() == 0) {
        okBtn->setEnabled(false);
    }

    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

void CSelectSerialDialog::onCurrentIndexChanged(const QString &text)
{
    selectedSerial = text;
}

QString CSelectSerialDialog::getSelectedSerial(void)
{
    return selectedSerial;
}
