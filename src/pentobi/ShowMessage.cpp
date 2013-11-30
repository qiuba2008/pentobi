//-----------------------------------------------------------------------------
/** @file pentobi/ShowMessage.cpp
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ShowMessage.h"

#include <QApplication>
#include <QMessageBox>
#include "Util.h"

//-----------------------------------------------------------------------------

namespace {

void showMessage(QWidget* parent, QMessageBox::Icon icon, const QString& text,
                 const QString& infoText, const QString& detailText)
{
    // Workaround to avoid very small widths if the main text is short, which
    // causes ugly word wrapping with single-word lines in the informative text.
    // Why does QMessageBox::setMinimumWidth() not work (tested in Qt 4.7)?
    QString expandedText = text;
    QFontMetrics metrics(qApp->font("QLabel"));
    int minWidth = 30 * metrics.averageCharWidth();
    while (metrics.width(expandedText) < minWidth)
        expandedText.append(" ");
    QMessageBox msgBox(parent);
    Util::setNoTitle(msgBox);
    msgBox.setIcon(icon);
    msgBox.setText(expandedText);
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
    // Workaround to avoid very small widths if the main text is short, which
    // causes ugly word wrapping with single-word lines in the informative text.
    // Why does QMessageBox::setMinimumWidth() not work (tested in Qt 4.7)?
    QString expandedText = text;
    QFontMetrics metrics(qApp->font("QLabel"));
    int minWidth = 30 * metrics.averageCharWidth();
    while (metrics.width(expandedText) < minWidth)
        expandedText.append(" ");
    msgBox.setText(expandedText);
    msgBox.setInformativeText(infoText);
}

void showFatal(const QString& detailedText)
{
    // Don't translate these error messages. They shouldn't occur if the
    // program is correct and if it is not, they can occur in situations
    // when the translators are not yet installed.
    QMessageBox msgBox;
    msgBox.setWindowTitle("Unexpected Error");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("An unexpected error occurred.");
    QString infoText =
        "Please report this error together with any details available with"
        " the button below and other context information at the Pentobi"
        " <a href=\"http://sf.net/p/pentobi/bugs\">bug tracker</a>.";
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
