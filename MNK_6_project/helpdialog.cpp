#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QPixmap>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    //for red exit button:
    QPalette Pal(palette());
    Pal.setColor(QPalette::Button, QColor(33,0,0));
    ui->goBackButton->setAutoFillBackground(true);
    ui->goBackButton->setPalette(Pal);
    //connecting
    connect(ui->goBackButton, &QPushButton::clicked, this, &HelpDialog::goBackClicked);
    //working with UI
    this->setWindowFlags(Qt::Window);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->label_3->setTextInteractionFlags(Qt::TextSelectableByMouse);//email can be selected and copied to the clipboard
    ui->label->setWordWrap(true);
    ui->label_1->setWordWrap(true);
    ui->label_3->setWordWrap(true);
    ui->label_4->setWordWrap(true);
    ui->label_7->setWordWrap(true);
    ui->label_8->setWordWrap(true);
    ui->label_10->setWordWrap(true);
    ui->label_11->setWordWrap(true);
    ui->label_13->setWordWrap(true);
    ui->label_15->setWordWrap(true);
    ui->label_16->setWordWrap(true);
    ui->label_17->setWordWrap(true);
    ui->label_18->setWordWrap(true);
    ui->label_19->setWordWrap(true);
    //loading formula photo
    QPixmap image("images/error_formula.png");
    ui->label_20->setPixmap(image);
    ui->label_20->setFixedSize(image.size());
    ui->label_20->setScaledContents(true);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::goBackClicked()
{
    close();
}

void HelpDialog::closeEvent(QCloseEvent *event)
{
    emit closes();
    event->accept();
}

void HelpDialog::resizeEvent(QResizeEvent *event)
{
    ui->scrollArea->resize(this->size());//fitting the scrollarea to the parent window
    int windowHeight = this->size().height();
    int windowWidth = this->size().width();
    int buttonHeight = ui->goBackButton->height();
    int buttonWidth = ui->goBackButton->width();
    int widthOffset = 40;
    int heightOffset = 30;
    //button always stays at the right bottow of the window
    ui->goBackButton->setGeometry(windowWidth - buttonWidth - widthOffset, windowHeight - buttonHeight - heightOffset, buttonWidth, buttonHeight);
    event->accept();
}


void HelpDialog::reject()
{
    emit closes();
    QDialog::reject();
}
