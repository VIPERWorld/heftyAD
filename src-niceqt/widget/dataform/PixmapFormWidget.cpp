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

    connect(this, &PixmapFormWidget::pixmapChanged, this, &PixmapFormWidget::onPixmapChanged);
    //
    connect(&m_choose, &QToolButton::pressed,  this, &PixmapFormWidget::onChooseButtonPressed);
    connect(&m_clear,  &QToolButton::pressed,  this, &PixmapFormWidget::onClearButtonPressed);
    connect(&m_slider, &QSlider::valueChanged, this, &PixmapFormWidget::onSliderValueChanged);

    m_slider.setValue(50); // at the beginning
}

QPixmap PixmapFormWidget::pixmap() const {return m_pixmap;}

void PixmapFormWidget::setPixmap(const QPixmap &pixmap)
{
    m_filePath = "";
    changePixmap(pixmap);
}

void PixmapFormWidget::setPixmap(const QString &filePath)
{
    if(m_filePath != filePath) {
        m_filePath = filePath;
        changePixmap(QPixmap(filePath));
    }
}

QString PixmapFormWidget::filePath() const {return m_filePath;}
QPixmap PixmapFormWidget::visualPixmap() const {return m_visual.pixmap() ? *m_visual.pixmap() : QPixmap();}

QToolButton* PixmapFormWidget::chooseButton() const {return const_cast<QToolButton*>(&m_choose);}
QToolButton* PixmapFormWidget::clearButton() const {return const_cast<QToolButton*>(&m_clear);}
QLineEdit* PixmapFormWidget::lineEdit() const {return const_cast<QLineEdit*>(&m_lineEdit);}

void PixmapFormWidget::setChooseButtonIcon(const QIcon &icon) {m_choose.setIcon(icon);}
void PixmapFormWidget::setClearButtonIcon(const QIcon &icon) {m_clear.setIcon(icon);}
void PixmapFormWidget::setLineEditPlaceholder(const QString &text) {m_lineEdit.setPlaceholderText(text);}

void PixmapFormWidget::changePixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    emit pixmapChanged();
}

void PixmapFormWidget::onPixmapChanged()
{
    m_lineEdit.setText(m_filePath);
    m_lineEdit.setToolTip(m_filePath);
    onSliderValueChanged();
}

void PixmapFormWidget::onChooseButtonPressed()
{
    QStringList filters;
    for(QByteArray b : QImageReader::supportedImageFormats()) {
        filters << "*."+QString(b);
    }

    const QString &filePath = QFileDialog::getOpenFileName(this, "", "", filters.join(" "));
    if(!filePath.isEmpty()) {
        setPixmap(filePath);
    }
}

void PixmapFormWidget::onClearButtonPressed()
{
    setPixmap(QPixmap()); // We use that form since setPixmap("") will not work if the pixmapFilePath() is already empty
}

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
