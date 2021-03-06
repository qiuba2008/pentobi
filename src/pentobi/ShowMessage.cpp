//-----------------------------------------------------------------------------
/** @file pentobi/ShowMessage.cpp
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ShowMessage.h"

#include <QMessageBox>
#include "Util.h"

//-----------------------------------------------------------------------------

namespace {

void showMessage(QWidget* parent, QMessageBox::Icon icon, const QString& text,
                 const QString& infoText, const QString& detailText)
{
    QMessageBox msgBox(parent);
    Util::setNoTitle(msgBox);
    // Both setStandardButtons() and setEscapeButton() are necessary to make
    // the window close button work if detailedText is not empty (last tested
    // with Qt 5.6 on Linux)
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setEscapeButton(QMessageBox::Ok);
    msgBox.setIcon(icon);
    msgBox.setText(text);
    msgBox.setInformativeText(infoText);
    msgBox.setDetailedText(detailText);
    msgBox.exec();
}

} // namespace

//-----------------------------------------------------------------------------

void initQuestion(QMessageBox& msgBox, const QString& text,
                  const QString& infoText)
{
    Util::setNoTitle(msgBox);
    msgBox.setText(text);
    msgBox.setInformativeText(infoText);
}

void showFatal(const QString& detailedText)
{
    // Don't translate these error messages. They shouldn't occur if the
    // program is correct and if it is not, they can occur in situations
    // when the translators are not yet installed.
    QMessageBox msgBox;
    msgBox.setWindowTitle("Pentobi");
    msgBox.setIcon(QMessageBox::Critical);
    // Both setStandardButtons() and setEscapeButton() are necessary to make
    // the window close button work if detailedText is not empty (last tested
    // with Qt 5.6 on Linux)
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setEscapeButton(QMessageBox::Ok);
    msgBox.setText("An unexpected error occurred.");
    QString infoText =
        "Please report this error together with any details available with"
        " the button below and other context information at the Pentobi"
        " <a href=\"https://sf.net/p/pentobi/bugs\">bug tracker</a>.";
    msgBox.setInformativeText("<html>" + infoText);
    msgBox.setDetailedText(detailedText);
    msgBox.exec();
}

void showError(QWidget* parent, const QString& text, const QString& infoText,
               const QString& detailText)
{
    showMessage(parent,QMessageBox::Critical, text, infoText, detailText);
}

void showInfo(QWidget* parent, const QString& text, const QString& infoText,
              const QString& detailText, bool withIcon)
{
    showMessage(parent,
                withIcon ? QMessageBox::Information : QMessageBox::NoIcon,
                text, infoText, detailText);
}

//-----------------------------------------------------------------------------
