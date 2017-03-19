#include "BasicXmlSyntaxHighlighter.h"

// Special treatement for this rule in BasicXmlSyntaxHighlighter::highlightBlock
QTextCharFormat m_xmlElementFormat;
QRegExp m_xmlElementRegex;

BasicXmlSyntaxHighlighter::BasicXmlSyntaxHighlighter(QTextDocument *parent)
    : SyntaxHighlighter(parent)
{
    // xml keywords
    {
        QTextCharFormat format;
        QStringList patterns;
        format.setForeground(Qt::blue);
        format.setFontWeight(QFont::Bold);
        patterns << "<" << "<\\?" << ">" << "/>" << "\\?>" << "\\b?xml\\b";
        registerRules(format, patterns);
    }

    // xml elements
    {
        QTextCharFormat format;
        QStringList patterns;
        format.setForeground(Qt::darkMagenta);
        format.setFontWeight(QFont::Bold);
        patterns << "<[\\s]*[/]?[\\s]*([^\\n]\\w*)(?=[\\s/>])";

        // this rule is not registered, since it's treated in a particular way.
        m_xmlElementFormat = format;
        m_xmlElementRegex  = QRegExp(patterns.first());
    }

    // xml attributes
    {
        QTextCharFormat format;
        QStringList patterns;
        format.setForeground(Qt::darkGreen);
        format.setFontWeight(QFont::Bold);
        //format.setFontItalic(true);
        patterns << "\\w+(?=\\=)";
        registerRules(format, patterns);
    }

    // xml values
    {
        QTextCharFormat format;
        QStringList patterns;
        format.setForeground(Qt::darkRed);
        patterns << "\"[^\\n\"]+\"(?=[\\s/>])";
        registerRules(format, patterns);
    }

    // xml comments
    {
        QTextCharFormat format;
        QStringList patterns;
        format.setForeground(Qt::gray);
        patterns << "<!--[^\\n]*-->";
        registerRules(format, patterns);
    }
}

void BasicXmlSyntaxHighlighter::highlightBlock(const QString &text)
{
    // Special treatment for xml element regex as we use captured text to emulate lookbehind
    int xmlElementIndex = m_xmlElementRegex.indexIn(text);
    while(xmlElementIndex >= 0)
    {
        int matchedPos = m_xmlElementRegex.pos(1);
        int matchedLength = m_xmlElementRegex.cap(1).length();
        setFormat(matchedPos, matchedLength, m_xmlElementFormat);

        xmlElementIndex = m_xmlElementRegex.indexIn(text, matchedPos + matchedLength);
    }

    SyntaxHighlighter::highlightBlock(text);
}
