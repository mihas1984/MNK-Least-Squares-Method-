#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();
private slots:
    /// \brief goBackClicked()
    ///closes current Helpdialog
    void goBackClicked();
signals:
    /// \brief closes()
    ///emitted to mainwindow, when this dialog closes
    void closes();
private:
    /// \brief closeEvent()
    /// intercepted to emit closes() signal
    /// \param event - close event (will be accepted)
    void closeEvent(QCloseEvent* event);
    /// \brief resizeEvent()
    /// when dialog resizes scrollarea child resizes as well (to fill the dialog)
    /// and also position of goBack button is changed to keep it on the right bottom corner
    /// (such a construction created for keeping button fixed and above all layouts without losing layouts' advantages)
    /// \param event - resize event (will be accepted)
    void resizeEvent(QResizeEvent *event);
    /// \brief reject()
    /// intercepted for times when dialog is closed non-traditionally (with Esc for example)
    /// emits closes()
    void reject();

    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
