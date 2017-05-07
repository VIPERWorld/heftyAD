#ifndef PIXMAPFORMWIDGET_H
#define PIXMAPFORMWIDGET_H

#include "DataFormWidget.h"

#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QSlider>
#include <QToolButton>

class PixmapFormWidget : public DataFormWidget
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
    /**
     * Returns the pixmap one sees on the screen.
     * The returned value may differ from the one returned by pixmap(), since the pixmap can be scaled.
     */
    QPixmap visualPixmap() const;

    QToolButton* chooseButton() const;
    QToolButton* clearButton() const;
    QLineEdit* lineEdit() const;

    // shortcut functions
    void setChooseButtonIcon(const QIcon &icon);
    void setClearButtonIcon(const QIcon &icon);
    void setLineEditPlaceholder(const QString &text);

protected:
    void changePixmap(const QPixmap &pixmap, bool emitPixmapEdited);
    void changePixmap(const QString &filePath, bool emitPixmapEdited);

private:
    void usePixmap(const QPixmap &pixmap, bool emitPixmapEdited);

signals:
    /**
     * This signal is emitted whenever the pixmap changes, even programmatically.
     */
    void pixmapChanged();
    /**
     * This signal is emitted whenever the color is edited (i.e. whenever it changes excluded programmatically).
     */
    void pixmapEdited();

private slots:
    void onChooseButtonPressed();
    void onClearButtonPressed();
    void onSliderValueChanged();
};

#endif // PIXMAPFORMWIDGET_H
