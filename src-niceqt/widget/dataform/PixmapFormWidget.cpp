#include "PixmapFormWidget.h"

#include <QFileDialog>
#include <QImageReader>

PixmapFormWidget::PixmapFormWidget(QWidget *parent)
    : FormWidget(parent)
{
    addRow(QWidgetList() << &m_choose << &m_lineEdit << &m_clear, Qt::Horizontal);
    addRow(QWidgetList() << &m_slider << &m_lcd,                  Qt::Horizontal);
    addRow(&m_visual);

    m_choose.setText("---");
    m_choose.setCursor(Qt::PointingHandCursor);
    m_lineEdit.setContextMenuPolicy(Qt::NoContextMenu);
    m_lineEdit.setPlaceholderText("image file path");
    m_lineEdit.setReadOnly(true);
    m_clear.setCursor(Qt::PointingHandCursor);
    //
    m_slider.setOrientation(Qt::Horizontal);
    m_slider.setTickPosition(QSlider::TicksBelow);
    m_slider.setRange(10, 100);
    m_lcd.setDigitCount(4);
    //
    m_visual.setAlignment(Qt::AlignCenter);

    connect(&m_choose, &QToolButton::pressed,  this, &PixmapFormWidget::onChooseButtonPressed);
    connect(&m_clear,  &QToolButton::pressed,  this, &PixmapFormWidget::onClearButtonPressed);
    connect(&m_slider, &QSlider::valueChanged, this, &PixmapFormWidget::onSliderValueChanged);

    m_slider.setValue(50); // at the beginning
}

QPixmap PixmapFormWidget::pixmap() const {return m_pixmap;}
void PixmapFormWidget::setPixmap(const QPixmap &pixmap) {changePixmap(pixmap, false);}
void PixmapFormWidget::setPixmap(const QString &filePath) {changePixmap(filePath, false);}

QString PixmapFormWidget::filePath() const {return m_filePath;}
QPixmap PixmapFormWidget::visualPixmap() const {return m_visual.pixmap() ? *m_visual.pixmap() : QPixmap();}

QToolButton* PixmapFormWidget::chooseButton() const {return const_cast<QToolButton*>(&m_choose);}
QToolButton* PixmapFormWidget::clearButton() const {return const_cast<QToolButton*>(&m_clear);}
QLineEdit* PixmapFormWidget::lineEdit() const {return const_cast<QLineEdit*>(&m_lineEdit);}

void PixmapFormWidget::setChooseButtonIcon(const QIcon &icon) {m_choose.setIcon(icon);}
void PixmapFormWidget::setClearButtonIcon(const QIcon &icon) {m_clear.setIcon(icon);}
void PixmapFormWidget::setLineEditPlaceholder(const QString &text) {m_lineEdit.setPlaceholderText(text);}

void PixmapFormWidget::changePixmap(const QPixmap &pixmap, bool emitPixmapEdited)
{
    m_filePath = "";
    usePixmap(pixmap, emitPixmapEdited);
}

void PixmapFormWidget::changePixmap(const QString &filePath, bool emitPixmapEdited)
{
    /*
     * We don't check wether m_filePath != filePath since a filepath describing an image img1 at a given time
     * may later refer to another image img2 (after renaming, deletion, etc).
     */
    m_filePath = filePath;
    usePixmap(QPixmap(m_filePath), emitPixmapEdited);
}

void PixmapFormWidget::usePixmap(const QPixmap &pixmap, bool emitPixmapEdited)
{
    m_pixmap = pixmap;

    m_lineEdit.setText(m_filePath);
    m_lineEdit.setToolTip(m_filePath);
    onSliderValueChanged();

    if(emitPixmapEdited) { // not mandatory but we emit the pixmap edited signal first
        emit pixmapEdited();
    }
    emit pixmapChanged();
}

void PixmapFormWidget::onChooseButtonPressed()
{
    QStringList filters;
    for(const QByteArray &b : QImageReader::supportedImageFormats()) {
        filters << "*."+QString(b);
    }

    const QString &filePath = QFileDialog::getOpenFileName(this, "", "", filters.join(" "));
    if(!filePath.isEmpty()) { // make sure a path was choosen
        changePixmap(filePath, true);
    }
}

void PixmapFormWidget::onClearButtonPressed() {changePixmap("", true);}

void PixmapFormWidget::onSliderValueChanged()
{
    const int value = m_slider.value();
    QPixmap newPixmap;

    m_lcd.display(value);
    if(!m_pixmap.isNull()) { // to make sure QPixmap::scaled doesn't issue a warning
        const QSize &size = m_pixmap.size();
        const QSize &newSize = size*value/100;
        newPixmap = m_pixmap.scaled(newSize);
    }

    m_visual.setPixmap(newPixmap);
}
