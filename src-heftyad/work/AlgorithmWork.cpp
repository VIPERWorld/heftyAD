#include "AlgorithmWork.h"
#include "JSFileParser.h"

#include <QFile>
#include <QTextStream>

AlgorithmWork::AlgorithmWork(QWidget *parent)
    : Work(parent)
{
    // add widgets

    m_splitter.addWidget(&m_editor);
    m_splitter.addWidget(&m_console);
    addWidget(&m_splitter, 0, 0);

    // customize widgets

    setEditorInitialText();
    m_editor.setTabWidth(4);
    m_editor.setModified(false);

    m_console.setReadOnly(true);
    m_console.setContextMenuPolicy(Qt::NoContextMenu);
    m_console.setStyleSheet("QTextEdit {font-size: 17px; font-family: Consolas; color: white; background-color: rgb(25, 25, 25);}");

    m_splitter.setOrientation(Qt::Vertical);
    m_splitter.setCollapsible(0, false);
    m_splitter.setStretchFactor(0, 5);

    // connect signals/slots

    connect(&m_editor, &SciCodeEditor::modificationChanged, this, &AlgorithmWork::dirtyChanged);
    connect(&m_editor, &SciCodeEditor::textChanged, [this]() {
        m_console.clear();
    });
}

bool AlgorithmWork::allowsExtraFeature(int feature) {return feature == Compile;}

bool AlgorithmWork::isDirty() const {return m_editor.isModified();}
void AlgorithmWork::setDirty(bool dirty) {m_editor.setModified(dirty);}

bool AlgorithmWork::saveTo(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_editor.text().replace("\r\n", "\n");
        return true;
    }

    return false;
}

bool AlgorithmWork::loadFrom(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_editor.setText(file.readAll());
        return true;
    }

    return false;
}

void AlgorithmWork::retranslate()
{
}

void AlgorithmWork::setEditorInitialText()
{
    // We didn't call loadFrom() since the function is virtual and we're in a constructor.

    QFile file(":/rsrc/algorithm/template.js");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_editor.setText(file.readAll());
    }
}

void AlgorithmWork::execExtraFeature(int feature)
{
    if(feature == Compile) {
        JSFileParser parser;
        if(!parser.loadProgram(m_editor.text())) {
            m_console.setText(parser.errorText());
            return;
        }

        m_console.append(trUtf8("Votre algorithme est <B>syntaxiquement</B> correct. Vous pouvez l'exécuter."));

        m_console.append("");
        m_console.append("<B><U>"+trUtf8("nom")+"</B></U><br />");
        m_console.append(parser.algorithmName());

        m_console.append("");
        m_console.append("<B><U>"+trUtf8("description")+"</B></U><br />");
        m_console.append(parser.algorithmDescription());

        m_console.append("");
        m_console.append("<B><U>"+trUtf8("auteurs")+"</B></U>");
        for(const JSFileParser::Author &author : parser.algorithmAuthors()) {
            m_console.append("");
            m_console.append(author.name);
            m_console.append(author.description);
        }
    }
}
