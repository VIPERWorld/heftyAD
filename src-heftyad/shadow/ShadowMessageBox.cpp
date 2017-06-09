#include "Resource.h"
#include "ShadowMessageBox.h"

#include <QAbstractButton>
#include <QIcon>

ShadowMessageBox::ShadowMessageBox(QWidget *parent)
    : QMessageBox(parent)
{
    if(parent == nullptr) { // Do that only if the message box has no parent widget.
        setWindowIcon(Resource::windowIcon());
    }
}

void ShadowMessageBox::setIcon(Icon icon)
{
    setIconPixmap(shadowIconFor(icon).pixmap(50, 50));
}

ShadowMessageBox& ShadowMessageBox::withIcon(QMessageBox::Icon icon) {setIcon(icon); return *this;}
ShadowMessageBox& ShadowMessageBox::withWindowTitle(const QString &title) {setWindowTitle(title); return *this;}
ShadowMessageBox& ShadowMessageBox::withWindowIcon(const QIcon &icon) {setWindowIcon(icon); return *this;}
ShadowMessageBox& ShadowMessageBox::withTextFormat(Qt::TextFormat format) {setTextFormat(format); return *this;}
ShadowMessageBox& ShadowMessageBox::withText(const QString &text) {setText(text); return *this;}
ShadowMessageBox& ShadowMessageBox::withInformativeText(const QString &text) {setInformativeText(text); return *this;}
ShadowMessageBox& ShadowMessageBox::withDetailedText(const QString &text) {setDetailedText(text); return *this;}
ShadowMessageBox& ShadowMessageBox::withStandardButtons(QMessageBox::StandardButtons buttons)
{
    setStandardButtons(buttons);
    for(QAbstractButton *b : this->buttons()) {
        b->setCursor(Qt::PointingHandCursor);
    }

    return *this;
}

ShadowMessageBox& ShadowMessageBox::withButtonTexts(const QStringList &texts)
{
    const QList<QAbstractButton*> &btns(buttons());
    const int &s1(btns.size());
    const int &s2(texts.size());

    for(int i=0; i<s1 && i<s2; i++) {
        QAbstractButton *b = btns[i];
        b->setText(texts[i]);
    }

    return *this;
}
ShadowMessageBox& ShadowMessageBox::withDefaultButton(QMessageBox::StandardButton button) {setDefaultButton(button); return *this;}

QIcon ShadowMessageBox::shadowIconFor(QMessageBox::Icon icon) const
{
    QString image;

    switch(icon) {
        case NoIcon:      image = "shadow.png";         break;
        case Question:    image = "shadow.png";         break;

        case Information: image = "shadow_info.png";    break;
        case Warning:     image = "shadow_warning.png"; break;
        case Critical:    image = "shadow_error.png";   break;

        default: break;
    }

    return Resource::shadowIcon(image);
}
