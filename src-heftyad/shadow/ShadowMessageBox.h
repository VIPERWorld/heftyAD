#ifndef SHADOWMESSAGEBOX_H
#define SHADOWMESSAGEBOX_H

#include <QMessageBox>

class ShadowMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    explicit ShadowMessageBox(QWidget *parent = 0);

    void setIcon(Icon icon);

    ShadowMessageBox& withIcon(Icon icon);
    ShadowMessageBox& withWindowTitle(const QString &title);
    ShadowMessageBox& withWindowIcon(const QIcon &icon);
    ShadowMessageBox& withTextFormat(Qt::TextFormat format);
    ShadowMessageBox& withText(const QString &text);
    ShadowMessageBox& withInformativeText(const QString &text);
    ShadowMessageBox& withDetailedText(const QString &text);
    ShadowMessageBox& withStandardButtons(StandardButtons buttons);
    ShadowMessageBox& withButtonTexts(const QStringList &texts);
    ShadowMessageBox& withDefaultButton(StandardButton button);

protected:
    QIcon shadowIconFor(Icon icon) const;

signals:

public slots:
};

#endif // SHADOWMESSAGEBOX_H
