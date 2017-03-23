#ifndef PIXMAPFORMWIDGET_H
#define PIXMAPFORMWIDGET_H

#include "widget/FormWidget.h"

#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QSlider>
#include <QToolButton>

class PixmapFormWidget : public FormWidget
{
    Q_OBJECT

private:
    QPixmap m_pixmap;
    QString m_filePath;

    QToolButton m_choose; QLineEdit m_lineEdit; QToolButton m_clear;
    QSlider m_slider; QLCDNumber m_lcd;
    QLabel m_visual;

public:
    explicit PixmapFormWidget(QWidget *parent = 0);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);
    void setPixmap(const QString &filePath);

    QString filePath() const;
    QPixmap visualPixmap() const;

    QToolButton* chooseButton() const;
    QToolButton* clearButton() const;
    QLineEdit* lineEdit() const;

    // shortcut functions
    void setChooseButtonIcon(const QIcon &icon);
    void setClearButtonIcon(const QIcon &icon);
    void setLineEditPlaceholder(const QString &text);

private:
    void changePixmap(const QPixmap &pixmap);

signals:
    void pixmapChanged();

private slots:
    void onPixmapChanged();

    void onChooseButtonPressed();
    void onClearButtonPressed();
    void onSliderValueChanged();
};

#endif // PIXMAPFORMWIDGET_H
